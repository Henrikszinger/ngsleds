
// ngsledsDlg.h : header file
//

#pragma once

#include "Led.h"
#include "AGSI.h"
#include <chrono>
#include <bitset>

// CngsledsDlg dialog
class CngsledsDlg : public CDialog
{
// Construction
public:
	//CngsledsDlg(CWnd* pParent = nullptr);	// standard constructor
	CngsledsDlg(AGSIMENU* pMen, CWnd* pWnd);
	void Update(void);
	AGSIMENU *pM;
	virtual ~CngsledsDlg();
	CngsledsDlg() = default;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NGSLEDS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	//HICON m_hIcon;
	virtual void PostNcDestroy();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	virtual void WatchSFR();
	CLed Ledsor[8];
	BOOL buttons[8] = { 0 };
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	std::bitset<8> currRead = 0xFF;
	std::bitset<8> lastRead = 0xFF;
	std::chrono::time_point<std::chrono::steady_clock> timeStart;
	bool storeLed[8] = { 0 };
	bool endTimerReset = false;
	bool doPrint = false;

private:
	void HandleButton(int number, int btnID);
	virtual void DrawLed();
	std::bitset<8> btnTemp = 0;
	
};

extern AGSIMENU PeriMenu;
extern BYTE initflag;
extern CngsledsDlg* pCPeriDialog;