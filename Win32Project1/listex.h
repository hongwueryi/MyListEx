#pragma once
#include "stdafx.h"
//此类是对列表的扩展，实现自定义List

class CListUIEx :public CListUI, public IDialogBuilderCallback
{
public:
    CListUIEx();
    ~CListUIEx(){}

    virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;
    void updateList(vector<CString> SearchListItemInfo);
};