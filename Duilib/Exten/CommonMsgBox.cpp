#include "stdafx.h"
#include "CommonMsgBox.h"
using namespace DuiLib;


DuiLib::CommonMsgBoxWindow* DuiLib::CommonMsgBoxWindow::CreateMsgBox(HWND parentWnd, CString title, CString content, MSGBOX_ICON_TYPE icon /*= TIP_ICON_TYPE_MARK*/, MSGBOX_BUTTON_TYPE type /*= MSG_STYLE_OKCANCEL*/,CString okText /*= _T("确认")*/, CString cancelText /*= _T("取消")*/, BOOL topMost, int resourceId, bool bContentHtml/* = true*/)
{
	CommonMsgBoxWindow *pWin = new CommonMsgBoxWindow();
	
	pWin->m_parentWnd = parentWnd;
	pWin->m_strTitle = title;
	pWin->m_strContent = content;
	pWin->m_iconType = icon;
	pWin->m_styleType = type;
	pWin->m_okText = okText;
	pWin->m_cancelText = cancelText;
	pWin->m_resourceId = resourceId;
	pWin->m_topMost = topMost;
	pWin->m_defaultOKButton = false;
	pWin->m_bContentHtml = bContentHtml;

	return pWin;
}

DuiLib::MSGBOX_CLICK_RESULT DuiLib::CommonMsgBoxWindow::ShowMsgBox()
{
	MSGBOX_CLICK_RESULT bRet = MSG_CLICK_NONE;
	this->m_clicktype = &bRet;
	
	this->Create(m_parentWnd, _T("LENOVO-MsgBox"), UI_WNDSTYLE_DIALOG, 0);

	this->CenterWindow();

	if (m_topMost)
	{
		SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	if (m_defaultOKButton)
	{
		m_pOKBtn->SetFocus();
	}
	else
	{
		m_pCancelBtn->SetFocus();
	}
	
	this->ShowModal();

	return bRet;
}

DuiLib::CommonMsgBoxWindow::CommonMsgBoxWindow()
{
}

DuiLib::CDuiString DuiLib::CommonMsgBoxWindow::GetSkinFolder()
{
	return _T(".\\skin\\");
}

void DuiLib::CommonMsgBoxWindow::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CommonMsgBoxWindow::~CommonMsgBoxWindow(void)
{
	
}

DuiLib::UILIB_RESOURCETYPE CommonMsgBoxWindow::GetResourceType() const
{
	return UILIB_FILE;
}

LPCTSTR CommonMsgBoxWindow::GetResourceID() const
{
	//return MAKEINTRESOURCE(m_resourceId);
	return _T("");
}

DuiLib::CDuiString CommonMsgBoxWindow::GetSkinFile()
{
	return _T("resource\\MessageBoxWindow\\MsgBox.dat");
}

void CommonMsgBoxWindow::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_LINK) {
		ShellExecuteW(NULL, L"open", ((CTextUI*)msg.pSender)->GetLinkContent(msg.wParam)->GetData(), _T(""), NULL, SW_SHOWNORMAL);
	}
	else if (msg.sType == _T("click")) {
		CDuiString name = msg.pSender->GetName();
		if (msg.pSender == m_pCancelBtn)
		{
			*m_clicktype = MSG_CLICK_CANCEL;
			return Close();
		}
		else if (msg.pSender == m_pOKBtn)
		{
			*m_clicktype = MSG_CLICK_OK;
			return Close();
		}
		else if (msg.pSender == m_pCloseBtn)
		{
			*m_clicktype = MSG_CLICK_CLOSE;
			return Close();
		}
	}
}

LPCTSTR CommonMsgBoxWindow::GetWindowClassName(void) const
{
	return _T("UIMessageBox");
}

void CommonMsgBoxWindow::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnclose")));
	m_pOKBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("idok_btn")));
	m_pCancelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("idcancel_btn")));
	m_pContect = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("content_text")));
	m_pIcon = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("msg_icon")));
	m_pBkImg = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("bkimg")));
	m_controlSepartor = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("controlSepartor")));
	m_pContect->SetShowHtml(m_bContentHtml);
	if (m_pContect)
	{
		CString str;
		str.Format(_T("{f 11}%s{\/f}"), m_strContent);
		m_pContect->SetText(str);
		
		m_pContect->SetTextStyle(m_pContect->GetTextStyle() | DT_VCENTER);
	}
	SetTipIconShow();
	SetMsgStyleTypeShow();
}

void CommonMsgBoxWindow::SetTipIconShow()
{
	if (m_iconType == MSG_ICON_TYPE_NONE)
	{
		CHorizontalLayoutUI* iconLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("iconlayout")));
		iconLayout->SetVisible(false);
	}
	else if (m_iconType == MSG_ICON_TYPE_ERROR)
	{
		m_pIcon->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_icon_info@1x.png"));
		m_pBkImg->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_img_popup_info@1x.png"));
	}
	else if (m_iconType == MSG_ICON_TYPE_MARK)
	{
		m_pIcon->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_icon_driver_warning@1x.png"));
		m_pBkImg->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_img_popup_driver_error@1x.png"));
	}
	else if (m_iconType == MSG_ICON_TYPE_QUESTION)
	{
		m_pIcon->SetBkImage(_T("resource\\MessageBoxWindow/ico_question.png"));
		m_pBkImg->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_img_popup_driver_error@1x.png"));
	}
	else if (m_iconType == MSG_ICON_TYPE_WIFI)
	{
		m_pIcon->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_icon_driver_notconnect@1x.png"));
		m_pBkImg->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_img_popup_driver_error@1x.png"));
	}
	else
	{
		m_pIcon->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_icon_driver_warning.png"));
		m_pBkImg->SetBkImage(_T("resource\\MessageBoxWindow/LPCM_img_popup_driver_error@1x.png"));
	}
}

void CommonMsgBoxWindow::SetMsgStyleTypeShow()
{
	m_pOKBtn->SetText(m_okText);
	m_pCancelBtn->SetText(m_cancelText);

	if (m_styleType == MSG_BUTTONTYPE_OK)
	{
		m_controlSepartor->SetVisible(FALSE);
		m_pCancelBtn->SetVisible(false);
	}
}
