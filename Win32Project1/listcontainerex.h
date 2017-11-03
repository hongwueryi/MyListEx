#pragma once
#include "stdafx.h"
#include <atlstr.h>
#include "listex.h"
//�����Ƕ��б��е���չ��ʵ���Զ���ListContainerElemen

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