#include "stdafx.h"
#include "CProgressEx.h"

void DuiLib::CProgressExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("lineforeimage")) == 0)
	{
		strForeImage = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("linebkcolor")) == 0) {
		while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dBkColor = _tcstoul(pstrValue, &pstr, 16);
	}
	else if (_tcscmp(pstrName, _T("point")) == 0) {
		m_pointPic = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("animation")) == 0) {
		m_animationPic = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("interval")) == 0) {
		m_interval = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("auto")) == 0) {
		m_autoStart = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("loop")) == 0) {
		m_loop = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("min")) == 0)
	{
		m_nMin = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("max")) == 0)
	{
		m_nMax = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("value")) == 0)
	{
		m_nValue = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("width")) == 0)
	{
		m_nWidth = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("height")) == 0)
	{
		m_nHeight = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("lineheight")) == 0)
	{
		m_nLineHeight = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("pointheight")) == 0)
	{
		m_nPointHeight = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("pointwidth")) == 0)
	{
		m_nPointWidth = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("animationheight")) == 0)
	{
		m_nAnimationHeight = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("animationwidth")) == 0)
	{
		m_nAnimationWidth = _ttoi(pstrValue);
	}
	
	__super::SetAttribute(pstrName, pstrValue);
}

void DuiLib::CProgressExUI::Init()
{
	m_lineLayout = new CVerticalLayoutUI();
	CContainerUI* topContainer = new CContainerUI();
	m_foreImg = new CContainerUI();
	CContainerUI* bottomContainer = new CContainerUI();

	m_lineLayout->Add(topContainer);
	m_lineLayout->Add(m_foreImg);
	m_lineLayout->Add(bottomContainer);

	m_CPointImg = new CContainerUI();

	m_linebkLayout = new CVerticalLayoutUI();
	CContainerUI* bktopContainer = new CContainerUI();
	m_bkImg = new CContainerUI();
	CContainerUI* bkbottomContainer = new CContainerUI();
	m_linebkLayout->Add(bktopContainer);
	m_linebkLayout->Add(m_bkImg);
	m_linebkLayout->Add(bkbottomContainer);

	Add(m_lineLayout);
	Add(m_CPointImg);
	Add(m_linebkLayout);

	m_rotata = new CRotateAnimateUI();
	m_rotata->SetFloat(true);
	Add(m_rotata);

	m_CPointImg->SetBkImage(m_pointPic);
	m_CPointImg->SetFixedWidth(m_nPointWidth);
	m_CPointImg->SetFixedHeight(m_nPointHeight);
	m_CPointImg->SetPadding({ 0,(m_nAnimationHeight - m_nPointHeight)/2,0,0 });
	m_rcPos.left = m_rcPos.top = 0;
	m_rcPos.right = m_rcPos.bottom = m_nAnimationHeight;

	m_rotata->SetAttribute(_T("interval"), m_interval);
	m_rotata->SetAttribute(_T("auto"), m_autoStart);
	m_rotata->SetAttribute(_T("loop"), m_loop);
	m_rotata->SetBkImage(m_animationPic);
	m_rotata->SetFixedWidth(m_nAnimationWidth);
	m_rotata->SetFixedHeight(m_nAnimationHeight);
	m_rotata->SetPos(m_rcPos);
	m_rotata->StartAnim();

	m_foreImg->SetFixedHeight(m_nLineHeight);

	m_bkImg->SetFixedHeight(m_nLineHeight);
	m_bkImg->SetBkColor(m_dBkColor);
	UpdataProgressValue();
}

LPVOID DuiLib::CProgressExUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("ProgressEx")) == 0)
		return static_cast<CProgressExUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void DuiLib::CProgressExUI::DoEvent(TEventUI& event)
{

}

void DuiLib::CProgressExUI::PaintBkImage(HDC hDC)
{
	__super::PaintBkImage(hDC);
}

void DuiLib::CProgressExUI::SetVisible(bool bVisible /*= true*/)
{
	__super::SetVisible(bVisible);
}

LPCTSTR DuiLib::CProgressExUI::GetForeImage() const
{
	return strForeImage;
}

void DuiLib::CProgressExUI::SetForeImage(LPCTSTR pStrImage)
{
	strForeImage = pStrImage;
	UpdataProgressValue();
}

int DuiLib::CProgressExUI::GetValue() const
{
	return m_nValue;
}

void DuiLib::CProgressExUI::SetValue(int nValue)
{
	m_nValue = nValue;
	if (m_nValue > m_nMax) m_nValue = m_nMax;
	if (m_nValue < m_nMin) m_nValue = m_nMin;
	UpdataProgressValue();
}

void DuiLib::CProgressExUI::UpdataProgressValue()
{
	int iForewidth = (m_nWidth*m_nValue) / m_nMax;
	if (iForewidth == 0)
	{
		iForewidth = 1;
	}
	if (m_nMax == m_nValue)
	{
		m_CPointImg->SetVisible(false);
		m_rotata->SetVisible(false);
	}
	else
	{
		if (!m_CPointImg->IsVisible())
		{
			m_CPointImg->SetVisible(true);
			m_rotata->SetVisible(true);
		}
	}

	m_rcPos.left = iForewidth - ((m_nAnimationWidth - m_nPointWidth) / 2);
	m_rcPos.right = m_rcPos.left + m_nAnimationWidth;
	m_rotata->SetPos(m_rcPos);

	m_lineLayout->SetFixedWidth(iForewidth);
	m_foreImg->SetFixedWidth(iForewidth);
	CString strImgFile;
	strImgFile.Format(_T("file='%s' source='0,0,%d,%d'"), strForeImage, iForewidth+3, m_nLineHeight);
	m_foreImg->SetBkImage(strImgFile);
	Invalidate();
}
