#include "stdafx.h"
#include "UICustomButton.h"

namespace DuiLib
{
	CCustomButtonUI::CCustomButtonUI()
	{
	}

	LPCTSTR CCustomButtonUI::GetClass() const
	{
		return _T("CustomButtonUI");
	}

	LPVOID CCustomButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_CUSTOMBUTTON) == 0 ) return static_cast<CCustomButtonUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}


	void CCustomButtonUI::SetTitleHotTextColor(DWORD dwColor)
	{
		m_dwHotTextTitleColor = dwColor;
	}
	DWORD CCustomButtonUI::GetTitleHotTextColor() const
	{
		return m_dwHotTextTitleColor;
	}
	void CCustomButtonUI::SetTitleTextColor(DWORD dwColor)
	{
		m_dwTextTitleColor = dwColor;
	}
	DWORD CCustomButtonUI::GetTitleTextColor() const
	{
		return m_dwTextTitleColor;
	}
	RECT CCustomButtonUI::GetTitleTextPadding() const
	{
		return m_rcTextPadding_Title;
	}
	void CCustomButtonUI::SetTitleTextPadding(RECT rc)
	{
		m_rcTextPadding_Title = rc;
		NeedParentUpdate();
	}
	void CCustomButtonUI::SetTitleFont(int index)
	{
		m_iFont_Title = index;
		Invalidate();
	}
	int CCustomButtonUI::GetTitleFont() const
	{
		return m_iFont_Title;
	}
	void CCustomButtonUI::SetTitleText(LPCTSTR pstrText)
	{
		if (m_sTextTitle == pstrText) return;
		m_sTextTitle = pstrText;
		Invalidate();
	}
	CDuiString CCustomButtonUI::GetTitleText() const
	{
		return m_sTextTitle;
	}

	void CCustomButtonUI::SetValueHotTextColor(DWORD dwColor)
	{
		m_dwHotTextValueColor = dwColor;
	}
	DWORD CCustomButtonUI::GetValueHotTextColor() const
	{
		return m_dwHotTextValueColor;
	}
	void CCustomButtonUI::SetValueTextColor(DWORD dwColor)
	{
		m_dwTextValueColor = dwColor;
	}
	DWORD CCustomButtonUI::GetValueTextColor() const
	{
		return m_dwTextValueColor;
	}
	RECT CCustomButtonUI::GetValueTextPadding() const
	{
		return m_rcTextPadding_Value;
	}
	void CCustomButtonUI::SetValueTextPadding(RECT rc)
	{
		m_rcTextPadding_Value = rc;
		NeedParentUpdate();
	}
	void CCustomButtonUI::SetValueFont(int index)
	{
		m_iFont_Value = index;
		Invalidate();
	}
	int CCustomButtonUI::GetValueFont() const
	{
		return m_iFont_Value;
	}
	void CCustomButtonUI::SetValueText(LPCTSTR pstrText)
	{
		if (m_sTextValue == pstrText) return;
		m_sTextValue = pstrText;
		Invalidate();
	}
	CDuiString CCustomButtonUI::GetValueText() const
	{
		return m_sTextValue;
	}

	void CCustomButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("TitleText")) == 0 ) SetTitleText(pstrValue);
		else if( _tcscmp(pstrName, _T("ValueText")) == 0 ) SetValueText(pstrValue);
		else if (_tcscmp(pstrName, _T("TitleFont")) == 0) SetTitleFont(_ttoi(pstrValue));
		else if (_tcscmp(pstrName, _T("ValueFont")) == 0) SetValueFont(_ttoi(pstrValue));
		else if (_tcscmp(pstrName, _T("textTitlepadding")) == 0) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetTitleTextPadding(rcTextPadding);
		}
		else if (_tcscmp(pstrName, _T("textValuepadding")) == 0) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetValueTextPadding(rcTextPadding);
		}
		else if( _tcscmp(pstrName, _T("hotTitletextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTitleHotTextColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("hotValuetextcolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetValueHotTextColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("Titletextcolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTitleTextColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("Valuetextcolor")) == 0)
		{
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetValueTextColor(clrColor);
		}
		else CButtonUI::SetAttribute(pstrName, pstrValue);
	}

	void CCustomButtonUI::PaintText(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if (!m_sText.IsEmpty())
		{
			int nLinks = 0;
			int font = m_iFont;
			RECT rc = m_rcItem;
			rc.left += m_rcTextPadding.left;
			rc.right -= m_rcTextPadding.right;
			rc.top += m_rcTextPadding.top;
			rc.bottom -= m_rcTextPadding.bottom;

			DWORD clrColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;

			if (((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0))
				clrColor = GetPushedTextColor();
			else if (((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0)) {
				clrColor = GetHotTextColor();
				if (m_iHoverFont >= 0)
				{
					font = m_iHoverFont;
				}
			}
			else if (((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0))
				clrColor = GetFocusedTextColor();

			if (m_bShowHtml)
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, clrColor, \
					NULL, NULL, nLinks, m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, clrColor, \
					font, m_uTextStyle);
		}
		if (!m_sTextTitle.IsEmpty())
		{
			RECT rc = m_rcItem;
			rc.left += m_rcTextPadding_Title.left;
			rc.right -= m_rcTextPadding_Title.right;
			rc.top += m_rcTextPadding_Title.top;
			rc.bottom -= m_rcTextPadding_Title.bottom;
			DWORD clrColor = m_dwTextTitleColor;

			if (((m_uButtonState & UISTATE_HOT) != 0) && (GetTitleHotTextColor() != 0)) {
				clrColor = GetTitleHotTextColor();
			}
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sTextTitle, clrColor, \
					m_iFont_Title, m_uTextStyle);
		}

		if (!m_sTextValue.IsEmpty())
		{
			RECT rc = m_rcItem;
			rc.left += m_rcTextPadding_Value.left;
			rc.right -= m_rcTextPadding_Value.right;
			rc.top += m_rcTextPadding_Value.top;
			rc.bottom -= m_rcTextPadding_Value.bottom;
			DWORD clrColor = m_dwTextValueColor;

			if (((m_uButtonState & UISTATE_HOT) != 0) && (GetValueHotTextColor() != 0)) {
				clrColor = GetValueHotTextColor();
			}
			CRenderEngine::DrawText(hDC, m_pManager, rc, m_sTextValue, clrColor, \
				m_iFont_Value, m_uTextStyle);
		}
	}
}