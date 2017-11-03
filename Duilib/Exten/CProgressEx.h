#pragma once
#include "RotateAnimateUI.h"
namespace DuiLib
{
	class UILIB_API CProgressExUI : public CHorizontalLayoutUI
	{
	public:
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual void Init() override;

		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void DoEvent(TEventUI& event) override;

		virtual void PaintBkImage(HDC hDC) override;

		virtual void SetVisible(bool bVisible = true) override;

		LPCTSTR GetForeImage() const;

		void SetForeImage(LPCTSTR pStrImage);

		int GetValue() const;

		void SetValue(int nValue);

		void UpdataProgressValue();

	private:
		CContainerUI*	m_foreImg;
		CContainerUI*	m_bkImg;
		CVerticalLayoutUI*	m_lineLayout;
		CVerticalLayoutUI*	m_linebkLayout;
		CRotateAnimateUI*	m_rotata;
		CContainerUI* m_CPointImg;
		RECT m_rcPos;
		DWORD m_dBkColor = 0;
		CString strForeImage;

		CDuiString m_pointPic;
		CDuiString m_animationPic;
		CDuiString m_interval = _T("50");

		CDuiString m_autoStart = true;
		CDuiString m_loop = true;

		int m_nMax = 0;
		int m_nMin = 0;
		int m_nValue ;
		int m_nWidth = 0;
		int m_nHeight = 0;
		int m_nPointHeight = 0;
		int m_nPointWidth = 0;
		int m_nAnimationHeight = 0;
		int m_nAnimationWidth = 0;
		int m_nLineHeight = 0;
	};
}