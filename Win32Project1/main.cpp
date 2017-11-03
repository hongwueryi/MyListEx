#include<windows.h>
#include "mainframe.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	MainFrameWnd* pFrame = new MainFrameWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("LenovoDuilib½çÃæ¿ò¼Ü"),
		UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 500, 300);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOWNORMAL);
	CPaintManagerUI::MessageLoop();

	return 0;
}