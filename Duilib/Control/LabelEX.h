#ifndef LABELEX_H
#define LABELEX_H
#include"UILabel.h"

#include <GdiPlus.h>
#pragma comment( lib, "GdiPlus.lib" )
using namespace Gdiplus;
class UILIB_API Gdiplus::RectF;
struct UILIB_API Gdiplus::GdiplusStartupInput;

namespace DuiLib
{
	class CLabelUIEx :public CLabelUI
	{
	public:
		CLabelUIEx();
		~CLabelUIEx();

		//inherits
		virtual void PaintText(HDC hDC);
		void DoEvent(TEventUI& event);
		//local

	protected:
		//locals
		Color _MakeRGB(int a, Color cl);
		Color _MakeRGB(int r, int g, int b);
	private:
		//CPaintManagerUI& m_paint;
		//CDialogBuilder m_dlgBuilder;

		bool m_bSetteled;
		RECT m_rtHotArea;
	};
}


#endif