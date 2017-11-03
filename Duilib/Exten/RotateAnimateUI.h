#pragma once

namespace DuiLib
{
	class UILIB_API CRotateAnimateUI : public CControlUI
	{
	public:
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual void Init() override;

	public:
		void StartAnim();

		virtual void DoEvent(TEventUI& event) override;

		void StopAnim();

		virtual void PaintBkImage(HDC hDC) override;

	private:
		int m_interval = 50;
		bool m_autoStart = true;
		bool m_loop = true;
		bool m_stop = true;
		int m_rotate = 0;

	};
}