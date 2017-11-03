#include "stdafx.h"
#include "UISequencePicture.h"

#define  SEQUENCE_PICTURE_TIMER_ID 901


DuiLib::CSequencePictureUI::CSequencePictureUI():m_autoStart(TRUE)
{

}

DuiLib::CSequencePictureUI::~CSequencePictureUI()
{

}

void DuiLib::CSequencePictureUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("startindex")) == 0) {
		m_startIndex = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("endindex")) == 0) {
		m_endIndex = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("picture")) == 0) {
		m_picture = pstrValue;
	}
	else if (_tcscmp(pstrName, _T("interval")) == 0) {
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

void DuiLib::CSequencePictureUI::Init()
{
	if (m_autoStart)
	{
		m_stop = true;
		StartAnim();
	}
}

LPVOID DuiLib::CSequencePictureUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("SequencePic")) == 0)
		return static_cast<CSequencePictureUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void DuiLib::CSequencePictureUI::StartAnim()
{
	if (!m_stop)
	{
		return;
	}
	m_stop = false;
	m_bAlreadyReadBoundary = false;
	GetManager()->SetTimer(this, SEQUENCE_PICTURE_TIMER_ID, m_interval);
}

void DuiLib::CSequencePictureUI::StopAnim(BOOL KeepStatus)
{
	m_stop = true;
	//gaohw3 added,stop之后设置当前index为初始值
	if(!KeepStatus)
		m_currentIndex = m_startIndex;
	if (m_bAutoResetForward)
		m_bBackward = false;
	GetManager()->KillTimer(this);
}

void DuiLib::CSequencePictureUI::SetStartIndex(int index)
{
	m_startIndex = index;
}

void DuiLib::CSequencePictureUI::SetEndIndex(int index)
{
	m_endIndex = index;
}

void DuiLib::CSequencePictureUI::SetCurrentIndex(int index)
{
	m_currentIndex = index;
}

int DuiLib::CSequencePictureUI::GetInterval()
{
	return m_interval;
}

void DuiLib::CSequencePictureUI::SetStep(int nStep)
{
	m_step = nStep;
}

void DuiLib::CSequencePictureUI::SetBackward(BOOL bAutoResetForward)
{
	m_bBackward = true;
	m_bAutoResetForward = bAutoResetForward;
}

void DuiLib::CSequencePictureUI::SetReachBoundary(bool bReachBoundary)
{
	m_bReachBoundary = bReachBoundary;
}

void DuiLib::CSequencePictureUI::SetAutoStopKeepStatus()
{
	m_bAutoStopKeepStatus = true;
}

void DuiLib::CSequencePictureUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER && event.wParam == SEQUENCE_PICTURE_TIMER_ID)
	{
		OnSequenceTimer();
	}
	else
	{
		if (this->OnEvent(&event)) CControlUI::DoEvent(event);
	}
}

void DuiLib::CSequencePictureUI::OnSequenceTimer()
{
	if (!IsVisible())
	{
		return;
	}

	if (m_bBackward)
		m_currentIndex -= m_step;
	else
		m_currentIndex += m_step;

	if (this->m_picture.GetLength() == 0)
	{
		return;
	}

	if (m_currentIndex < m_startIndex)
	{
		if (m_loop && m_bBackward)
		{
			m_currentIndex = m_endIndex;
		}
		else if (m_bBackward)
		{
			if (m_bReachBoundary && !m_bAlreadyReadBoundary)
			{
				m_currentIndex = m_startIndex;
				m_bAlreadyReadBoundary = true;
			}
			else
			{
				StopAnim(m_bAutoStopKeepStatus);
				return;
			}
		}
		else
		{
			m_currentIndex = m_startIndex;
		}
	}

	if (m_currentIndex > m_endIndex)
	{
		if (m_loop)
		{
			m_currentIndex = m_startIndex;
		}
		else if (m_bReachBoundary && !m_bAlreadyReadBoundary)
		{
			m_currentIndex = m_endIndex;
			m_bAlreadyReadBoundary = true;
		}
		else
		{
			StopAnim(m_bAutoStopKeepStatus);
			return;
		}
	}

	//todo:此处可以做性能优化
	CDuiString bkImage;
	int pos = m_picture.ReverseFind(_T('.'));
	if (pos > 0)
	{
		CDuiString left = m_picture.Left(pos);
		CDuiString right = m_picture.Right(m_picture.GetLength() - pos);
		bkImage.Format(_T("%s%d%s"), left.GetData(), m_currentIndex, right.GetData());
	}
	else
	{
		bkImage.Format(_T("%s%d"), m_picture, m_currentIndex);
	}

	SetBkImage(bkImage);
}

void DuiLib::CSequencePictureUI::PaintBkImage(HDC hDC)
{
	__super::PaintBkImage(hDC);
}

void DuiLib::CSequencePictureUI::SetVisible(bool bVisible /*= true*/)
{
	//todo: 此处需要做优化处理
	__super::SetVisible(bVisible);

}

bool DuiLib::CSequencePictureUI::GetSequenceStatus()
{
	return m_stop;
}
