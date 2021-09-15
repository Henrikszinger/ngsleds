
// ngsledsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "framework.h"
#include "ngsleds.h"
#include "ngsledsDlg.h"
#include "afxdialogex.h"
#include "AGSI.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/

// CngsledsDlg dialog

static void PeriUpdate(void);
static void PeriKill(AGSIDLGD *pM);
static void PeriDisp(AGSIMENU *pM);

CngsledsDlg *pCPeriDialog = NULL;

AGSIDLGD PeriDlg = { 0,NULL,NULL, {-1,-1,-1,-1}, PeriUpdate, PeriKill };
AGSIMENU PeriMenu = { 1,"&NGS_LEDS", PeriDisp, 0, IDD_NGSLEDS_DIALOG, &PeriDlg };

static void PeriUpdate(void) {            // Update Function
	if (pCPeriDialog) pCPeriDialog->Update();
}

static void PeriKill(AGSIDLGD *pM) {      // Kill Function
	if (pCPeriDialog == NULL) return;
	pCPeriDialog->SendMessage(WM_CLOSE);
	pCPeriDialog = NULL;
	pM->iOpen = 0;
	pM->hw = NULL;
}

static void PeriDisp(AGSIMENU *pM) {
	if (pM->pDlg->hw != NULL) {              // created
		PeriKill(pM->pDlg);                    // close
	}
	else {
		AFX_MANAGE_STATE(AfxGetStaticModuleState()); // -- not necessary.
		pCPeriDialog = new CngsledsDlg(pM, NULL);     // modeless construction
		if (pCPeriDialog != NULL) {                   // construction was Ok.
			pM->pDlg->hw = pCPeriDialog->m_hWnd;         // Dialog handle
		}
	}
}

/*
CngsledsDlg::CngsledsDlg(AGSIMENU * pMen, CWnd * pWnd)
	: CDialogEx(IDD_NGSLEDS_DIALOG, pWnd)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pCPeriDialog = this;
} */

CngsledsDlg::CngsledsDlg(AGSIMENU * pMen, CWnd * pWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	pM = pMen;                     // save DYM-Descriptor locally.
	Create(IDD_NGSLEDS_DIALOG, pWnd);
	pCPeriDialog = this;

}

CngsledsDlg::~CngsledsDlg()
{
}

void CngsledsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CngsledsDlg, CDialog)
	//ON_WM_PAINT()
	//ON_WM_QUERYDRAGICON()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK8, &CngsledsDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK7, &CngsledsDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK6, &CngsledsDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK5, &CngsledsDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK4, &CngsledsDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK3, &CngsledsDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK2, &CngsledsDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK1, &CngsledsDlg::OnBnClickedCheck8)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CngsledsDlg message handlers

void CngsledsDlg::Update()
{
}

void CngsledsDlg::PostNcDestroy()
{
	delete this;              // delete the new'ed object
	pCPeriDialog = NULL;      // clear external Object pointer here.

	//CDialog::PostNcDestroy();
}

BOOL CngsledsDlg::OnInitDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	// Restore Position (Only moving without resizing)
	if (PeriDlg.rc.left != -1) {
		SetWindowPos(NULL,                        /* placement order - not used */
			PeriDlg.rc.left,             /* left */
			PeriDlg.rc.top,              /* top  */
			0,                           /* width - not used */
			0,                           /* height - not used */
			SWP_NOSIZE | SWP_NOZORDER);  /* flags */
	}

	CRect rect;			
	int i = 0;
	while (i < 8) {
		rect.left = 12 + (i * 26);
		rect.top = 20;
		rect.right = 34 + (i * 26);
		rect.bottom = 42;
		Ledsor[7-i].Create(NULL, WS_CHILD | WS_VISIBLE, rect, this);
		i++;
	}

	// do an initial draw
	WatchSFR();

	 //timeStart = std::chrono::steady_clock::now();
	 timeStart = std::chrono::high_resolution_clock::now();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CngsledsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	switch (nState) {
	case WA_INACTIVE:
		Agsi.HandleFocus(NULL);      // Clear Modeless Handle
		break;
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		Agsi.HandleFocus(m_hWnd);    // Set Modeless Handle
		break;
	}
}

void CngsledsDlg::OnClose()
{

	GetWindowRect(&pM->pDlg->rc);  // save Window position
	pM->pDlg->hw = NULL;            // clear m_hWnd
	pCPeriDialog = NULL;
	DestroyWindow();                //--- modeless

	//CDialog::OnClose();
}

void CngsledsDlg::WatchSFR()		
{
	DWORD tempCurrent, tempLast;

	if (LEDSWatchType) {
		Agsi.ReadSFR(sfraddress[LEDSWatchAddress],&tempCurrent,&tempLast,0xFF);

		if (tempCurrent != tempLast) {
			currRead = tempCurrent;
			lastRead = tempLast;

			for (char n = 0; n < 8; n++) {
				storeLed[n] |= currRead[n];
			}
			DrawLed();
		}
	}

	else {
		BYTE justonebyte = 0xFF;
		Agsi.ReadMemory(LEDSWatchAddress, 1, &justonebyte);
		currRead = ~justonebyte;

		for (char n = 0; n < 8; n++) {
			storeLed[n] |= currRead[n];
		}
		DrawLed();

	}
}

void CngsledsDlg::DrawLed()
{
	//auto now = std::chrono::steady_clock::now();
	//std::chrono::duration<double> diff = now - timeStart;
	auto now = std::chrono::high_resolution_clock::now();		
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeStart).count();
	
	//if (diff.count() > 0.010) {
	if (duration >= 17) { 
		timeStart = now;	
		for (char n = 0; n < 8; n++) {	
			if (!storeLed[n]) {
				Ledsor[n].SetLed(CLed::LED_OFF);		
			}
			else {
				Ledsor[n].SetLed(CLed::LED_ON);
				//Ledsor[n].OffDelay(10);
			}
			storeLed[n] = 0;
		}
	}
	else {		
		if (endTimerReset)		
		{						
			KillTimer(2);
		}
		endTimerReset = true;
		SetTimer(2, 50, NULL);
	}

}

void CngsledsDlg::OnTimer(UINT nIDEvent)		// ez hívódik meg 50ms után, ha nem történt DrawLed hívás
{

	if (nIDEvent == 2)
	{
		KillTimer(nIDEvent);
		endTimerReset = false;
		DrawLed();
	}

	//CStatic::OnTimer(nIDEvent);
}

void CngsledsDlg::OnBnClickedCheck1()		
{
	HandleButton(0, IDC_CHECK8);
}

void CngsledsDlg::OnBnClickedCheck2()
{
	HandleButton(1, IDC_CHECK7);
}

void CngsledsDlg::OnBnClickedCheck3()
{
	HandleButton(2, IDC_CHECK6);
}

void CngsledsDlg::OnBnClickedCheck4()
{
	HandleButton(3, IDC_CHECK5);
}

void CngsledsDlg::OnBnClickedCheck5()
{
	HandleButton(4, IDC_CHECK4);
}

void CngsledsDlg::OnBnClickedCheck6()
{
	HandleButton(5, IDC_CHECK3);
}

void CngsledsDlg::OnBnClickedCheck7()
{
	HandleButton(6, IDC_CHECK2);
}

void CngsledsDlg::OnBnClickedCheck8()
{
	HandleButton(7, IDC_CHECK1);
}

void CngsledsDlg::HandleButton(int number,int btnID)
{
	if (NGSWatchType) {

		if (((CButton*)GetDlgItem(btnID))->GetCheck()) {
			Agsi.WriteSFR(sfraddress[NGSWatchAddress], 0x00, (0x01 << number));
		}
		else {
			Agsi.WriteSFR(sfraddress[NGSWatchAddress], 0xFF, (0x01 << number));
		}
	}

	else {
		BYTE justonebyte = 0xFF;
		Agsi.ReadMemory(NGSWatchAddress, 1, &justonebyte);
		
		if (((CButton*)GetDlgItem(btnID))->GetCheck()) {
			justonebyte &= ~(0x01 << number);
			Agsi.WriteMemory(NGSWatchAddress, 1, &justonebyte);
		}
		else {
			justonebyte |= (0x01 << number);
			Agsi.WriteMemory(NGSWatchAddress, 1, &justonebyte);
		}
	}
}