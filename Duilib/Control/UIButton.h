#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CButtonUI : public CLabelUI
	{
	public:
		CButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);

		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetPushedImage();
		void SetPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		LPCTSTR GetForeImage();
		void SetForeImage(LPCTSTR pStrImage);
		LPCTSTR GetHotForeImage();
		void SetHotForeImage(LPCTSTR pStrImage);

		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;
		void SetAngle(DWORD dwAngle);
		DWORD GetAngle() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;
		void SetFocusedTextColor(DWORD dwColor);
		DWORD GetFocusedTextColor() const;
		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);

		//void SetEllipseFlag(BOOL bFlag, RECT rc);

		void SetEllpseStatus(bool bEllpse = false);
		void SetEllipseImage(LPCTSTR pStrImage);
		void SetEllipseOffset(RECT rcOffset);

	protected:
		UINT m_uButtonState;

		DWORD m_dwHotBkColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwPushedTextColor;
		DWORD m_dwFocusedTextColor;
		DWORD m_dwAngle;
		//BOOL m_bEllipseFlag = FALSE;
		//RECT m_rcBtnPaint;

		BOOL m_bIsEllpse;
		RECT m_rcEllipseOffSet;
		RECT m_rcTranslate;

		TDrawInfo m_diNormal;
		TDrawInfo m_diHot;
		TDrawInfo m_diHotFore;
		TDrawInfo m_diPushed;
		TDrawInfo m_diPushedFore;
		TDrawInfo m_diFocused;
		TDrawInfo m_diDisabled;
		TDrawInfo m_diEllipse;
	};

}	// namespace DuiLib

#endif // __UIBUTTON_H__