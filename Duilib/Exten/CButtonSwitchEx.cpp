#include "stdafx.h"
#include "CButtonSwitchEx.h"

namespace DuiLib
{
	CButtonSwitchExUI::CButtonSwitchExUI()
		: m_uThumbState(0)
	{
	}

	LPCTSTR CButtonSwitchExUI::GetClass() const
	{
		return _T("CButtonSwitchExUI");
	}

	LPVOID CButtonSwitchExUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_CHECKBOX) == 0) return static_cast<CButtonSwitchExUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	void DuiLib::CButtonSwitchExUI::DoEvent(TEventUI& event)
	{
		CString str;
		str.Format(_T("event.Type:%d, event.ptMouse.x:%d:%d, m_uThumbState=%d"), event.Type, event.ptMouse.x, event.ptMouse.y, m_uThumbState);
		OutputDebugString(str);
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
		{
			if (!IsEnabled()) return;


			m_rcThumb = GetPos();
			if (m_nMax > m_nMin)
			{
				m_rcThumb.left = m_rcThumb.left + (m_nValue - m_nMin) * (GetWidth() - m_nPointWidth) / (m_nMax - m_nMin);
			}
			m_rcThumb.right = m_rcThumb.left + m_nPointWidth;
			m_rcThumb.bottom = m_rcThumb.top + m_nPointHeight;

			str.Format(_T("m_rcThumb:%d,%d,%d,%d"), m_rcThumb.left, m_rcThumb.top, m_rcThumb.right, m_rcThumb.bottom);
			OutputDebugString(str);
			if (::PtInRect(&m_rcThumb, event.ptMouse)) {
				m_pManager->SetTimer(this, DEFAULT_TIMERID, 50U);
				m_uThumbState |= UISTATE_CAPTURED | UISTATE_PUSHED;
				m_ptLastMouse = event.ptMouse;
			}
			else
			{
				if (m_nMax > m_nMin)
				{
					float step = (GetWidth() - m_nPointWidth) / (m_nMax - m_nMin);
					str.Format(_T("GetWidth():%d,step=%d"), GetWidth(), step);
					OutputDebugString(str);
					for (float i = m_nMin; i <= m_nMax; i = i + 1.00)
					{
						RECT rt = GetPos();
						if ((i - 0.50) * step + rt.left <= event.ptMouse.x
							&& (i + 0.50) * step + rt.left >= event.ptMouse.x)
						{
							SetValue(i, TRUE);
							break;
						}
					}
				}
			}
		}
		if (event.Type == UIEVENT_BUTTONUP)
		{
			if ((m_uThumbState & UISTATE_CAPTURED) != 0) {
				m_pManager->KillTimer(this, DEFAULT_TIMERID);
				m_uThumbState &= ~(UISTATE_CAPTURED | UISTATE_PUSHED);
				m_Offset = 0;
				if (m_nMax > m_nMin)
				{
					float step = (GetWidth() - m_nPointWidth) / (m_nMax - m_nMin);
					for (float i = m_nMin; i <= m_nMax; i = i + 1.00)
					{
						RECT rt = GetPos();
						if ((i - 0.50) * step + rt.left <= event.ptMouse.x
							&& (i + 0.50) * step + rt.left >= event.ptMouse.x)
						{
							SetValue(i, TRUE);
							break;
						}
					}
				}
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			if ((m_uThumbState & UISTATE_CAPTURED) != 0) {
				m_Offset = event.ptMouse.x - m_ptLastMouse.x;
			}
			return;
		}
		if (event.Type == UIEVENT_TIMER && event.wParam == DEFAULT_TIMERID)
		{
			UpdataProgressValue(m_Offset);
			return;
		}

		CButtonUI::DoEvent(event);
	}

	void DuiLib::CButtonSwitchExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("pointImage")) == 0)
		{
			m_strPointImage = pstrValue;
		}
		else if (_tcscmp(pstrName, _T("pointheight")) == 0)
		{
			m_nPointHeight = _ttoi(pstrValue);
		}
		else if (_tcscmp(pstrName, _T("pointwidth")) == 0)
		{
			m_nPointWidth = _ttoi(pstrValue);
		}
		else if (_tcscmp(pstrName, _T("max")) == 0)
		{
			m_nMax = _ttoi(pstrValue);
		}
		else if (_tcscmp(pstrName, _T("min")) == 0)
		{
			m_nMin = _ttoi(pstrValue);
		}
		else if (_tcscmp(pstrName, _T("value")) == 0)
		{
			SetValue(_ttoi(pstrValue), FALSE);
		}

		CButtonUI::SetAttribute(pstrName, pstrValue);
		SetValue(GetValue(), FALSE);
	}

	int DuiLib::CButtonSwitchExUI::GetValue() const
	{
		return m_nValue;
	}

	void DuiLib::CButtonSwitchExUI::SetValue(int nValue, BOOL fireEvent)
	{
		CString str;
		str.Format(_T("nValue=%d"), nValue);
		OutputDebugString(str);
		if (m_nValue == nValue)
		{
			UpdataProgressValue();
		}
		else
		{
			m_nValue = nValue;
			if (m_nValue > m_nMax) m_nValue = m_nMax;
			if (m_nValue < m_nMin) m_nValue = m_nMin;
			UpdataProgressValue();
			if (fireEvent)
				m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
		}
	}

	void DuiLib::CButtonSwitchExUI::UpdataProgressValue(int Offset)
	{
		int left = 0;
		if (m_nMax > m_nMin)
		{
			left = (m_nValue - m_nMin) * (GetWidth() - m_nPointWidth) / (m_nMax - m_nMin);
		}
		
		if (0 != Offset)
		{
			if (Offset + left > GetWidth() - m_nPointWidth)
			{
				Offset = GetWidth() - m_nPointWidth - left;
			}
			else if (Offset + left < 0)
			{
				Offset = 0 - left;
			}
		}

		CString strImgFile;
		strImgFile.Format(_T("file='%s' dest='%d,%d,%d,%d'"), m_strPointImage, left + Offset, 0, left + Offset + m_nPointWidth, m_nPointHeight);
		SetNormalImage(strImgFile);
		OutputDebugString(strImgFile);
		Invalidate();
	}
}
