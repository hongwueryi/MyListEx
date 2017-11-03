#include "stdafx.h"
#include "UIAnimationOption.h"

#define  ANIMATIONOPTION_PICTURE_HOVER_TIMER_ID 902
#define  ANIMATIONOPTION_PICTURE_WORKING_TIMER_ID 903

int DuiLib::CAnimationOptionUI::INVALID_INDEX = -1;
int DuiLib::CAnimationOptionUI::INVALID_INTERVAL = 0;

DuiLib::CAnimationOptionUI::CAnimationOptionUI()
{
}


DuiLib::CAnimationOptionUI::~CAnimationOptionUI()
{
}

void DuiLib::CAnimationOptionUI::DoEvent(TEventUI & event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
		if (m_pParent != NULL) m_pParent->DoEvent(event);
		else CLabelUI::DoEvent(event);
		return;
	}

	if (!IsVisible())
	{
		return;
	}

	switch (event.Type)
	{
	case UIEVENT_TIMER:
	{
		AnimationType type = AnimationType_Unknown;
		if (event.wParam == ANIMATIONOPTION_PICTURE_HOVER_TIMER_ID)
			type = AnimationType_Hover;
		else if (event.wParam == ANIMATIONOPTION_PICTURE_WORKING_TIMER_ID)
		{
			type = AnimationType_Working;
			if ((m_uLogicalState & LogicalState_Working) == 0)
			{
				StopAnim(AnimationType_Working);
				return;
			}
		}
		if ((type & AnimationType_Mask) != 0)
		{
			drawAnimationPicture(type);
		}
		break;
	}
	case UIEVENT_MOUSEENTER:
	{
		if (IsEnabled()) {
			m_uButtonState |= UISTATE_HOT;
			m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER, event.wParam, event.lParam);
			if ((m_animState | AnimationState_None) == 0)
			{
				StartAnim(AnimationType_Hover);
			}
			else
			{
				Invalidate();
			}
		}
		break;
	}
	case UIEVENT_MOUSELEAVE:
	{
		if (IsEnabled()) {
			m_uButtonState &= ~UISTATE_HOT;
			m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE, event.wParam, event.lParam);
			Invalidate();
		}
		StopAnim(AnimationType_Hover);
		if ((m_animState | AnimationState_None) == 0)
		{
			SetForeImage(L"");
		}
		break;
	}
	case UIEVENT_BUTTONUP:
	{
		if ((m_uButtonState & UISTATE_CAPTURED) != 0)
		{
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
			if (::PtInRect(&m_rcItem, event.ptMouse))
			{
				if ((m_animState & AnimationState_Working) != 0)
				{
					SetForeImage(m_diSelected.sDrawString);
					StopAnim(AnimationType_Working);
				}
				Activate();
			}
		}
		break;
	}
	default:
		CButtonUI::DoEvent(event);
		break;
	}
	
}

LPVOID DuiLib::CAnimationOptionUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, DUI_CTR_ANIMATIONOPTION) == 0)
		return static_cast<CAnimationOptionUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void DuiLib::CAnimationOptionUI::Selected(bool bSelected, BOOL fireEvent)
{
	if (!bSelected)
	{
		if ((m_uLogicalState & LogicalState_Working) != 0 && (m_animState & AnimationState_Working) == 0)
		{
			StartAnim(AnimationType_Working);
		}
	}
	__super::Selected(bSelected, fireEvent);
}

void DuiLib::CAnimationOptionUI::StartAnim(AnimationType type)
{
	UINT nTimerID = 0;
	if (type == AnimationType_Hover)
	{
		m_animState |= AnimationState_Hover;
		nTimerID = ANIMATIONOPTION_PICTURE_HOVER_TIMER_ID;
	}
	else if (type == AnimationType_Working)
	{
		if ((m_uLogicalState & LogicalState_Working) != 0)
		{
			m_animState |= AnimationState_Working;
			nTimerID = ANIMATIONOPTION_PICTURE_WORKING_TIMER_ID;
		}		
	}		

	int nInterval = getInverval(type);

	if (nInterval != INVALID_INTERVAL)
	{
		setCurrentIndex(type, getStartIndex(type));
		m_diFore.sDrawString = getPaintPicture(type, getStartIndex(type));
		GetManager()->SetTimer(this, nTimerID, nInterval);
	}
	else
	{
		assert(!L"interval value is not right!");
	}	
}

void DuiLib::CAnimationOptionUI::StopAnim(AnimationType type/* = AnimationType_Mask*/)
{
	m_animState &= ~type;
	if ((type & AnimationType_Hover) != 0)
	{
		GetManager()->KillTimer(this, ANIMATIONOPTION_PICTURE_HOVER_TIMER_ID);
	}
	if ((type & AnimationType_Working) != 0)
	{
		GetManager()->KillTimer(this, ANIMATIONOPTION_PICTURE_WORKING_TIMER_ID);
	}
	SetForeImage(L"");
}

void DuiLib::CAnimationOptionUI::SetLogicalState(LogicalState state, bool bSet/* = true*/)
{
	if (bSet)
	{
		m_uLogicalState |= state;
	}
	else
	{
		m_uLogicalState &= ~state;
	}
}

void DuiLib::CAnimationOptionUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("startindexhover")) == 0) {
		m_mapAnimationStartEndIndex[AnimationType_Hover].first = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("endindexhover")) == 0) {
		m_mapAnimationStartEndIndex[AnimationType_Hover].second = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("startindexworking")) == 0) {
		m_mapAnimationStartEndIndex[AnimationType_Working].first = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("endindexworking")) == 0) {
		m_mapAnimationStartEndIndex[AnimationType_Working].second = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("picturehover")) == 0) {
		m_mapAnimationPicture[AnimationType_Hover] = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("pictureworking")) == 0) {
		m_mapAnimationPicture[AnimationType_Working] = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("intervalhover")) == 0) {
		m_mapAnimationInterval[AnimationType_Hover] = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("intervalworking")) == 0) {
		m_mapAnimationInterval[AnimationType_Working] = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("loophover")) == 0) {
		m_mapAnimationLoopOption[AnimationType_Hover] = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else if (_tcscmp(pstrName, _T("loopworking")) == 0) {
		m_mapAnimationLoopOption[AnimationType_Working] = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else __super::SetAttribute(pstrName, pstrValue);
}

void DuiLib::CAnimationOptionUI::PaintStatusImage(HDC hDC)
{
	if ((m_animState & AnimationState_Mask) == 0)
	{
		__super::PaintStatusImage(hDC);
	}
	else
	{
		DWORD dwColor = 0;
		if ((m_uButtonState & UISTATE_HOT) != 0)
		{
			dwColor = GetAdjustColor(m_dwHotBkColor);
		}
		else if ((m_uButtonState & UISTATE_SELECTED) != 0)
		{
			dwColor = GetAdjustColor(m_dwSelectedBkColor);
		}
		if (dwColor != 0)
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(dwColor));

		DrawImage(hDC, m_diFore);
	}
}

DuiLib::CDuiString DuiLib::CAnimationOptionUI::GetAttribute(LPCTSTR pstrName)
{
	CDuiString sValue;
	if (_tcscmp(pstrName, _T("startindexhover")) == 0)
	{
		std::map<AnimationType, std::pair<int, int>>::const_iterator iter = m_mapAnimationStartEndIndex.find(AnimationType_Hover);
		if (iter != m_mapAnimationStartEndIndex.end())
		{
			sValue.Format(L"%d", iter->second.first);
		}
	}
	else if (_tcscmp(pstrName, _T("endindexhover")) == 0)
	{
		std::map<AnimationType, std::pair<int, int>>::const_iterator iter = m_mapAnimationStartEndIndex.find(AnimationType_Hover);
		if (iter != m_mapAnimationStartEndIndex.end())
		{
			sValue.Format(L"%d", iter->second.second);
		}
	}
	else if (_tcscmp(pstrName, _T("startindexworking")) == 0)
	{
		std::map<AnimationType, std::pair<int, int>>::const_iterator iter = m_mapAnimationStartEndIndex.find(AnimationType_Working);
		if (iter != m_mapAnimationStartEndIndex.end())
		{
			sValue.Format(L"%d", iter->second.first);
		}
	}
	else if (_tcscmp(pstrName, _T("endindexworking")) == 0)
	{
		std::map<AnimationType, std::pair<int, int>>::const_iterator iter = m_mapAnimationStartEndIndex.find(AnimationType_Working);
		if (iter != m_mapAnimationStartEndIndex.end())
		{
			sValue.Format(L"%d", iter->second.second);
		}
	}
	else if (_tcscmp(pstrName, _T("picturehover")) == 0)
	{
		std::map<AnimationType, CDuiString>::const_iterator iter = m_mapAnimationPicture.find(AnimationType_Hover);
		if (iter != m_mapAnimationPicture.end())
		{
			sValue = iter->second;
		}
	}
	else if (_tcscmp(pstrName, _T("pictureworking")) == 0)
	{
		std::map<AnimationType, CDuiString>::const_iterator iter = m_mapAnimationPicture.find(AnimationType_Working);
		if (iter != m_mapAnimationPicture.end())
		{
			sValue = iter->second;
		}
	}
	else if (_tcscmp(pstrName, _T("intervalhover")) == 0)
	{
		std::map<AnimationType, int>::const_iterator iter = m_mapAnimationInterval.find(AnimationType_Hover);
		if (iter != m_mapAnimationInterval.end())
		{
			sValue.Format(L"%d", iter->second);
		}
	}
	else if (_tcscmp(pstrName, _T("intervalworking")) == 0)
	{
		std::map<AnimationType, int>::const_iterator iter = m_mapAnimationInterval.find(AnimationType_Working);
		if (iter != m_mapAnimationInterval.end())
		{
			sValue.Format(L"%d", iter->second);
		}
	}
	else if (_tcscmp(pstrName, _T("loophover")) == 0)
	{
		std::map<AnimationType, bool>::const_iterator iter = m_mapAnimationLoopOption.find(AnimationType_Hover);
		if (iter != m_mapAnimationLoopOption.end())
		{
			sValue = iter->second ? L"true" : L"false";
		}
	}
	else if (_tcscmp(pstrName, _T("loopworking")) == 0)
	{
		std::map<AnimationType, bool>::const_iterator iter = m_mapAnimationLoopOption.find(AnimationType_Working);
		if (iter != m_mapAnimationLoopOption.end())
		{
			sValue = iter->second ? L"true" : L"false";
		}
	}
	else
	{
		sValue = __super::GetAttribute(pstrName);
	}

	return sValue;
}

void DuiLib::CAnimationOptionUI::drawAnimationPicture(AnimationType type)
{
	int nCurrentIndex = getCurrntIndex(type);
	int nStartIndex = getStartIndex(type);
	int nEndIndex = getEndIndex(type);
	bool bLoop = isAnimationLoop(type);

	setCurrentIndex(type, ++nCurrentIndex);

	if (nCurrentIndex > nEndIndex)
	{
		if (!bLoop)
		{
			StopAnim(type);
			return;
		}
		else
		{
			nCurrentIndex = getStartIndex(type);
			setCurrentIndex(type, getStartIndex(type));
		}
	}
	SetForeImage(getPaintPicture(type, nCurrentIndex));
}


DuiLib::CDuiString DuiLib::CAnimationOptionUI::getPicture(AnimationType type)
{
	std::map<AnimationType, CDuiString>::const_iterator iter = m_mapAnimationPicture.find(type);
	if (iter != m_mapAnimationPicture.end())
	{
		return iter->second;
	}
	else
	{
		return CDuiString();
	}
}

DuiLib::CDuiString DuiLib::CAnimationOptionUI::getPaintPicture(AnimationType type, int index)
{
	CDuiString bkImage;
	CDuiString strPicture = getPicture(type);
	int pos = strPicture.ReverseFind(_T('.'));
	if (pos > 0)
	{
		CDuiString left = strPicture.Left(pos);
		CDuiString right = strPicture.Right(strPicture.GetLength() - pos);
		bkImage.Format(_T("%s%d%s"), left.GetData(), index, right.GetData());
	}
	else
	{
		bkImage.Format(_T("%s%d"), strPicture, index);
	}
	return bkImage;
}

int DuiLib::CAnimationOptionUI::getStartIndex(AnimationType type)
{
	std::map<AnimationType, std::pair<int, int>>::const_iterator iter = m_mapAnimationStartEndIndex.find(type);
	if (iter != m_mapAnimationStartEndIndex.end())
	{
		return iter->second.first;
	}
	else
	{
		return INVALID_INDEX;
	}
}

int DuiLib::CAnimationOptionUI::getEndIndex(AnimationType type)
{
	std::map<AnimationType, std::pair<int, int>>::const_iterator iter = m_mapAnimationStartEndIndex.find(type);
	if (iter != m_mapAnimationStartEndIndex.end())
	{
		return iter->second.second;
	}
	else
	{
		return INVALID_INDEX;
	}
}

int DuiLib::CAnimationOptionUI::getCurrntIndex(AnimationType type)
{
	std::map<AnimationType, int>::const_iterator iter = m_mapAnimationCurrentIndex.find(type);
	if (iter != m_mapAnimationCurrentIndex.end())
	{
		return iter->second;
	}
	else
	{
		return INVALID_INDEX;
	}
}

int DuiLib::CAnimationOptionUI::getInverval(AnimationType type)
{
	std::map<AnimationType, int>::const_iterator iter = m_mapAnimationInterval.find(type);
	if (iter != m_mapAnimationInterval.end())
	{
		return iter->second;
	}
	else
	{
		return INVALID_INTERVAL;
	}
}

bool DuiLib::CAnimationOptionUI::isAnimationLoop(AnimationType type)
{
	std::map<AnimationType, bool>::const_iterator iter = m_mapAnimationLoopOption.find(type);
	if (iter != m_mapAnimationLoopOption.end())
	{
		return iter->second;
	}
	else
	{
		return false;
	}
}

void DuiLib::CAnimationOptionUI::setCurrentIndex(AnimationType type, int index)
{
	m_mapAnimationCurrentIndex[type] = index;
}

UINT DuiLib::CAnimationOptionUI::getTimerID(AnimationType type)
{
	if (type == AnimationType_Hover)
	{
		return ANIMATIONOPTION_PICTURE_HOVER_TIMER_ID;
	}
	else if (type == AnimationType_Working)
	{
		return ANIMATIONOPTION_PICTURE_WORKING_TIMER_ID;
	}

	return 0;
}
