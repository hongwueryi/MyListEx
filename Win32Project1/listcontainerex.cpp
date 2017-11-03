#include "listcontainerex.h"

CListContainerElementUIEx::CListContainerElementUIEx()
{

}

void CListContainerElementUIEx::Init()
{
    InitListItem();

}

void CListContainerElementUIEx::InitListItem()
{
    pDelItemBtn = static_cast<CButtonUI*>(this->FindSubControl(_T("deleteItemEx")));
    if (pDelItemBtn)
    {
        pDelItemBtn->OnEvent += MakeDelegate(this, &CListContainerElementUIEx::OnBtnEvent);
    }

    pAlertBtn = static_cast<CButtonUI*>(this->FindSubControl(_T("alertbtnEx")));
    if (pAlertBtn)
    {
        pAlertBtn->OnEvent += MakeDelegate(this, &CListContainerElementUIEx::OnBtnEvent);
    }

    pEdit = static_cast<CEditUI*>(this->FindSubControl(_T("doedit")));
    if (pEdit)
    {
        pEdit->SetText(msginfo_);
    }
}

CListContainerElementUIEx* CListContainerElementUIEx::CreateListContainerEx(
    CListUIEx* pListEx, CString info)
{
    CDialogBuilder builder;
    CListContainerElementUIEx*pListContainerEx = nullptr;
    pListContainerEx = static_cast<CListContainerElementUIEx*>(
        builder.Create(_T("listItem.xml"), 0, pListEx, pListEx->GetManager()));

    if (pListContainerEx)
    {
        pListContainerEx->pListEx_ = pListEx;
        pListContainerEx->msginfo_ = info;
    }

    return pListContainerEx;
}

bool CListContainerElementUIEx::OnBtnEvent(LPVOID param)
{
    TEventUI* pMsg = (TEventUI*)param;

    if (pMsg->Type == UIEVENT_BUTTONUP)
    {
        if (pMsg->pSender == pAlertBtn)
        {            
            MessageBox(NULL, msginfo_, _T("込込込"), MB_OK);
        }

        if (pMsg->pSender == pDelItemBtn)
        {
            //MessageBox(NULL, msginfo_, _T("込込込"), MB_OK);
            pListEx_->Remove(this);
        }
    }

    return true;
}

