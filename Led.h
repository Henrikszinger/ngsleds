#if !defined(AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_)
#define AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
///////////////////////////////////////////////////////////////////////////////

#define LED_SIZE	22			// Led 22 pixels

/////////////////////////////////////////////////////////////////////////////
// CLed window
class CLed : public CStatic
{

protected:
	int m_nLedMode;
	DWORD m_dwPingTimeout;
	BOOL m_bDelayEnabled;
	CBitmap m_LedBitmap;

public:

	enum {
		LED_ON = 0,
		LED_OFF = 1,
	};
	// Construction
public:
	CLed();

public:
	void SetLed(int nMode);
	int GetLedMode() { return m_nLedMode; }
	void OffDelay(DWORD dwTimeout = 1000);

	void LedInit(CDC *pDC);

	// Overrides

	// Implementation
public:
	void DrawLed(CDC *pDC, int nMode);
	virtual ~CLed();


	// Generated message map functions
protected:

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif