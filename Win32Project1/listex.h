#pragma once
#include "stdafx.h"
//�����Ƕ��б����չ��ʵ���Զ���List

class CListUIEx :public CListUI, public IDialogBuilderCallback
{
public:
    CListUIEx();
    ~CListUIEx(){}

    virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;
    void updateList(vector<CString> SearchListItemInfo);
};