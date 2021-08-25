
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

BYTE initflag = 0;

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

	CRect rect;			//hasonl�an a Ledm�trixhoz, itt lesz l�trehozva a "ledsor", meg vannak adva alap "koordin�t�k" egy rect t�pusnak, a ".Create" pedig l�trehozza hozz� a controlt
	int i = 0;
	while (i < 8) {
		rect.left = 12 + (i * 26);
		rect.top = 20;
		rect.right = 34 + (i * 26);
		rect.bottom = 42;
		Ledsor[7-i].Create(NULL, WS_CHILD | WS_VISIBLE, rect, this);
		i++;
	}

	// create masks for reading
	for (char x = 0; x <= 4; x++) {
		if (Filekez::portExt[x] != 0) {  // megn�zi hogy adott "b�jt" null�t�l elt�r�, ha igen a 8 elem� bit arrayb�l k�sz�t egy maszkot
			PortMask[x] = Filekez::portExt[x].to_ulong();
		}
	}

	// if bits are 1 on startup, do an initial draw
	for (char x = 0; x <= 4; x++) {
		DWORD currPortTemp, lastPortTemp;
		if (Filekez::portExt[x] != 0) {
			Agsi.ReadSFR(Filekez::sfraddress[x], &currPortTemp, &lastPortTemp, PortMask[x]);
			currPort[x] = currPortTemp;
			lastPort[x] = lastPortTemp;
		}
		DrawLed();
	}

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

void CngsledsDlg::WatchSFR()		// Az AGSI watchok ezt a f�ggv�nyt h�vj�k meg minden v�ltoz�sn�l, ez a "main" r�sze igaz�b�l a k�dnak ahova maga a m�k�d�s ker�l
{
	
	for (char x = 0; x <= 4; x++) {		// AGSIwatch megh�vta a f�ggv�nyt, el�sz�r fel kell olvasni hogy mi is a v�ltoz�s
		DWORD currPortTemp, lastPortTemp;
		if (Filekez::portExt[x] != 0) {
			Agsi.ReadSFR(Filekez::sfraddress[x], &currPortTemp, &lastPortTemp, PortMask[x]);  // inicializ�l�sn�l l�trehozott portmask alapj�n az ideiglenes currPortTemp �s lastPortTemp v�ltoz�kba olvas be
			currPort[x] = currPortTemp;		// a DWORD t�pusu ideiglenes v�ltoz�b�l �tadjuk a bitset<8> t�pus� currPort v�ltoz�nak, teh�t a port "b�jtok" lesznek felt�ltve
			lastPort[x] = lastPortTemp;
		}
	}

	if (currPort != lastPort) {		// a Ledm�trixt�l elt�r�en nincsenek speci�lis vez�rl� bitek, sima v�ltoz�st figyel�nk
		for (char n = 0; n < 8; n++) {		// 0-7ig vannak a ledek bitjei, a gombokr�l nem olvasunk vissza st�tuszt
			if (currPort[Filekez::portArrayExt[n]][Filekez::bitArrayExt[n]] /*&& !lastPort[Filekez::portArrayExt[n]][Filekez::bitArrayExt[n]]*/) {
				storeLed[n] = 1;
			}
			else {
				storeLed[n] = 0;
			}
		}
			DrawLed();
	}
}

void CngsledsDlg::DrawLed()
{
	//auto now = std::chrono::steady_clock::now();
	//std::chrono::duration<double> diff = now - timeStart;
	auto now = std::chrono::high_resolution_clock::now();		// az inicializ�l�skor ind�tva lett egy timer "timeStart", valamit minden kirajzol�s ut�n "�jraindul" ez a timer "timeStart = now"
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeStart).count();
	
	//if (diff.count() > 0.010) {
	if (duration >= 17) { // ~17ms a 60Hz-re nem t�l pontosan kikalkul�lva, sok jelent�s�ge nincs a pontoss�gnak, l�nyeg hogy 17ms-en bel�l ne legyen t�bb �jrarajzol�s
		timeStart = now;	//mivel ide bel�pve lesz k�perny� rajzol�s, a timer idej�t egyenl�v� tessz�k a jelenlegivel, innent�l sz�m�tja a k�vetkez� 17ms eltelt�t
		for (char n = 0; n < 8; n++) {	// mivel nincs rajzol�s minden v�ltoz�s ut�n, �gy az elt�rolt 1-esbe �ll�tott ledeket rajzoljuk itt ki
			if (storeLed[n]) {
				Ledsor[n].SetLed(CLed::LED_OFF);		
			}
			else {
				Ledsor[n].SetLed(CLed::LED_ON);
				//Ledsor[n].OffDelay(10);
			}
		}
	}
	else {		// sz�ks�g van egy m�sik fajta timerre is, ennek lej�rtakor automatikusan h�v�dik az OnTimer function -- ha a Keilben le�ll a k�d �s a kirajzol�s ut�n a 17ms-en bel�l t�rt�nt m�g v�ltoz�s
		if (endTimerReset)		// azt is meg kell jelen�teni valahogy, mivel nincs CLK bit, ha 50ms-ig nem t�rt�nik DrawLed h�v�s mert pl le�llt a k�d, akkor megh�v�dik m�g utolj�ra
		{						// ha egyszer bel�p az else �gba, akkor minden bel�p�skor reseteli a timert, nyilv�n ha 50ms alatt nem j�tt reset akkor h�v�dik az OnTimer fuction
			KillTimer(2);
		}
		endTimerReset = true;
		SetTimer(2, 50, NULL);
	}

	/*
	for (char n = 0; n < 8; n++) {
		if (storeLed[n]) {
			Ledsor[n].SetLed(!currPort[Filekez::portArrayExt[n]][Filekez::bitArrayExt[n]]);
		}
		else if (diff.count() > 0.010) {
			timeStart = now;
			if (!(Ledsor[n].GetLedMode())) {
				Ledsor[n].OffDelay(5);
			}
		}
	}
	*/
}

void CngsledsDlg::OnTimer(UINT nIDEvent)		// ez h�v�dik meg 50ms ut�n, ha nem t�rt�nt DrawLed h�v�s
{

	if (nIDEvent == 2)
	{
		KillTimer(nIDEvent);
		endTimerReset = false;
		DrawLed();
	}

	//CStatic::OnTimer(nIDEvent);
}

void CngsledsDlg::OnBnClickedCheck1()		//a bit/portArrayExternal 8-15ig vannak a gombok "bitjei" elt�rolva
{
	//buttons[0] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[8]);		// ha az els� gombr�l tudjuk hogy pl 7. bitre lett configolva, akkor ez egy ideiglenes 8bites v�ltoz� 7. bitj�t beseteli, egy �talak�t�s t�rt�nik l�nyeg�ben
	if (((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()) {		//gomb �llapot vizsg�lata
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[8]], 0, btnTemp.to_ulong());	//az els� gombhoz tartoz� port 0-4ig megadja, ez alapj�n lesz kiv�lasztva melyik porthoz tartoz� SFR �rt�kre �rjon 0-t
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[8]], btnTemp.to_ulong(), btnTemp.to_ulong());	// a to_ulong egy sima castol�s, bitsetet csak �gy nem lehet kik�ldeni
	}
	btnTemp.reset(Filekez::bitArrayExt[8]);	// az el�bb besetelt bitet vissza teszi 0-ba
}

void CngsledsDlg::OnBnClickedCheck2()
{
	//buttons[1] = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[9]);
	if (((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck()) {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[9]], 0, btnTemp.to_ulong());
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[9]], btnTemp.to_ulong(), btnTemp.to_ulong());
	}
	btnTemp.reset(Filekez::bitArrayExt[9]);
}

void CngsledsDlg::OnBnClickedCheck3()
{
	//buttons[2] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[10]);
	if (((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()) {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[10]], 0, btnTemp.to_ulong());
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[10]], btnTemp.to_ulong(), btnTemp.to_ulong());
	}
	btnTemp.reset(Filekez::bitArrayExt[10]);
}

void CngsledsDlg::OnBnClickedCheck4()
{
	//buttons[3] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[11]);
	if (((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()) {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[11]], 0, btnTemp.to_ulong());
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[11]], btnTemp.to_ulong(), btnTemp.to_ulong());
	}
	btnTemp.reset(Filekez::bitArrayExt[11]);
}

void CngsledsDlg::OnBnClickedCheck5()
{
	//buttons[4] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[12]);
	if (((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()) {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[12]], 0, btnTemp.to_ulong());
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[12]], btnTemp.to_ulong(), btnTemp.to_ulong());
	}
	btnTemp.reset(Filekez::bitArrayExt[12]);
}

void CngsledsDlg::OnBnClickedCheck6()
{
	//buttons[5] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[13]);
	if (((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()) {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[13]], 0, btnTemp.to_ulong());
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[13]], btnTemp.to_ulong(), btnTemp.to_ulong());
	}
	btnTemp.reset(Filekez::bitArrayExt[13]);
}

void CngsledsDlg::OnBnClickedCheck7()
{
	//buttons[6] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[14]);
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()) {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[14]], 0, btnTemp.to_ulong());
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[14]], btnTemp.to_ulong(), btnTemp.to_ulong());
	}
	btnTemp.reset(Filekez::bitArrayExt[14]);
}

void CngsledsDlg::OnBnClickedCheck8()
{
	//buttons[7] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[15]);
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()) {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[15]], 0, btnTemp.to_ulong());
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[15]], btnTemp.to_ulong(), btnTemp.to_ulong());
	}
	btnTemp.reset(Filekez::bitArrayExt[15]);
}
