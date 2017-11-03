#pragma once  
#include "wke/wke.h"
#include "../Duilib/UIlib.h"
using namespace DuiLib;

/************************************************************************/
/* 无窗口的wke浏览器控件;                                               */
/************************************************************************/

struct TNativeMethodInfo
{
	CString name;
	CString params;
	CString ret;
};

class CWkeWindowlessUI : public CControlUI, public _wkeHandler, IMessageFilterUI
{
public:
	CWkeWindowlessUI(void);
	~CWkeWindowlessUI(void);
	virtual LPCTSTR GetClass()const;
	virtual LPVOID  GetInterface(LPCTSTR pstrName);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void    DoEvent(TEventUI& event);
	virtual void    SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual void    DoInit();
	virtual void    PaintBkImage(HDC hDC);
	virtual void    SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

private:
	POINT GetMouseClientPos();
	bool IsMouseInThisCtrl(POINT pt);


	//////////////////////////////////////  
public:
	void	Init(UINT nTimerID);
	void	Resize(int width, int height);
	void	SetTransparent(bool transparent);
	const   CString& GetUrl()const;
	bool    CanGoBack() const;
	bool    GoBack();
	bool    CanGoForward() const;
	bool    GoForward();
	void    StopLoad();
	void    Reload();
	wkeWebView  GetWebView();
	void    LoadURL(LPCTSTR lpUrl);
	void    LoadFile(LPCTSTR lpFile);
	void    LoadHtml(LPCTSTR lpHtml);
	void	ShowContent(bool bShow);

	CString RunJS(CString strValue);
	jsExecState GetGlobalExec();
	wkeWebView GetWkeWebview() { return m_pWebView; }

	virtual void onBufUpdated(const HDC hdc, int x, int y, int cx, int cy) override;
	virtual void onTitleChanged(const wkeString title) override;
	virtual void onURLChanged(const wkeString url) override;
	virtual void onLoaded() override;
	virtual void onLoadFailed() override;
	virtual void onDocumentReady() override;

	static int CheckHttpErrorCode(const CString title);

	void AddJsExternalObj();

private:
	CString     m_strUrl;
	wkeWebView  m_pWebView;
	UINT		m_nTimerID;
	bool		m_bShowWebContent;
};

#define DUI_MSGTYPE_WKEJSCALL			(_T("wkejscall"))
#define DUI_MSGTYPE_TITLECHANGED		(_T("titlechanged"))
#define DUI_MSGTYPE_URLCHANGED			(_T("urlchanged"))
#define DUI_MSGTYPE_LOADED				(_T("loaded"))
#define DUI_MSGTYPE_LOADFAILED			(_T("loadefailed"))
#define DUI_MSGTYPE_DOCUMENTREADY		(_T("documentready"))

void WkeControl_Init();
void WkeControl_Shutdown();



