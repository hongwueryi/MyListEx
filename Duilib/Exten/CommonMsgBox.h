#pragma once
#include <atlstr.h>
#include <windowsx.h>
namespace DuiLib
{
	enum MSGBOX_ICON_TYPE
	{
		MSG_ICON_TYPE_NONE = 0,
		MSG_ICON_TYPE_QUESTION = 1, //问号
		MSG_ICON_TYPE_MARK =2, //叹号
		MSG_ICON_TYPE_ERROR =3, //错误
		MSG_ICON_TYPE_WIFI,	//断网
	};

	enum MSGBOX_BUTTON_TYPE
	{
		MSG_BUTTONTYPE_OK = 0,	//只有一个确定按钮
		MSG_BUTTONTYPE_OKCANCEL = 1,	//确定取消按钮
	};

	enum MSGBOX_CLICK_RESULT
	{
		MSG_CLICK_NONE = 0,
		MSG_CLICK_OK = 1,
		MSG_CLICK_CANCEL =2,
		MSG_CLICK_CLOSE = 3,
	};
	class CommonMsgBoxWindow :public WindowImplBase
	{
	public:
		static CommonMsgBoxWindow* CreateMsgBox(HWND parentWnd, CString title, CString content, MSGBOX_ICON_TYPE icon = MSG_ICON_TYPE_MARK,
			MSGBOX_BUTTON_TYPE type = MSG_BUTTONTYPE_OKCANCEL,CString okText = _T("确认"), CString cancelText = _T("取消"), BOOL topMost = FALSE, int resourceId = 131, bool bContentHtml = true);

		MSGBOX_CLICK_RESULT ShowMsgBox();

		virtual UILIB_RESOURCETYPE GetResourceType() const override;

		virtual LPCTSTR GetResourceID() const override;

		CommonMsgBoxWindow();

		virtual ~CommonMsgBoxWindow(void);

		virtual CDuiString GetSkinFile() override;

		virtual void Notify(TNotifyUI& msg) override;

		virtual LPCTSTR GetWindowClassName(void) const override;

		virtual void InitWindow();

		void SetTipIconShow();

		void SetMsgStyleTypeShow();

		virtual CDuiString GetSkinFolder() override;

		virtual void OnFinalMessage(HWND hWnd) override;

	public:
		MSGBOX_CLICK_RESULT* m_clicktype;
		MSGBOX_ICON_TYPE m_iconType;
		MSGBOX_BUTTON_TYPE m_styleType;

		int m_resourceId;
		HWND m_parentWnd;
		CString m_okText;
		CString m_cancelText;

		CString m_strTitle;
		CString m_strContent;
		bool m_topMost;
		bool m_bContentHtml = true;

		CButtonUI*	m_pCloseBtn;
		CButtonUI*	m_pOKBtn;
		CButtonUI*	m_pCancelBtn;
		CTextUI*	m_pContect;
		CControlUI*	m_pIcon;
		CControlUI* m_pBkImg;

		CControlUI* m_controlSepartor;

		//默认OK按钮
		bool m_defaultOKButton;
	};
}