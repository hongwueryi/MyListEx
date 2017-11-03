#include "mainframe.h"
#include "..\Duilib\Utils\UIDelegate.h"


MainFrameWnd::MainFrameWnd()
{

}

MainFrameWnd::~MainFrameWnd()
{

}

void MainFrameWnd::OnFinalMessage(HWND hWnd)
{

}


bool MainFrameWnd::OnBtnEvent(LPVOID param)
{
    TEventUI* pMsg = (TEventUI*)param;

    if (pMsg->Type == UIEVENT_BUTTONUP)
    {
        if (pMsg->pSender == pDelItemBtn)
        {
            MessageBox(NULL, 0, 0, MB_OK);
        }
    }
    return true;
}

void MainFrameWnd::InitWindow()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_MAXIMIZEBOX;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

    pList_ = static_cast<CListUIEx*>(m_PaintManager.FindControl(_T("listview")));
    pBtnAddItem = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnAddListItem")));
	if (nullptr == pBtnAddItem || nullptr == pList_)
		PostQuitMessage(0);
    
    vector<CString> vec;
    vec.push_back(L"联想电脑管家");
    vec.push_back(L"联想软件商店");
    vec.push_back(L"爱奇艺");
    vec.push_back(L"王者农药");
    vec.push_back(L"英雄联盟");
    RefreshListEx(vec);
    
}

CDuiString MainFrameWnd::GetSkinFolder()
{
	return _T("skin");
}

CDuiString MainFrameWnd::GetSkinFile()
{
	return _T("mainframe.xml");
}

LPCTSTR MainFrameWnd::GetWindowClassName() const
{
	return _T("UILevonoDtest");
}

UILIB_RESOURCETYPE MainFrameWnd::GetResourceType() const
{
	return UILIB_FILE;
}

CControlUI* MainFrameWnd::CreateControl(LPCTSTR pstrClass)
{
    if (_tcsicmp(pstrClass, _T("ListEx")) == 0)
    {
        return new CListUIEx();
    }
	return NULL;
}

LRESULT MainFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

void MainFrameWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{        
		OnClick(msg);
	}
}

void MainFrameWnd::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("btnclose"))
	{       
		PostQuitMessage(0);
	}
    else if (msg.pSender->GetName() == _T("btnAddListItem"))
    {
        vector<CString> vec;
        //vec.push_back(L"网易云音乐");
        //vec.push_back(L"欢乐斗地主");
        vec.push_back(L"QQ");
        vec.push_back(L"微信");
        //vec.push_back(L"有道云笔记");
        RefreshListEx(vec);
    }
}

void MainFrameWnd::RefreshListEx(vector<CString>vec)
{
    if (vec.empty())
    {
        return;
    }
   
    if (pList_)
    {
        pList_->updateList(vec);
    }
}


