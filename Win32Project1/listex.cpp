#include "listex.h"
#include "listcontainerex.h"


CListUIEx::CListUIEx()
{

}

void CListUIEx::updateList(vector<CString> SearchListItemInfo)
{
    if (SearchListItemInfo.empty())
    {
        return;
    }

    vector<CString>::iterator it;
    for (it = SearchListItemInfo.begin(); it != SearchListItemInfo.end(); ++it)
    {
        CListContainerElementUIEx* pListContainerEx =
            CListContainerElementUIEx::CreateListContainerEx(this, *it);

        if (pListContainerEx)
        {
            pListContainerEx->SetFixedHeight(80);
            this->Add(pListContainerEx);
        }
    }
}

CControlUI* CListUIEx::CreateControl(LPCTSTR pstrClass)
{
    if (_tcsicmp(pstrClass, _T("ListContainerElementEx")) == 0)
    {
        return new CListContainerElementUIEx;
    }

    return NULL;
}
