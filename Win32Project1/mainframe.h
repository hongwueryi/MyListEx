#pragma once
#include "listex.h"
//#pragma comment(lib, "DuiLib.lib")
//#pragma comment(lib, "DuilibWke.lib")


class MainFrameWnd : public WindowImplBase
{
public:
	MainFrameWnd();
	~MainFrameWnd();

public:
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
    bool OnBtnEvent(LPVOID param);
    void RefreshListEx(vector<CString>vec);

private:
    CButtonUI* pBtnAddItem;
    CButtonUI* pDelItemBtn;
    CListUIEx* pList_;  
};