#include "StdAfx.h"
///////////////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z, unsigned int len, DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib
{
	//write by wangji
	struct CGifAnimUI::Imp
	{
		bool				m_bRealStop			;//Íâ²¿Í£Ö¹ÁË
		bool				m_bLoadImg			;//ÊÇ·ñ¼ÓÔØ¹ýÍ¼Æ¬
		bool				m_bTimer			;//ÊÇ·ñÆô¶¯¶¨Ê±Æ÷
		bool				m_bAutoStart		;//ÊÇ·ñ×Ô¶¯¿ªÊ¼
		bool				m_bLoop;//ÊÇ·ñ×Ô¶¯¿ªÊ¼
		int					m_nDelay			;//Ñ­»·ºÁÃëÊý
		UINT				m_nFrameCount		;//gifÍ¼Æ¬×ÜÖ¡Êý
		UINT				m_nFramePosition	;//µ±Ç°·Åµ½µÚ¼¸Ö¡
		Image*				m_pGifImage			;//gif¶ÔÏó
		PropertyItem*		m_pPropertyItem		;//Ö¡ÓëÖ¡Ö®¼ä¼ä¸ôÊ±¼ä
		CPaintManagerUI*&	m_pManager			;
		CGifAnimUI*			m_pOwer				;//ÓµÓÐÕß
		Imp(CPaintManagerUI* & pManager):m_pManager(pManager),
		m_bLoadImg(false),m_bTimer(false),
		m_nDelay(100),m_pGifImage(NULL),m_nFrameCount(0U),
		m_pPropertyItem(NULL),m_nFramePosition(0U),
		m_pOwer(NULL),m_bRealStop(false),m_bAutoStart(true), m_bLoop(true)
		{
		}
		void SetOwer(CGifAnimUI *pOwer)
		{
			m_pOwer = pOwer;
		}
		~Imp()
		{
			if ( m_pGifImage != NULL )
			{
				delete m_pGifImage;
				m_pGifImage = NULL;
			}
			if ( m_pPropertyItem != NULL )
			{
				delete[] m_pPropertyItem ;
				m_pPropertyItem = NULL;
			}
		}
		inline void CheckLoadImage()
		{
			if(!m_bLoadImg)
				LoadGifImage();
		}
		inline bool IsLoadImage(){return m_bLoadImg;}
		virtual void LoadGifImage()
		{
			CDuiString sImag = m_pOwer->GetBkImage();
			m_bLoadImg = true;
			
			//m_pGifImage	=	CRenderEngine::LoadGifImage(STRINGorID(sImag),0, 0);
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			//if(1)
			if (CPaintManagerUI::GetResourceZip().IsEmpty())
			{
				sFile += sImag;
				m_pGifImage = new Image(sFile.GetData());
			}
			else 
			{
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if (CPaintManagerUI::IsCachedResourceZip()) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				else hz = OpenZip((void*)sFile.GetData(), 0, 2);
				if (hz == NULL) return ;
				ZIPENTRY ze;
				int i;
				if (FindZipItem(hz, sImag, true, &i, &ze) != 0) return;// _Failed(_T("Could not find ziped file"));
				DWORD dwSize = ze.unc_size;
				if (dwSize == 0) return;// _Failed(_T("File is empty"));
				//if (dwSize > 4096 * 1024) return //_Failed(_T("File too large"));
				BYTE* pByte = new BYTE[dwSize];
				int res = UnzipItem(hz, i, pByte, dwSize, 3);
				if (res != 0x00000000 && res != 0x00000600) {
					delete[] pByte;
					if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
					return;// _Failed(_T("Could not unzip file"));
				}
				if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
				
				HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize);
				void *  pData = GlobalLock(hGlobal);
				memcpy(pData, pByte, dwSize);
				delete[] pByte;
				GlobalUnlock(hGlobal);

				IStream *  pStream = NULL;
				if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
				{
					m_pGifImage = new Image(pStream);
					pStream->Release();
				}
				//GlobalFree(hGlobal);
				
			}

			if ( NULL == m_pGifImage ) return;
			const UINT nCount	=	m_pGifImage->GetFrameDimensionsCount();
			ASSERT(nCount > 0);
			if(0 == nCount)return;
			GUID* pDimensionIDs	=	new GUID[ nCount ];
			m_pGifImage->GetFrameDimensionsList( pDimensionIDs, nCount );
			m_nFrameCount	=	m_pGifImage->GetFrameCount( &pDimensionIDs[0] );
			const int nSize		=	m_pGifImage->GetPropertyItemSize( PropertyTagFrameDelay );
			if(0 == nSize)
			{
				delete  []pDimensionIDs;
				return;
			}
			m_pPropertyItem	= new PropertyItem[ nSize ];
			m_pGifImage->GetPropertyItem( PropertyTagFrameDelay, nSize, m_pPropertyItem );
			delete  []pDimensionIDs;
			pDimensionIDs = NULL;

			m_nDelay = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
			if (m_nDelay <= 0 ) 
				m_nDelay = 100;
			if (!m_bAutoStart)
				m_bRealStop = true;
			if(m_bAutoStart && m_pOwer->IsVisible())
				StartAnim();
		}
		void SetAutoStart(bool bAuto)
		{
			m_bAutoStart = bAuto;
		}
		void SetAutoLoop(bool bloop)
		{
			m_bLoop = bloop;
		}
		void StartAnim()
		{
			if(!m_bTimer)
			{
				if(!IsLoadImage())
					LoadGifImage();
				if(m_pGifImage)
					m_nFramePosition = 0;
					m_pManager->SetTimer( m_pOwer, EVENT_TIEM_ID, m_nDelay );
				m_bTimer = true;
			}
		}
		void StopAnim(bool bGoFirstFrame)//bGoFirstFrame ÊÇ·ñÅÜµ½µÚÒ»Ö¡
		{
			if(m_bTimer)
			{
				m_pManager->KillTimer( m_pOwer, EVENT_TIEM_ID );
				m_bTimer = false;
			}
		}
		void EventOnTimer(const WPARAM idEvent )
		{
			if ( idEvent != EVENT_TIEM_ID )
				return;
			if(!m_pOwer->IsVisible())return;
			GUID pageGuid = Gdiplus::FrameDimensionTime;
			m_pGifImage->SelectActiveFrame(&pageGuid, m_nFramePosition++);
			if(m_nFramePosition >= m_nFrameCount){
				if (m_bLoop)
				{
					m_nFramePosition = 0;
				}
				else
				{
					m_pManager->KillTimer(m_pOwer, EVENT_TIEM_ID);
					m_bTimer = false;
				}
			}
			m_pOwer->Invalidate();
		}
		void DrawFrame( HDC hDC,const RECT& rcPaint,const RECT &rcItem)
		{
			if ( NULL == hDC || NULL == m_pGifImage ) return;
			Graphics graphics( hDC );
			graphics.DrawImage( m_pGifImage, m_pOwer->m_rcItem.left, m_pOwer->m_rcItem.top,m_pOwer->GetWidth(),m_pOwer->GetHeight() );
		}
		void EventSetVisible(bool bVisible)
		{
			if(bVisible)
			{
				if(!m_bRealStop)
					StartAnim();
			}
			else
				StopAnim(true);
		}
	};
	CGifAnimUI::CGifAnimUI(void):m_pImp(new CGifAnimUI::Imp(m_pManager))
	{
		this;
		m_pImp->SetOwer(this);
	}
	CGifAnimUI::~CGifAnimUI(void)
	{
		m_pImp->StopAnim(false);
		delete m_pImp;
		m_pImp = nullptr;
	}

	//************************************
	// Method:    GetClass
	// FullName:  CGifAnimUI::GetClass
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier: const
	// Note:	  
	//************************************
	LPCTSTR CGifAnimUI::GetClass() const
	{
		return _T("GifAnimUI");
	}

	//************************************
	// Method:    GetInterface
	// FullName:  CGifAnimUI::GetInterface
	// Access:    public 
	// Returns:   LPVOID
	// Qualifier:
	// Parameter: LPCTSTR pstrName
	// Note:	  
	//************************************
	LPVOID CGifAnimUI::GetInterface( LPCTSTR pstrName )
	{
			if( _tcscmp(pstrName, _T("GifAnim")) == 0 ) 
				return static_cast<CGifAnimUI*>(this);
			return CLabelUI::GetInterface(pstrName);
	}
	void CGifAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("auto")) == 0 ) 
			m_pImp->SetAutoStart(_tcscmp(pstrValue, _T("true")) == 0);
		if (_tcscmp(pstrName, _T("loop")) == 0)
			m_pImp->SetAutoLoop(_tcscmp(pstrValue, _T("true")) == 0);
		else
			__super::SetAttribute(pstrName, pstrValue);
	}
	void CGifAnimUI::Init()
	{
		__super::Init();
		m_pImp->CheckLoadImage();
	}
	void CGifAnimUI::SetVisible(bool bVisible /*= true*/)
	{
		__super::SetVisible(bVisible);
		m_pImp->EventSetVisible(bVisible);
	}
	void CGifAnimUI::SetInternVisible(bool bVisible/* = true*/)
	{
		__super::SetInternVisible(bVisible);
		m_pImp->EventSetVisible(bVisible);
	}
	//************************************
	// Method:    DoPaint
	// FullName:  CGifAnimUI::DoPaint
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: HDC hDC
	// Parameter: const RECT & rcPaint
	// Note:	  
	//************************************
	void CGifAnimUI::DoPaint( HDC hDC, const RECT& rcPaint )
	{
		if( !::IntersectRect( &m_rcPaint, &rcPaint, &m_rcItem ) ) return;
		m_pImp->DrawFrame( hDC,rcPaint,m_rcItem);
	}

	//************************************
	// Method:    DoEvent
	// FullName:  CGifAnimUI::DoEvent
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: TEventUI & event
	// Note:	  
	//************************************
	void CGifAnimUI::DoEvent( TEventUI& event )
	{
		this;
		WPARAM nID = event.wParam;
		if( event.Type == UIEVENT_TIMER )
			m_pImp->EventOnTimer(nID);
		else  if (event.Type == UIEVENT_BUTTONDOWN)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_BUTTONDOWN, event.wParam, event.lParam);
		}
		else if (event.Type == UIEVENT_MOUSEENTER) {
			//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		else if (event.Type == UIEVENT_MOUSEMOVE) {
			//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}

	}
	void CGifAnimUI::StartAnim()
	{
		if (m_pImp)
		{
			m_pImp->m_bRealStop = false;
			m_pImp->StartAnim();
		}
	}
	void CGifAnimUI::StopAnim()
	{
		if (m_pImp)
		{
			m_pImp->m_bRealStop = true;
			m_pImp->StopAnim(true);
		}
	}
}
