#include "stdafx.h"
#include "RotateAnimateUI.h"

#define  ROTATE_PICTURE_TIMER_ID 902

void DuiLib::CRotateAnimateUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("interval")) == 0) {
		m_interval = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("auto")) == 0) {
		m_autoStart = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else if (_tcscmp(pstrName, _T("loop")) == 0) {
		m_loop = (_tcscmp(pstrValue, _T("true")) == 0);
	}
	else __super::SetAttribute(pstrName, pstrValue);
}

void DuiLib::CRotateAnimateUI::Init()
{
	if (m_autoStart)
	{
		m_stop = true;
		StartAnim();
	}
}

void DuiLib::CRotateAnimateUI::StartAnim()
{
	if (m_diBk.pImageInfo == NULL)
	{
		return;
	}
	if (!m_stop)
	{
		return;
	}
	m_stop = false;
	GetManager()->SetTimer(this, ROTATE_PICTURE_TIMER_ID, m_interval);
}

void DuiLib::CRotateAnimateUI::DoEvent(TEventUI& event)
{
	if (event.Type != UIEVENT_TIMER || !this->IsVisible())
	{
		return __super::DoEvent(event);
	}
	m_rotate++;

	if (m_rotate > 36)
	{
		if (m_loop)
		{
			m_rotate = 0;
		}
		else
		{
			StopAnim();
			return;
		}
	}
	this->Invalidate();
}

void DuiLib::CRotateAnimateUI::StopAnim()
{
	m_stop = true;
	
	m_rotate = 0;

	GetManager()->KillTimer(this);
}

void DuiLib::CRotateAnimateUI::PaintBkImage(HDC hDC)
{
	if (m_stop)
		DrawImage(hDC, m_diBk);
	else
		DrawImage(hDC, m_diBk,m_rotate*10);
}
