#include "stdafx.h"
#include "UIButton.h"

namespace DuiLib
{
	CButtonUI::CButtonUI()
		: m_uButtonState(0)
		, m_dwHotTextColor(0)
		, m_dwPushedTextColor(0)
		, m_dwFocusedTextColor(0)
		, m_dwHotBkColor(0)
		, m_dwAngle(0)
		, m_bIsEllpse(FALSE)
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
		m_rcEllipseOffSet = { 0 };
	}

	LPCTSTR CButtonUI::GetClass() const
	{
		return _T("ButtonUI");
	}

	LPVOID CButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_BUTTON) == 0 ) return static_cast<CButtonUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CButtonUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
					Activate();
					return;
				}
			}
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 )
			{
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
				if( ::PtInRect(&m_rcItem, event.ptMouse) )
					Activate();
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() ) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
			//	m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER, event.wParam, event.lParam);
				Invalidate();
			}
			// return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
			//	m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE, event.wParam, event.lParam);
				Invalidate();
			}
			// return;
		}
		if( event.Type == UIEVENT_SETCURSOR ) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		CLabelUI::DoEvent(event);
	}

	bool CButtonUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		return true;
	}

	void CButtonUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	//************************************
	// Method:    SetHotBkColor
	// FullName:  CButtonUI::SetHotBkColor
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD dwColor
	// Note:	  
	//************************************
	void CButtonUI::SetHotBkColor( DWORD dwColor )
	{
		m_dwHotBkColor = dwColor;
	}

	//************************************
	// Method:    GetHotBkColor
	// FullName:  CButtonUI::GetHotBkColor
	// Access:    public 
	// Returns:   DWORD
	// Qualifier: const
	// Note:	  
	//************************************
	DWORD CButtonUI::GetHotBkColor() const
	{
		return m_dwHotBkColor;
	}
	//************************************
	// Method:    SetAngle
	// FullName:  CButtonUI::SetAngle
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD dwAngle
	// Note:	  
	//************************************
	void CButtonUI::SetAngle(DWORD dwAngle)
	{
		m_dwAngle = dwAngle;
		Invalidate();
	}

	//************************************
	// Method:    GetAngle
	// FullName:  CButtonUI::GetAngle
	// Access:    public 
	// Returns:   DWORD
	// Qualifier: const
	// Note:	  
	//************************************
	DWORD CButtonUI::GetAngle() const
	{
		return m_dwAngle;
	}

	void CButtonUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}

	DWORD CButtonUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}

	void CButtonUI::SetPushedTextColor(DWORD dwColor)
	{
		m_dwPushedTextColor = dwColor;
	}

	DWORD CButtonUI::GetPushedTextColor() const
	{
		return m_dwPushedTextColor;
	}

	void CButtonUI::SetFocusedTextColor(DWORD dwColor)
	{
		m_dwFocusedTextColor = dwColor;
	}

	DWORD CButtonUI::GetFocusedTextColor() const
	{
		return m_dwFocusedTextColor;
	}

	LPCTSTR CButtonUI::GetNormalImage()
	{
		return m_diNormal.sDrawString;
	}

	void CButtonUI::SetNormalImage(LPCTSTR pStrImage)
	{
		if( m_diNormal.sDrawString == pStrImage && m_diNormal.pImageInfo != NULL ) return;
		m_diNormal.Clear();
		m_diNormal.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetHotImage()
	{
		return m_diHot.sDrawString;
	}

	void CButtonUI::SetHotImage(LPCTSTR pStrImage)
	{
		if( m_diHot.sDrawString == pStrImage && m_diHot.pImageInfo != NULL ) return;
		m_diHot.Clear();
		m_diHot.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetPushedImage()
	{
		return m_diPushed.sDrawString;
	}

	void CButtonUI::SetPushedImage(LPCTSTR pStrImage)
	{
		if( m_diPushed.sDrawString == pStrImage && m_diPushed.pImageInfo != NULL ) return;
		m_diPushed.Clear();
		m_diPushed.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetFocusedImage()
	{
		return m_diFocused.sDrawString;
	}

	void CButtonUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		if( m_diFocused.sDrawString == pStrImage && m_diFocused.pImageInfo != NULL ) return;
		m_diFocused.Clear();
		m_diFocused.sDrawString = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetDisabledImage()
	{
		return m_diDisabled.sDrawString;
	}

	void CButtonUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		if( m_diDisabled.sDrawString == pStrImage && m_diDisabled.pImageInfo != NULL ) return;
		m_diDisabled.Clear();
		m_diDisabled.sDrawString = pStrImage;
		Invalidate();
	}

	//************************************
	// Method:    GetForeImage
	// FullName:  CButtonUI::GetForeImage
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier:
	// Note:	  
	//************************************
	LPCTSTR CButtonUI::GetForeImage()
	{
		return m_diFore.sDrawString;
	}

	//************************************
	// Method:    SetForeImage
	// FullName:  CButtonUI::SetForeImage
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pStrImage
	// Note:	  
	//************************************
	void CButtonUI::SetForeImage( LPCTSTR pStrImage )
	{
		if( m_diFore.sDrawString == pStrImage && m_diFore.pImageInfo != NULL ) return;
		m_diFore.Clear();
		m_diFore.sDrawString = pStrImage;
		Invalidate();
	}

	//************************************
	// Method:    GetHotForeImage
	// FullName:  CButtonUI::GetHotForeImage
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier:
	// Note:	  
	//************************************
	LPCTSTR CButtonUI::GetHotForeImage()
	{
		return m_diHotFore.sDrawString;
	}

	//************************************
	// Method:    SetHotForeImage
	// FullName:  CButtonUI::SetHotForeImage
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pStrImage
	// Note:	  
	//************************************
	void CButtonUI::SetHotForeImage( LPCTSTR pStrImage )
	{
		if( m_diHotFore.sDrawString == pStrImage && m_diHotFore.pImageInfo != NULL ) return;
		m_diHotFore.Clear();
		m_diHotFore.sDrawString = pStrImage;
		Invalidate();
	}

	SIZE CButtonUI::EstimateSize(SIZE szAvailable)
	{
		if (m_bAutoCalcWidth)
		{
			return CLabelUI::EstimateSize(szAvailable);
		}
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotforeimage")) == 0 ) SetHotForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBkColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("hottextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("pushedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("focusedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusedTextColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("Angle")) == 0)
		{
			LPTSTR pstr = NULL;
			DWORD Angle = _tcstoul(pstrValue, &pstr, 16);
			SetAngle(Angle);
		}
		else if (_tcscmp(pstrName, _T("Ellipse")) == 0)
		{
			SetEllpseStatus(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if (_tcscmp(pstrName, _T("Ellipseimage")) == 0)
		{
			SetEllipseImage(pstrValue);
		}
		else if (_tcscmp(pstrName, _T("Ellipsepadding")) == 0)
		{
			LPTSTR pstr = NULL;
			m_rcEllipseOffSet.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			m_rcEllipseOffSet.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			m_rcEllipseOffSet.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			m_rcEllipseOffSet.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		}
		else if (_tcscmp(pstrName, _T("Translatepadding")) == 0)
		{			
			LPTSTR pstr = NULL;
			m_rcTranslate.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			m_rcTranslate.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			m_rcTranslate.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			m_rcTranslate.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CButtonUI::PaintText(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;
		int nLinks = 0;
		int font = m_iFont;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;

		if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
			clrColor = GetPushedTextColor();
		else if (((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0)) {
			clrColor = GetHotTextColor();
			if (m_iHoverFont >= 0)
			{
				font = m_iHoverFont;
			}
		}
		else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
			clrColor = GetFocusedTextColor();

		DrawText(hDC, rc, clrColor, font);
	}

	void CButtonUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;
		if (m_bIsEllpse) m_uButtonState |= UISTATE_ELLIPSE;
		else m_uButtonState &= ~UISTATE_ELLIPSE;

		if ((m_uButtonState & UISTATE_ELLIPSE) != 0)
		{
			m_diNormal.Clear();
			m_diHot.Clear();
			m_diBk.Clear();
			RECT rcPos = GetPos();
			Gdiplus::Graphics graphics(hDC);
			Image img(m_diEllipse.sDrawString);
			int iImgH =  m_rcEllipseOffSet.bottom - m_rcEllipseOffSet.top;
			int iImgW = m_rcEllipseOffSet.right - m_rcEllipseOffSet.left;
			Image *pThumbnail = img.GetThumbnailImage(iImgW, iImgH, NULL, NULL);
			TextureBrush picBrush(pThumbnail);
			Matrix mx;
			mx.Translate(m_rcTranslate.left, m_rcTranslate.top);
			picBrush.SetTransform(&mx);
			graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
			graphics.FillEllipse(&picBrush, rcPos.left + m_rcEllipseOffSet.left, rcPos.top + m_rcEllipseOffSet.top, iImgW, iImgH);
			return;
		}
		else
		{
			if ((m_uButtonState & UISTATE_DISABLED) != 0) {
				if (DrawImage(hDC, m_diDisabled, m_dwAngle)) goto Label_ForeImage;
			}
			else if ((m_uButtonState & UISTATE_PUSHED) != 0) {
				if (!DrawImage(hDC, m_diPushed, m_dwAngle))
					DrawImage(hDC, m_diNormal, m_dwAngle);
				if (DrawImage(hDC, m_diPushedFore, m_dwAngle)) return;
				else goto Label_ForeImage;
			}
			else if ((m_uButtonState & UISTATE_HOT) != 0) {
				if (!DrawImage(hDC, m_diHot, m_dwAngle))
					DrawImage(hDC, m_diNormal, m_dwAngle);
				if (m_dwHotBkColor != 0) {
					CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
					//return;
				}
				if (DrawImage(hDC, m_diHotFore, m_dwAngle)) return;
				else goto Label_ForeImage;
			}
			else if ((m_uButtonState & UISTATE_FOCUSED) != 0) {
				if (DrawImage(hDC, m_diFocused, m_dwAngle)) goto Label_ForeImage;
			}
			DrawImage(hDC, m_diNormal, m_dwAngle);
		}

	Label_ForeImage:
		DrawImage(hDC, m_diFore, m_dwAngle);
	}

	//void CButtonUI::SetEllipseFlag(BOOL bFlag,RECT rc)
	//{
	//	m_bEllipseFlag = bFlag;
	//	m_rcBtnPaint = rc;
	//}

	void CButtonUI::SetEllpseStatus(bool bEllpse /*= false*/)
	{
		m_bIsEllpse = bEllpse;
	}

	void CButtonUI::SetEllipseImage(LPCTSTR pStrImage)
	{
		if (m_diEllipse.sDrawString == pStrImage && m_diEllipse.pImageInfo != NULL) return;
		m_diEllipse.Clear();
		m_diEllipse.sDrawString = pStrImage;
		Invalidate();
	}

	void CButtonUI::SetEllipseOffset(RECT rcOffset)
	{
		m_rcEllipseOffSet = rcOffset;
		Invalidate();
	}

}