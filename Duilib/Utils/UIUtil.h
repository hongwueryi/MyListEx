#ifndef _UIUTIL_H_
#define _UIUTIL_H_

namespace DuiLib
{

	//////////////////////////////////////////////////////////
	// Structure for base control position
	typedef struct tagTControlPosInfo
	{
		bool bCenter;
		bool bMinus;
		int  nPos;
	} TControlPosInfo;

	// Structure for control posision
	typedef struct tagTControlPos
	{
		int nType;      // 1:width+height; 2:left+height; 4:l,t,r,b
		union
		{
			struct
			{
				TControlPosInfo nLeft;
				TControlPosInfo nTop;
				TControlPosInfo nRight;
				TControlPosInfo nBottom;
			};
			struct
			{
				TControlPosInfo nWidth;
				TControlPosInfo nHeight;
			};
			TControlPosInfo Item[4];
		};
	}TControlPos;

	// Structure for control blend
	typedef struct tagBlend
	{
		bool            bBlend;         // 是否进行融合操作
		UINT            uAlpha;         // 融合图像的透明度
		CDuiString      sName;          // 融合窗口的name
		TControlPos     tPos;           // 融合图像的位置偏移比例，数值为百分之N
	} TBlend;

	// Structure for control drawimage
	typedef struct tagDrawImage
	{
		enum DrawImagePlayMode
		{
			DrawImagePlayMode_None = 0,
			DrawImagePlayMode_Once = 1,
			DrawImagePlayMode_OnceReset = 2,
			DrawImagePlayMode_Loop = 3,
			DrawImagePlayMode_Bounce = 4,
		};


		CDuiString      sFile;
		CDuiString      sRes;
		CDuiString      sResType;
		TControlPos     tSource;
		TControlPos     tDest;
		TControlPos     tCorner;
		DWORD           dwMask;
		int             nFade;
		bool            bHole;
		bool            bXTiled;
		bool            bYTiled;
		SIZE            szBlockSize;        // 图像块的大小（宽度+高度）
		int             nBlockCount;        // 图像块的数目
		int             nBlockIndex;        // 图像块的序号
		bool            bDynamic;           // 是否动态显示图像，true会根据blocksize，从blockindex到blockcount开始序列播放图像块
		bool            bPlay;              // 是否播放
		int             nPlayMode;          // 播放模式 once,loop
		int             nPlayFrame;         // 当前播放帧，与图像块序号对应
		int				nPlayStep;          // 播放方向变化数值，1表示正向增量（动画正向播放），-1表示方向增量（动画方向播放）
		int             nFrames;            // 播放帧数计时器间隔，单位ms

		tagDrawImage()
		{
			sFile = _T("");
			sRes = _T("");
			sResType = _T("");
			::ZeroMemory(&tSource, sizeof(tSource));
			::ZeroMemory(&tDest, sizeof(tDest));
			::ZeroMemory(&tCorner, sizeof(tCorner));
			dwMask = 0;
			nFade = 0xFF;
			bHole = false;
			bXTiled = false;
			bYTiled = false;
			szBlockSize.cx = szBlockSize.cy = 0;
			nBlockCount = 0;
			nBlockIndex = 0;
			bDynamic = false;
			bPlay = false;
			nPlayMode = DrawImagePlayMode_Once;
			nPlayFrame = 0;
			nPlayStep = 1;
			nFrames = 0;
		}

	} TDrawImage;

	// Enum
	// Window版本号
	enum EM_OS
	{
		EM_OS_UNKNOWN = 0,
		EM_OS_WIN95,
		EM_OS_WIN98,
		EM_OS_WINME,
		EM_OS_WIN2000,
		EM_OS_WINXP,
		EM_OS_WINXP_HOME,
		EM_OS_WINXP_PROFESSIONAL,
		EM_OS_WIN_SERVER2003,
		EM_OS_VISTA,
		EM_OS_WIN7,
		EM_OS_WINSERVER2008,
		EM_OS_WINSERVER2008R2,
		EM_OS_WIN8,
		EM_OS_WINSERVER2012,
	};


class UILIB_API CUIUtil
{
public:
    static UINT         GetWindowsVersion();

    // 位置计算
    static RECT         CalculateControlPos(const TControlPos& pos, const RECT& rcParent);
    static void         ScreenToClient(HWND hWnd, RECT& rc);
    static void         ClientToScreen(HWND hWnd, RECT& rc);

    // 绘制
    static HBITMAP      CreateBitmap(int width, int height);

    // 格式转换
    static bool         String2Bool(const CDuiString& sValue);
    static CDuiString   Bool2String(bool bValue);
    static int          String2Int(const CDuiString& sValue);
    static CDuiString   Int2String(int nValue);

    static SIZE         String2Size(const CDuiString& sValue);
    static CDuiString   Size2String(const SIZE& szValue);
    static RECT         String2Rect(const CDuiString& sValue);
    static CDuiString   Rect2String(const RECT& rcValue);
    static COLORREF     String2Color(const CDuiString& sValue);
    static CDuiString   Color2String(COLORREF clrValue);

    static TControlPos  String2Vertex4(const CDuiString& sValue);
    static CDuiString   Vertex42String(const TControlPos& tValue);
    static TBlend       String2Blend(const CDuiString& sValue);
    static CDuiString   Blend2String(const TBlend& tValue);
    static TDrawImage   String2DrawImage(const CDuiString& sValue);
    static CDuiString   DrawImage2String(const TDrawImage& tValue);

    static UINT         String2WinVer(const CDuiString& sWinVer);
    static CDuiString   WinVer2String(UINT uWinVer);

	//如果指定的窗口有阴影 则返回阴影的窗口，否则返回原窗口
	static HWND	GetShadowWindow(HWND wnd);
};

}

#endif