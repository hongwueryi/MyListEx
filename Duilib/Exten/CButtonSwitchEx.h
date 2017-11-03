#pragma once
namespace DuiLib
{
	#define	DEFAULT_TIMERID 10
	class UILIB_API CButtonSwitchExUI : public CButtonUI
	{
	public:
		CButtonSwitchExUI();
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		int GetValue() const;

		void SetValue(int nValue, BOOL fireEvent);

		void UpdataProgressValue(int Offset = 0);

	private:
		RECT	m_rcThumb;
		POINT	m_ptLastMouse;
		UINT	m_uThumbState;
		CString m_strPointImage;
		int		m_nPointHeight = 0;
		int		m_nPointWidth = 0;
		int		m_nMax = 0;
		int		m_nMin = 0;
		int		m_nValue = 0;
		int		m_Offset = 0;
	};
}