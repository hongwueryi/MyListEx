#include "Stdafx.h"
#include "UIUtil.h"
namespace DuiLib 
{

UINT CUIUtil::GetWindowsVersion()
{
    static UINT s_uWinVersion = EM_OS_UNKNOWN;
    if (EM_OS_UNKNOWN == s_uWinVersion)
    {
        EM_OS emOS = EM_OS_UNKNOWN;

        OSVERSIONINFOEX OS;
        OS.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        BOOL bVerEx = GetVersionEx((OSVERSIONINFO*)&OS);

        switch (OS.dwPlatformId)
        {
        case VER_PLATFORM_WIN32_WINDOWS:
            switch (OS.dwMinorVersion)
            {
            case 0:
                emOS = EM_OS_WIN95;
                break;
            case 10:
                emOS = EM_OS_WIN98;
                break;
            case 90:
                emOS = EM_OS_WINME;
                break; 
            }
            break;
        case VER_PLATFORM_WIN32_NT:
            if (OS.dwMajorVersion <= 4)  
            {
                //do nothing
            }
            else if (OS.dwMajorVersion == 5)
            {
                switch (OS.dwMinorVersion)
                {
                case 0:
                    emOS = EM_OS_WIN2000;
                    break;
                case 1:
                    if (bVerEx)
                    {
                        if (OS.wProductType == VER_NT_WORKSTATION)
                        {
                            if (OS.wSuiteMask & VER_SUITE_PERSONAL)
                            {
                                //if (::GetSystemMetrics(SM_MEDIACENTER) != 0
                                //    || ::GetSystemMetrics(SM_STARTER) != 0
                                //    || ::GetSystemMetrics(SM_TABLETPC) != 0)
                                //    emOS = EM_OS_WINXP;
                                //else
                                    emOS = EM_OS_WINXP_HOME;
                            }
                            else
                            {
                                emOS = EM_OS_WINXP_PROFESSIONAL;
                            }
                        }
                        else
                        {
                            emOS = EM_OS_WINXP;
                        }
                    }
                    else
                    {
                        emOS = EM_OS_WINXP;
                    }
                    break;
                case 2:
                    emOS = EM_OS_WIN_SERVER2003;
                    break;
                }
            }
            else if (OS.dwMajorVersion == 6)
            {
                if (bVerEx)
                {
                    if (OS.wProductType == VER_NT_WORKSTATION)
                    {
                        if (OS.dwMinorVersion == 0)
                        {
                            emOS = EM_OS_VISTA;
                        }
                        else if(OS.dwMinorVersion == 1)
                        {
                            emOS = EM_OS_WIN7;
                        } 
                        else if(OS.dwMinorVersion == 2)
                        {
                            emOS = EM_OS_WIN8;
                        } 
                    }
                    else if (OS.wProductType == VER_NT_SERVER)
                    {
                        emOS = EM_OS_WINSERVER2008;

                        if (OS.dwMinorVersion == 0)
                        {
                            emOS = EM_OS_WINSERVER2008;
                        }
                        else if(OS.dwMinorVersion == 1)
                        {
                            emOS = EM_OS_WINSERVER2008R2;
                        } 
                        else if(OS.dwMinorVersion == 2)
                        {
                            emOS = EM_OS_WINSERVER2012;
                        } 
                    }
                }
            }
            break;
        }

        s_uWinVersion = emOS;
    }
    return s_uWinVersion;
}

RECT CUIUtil::CalculateControlPos(const TControlPos& pos, const RECT& rcParent)
{
    RECT rc = {};

    if (4 == pos.nType)
    {
        int widthParent = rcParent.right - rcParent.left;
        int heightParent = rcParent.bottom - rcParent.top;

        if (pos.nLeft.bCenter)
            rc.left = rcParent.left + widthParent / 2 + (pos.nLeft.bMinus ? -1 : 1) *  pos.nLeft.nPos;
        else if (pos.nLeft.bMinus)
            rc.left = rcParent.right - pos.nLeft.nPos;
        else
            rc.left = rcParent.left + pos.nLeft.nPos;

        if (pos.nRight.bCenter)
            rc.right = rcParent.left + widthParent / 2 + (pos.nRight.bMinus ? -1 : 1) *  pos.nRight.nPos;
        else if (pos.nRight.bMinus)
            rc.right = rcParent.right - pos.nRight.nPos;
        else
            rc.right = rcParent.left + pos.nRight.nPos;

        if (pos.nTop.bCenter)
            rc.top = rcParent.top + heightParent / 2 + (pos.nTop.bMinus ? -1 : 1) *  pos.nTop.nPos;
        else if (pos.nTop.bMinus)
            rc.top = rcParent.bottom - pos.nTop.nPos;
        else
            rc.top = rcParent.top + pos.nTop.nPos;

        if (pos.nBottom.bCenter)
            rc.bottom = rcParent.top + heightParent / 2 + (pos.nBottom.bMinus ? -1 : 1) *  pos.nBottom.nPos;
        else if (pos.nBottom.bMinus)
            rc.bottom = rcParent.bottom - pos.nBottom.nPos;
        else
            rc.bottom = rcParent.top + pos.nBottom.nPos;
    }

    return rc;
}

void CUIUtil::ScreenToClient(HWND hWnd, RECT& rc)
{
    if (::IsWindow(hWnd))
    {
        ::ScreenToClient(hWnd, (LPPOINT)&rc);
        ::ScreenToClient(hWnd, ((LPPOINT)&rc)+1);
    }
}

void CUIUtil::ClientToScreen(HWND hWnd, RECT& rc)
{
    if (::IsWindow(hWnd))
    {
        ::ClientToScreen(hWnd, (LPPOINT)&rc);
        ::ClientToScreen(hWnd, ((LPPOINT)&rc)+1);
    }
}

HBITMAP CUIUtil::CreateBitmap(int width, int height)
{
    HBITMAP hBitmap = NULL;

    BYTE* pBits = NULL;   
    BITMAPINFOHEADER bmih;   
    ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));   
    bmih.biSize             = sizeof(BITMAPINFOHEADER) ;   
    bmih.biWidth            = width;   
    bmih.biHeight           = height;   
    bmih.biPlanes           = 1;   
    bmih.biBitCount         = 32;
    bmih.biCompression      = BI_RGB;   
    bmih.biSizeImage        = 0;   
    bmih.biXPelsPerMeter    = 0;   
    bmih.biYPelsPerMeter    = 0;   
    bmih.biClrUsed          = 0;   
    bmih.biClrImportant     = 0;   
    hBitmap = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);

    return hBitmap;
}

bool CUIUtil::String2Bool(const CDuiString& sValue)
{
    bool bValue = (0 == sValue.CompareNoCase(_T("true")));
    //_tcscmp(pstrValue, _T("true")) == 0
    return bValue;
}

CDuiString CUIUtil::Bool2String(bool bValue)
{
    CDuiString sValue = _T("false");
    if (bValue)
    {
        sValue = _T("true");
    }
    return sValue;
}

int CUIUtil::String2Int(const CDuiString& sValue)
{
    int nValue = 0;
    nValue = _ttoi(sValue);
    return nValue;
}

CDuiString CUIUtil::Int2String(int nValue)
{
    CDuiString sValue;
    sValue.Format(_T("%d"), nValue);
    return sValue;
}

SIZE CUIUtil::String2Size(const CDuiString& sValue)
{
    SIZE sz = {};

    CDuiString sSize = sValue;
    sSize.Remove(L' ');
    sSize.Remove(L'\t');
    LPCTSTR pstrSize = sSize.GetData();
    if (pstrSize)
    {
        LPTSTR pstr = NULL;
        sz.cx = _tcstol(pstrSize, &pstr, 10);  
        sz.cy = _tcstol(pstr + 1, &pstr, 10); 
    }

    return sz;
}

CDuiString CUIUtil::Size2String(const SIZE& szValue)
{
    CDuiString sValue;
    sValue.Format(_T("%d,%d"), szValue.cx, szValue.cy);
    return sValue;
}

RECT CUIUtil::String2Rect(const CDuiString& sValue)
{
    RECT rc = {};

    CDuiString sRect = sValue;
    sRect.Remove(L' ');
    sRect.Remove(L'\t');
    LPCTSTR pstrRect = sRect.GetData();
    if (pstrRect)
    {
        LPTSTR pstr = NULL;
        rc.left   = _tcstol(pstrRect, &pstr, 10);  
        rc.top    = _tcstol(pstr + 1, &pstr, 10); 
        rc.right  = _tcstol(pstr + 1, &pstr, 10);  
        rc.bottom = _tcstol(pstr + 1, &pstr, 10); 
    }

    return rc;
}

CDuiString CUIUtil::Rect2String(const RECT& rcValue)
{
    CDuiString sValue;
    sValue.Format(_T("%d,%d,%d,%d"), rcValue.left, rcValue.top, rcValue.right, rcValue.bottom);
    return sValue;
}

COLORREF CUIUtil::String2Color(const CDuiString& sValue)
{
    COLORREF clrColor = CLR_INVALID;

    CDuiString sColor = sValue;
    LPCTSTR pstrColor = sColor.GetData();
    if (pstrColor)
    {
        while( *pstrColor > _T('\0') && *pstrColor <= _T(' ') ) pstrColor = ::CharNext(pstrColor);
        if( *pstrColor == _T('#')) pstrColor = ::CharNext(pstrColor);

        LPTSTR pstr = NULL;
        clrColor = _tcstoul(pstrColor, &pstr, 16);
    }

    return clrColor;
}

CDuiString CUIUtil::Color2String(COLORREF clrValue)
{
    CDuiString sValue;

    int r = GetRValue(clrValue);
    int g = GetGValue(clrValue);
    int b = GetBValue(clrValue);
    int a = (clrValue >> 24) & 0xFF;
    sValue.Format(_T("#%x%x%x%x"), a, r, g, b);

    return sValue;
}

TControlPos CUIUtil::String2Vertex4(const CDuiString& sValue)
{
    TControlPos pos = {};

    CDuiString strValue = sValue;
    if (!strValue.IsEmpty())
    {
        strValue.Remove(L' ');
        strValue.Remove(L'\t');

        LPCTSTR lpszValue = strValue.GetData();
        int nPos = 0, nPosPrev = 0;

        for (pos.nType = 0; pos.nType < 4; pos.nType++)
        {
            nPosPrev = nPos;
            nPos = strValue.Find(',', nPosPrev) + 1;

            pos.Item[pos.nType].bCenter = ('|' == lpszValue[nPosPrev]);
            if (pos.Item[pos.nType].bCenter)
                nPosPrev ++;
            pos.Item[pos.nType].bMinus = ('-' == lpszValue[nPosPrev]);
            if (pos.Item[pos.nType].bMinus)
                nPosPrev ++;

            if (0 == nPos)
            {
                pos.Item[pos.nType].nPos = _ttoi(strValue.Mid(nPosPrev));
                pos.nType++;
                break;
            }

            pos.Item[pos.nType].nPos = _ttoi(strValue.Mid(nPosPrev, nPos - nPosPrev));
        }
    }

    return pos;
}

CDuiString CUIUtil::Vertex42String(const TControlPos& tValue)
{
    CDuiString pos;

    for (int i = 0; i < tValue.nType; ++i)
    {
        if (tValue.Item[i].bCenter)
        {
            pos.Append(_T("|"));
        }

        if (tValue.Item[i].bMinus)
        {
            pos.Append(_T("-"));
        }

        CDuiString strValue;
        strValue.Format(L"%d", tValue.Item[i].nPos);
        pos.Append(strValue);

        if (i + 1 < tValue.nType)
        {
            pos.Append(_T(","));
        }
    }

    return pos;
}

TBlend CUIUtil::String2Blend(const CDuiString& sValue)
{
    // 融合处理列表.blend="name='被融合窗口name', pos='融合绘图贴图时相对窗口客户区的位置偏移百分比', alpha='融合绘图透明度, host='被融合窗口是否为host,为host会被作为背景绘制'"

    TBlend blend = {};

    CDuiString strValue = sValue;
    if (!strValue.IsEmpty())
    {
        strValue.Remove(L' ');
        strValue.Remove(L'\t');

        LPCTSTR lpszValue = strValue.GetData();
        int nPos = 0, nPosPrev = 0;

        while (!strValue.IsEmpty())
        {
            int nKeyPos = strValue.Find('=') + 1;
            CDuiString sKeyword = strValue.Left(nKeyPos-1);

            strValue = strValue.Mid(nKeyPos+1, strValue.GetLength()-(nKeyPos+1));
            
            int nKeyValuePos = strValue.Find('\'') + 1;
            CDuiString sKeyvalue = strValue.Left(nKeyValuePos-1);

            if (sKeyword == _T("pos"))
            {
                blend.tPos = CUIUtil::String2Vertex4(sKeyvalue);
            }
            else if (sKeyword == _T("run"))
            {
                blend.bBlend = CUIUtil::String2Bool(sKeyvalue);
            }
            else if (sKeyword == _T("alpha"))
            {
                blend.uAlpha = CUIUtil::String2Int(sKeyvalue);
            }
            else if (sKeyword == _T("name"))
            {
                blend.sName = sKeyvalue;
            }

            strValue = strValue.Mid(nKeyValuePos+1, strValue.GetLength()-(nKeyValuePos+1));
        }
    }

    return blend;
}

CDuiString CUIUtil::Blend2String(const TBlend& tValue)
{
    CDuiString blend;

    blend.Append(_T("run='"));
    blend.Append(CUIUtil::Bool2String(tValue.bBlend));
    blend.Append(_T("',alpha='"));
    blend.Append(CUIUtil::Int2String(tValue.uAlpha));
    blend.Append(_T("',name='"));
    blend.Append(tValue.sName);
    blend.Append(_T("',pos='"));
    blend.Append(CUIUtil::Vertex42String(tValue.tPos));
    blend.Append(_T("'"));

    return blend;
}

TDrawImage CUIUtil::String2DrawImage(const CDuiString& sValue)
{
    TDrawImage drawimage;

    CDuiString strValue = sValue;
    if (!strValue.IsEmpty())
    {
        strValue.Remove(L' ');
        strValue.Remove(L'\t');

        LPCTSTR lpszValue = strValue.GetData();
        int nPos = 0, nPosPrev = 0;

        while (!strValue.IsEmpty())
        {
            int nKeyPos = strValue.Find('=') + 1;
            CDuiString sKeyword = strValue.Left(nKeyPos-1);

            strValue = strValue.Mid(nKeyPos+1, strValue.GetLength()-(nKeyPos+1));

            int nKeyValuePos = strValue.Find('\'') + 1;
            CDuiString sKeyvalue = strValue.Left(nKeyValuePos-1);

            if (sKeyword == _T("file"))
            {
                drawimage.sFile = sKeyvalue;
            }
            else if (sKeyword == _T("res"))
            {
                drawimage.sRes = sKeyvalue;
            }
            else if (sKeyword == _T("restype"))
            {
                drawimage.sResType = sKeyvalue;
            }
            else if (sKeyword == _T("source"))
            {
                drawimage.tSource = CUIUtil::String2Vertex4(sKeyvalue);
            }
            else if (sKeyword == _T("dest"))
            {
                drawimage.tDest = CUIUtil::String2Vertex4(sKeyvalue);
            }
            else if (sKeyword == _T("corner"))
            {
                drawimage.tCorner = CUIUtil::String2Vertex4(sKeyvalue);
            }
            else if (sKeyword == _T("mask"))
            {
                drawimage.dwMask = CUIUtil::String2Color(sKeyvalue);
            }
            else if (sKeyword == _T("fade"))
            {
                drawimage.nFade = CUIUtil::String2Int(sKeyvalue);
            }
            else if (sKeyword == _T("hole"))
            {
                drawimage.bHole = CUIUtil::String2Bool(sKeyvalue);
            }
            else if (sKeyword == _T("xtiled"))
            {
                drawimage.bXTiled = CUIUtil::String2Bool(sKeyvalue);
            }
            else if (sKeyword == _T("ytiled"))
            {
                drawimage.bYTiled = CUIUtil::String2Bool(sKeyvalue);
            }
            else if (sKeyword == _T("blocksize"))
            {
                drawimage.szBlockSize = CUIUtil::String2Size(sKeyvalue);
            }
            else if (sKeyword == _T("blockcount"))
            {
                drawimage.nBlockCount = CUIUtil::String2Int(sKeyvalue);
            }
            else if (sKeyword == _T("blockindex"))
            {
                drawimage.nBlockIndex = CUIUtil::String2Int(sKeyvalue);
            }
            else if (sKeyword == _T("dynamic"))
            {
                drawimage.bDynamic = CUIUtil::String2Bool(sKeyvalue);
            }
            else if (sKeyword == _T("play"))
            {
                drawimage.bPlay = CUIUtil::String2Bool(sKeyvalue);
            }
            else if (sKeyword == _T("playmode"))
            {
                if (sKeyvalue == _T("once"))
                {
                    drawimage.nPlayMode = TDrawImage::DrawImagePlayMode_Once;
                }
                else if (sKeyvalue == _T("oncerest"))
                {
                    drawimage.nPlayMode = TDrawImage::DrawImagePlayMode_OnceReset;
                }
                else if (sKeyvalue == _T("loop"))
                {
                    drawimage.nPlayMode = TDrawImage::DrawImagePlayMode_Loop;
                }
                else if (sKeyvalue == _T("bounce"))
                {
                    drawimage.nPlayMode = TDrawImage::DrawImagePlayMode_Bounce;
                }
            }
            else if (sKeyword == _T("playframe"))
            {
                drawimage.nPlayFrame = CUIUtil::String2Int(sKeyvalue);
            }
            else if (sKeyword == _T("playstep"))
            {
                drawimage.nPlayStep = CUIUtil::String2Int(sKeyvalue);
            }
            else if (sKeyword == _T("frames"))
            {
                drawimage.nFrames = CUIUtil::String2Int(sKeyvalue);
            }

            strValue = strValue.Mid(nKeyValuePos, strValue.GetLength()-(nKeyValuePos));
        }
    }

    return drawimage;
}

CDuiString CUIUtil::DrawImage2String(const TDrawImage& tValue)
{
    CDuiString drawimage;

    if (!tValue.sFile.IsEmpty())
    {
        drawimage.Append(_T("file='"));
        drawimage.Append(tValue.sFile);
        if (!tValue.sRes.IsEmpty())
        {
            drawimage.Append(_T("' res='"));
            drawimage.Append(tValue.sRes);
        }
        if (!tValue.sResType.IsEmpty())
        {
            drawimage.Append(_T("' restype='"));
            drawimage.Append(tValue.sResType);
        }
        if (tValue.tSource.nType == 4)
        {
            drawimage.Append(_T("' source='"));
            drawimage.Append(CUIUtil::Vertex42String(tValue.tSource));
        }
        if (tValue.tDest.nType == 4)
        {
            drawimage.Append(_T("' dest='"));
            drawimage.Append(CUIUtil::Vertex42String(tValue.tDest));
        }
        if (tValue.tCorner.nType == 4)
        {
            drawimage.Append(_T("' corner='"));
            drawimage.Append(CUIUtil::Vertex42String(tValue.tCorner));
        }
        if (tValue.dwMask > 0)
        {
            drawimage.Append(_T("' mask='"));
            drawimage.Append(CUIUtil::Color2String(tValue.dwMask));
        }
        if (tValue.nFade >= 0)
        {
            drawimage.Append(_T("' fade='"));
            drawimage.Append(CUIUtil::Int2String(tValue.nFade));
        }
        drawimage.Append(_T("' hole='"));
        drawimage.Append(CUIUtil::Bool2String(tValue.bHole));
        drawimage.Append(_T("' xtiled='"));
        drawimage.Append(CUIUtil::Bool2String(tValue.bHole));
        drawimage.Append(_T("' ytiled='"));
        drawimage.Append(CUIUtil::Bool2String(tValue.bHole));
        if (tValue.szBlockSize.cx > 0 && tValue.szBlockSize.cy > 0)
        {
            drawimage.Append(_T("' blocksize='"));
            drawimage.Append(CUIUtil::Size2String(tValue.szBlockSize));
        }
        if (tValue.nBlockCount > 0)
        {
            drawimage.Append(_T("' blockcount='"));
            drawimage.Append(CUIUtil::Int2String(tValue.nBlockCount));
        }
        if (tValue.nBlockIndex >= 0)
        {
            drawimage.Append(_T("' blockindex='"));
            drawimage.Append(CUIUtil::Int2String(tValue.nBlockIndex));
        }
        drawimage.Append(_T("' dynamic='"));
        drawimage.Append(CUIUtil::Bool2String(tValue.bDynamic));
        drawimage.Append(_T("' play='"));
        drawimage.Append(CUIUtil::Bool2String(tValue.bPlay));
        CDuiString sPlayMode = _T("");
        switch (tValue.nPlayMode)
        {
        case TDrawImage::DrawImagePlayMode_Once:
            sPlayMode = _T("once");
            break;
        case TDrawImage::DrawImagePlayMode_OnceReset:
            sPlayMode = _T("oncereset");
            break;
        case TDrawImage::DrawImagePlayMode_Loop:
            sPlayMode = _T("loop");
            break;
        case TDrawImage::DrawImagePlayMode_Bounce:
            sPlayMode = _T("bounce");
            break;
        default:
            break;
        }
        if (!sPlayMode.IsEmpty())
        {
            drawimage.Append(_T("' playmode='"));
            drawimage.Append(sPlayMode);
        }
        if (tValue.nPlayFrame >= 0)
        {
            drawimage.Append(_T("' playframe='"));
            drawimage.Append(CUIUtil::Int2String(tValue.nPlayFrame));
        }
        drawimage.Append(_T("' playstep='"));
        drawimage.Append(CUIUtil::Int2String(tValue.nPlayStep));
        if (tValue.nFrames)
        {
            drawimage.Append(_T("' frames='"));
            drawimage.Append(CUIUtil::Int2String(tValue.nFrames));
        }
        drawimage.Append(_T("'"));
    }



    return drawimage;
}

UINT CUIUtil::String2WinVer(const CDuiString& sWinVer)
{
    UINT uWinVer = EM_OS_UNKNOWN;
    if (!sWinVer.IsEmpty())
    {
        if (0 == sWinVer.CompareNoCase(_T("win95")))
        {
            uWinVer = EM_OS_WIN95;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win98")))
        {
            uWinVer = EM_OS_WIN98;
        }
        else if (0 == sWinVer.CompareNoCase(_T("winme")))
        {
            uWinVer = EM_OS_WINME;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win2000")))
        {
            uWinVer = EM_OS_WIN2000;
        }
        else if (0 == sWinVer.CompareNoCase(_T("winxp")))
        {
            uWinVer = EM_OS_WINXP;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win2003")))
        {
            uWinVer = EM_OS_WIN_SERVER2003;
        }
        else if (0 == sWinVer.CompareNoCase(_T("vista")))
        {
            uWinVer = EM_OS_VISTA;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win7")))
        {
            uWinVer = EM_OS_WIN7;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win2008")))
        {
            uWinVer = EM_OS_WINSERVER2008;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win2008r2")))
        {
            uWinVer = EM_OS_WINSERVER2008R2;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win8")))
        {
            uWinVer = EM_OS_WIN8;
        }
        else if (0 == sWinVer.CompareNoCase(_T("win2012")))
        {
            uWinVer = EM_OS_WINSERVER2012;
        }
        else
        {
            uWinVer = EM_OS_UNKNOWN;
        }
    }
    return uWinVer;
}

CDuiString CUIUtil::WinVer2String(UINT uWinVer)
{
    CDuiString sWinVer;

    switch(uWinVer)
    {
    case EM_OS_WIN95:
        sWinVer = _T("win95");
        break;
    case EM_OS_WIN98:
        sWinVer = _T("win98");
        break;
    case EM_OS_WINME:
        sWinVer = _T("winme");
        break;
    case EM_OS_WIN2000:
        sWinVer = _T("win2000");
        break;
    case EM_OS_WINXP:
    case EM_OS_WINXP_HOME:
    case EM_OS_WINXP_PROFESSIONAL:
        sWinVer = _T("winxp");
        break;
    case EM_OS_WIN_SERVER2003:
        sWinVer = _T("win2003");
        break;
    case EM_OS_VISTA:
        sWinVer = _T("vista");
        break;
    case EM_OS_WIN7:
        sWinVer = _T("win7");
        break;
    case EM_OS_WINSERVER2008:
        sWinVer = _T("win2008");
        break;
    case EM_OS_WINSERVER2008R2:
        sWinVer = _T("win2008r2");
        break;
    case EM_OS_WIN8:
        sWinVer = _T("win8");
        break;
    case EM_OS_WINSERVER2012:
        sWinVer = _T("win2012");
        break;
    default:
        sWinVer = _T("unknown");
        break;
    }

    return sWinVer;
}

HWND CUIUtil::GetShadowWindow(HWND wnd)
{
	WindowImplBase* pWindow = dynamic_cast<WindowImplBase*>(reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(wnd, GWLP_USERDATA)));
	if (pWindow && pWindow->GetPaintManager()->GetShadow()->GetShadowWnd())
	{
		return pWindow->GetPaintManager()->GetShadow()->GetShadowWnd();
	}
	return wnd;
}

}