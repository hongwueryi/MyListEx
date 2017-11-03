#include "StdAfx.h"  
#include "UIWkeWindowlessBrowser.h"  

#define NATIVE_METHOD_CLOSE_WINDOW		_T("close_window")
#define NATIVE_METHOD_MINIMIZE_WINDOW	_T("minimize_window")
#define NATIVE_METHOD_MOVE_WINDOW		_T("move_window")
#define NATIVE_METHOD_ALERT				_T("alert")

// 以下变量用于移动窗口;
static POINT ptmove, pemove;
static RECT rtmove, rtemove;

CWkeWindowlessUI::CWkeWindowlessUI(void)  
: m_pWebView(NULL)  
, m_nTimerID(-1)
, m_bShowWebContent(true)
{  
    m_pWebView = wkeCreateWebView(); 
	m_pWebView->setUserData(this);
}  
  
CWkeWindowlessUI::~CWkeWindowlessUI(void)  
{  
	m_pManager->KillTimer(this);
    m_pWebView->destroy();  
}  
  
LPCTSTR CWkeWindowlessUI::GetClass() const  
{  
    return L"WkeWindowlessUI";  
}  
  
LPVOID CWkeWindowlessUI::GetInterface( LPCTSTR pstrName )  
{  
    if( _tcscmp(pstrName, _T("WkeWindowlessUI")) == 0 )   
        return this;  
    return CControlUI::GetInterface(pstrName);  
}

POINT CWkeWindowlessUI::GetMouseClientPos()
{
	POINT pt; ::GetCursorPos(&pt);
	::ScreenToClient(m_pManager->GetPaintWindow(), &pt);

	return pt;
}

bool CWkeWindowlessUI::IsMouseInThisCtrl(POINT pt)
{
	CControlUI* pCtrl = GetManager()->FindControl(pt);
	 return pCtrl == this;
}

LRESULT CWkeWindowlessUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	// wke 对大多数消息的处理应该是对外面不产生任何影响的;
	bHandled = false;

	// 只处理在当控件的消息;
	POINT pt = GetMouseClientPos();

	switch (uMsg)
	{
	case WM_GESTURENOTIFY:
	{
		DUI__Trace(_T("手势消息：%s"), DUI__TraceMsg(uMsg));
		break;
	}
	case WM_GESTURE:
	{
		GESTUREINFO gi;
		gi.cbSize = sizeof(gi);
		BOOL bResult = GetGestureInfo((HGESTUREINFO) lParam, &gi);

		DUI__Trace(_T("手势消息：%s, ID:%d, flag:%d."), DUI__TraceMsg(uMsg), gi.dwID, gi.dwFlags);

		POINT gestureScreenPoint;
		POINT gesturePoint;
		gesturePoint.x = gi.ptsLocation.x;
		gesturePoint.y = gi.ptsLocation.y;
		gestureScreenPoint.x = gi.ptsLocation.x;
		gestureScreenPoint.y = gi.ptsLocation.y;
		ScreenToClient(m_pManager->GetPaintWindow(), &gesturePoint);

		static bool gestureTypeMove = false;

		CString js;
		js.Format(_T("wkeGestureMove(%d,%d);"), gesturePoint.x, gesturePoint.y);
		CString jsRet = RunJS(js);
		
		if (GID_PAN == gi.dwID && GF_BEGIN == gi.dwFlags)
		{
			gestureTypeMove = jsRet == _T("1") ? true : false;
		}
		if (GID_PAN == gi.dwID && GF_END == gi.dwFlags)
		{
			gestureTypeMove = false;
		}
		
		if (gestureTypeMove) // 如果支持窗口移动;
		{
			if (GID_PAN == gi.dwID)
			{
				if (GF_BEGIN == gi.dwFlags)
				{
					ptmove = gestureScreenPoint;
					GetWindowRect(GetManager()->GetPaintWindow(), &rtmove);
					rtemove.right = rtmove.right - rtmove.left;
					rtemove.bottom = rtmove.bottom - rtmove.top;
				}
				else if (GF_END == gi.dwFlags)
				{
					ptmove = { 0,0 };
				}
				else
				{
					HWND hwnd = m_pManager->GetPaintWindow();
					if ( hwnd != NULL && IsWindow(hwnd) && ptmove.x > 0 && ptmove.y > 0 && IsMouseInThisCtrl(gestureScreenPoint))
					{
						pemove = gestureScreenPoint;
						rtemove.left = rtmove.left + (pemove.x - ptmove.x);
						rtemove.top = rtmove.top + (pemove.y - ptmove.y);
						MoveWindow(hwnd, rtemove.left, rtemove.top, rtemove.right, rtemove.bottom, TRUE);
					}
				}
			}
		}
		else
		{
			unsigned int flags = 0;

			if (wParam & MK_CONTROL)	flags |= WKE_CONTROL;
			if (wParam & MK_SHIFT)		flags |= WKE_SHIFT;
			if (wParam & MK_LBUTTON)	flags |= WKE_LBUTTON;
			if (wParam & MK_MBUTTON)	flags |= WKE_MBUTTON;
			if (wParam & MK_RBUTTON)	flags |= WKE_RBUTTON;

			bool handled = m_pWebView->gesture(gi.dwID, gi.dwFlags, gesturePoint, gestureScreenPoint, flags);
		}

		CloseGestureInfoHandle((HGESTUREINFO) lParam);

		bHandled = true;
		break;
	}
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	{
		if (uMsg == WM_LBUTTONUP && ::PtInRect(&m_rcItem, pt) && IsEnabled()) {
			Invalidate();
			if (CControlUI::Activate() && m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		}

		ptmove = { 0,0 };

		// 这里不能有break;
	}
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		HWND hwnd = m_pManager->GetPaintWindow();
		if (uMsg == WM_MOUSEMOVE && hwnd != NULL && IsWindow(hwnd) && ptmove.x > 0 && ptmove.y > 0 && IsMouseInThisCtrl(pt))
		{
			GetCursorPos(&pemove);
			if (wParam == MK_LBUTTON)
			{
				rtemove.left = rtmove.left + (pemove.x - ptmove.x);
				rtemove.top = rtmove.top + (pemove.y - ptmove.y);
				//DUI__Trace(_T("POINT:%d-- %d"), rtemove.left, rtemove.top);
				MoveWindow(hwnd, rtemove.left, rtemove.top, rtemove.right, rtemove.bottom, TRUE);
			}
		}

		// 注意这里不能有break;
	}
	//case WM_MOUSEHOVER: wke不识别horver，会造成assert;
	case WM_MOUSELEAVE:// wke demo里面并没有处理 hover和leave;
	{
		if (IsMouseInThisCtrl(pt))
		{
			DUI__Trace(_T("鼠标消息：%s"), DUI__TraceMsg(uMsg));

			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			unsigned int flags = 0;

			if (wParam & MK_CONTROL)	flags |= WKE_CONTROL;
			if (wParam & MK_SHIFT)		flags |= WKE_SHIFT;
			if (wParam & MK_LBUTTON)	flags |= WKE_LBUTTON;
			if (wParam & MK_MBUTTON)	flags |= WKE_MBUTTON;
			if (wParam & MK_RBUTTON)	flags |= WKE_RBUTTON;

			x -= m_rcItem.left;
			y -= m_rcItem.top;

			bool handled = m_pWebView->mouseEvent(uMsg, x, y, flags);
		}

		break;
	}
	case WM_MOUSEWHEEL:
	{
		//POINT pt;
		//pt.x = GET_X_LPARAM(lParam);
		//pt.y = GET_Y_LPARAM(lParam);
		//ScreenToClient(m_pManager->GetPaintWindow(), &pt);

		if (IsMouseInThisCtrl(pt))
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);

			DUI__Trace(_T("MouseWheel delta:%d."), delta);

			unsigned int flags = 0;

			if (wParam & MK_CONTROL)	flags |= WKE_CONTROL;
			if (wParam & MK_SHIFT)		flags |= WKE_SHIFT;
			if (wParam & MK_LBUTTON)	flags |= WKE_LBUTTON;
			if (wParam & MK_MBUTTON)	flags |= WKE_MBUTTON;
			if (wParam & MK_RBUTTON)	flags |= WKE_RBUTTON;

			bool handled = m_pWebView->mouseWheel(pt.x, pt.y, delta, flags);
		}

		break;
	}
	case WM_KEYDOWN:
	{
		unsigned int virtualKeyCode = wParam;

		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)		flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)	flags |= WKE_EXTENDED;

		bool handled = m_pWebView->keyDown(virtualKeyCode, flags, false);

		break;
	}
	case WM_KEYUP:
	{
		unsigned int virtualKeyCode = wParam;

		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)		flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)	flags |= WKE_EXTENDED;

		bool handled = m_pWebView->keyUp(virtualKeyCode, flags, false);

		break;
	}
	case WM_CHAR:
	{
		unsigned int charCode = wParam;

		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)		flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)	flags |= WKE_EXTENDED;

		bool handled = m_pWebView->keyPress(charCode, flags, false);

		break;
	}
	case WM_IME_STARTCOMPOSITION:
	{
		wkeRect caret = m_pWebView->getCaret();

		RECT rcClient;
		GetClientRect(m_pManager->GetPaintWindow(), &rcClient);

		CANDIDATEFORM form;
		form.dwIndex = 0;
		form.dwStyle = CFS_EXCLUDE;
		form.ptCurrentPos.x = m_rcItem.left + caret.x;
		form.ptCurrentPos.y = m_rcItem.top + caret.y/* + caret.h*/;
		form.rcArea.top = m_rcItem.top + caret.y/* + rcClient.top*/;
		form.rcArea.bottom = m_rcItem.top + caret.y + caret.h/* + rcClient.top*/;
		form.rcArea.left = m_rcItem.left + caret.x/* + rcClient.left*/;
		form.rcArea.right = m_rcItem.left + caret.x + caret.w/* + rcClient.left*/;
		COMPOSITIONFORM compForm;
		compForm.ptCurrentPos = form.ptCurrentPos;
		compForm.rcArea = form.rcArea;
		compForm.dwStyle = CFS_POINT;

		HWND hWnd = m_pManager->GetPaintWindow();
		HIMC hIMC = ImmGetContext(hWnd);
		ImmSetCandidateWindow(hIMC, &form);
		ImmSetCompositionWindow(hIMC, &compForm);
		ImmReleaseContext(hWnd, hIMC);

		break;
	}
	case WM_CONTEXTMENU:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (pt.x != -1 && pt.y != -1) ScreenToClient(m_pManager->GetPaintWindow(), &pt);

		unsigned int flags = 0;

		if (wParam & MK_CONTROL)	flags |= WKE_CONTROL;
		if (wParam & MK_SHIFT)		flags |= WKE_SHIFT;
		if (wParam & MK_LBUTTON)	flags |= WKE_LBUTTON;
		if (wParam & MK_MBUTTON)	flags |= WKE_MBUTTON;
		if (wParam & MK_RBUTTON)	flags |= WKE_RBUTTON;

		bool handled = m_pWebView->contextMenuEvent(pt.x, pt.y, flags);

		break;
	}
	default:
		break;
	}

	return 0;
}

void CWkeWindowlessUI::DoEvent( TEventUI& event )  
{
	bool handled = true;
	if (!m_pWebView) return CControlUI::DoEvent(event);

	if (event.Type == UIEVENT_TIMER)
	{
		if (event.wParam == m_nTimerID)
		{
			m_pWebView->tick();
		}
	}
	// wke控件作为duilib窗口的控件之一，需要用duilib的机制处理焦点问题，以便在同一个窗口中的多个控件中切换焦点;
	else if (event.Type == UIEVENT_SETFOCUS)
	{
		m_pWebView->focus();
	}
	else if (event.Type == UIEVENT_KILLFOCUS)
	{
		m_pWebView->unfocus();
	}
	//else if (event.Type == UIEVENT_SETCURSOR)
	//{
	//	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	//}

	//if (!handled)
	{
		CControlUI::DoEvent(event);
	}
}  
  
void CWkeWindowlessUI::LoadURL( LPCTSTR lpUrl )
{  
	if (!m_pWebView) return;

	m_strUrl = lpUrl;
	m_pWebView->loadURL(lpUrl);
}  
  
void CWkeWindowlessUI::SetPos( RECT rc, bool bNeedInvalidate)
{  
    CControlUI::SetPos(rc, bNeedInvalidate);  
    if ( m_pWebView )  
        m_pWebView->resize(rc.right-rc.left, rc.bottom-rc.top);  
}  
  
void CWkeWindowlessUI::DoInit()  
{  
	memset(&ptmove, 0, sizeof(ptmove));
	memset(&pemove, 0, sizeof(pemove));
	memset(&rtmove, 0, sizeof(rtmove));
	memset(&rtemove, 0, sizeof(rtemove));

	m_pManager->AddMessageFilter(this);

    if ( !m_strUrl.IsEmpty() )  
		LoadURL(m_strUrl.GetBuffer());
}  
  
void CWkeWindowlessUI::PaintBkImage( HDC hDC )  
{  
	if (m_bShowWebContent)
	{
		CControlUI::PaintStatusImage(hDC);
		CDuiRect rect(m_rcItem);

		CDuiRect rcInvalid = rect;
		CDuiRect rcClient = rect;

		BitBlt(hDC, rcInvalid.left, rcInvalid.top,
			rcInvalid.right - rcInvalid.left, rcInvalid.bottom - rcInvalid.top,
			m_pWebView->getViewDC(), rcInvalid.left - rcClient.left, rcInvalid.top - rcClient.top, SRCCOPY);
	}
	else
	{
		int x = m_rcItem.left;
		int y = m_rcItem.top;
		int cx = m_rcItem.right - m_rcItem.left;
		int cy = m_rcItem.bottom - m_rcItem.top;

		HDC hdcMem = CreateCompatibleDC(hDC);
		HBITMAP hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		SelectObject(hdcMem, hBitmap);

		//PTSTR pText = TEXT("hello, world");
		//TextOut(hdcMem, 200, 100, pText, lstrlen(pText));

		BitBlt(hDC, x, x, cx, cy, hdcMem, 0, 0, SRCCOPY);

		DeleteDC(hdcMem);
		DeleteObject(hBitmap);
	}
}  
  
bool CWkeWindowlessUI::CanGoBack() const  
{  
    return m_pWebView?m_pWebView->canGoBack():false;  
}  
  
bool CWkeWindowlessUI::GoBack()  
{  
    return m_pWebView?m_pWebView->goBack():false;  
}  
  
bool CWkeWindowlessUI::CanGoForward() const  
{  
    return m_pWebView?m_pWebView->canGoForward():false;  
}  
  
bool CWkeWindowlessUI::GoForward()  
{  
    return m_pWebView?m_pWebView->goForward():false;  
}  
  
void CWkeWindowlessUI::StopLoad()  
{  
    if ( m_pWebView )  
        m_pWebView->stopLoading();  
}  
  
void CWkeWindowlessUI::Reload()  
{  
    if ( m_pWebView )  
    {  
        m_pWebView->reload();  
    }  
}  
  
wkeWebView CWkeWindowlessUI::GetWebView()  
{  
    return m_pWebView;  
}  
  
void CWkeWindowlessUI::LoadFile( LPCTSTR lpFile )  
{
	if (m_pWebView)
		m_pWebView->loadFile(lpFile);
}  
  
void CWkeWindowlessUI::LoadHtml( LPCTSTR lpHtml )  
{  
    if ( m_pWebView )  
        m_pWebView->loadHTML(lpHtml);  
}  

void CWkeWindowlessUI::ShowContent(bool bShow)
{
	m_bShowWebContent = bShow;

	//if (bShow) return;

	//int x = m_rcItem.left;
	//int y = m_rcItem.top;
	//int cx = m_rcItem.right - m_rcItem.left;
	//int cy = m_rcItem.bottom - m_rcItem.top;

	//HDC hDC = m_pManager->GetPaintDC();
	//HDC hdcMem = CreateCompatibleDC(hDC);
	//HBITMAP hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
	//SelectObject(hdcMem, hBitmap);

	//BitBlt(hDC, x, x, cx, cy, hdcMem, 0, 0, SRCCOPY);

	//DeleteDC(hdcMem);
	//DeleteObject(hBitmap);
}

CString CWkeWindowlessUI::RunJS(CString strValue)
{
	jsValue jsRet = m_pWebView->runJS(strValue.GetBuffer());
	return jsToStringW(m_pWebView->globalExec(), jsRet);
}
  
jsExecState CWkeWindowlessUI::GetGlobalExec()  
{  
    if ( m_pWebView )  
        return m_pWebView->globalExec();  
    return NULL;  
}  

void CWkeWindowlessUI::onBufUpdated(const HDC hdc, int x, int y, int cx, int cy)
{
	//RECT rcValide = { x, y, x + cx, y + cy };
	//::OffsetRect(&rcValide, m_rcItem.left, m_rcItem.top);
	//HWND hWnd = m_pManager->GetPaintWindow();
	//::InvalidateRect(hWnd, &rcValide, TRUE);

	this->Invalidate();
}

void CWkeWindowlessUI::onTitleChanged(const wkeString title)
{
	GetManager()->SendNotify(this, DUI_MSGTYPE_TITLECHANGED, (WPARAM) wkeToStringW(title), (LPARAM) 0);
}

void CWkeWindowlessUI::onURLChanged(const wkeString url)
{
	GetManager()->SendNotify(this, DUI_MSGTYPE_URLCHANGED, (WPARAM) wkeToStringW(url), (LPARAM) 0);
}

void CWkeWindowlessUI::onLoaded()
{
	AddJsExternalObj();
	GetManager()->SendNotify(this, DUI_MSGTYPE_LOADED, (WPARAM)0, (LPARAM) 0);
}

void CWkeWindowlessUI::onLoadFailed()
{
	GetManager()->SendNotify(this, DUI_MSGTYPE_LOADFAILED, (WPARAM) 0, (LPARAM) 0);
}

void CWkeWindowlessUI::onDocumentReady()
{
	GetManager()->SendNotify(this, DUI_MSGTYPE_DOCUMENTREADY, (WPARAM) 0, (LPARAM) 0);
}

void CWkeWindowlessUI::Init(UINT nTimerID)
{
	m_nTimerID = nTimerID;
	m_pManager->SetTimer(this, m_nTimerID, 3000);
	m_pWebView->setHandler(this);
}

void CWkeWindowlessUI::Resize(int width, int height)
{
	m_pWebView->resize(width, height);
}

void CWkeWindowlessUI::SetTransparent(bool transparent)
{
	m_pWebView->setTransparent(transparent);
}
  
const CString& CWkeWindowlessUI::GetUrl() const  
{  
    return m_strUrl;  
}  
  
void CWkeWindowlessUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )  
{  
    if ( _tcscmp(pstrName, _T("url")) == 0 )  
        m_strUrl = pstrValue;  
    else  
        CControlUI::SetAttribute(pstrName, pstrValue);  
}  

int CWkeWindowlessUI::CheckHttpErrorCode(const CString title)
{
	if (title.Find(_T("404 Not Found")) != -1) return 404;
	if (title.Find(_T("500 Internal Server Error")) != -1) return 500;

	return 0;
}

static jsValue JS_CALL NativeMethod(jsExecState es)
{
	//调用需要两个参数 第一个参数为方法名， 第二个为参数（多个参数可以将参数定义为json）;
	const wchar_t* method = jsToStringW(es, jsArg(es, 0));
	const wchar_t* params = jsToStringW(es, jsArg(es, 1));

	// 取得wke控件;
	wkeWebView  view = jsGetWebView(es);
	CWkeWindowlessUI* wkeControl = (CWkeWindowlessUI*)view->getUserData();
	if(!wkeControl) return jsUndefined();
	
	// 控件方法调用;
	CString strMethod = method;
	if (strMethod == NATIVE_METHOD_CLOSE_WINDOW)
	{
		::PostMessage(wkeControl->GetManager()->GetPaintWindow(), WM_CLOSE, 0, 0);
		return jsUndefined();
	}
	else if (strMethod == NATIVE_METHOD_MINIMIZE_WINDOW)
	{
		::PostMessage(wkeControl->GetManager()->GetPaintWindow(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
		return jsUndefined();
	}
	else if (strMethod == NATIVE_METHOD_MOVE_WINDOW)
	{
		GetCursorPos(&ptmove);
		GetWindowRect(wkeControl->GetManager()->GetPaintWindow(), &rtmove);
		rtemove.right = rtmove.right - rtmove.left;
		rtemove.bottom = rtmove.bottom - rtmove.top;
		return jsUndefined();
	}
	else if (strMethod == NATIVE_METHOD_ALERT)
	{
		::MessageBox(wkeControl->GetManager()->GetPaintWindow(), params, TEXT("Alert"), MB_ICONINFORMATION | MB_OK);
		return jsUndefined();
	}

	// 通用方法调用;
	TNativeMethodInfo* pInfo = new TNativeMethodInfo();
	pInfo->name = method;
	pInfo->params = params;
	wkeControl->GetManager()->SendNotify(wkeControl, DUI_MSGTYPE_WKEJSCALL, (WPARAM) pInfo, (LPARAM) 0);
	jsValue ret = jsStringW(es, pInfo->ret.GetBuffer());
	delete pInfo; pInfo = NULL;

	return ret;
}

void CWkeWindowlessUI::AddJsExternalObj()
{
	CString jsFuns = _T("															\
			var funWkeGestureMove =  function(x, y) {								\
					var ele = document.elementFromPoint(x,y);						\
					if(ele && ele.attributes['wkegesturemove'] && ele.attributes['wkegesturemove'].nodeValue == 'true') return '1';								\
					else return '0';												\
			};																		\
			var funLidLogin =  function() {											\
					CallNativeMethod('login', '');									\
			};																		\
			var funShowPrivilege = function() {										\
					CallNativeMethod('start_privilege', '');						\
			};																		\
			var funShowfeedback = function() {										\
					CallNativeMethod('start_bbs', '-answer');						\
			};																		\
			var funShowotoo = function() {											\
					CallNativeMethod('start_web_page_for_xx', '');					\
			};																		\
			var funAlert = function(msg){											\
					CallNativeMethod('alert', msg);									\
			};																		\
			var funOpen = function (url) {											\
					CallNativeMethod('open_url', url);								\
			};																		\
			if(!window.external)													\
			{																		\
				window.external = new Object() ;									\
				window.external.lidLogin = funLidLogin;								\
				window.external.showPrivilege = funShowPrivilege;					\
				window.external.showfeedback = funShowfeedback;						\
				window.external.showotoo = funShowotoo;								\
				window.wkeGestureMove = funWkeGestureMove;							\
				window.alert = funAlert;											\
				window.open = funOpen;												\
				window.external.getctstoken = function() {							\
					CallNativeMethod('getctstoken', '');							\
				};																	\
			}																		\
			var obj = document.querySelectorAll('iframe');							\
			for (var i = 0; i < obj.length; i++) {									\
				if(!obj[i].contentWindow.external)									\
				{																	\
					obj[i].contentWindow.external = new Object() ;					\
					obj[i].contentWindow.external.lidLogin = funLidLogin;			\
					obj[i].contentWindow.external.showPrivilege = funShowPrivilege;	\
					obj[i].contentWindow.external.showfeedback = funShowfeedback;	\
					obj[i].contentWindow.external.showotoo = funShowotoo;			\
					obj[i].contentWindow.alert = funAlert;							\
					obj[i].contentWindow.open = funOpen;							\
				}																	\
			}																		\
		");

	RunJS(jsFuns);
}

CControlUI* WkeCreateWndlessControl(LPCTSTR pstrClass) {

	if (_tcsicmp(pstrClass, _T("WkeWindowlessUI")) == 0)
	{
		CWkeWindowlessUI* wkeControl = new CWkeWindowlessUI();

		//wkeControl->AddJsExternalObj();

		return wkeControl;
	}
	return NULL;
}

void WkeControl_Init()
{
	wkeInit();
	jsBindFunction("CallNativeMethod", NativeMethod, 2); 
	CPaintManagerUI::GetPlugins()->Add(WkeCreateWndlessControl);
}

void WkeControl_Shutdown()
{
	wkeShutdown();
}
