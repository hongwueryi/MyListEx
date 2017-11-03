#pragma once
#include "stdafx.h"
#include <atlstr.h>
#include "listex.h"
//此类是对列表行的扩展，实现自定义ListContainerElemen

class CListContainerElementUIEx : public CListContainerElementUI
{
public:
    friend class CControlUI;
    CListContainerElementUIEx();
    ~CListContainerElementUIEx() {}

    virtual void Init();
    bool OnBtnEvent(LPVOID param);
    void InitListItem();
    static CListContainerElementUIEx* CreateListContainerEx(CListUIEx* pListEx, CString info);
 
protected:
    CListUIEx* pListEx_;
    CString msginfo_;

private:
    CButtonUI* pAlertBtn;
    CButtonUI* pDelItemBtn;
    CEditUI* pEdit;
};