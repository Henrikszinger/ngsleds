
#include "stdafx.h"
#include "resource.h"
#include "Led.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TIMER_ID_PING		1		// Timer OffDelay ID

CLed::CLed()
{
	m_LedBitmap.LoadBitmap(IDB_LED);
	m_nLedMode = LED_OFF;
}

CLed::~CLed()
{
	VERIFY(m_LedBitmap.DeleteObject());
}


BEGIN_MESSAGE_MAP(CLed, CStatic)
	
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//ON_WM_ERASEBKGND()
	
END_MESSAGE_MAP()

void CLed::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawLed(&dc, m_nLedMode);

	// Do not call CStatic::OnPaint() for painting messages
}


///////////////////////////////////////////////////////////////////////////////
// Name:		SetLed
// Description:	This method will draw the LED to the specified DC.
//
// Entry:
//				CDC *pDC - DC to draw to
//				int iMode - where mode is defined by:
//					LED_ON
//					LED_OFF
///////////////////////////////////////////////////////////////////////////////
void CLed::DrawLed(CDC *pDC, int nMode)
{

	CRect rect;
	GetClientRect(&rect);
	
	if (rect.Width() >= LED_SIZE && rect.Height() >= LED_SIZE)
	{
		int nWidth = rect.Width();
		int nHeight = rect.Height();
		rect.left += (nWidth - LED_SIZE) / 2;
		rect.top += (nHeight - LED_SIZE) / 2;
		rect.right -= (nWidth - LED_SIZE) / 2;
		rect.bottom -= (nHeight - LED_SIZE) / 2;
	}

	CBitmap *pBitmap = &m_LedBitmap;
	CDC srcDC;
	
	srcDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = srcDC.SelectObject(pBitmap);

	pDC->BitBlt(rect.left, rect.top, LED_SIZE, LED_SIZE, &srcDC, nMode*LED_SIZE, 0, SRCCOPY);
	srcDC.SelectObject(pOldBitmap);

	VERIFY(srcDC.DeleteDC());
	
}


// Name:		SetLed
// Description:	This method will draw and set led parameters.
//
//
//				int iMode - where mode is defined by:
//					LED_ON
//					LED_OFF
void CLed::SetLed(int nMode)
{
	m_nLedMode = nMode;

	CDC *pDC;
	pDC = GetDC();
	DrawLed(pDC, m_nLedMode);
	ReleaseDC(pDC);
}

void CLed::LedInit(CDC *pDC) {
	
}

///////////////////////////////////////////////////////////////////////////////
// Name:		OffDelay
// Description:	This method will turn the led on for dwTimeout milliseconds and
//				then turn it off.
//
// Entry:		DWORD dwTimeout - Time out in  milliseconds
///////////////////////////////////////////////////////////////////////////////
void CLed::OffDelay(DWORD dwTimeout)
{

	/*
	if (m_bDelayEnabled == TRUE)
	{
		KillTimer(TIMER_ID_PING);
	}
	*/
	//m_bDelayEnabled = TRUE;

	SetTimer(TIMER_ID_PING, dwTimeout, NULL);
	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CLed::OnTimer(UINT nIDEvent)
{
	
		if (nIDEvent == TIMER_ID_PING)
		{
			SetLed(CLed::LED_OFF);
			KillTimer(nIDEvent);
			//m_bDelayEnabled = FALSE;
		}

		CStatic::OnTimer(nIDEvent);
	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL CLed::OnEraseBkgnd(CDC* pDC)
{
	pDC->SelectStockObject(LTGRAY_BRUSH);
	CRect rect;
	GetClientRect(&rect);

	pDC->Rectangle(&rect);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
