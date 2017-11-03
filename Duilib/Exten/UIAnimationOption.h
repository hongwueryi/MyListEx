#pragma once
#include "..\Control\UIOption.h"


namespace DuiLib
{
	class UILIB_API CAnimationOptionUI : public COptionUI
	{
	public:
		enum AnimationType
		{
			AnimationType_Unknown = 0x00000000,
			AnimationType_Hover = 0x00000001,
			AnimationType_Working = 0x00000002,

			AnimationType_Mask = 0x000000FF
		};

		enum AnimationState
		{
			AnimationState_None = 0x00000000,
			AnimationState_Hover = 0x00000001,
			AnimationState_Working = 0x0000002,

			AnimationState_Mask = 0x000000FF
		};

		enum LogicalState
		{
			LogicalState_None = 0x00000000,
			LogicalState_Working = 0x00000001,
			LogicalState_Mask = 0x000000FF,
		};

	public:
		CAnimationOptionUI();
		~CAnimationOptionUI();

		virtual void DoEvent(TEventUI& event) override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual void Selected(bool bSelected, BOOL fireEvent = TRUE) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		virtual void PaintStatusImage(HDC hDC) override;
		virtual CDuiString  GetAttribute(LPCTSTR pstrName);

		void SetLogicalState(LogicalState state, bool bSet = true);

	protected:
		void StartAnim(AnimationType type);
		void StopAnim(AnimationType type = AnimationType_Mask);

	private:
		void drawAnimationPicture(AnimationType type);

		CDuiString getPicture(AnimationType type);
		CDuiString getPaintPicture(AnimationType type, int index);
		int getStartIndex(AnimationType type);
		int getEndIndex(AnimationType type);
		int getCurrntIndex(AnimationType type);
		int getInverval(AnimationType type);
		bool isAnimationLoop(AnimationType type);
		void setCurrentIndex(AnimationType type, int index);

		UINT  getTimerID(AnimationType type);

	private:
		std::map<AnimationType, std::pair<int, int>> m_mapAnimationStartEndIndex;
		std::map<AnimationType, bool> m_mapAnimationLoopOption;
		std::map<AnimationType, CDuiString> m_mapAnimationPicture;
		std::map<AnimationType, int> m_mapAnimationInterval;
		std::map<AnimationType, int> m_mapAnimationCurrentIndex;

		int m_interval = 50;
		int m_step = 1;

		UINT m_animState = AnimationState_None;
		UINT m_uLogicalState = LogicalState_None;


		static int INVALID_INDEX;
		static int INVALID_INTERVAL;
	};
}


