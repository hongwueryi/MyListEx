#pragma once

namespace DuiLib
{
	class UILIB_API CSequencePictureUI : public CControlUI
	{
	public:
		CSequencePictureUI();
		virtual ~CSequencePictureUI();

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual void Init() override;

		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		void StartAnim();

		void StopAnim(BOOL KeepStatus=FALSE);

		void SetStartIndex(int index);
		void SetEndIndex(int index);
		void SetCurrentIndex(int index);

		int GetInterval();

		void SetStep(int nStep);
		void SetBackward(BOOL bAutoResetForward = TRUE);
		void SetReachBoundary(bool bReachBoundary);
		void SetAutoStopKeepStatus();

		virtual void DoEvent(TEventUI& event) override;

		void OnSequenceTimer();

		virtual void PaintBkImage(HDC hDC) override;

		virtual void SetVisible(bool bVisible = true) override;

		bool GetSequenceStatus();
	private:
		int m_startIndex = 0;
		int m_endIndex = 0;
		CDuiString m_picture;
		int m_interval = 50;
		int m_step = 1;

		bool m_autoStart = true;
		bool m_loop = true;
		bool m_stop = true;
		bool m_bAutoResetForward = true;
		bool m_bBackward = false;
		bool m_bReachBoundary = false;
		bool m_bAlreadyReadBoundary = false;
		bool m_bAutoStopKeepStatus = false;

	private:
		int m_currentIndex = 0;
	};
}