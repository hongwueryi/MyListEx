#ifndef __UICUSTOMBUTTON_H__
#define __UICUSTOMBUTTON_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CCustomButtonUI : public CButtonUI
	{
	public:
		CCustomButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetTitleHotTextColor(DWORD dwColor);
		DWORD GetTitleHotTextColor() const;
		void SetTitleTextColor(DWORD dwColor);
		DWORD GetTitleTextColor() const;
		RECT GetTitleTextPadding() const;
		void SetTitleTextPadding(RECT rc);
		void SetTitleFont(int index);
		int GetTitleFont() const;
		void SetTitleText(LPCTSTR pstrText);
		CDuiString GetTitleText() const;

		void SetValueHotTextColor(DWORD dwColor);
		DWORD GetValueHotTextColor() const;
		void SetValueTextColor(DWORD dwColor);
		DWORD GetValueTextColor() const;
		RECT GetValueTextPadding() const;
		void SetValueTextPadding(RECT rc);
		void SetValueFont(int index);
		int GetValueFont() const;
		void SetValueText(LPCTSTR pstrText);
		CDuiString GetValueText() const;
		

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);

	protected:
		CDuiString	m_sTextTitle;
		int			m_iFont_Title;
		RECT		m_rcTextPadding_Title;
		DWORD		m_dwHotTextTitleColor;
		DWORD		m_dwTextTitleColor;

		CDuiString	m_sTextValue;
		int			m_iFont_Value;
		RECT		m_rcTextPadding_Value;
		DWORD		m_dwHotTextValueColor;
		DWORD		m_dwTextValueColor;
	};

}	// namespace DuiLib

#endif // __UIBUTTON_H__