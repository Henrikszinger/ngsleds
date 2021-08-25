
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

	CRect rect;			//hasonlóan a Ledmátrixhoz, itt lesz létrehozva a "ledsor", meg vannak adva alap "koordináták" egy rect típusnak, a ".Create" pedig létrehozza hozzá a controlt
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
		if (Filekez::portExt[x] != 0) {  // megnézi hogy adott "bájt" nullától eltérõ, ha igen a 8 elemû bit arraybõl készít egy maszkot
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

void CngsledsDlg::WatchSFR()		// Az AGSI watchok ezt a függvényt hívják meg minden változásnál, ez a "main" része igazából a kódnak ahova maga a mûködés kerül
{
	
	for (char x = 0; x <= 4; x++) {		// AGSIwatch meghívta a függvényt, elõször fel kell olvasni hogy mi is a változás
		DWORD currPortTemp, lastPortTemp;
		if (Filekez::portExt[x] != 0) {
			Agsi.ReadSFR(Filekez::sfraddress[x], &currPortTemp, &lastPortTemp, PortMask[x]);  // inicializálásnál létrehozott portmask alapján az ideiglenes currPortTemp és lastPortTemp változókba olvas be
			currPort[x] = currPortTemp;		// a DWORD típusu ideiglenes változóból átadjuk a bitset<8> típusú currPort változónak, tehát a port "bájtok" lesznek feltöltve
			lastPort[x] = lastPortTemp;
		}
	}

	if (currPort != lastPort) {		// a Ledmátrixtól eltérõen nincsenek speciális vezérlõ bitek, sima változást figyelünk
		for (char n = 0; n < 8; n++) {		// 0-7ig vannak a ledek bitjei, a gombokról nem olvasunk vissza státuszt
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
	auto now = std::chrono::high_resolution_clock::now();		// az inicializáláskor indítva lett egy timer "timeStart", valamit minden kirajzolás után "újraindul" ez a timer "timeStart = now"
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeStart).count();
	
	//if (diff.count() > 0.010) {
	if (duration >= 17) { // ~17ms a 60Hz-re nem túl pontosan kikalkulálva, sok jelentõsége nincs a pontosságnak, lényeg hogy 17ms-en belül ne legyen több újrarajzolás
		timeStart = now;	//mivel ide belépve lesz képernyõ rajzolás, a timer idejét egyenlõvé tesszük a jelenlegivel, innentõl számítja a következõ 17ms elteltét
		for (char n = 0; n < 8; n++) {	// mivel nincs rajzolás minden változás után, így az eltárolt 1-esbe állított ledeket rajzoljuk itt ki
			if (storeLed[n]) {
				Ledsor[n].SetLed(CLed::LED_OFF);		
			}
			else {
				Ledsor[n].SetLed(CLed::LED_ON);
				//Ledsor[n].OffDelay(10);
			}
		}
	}
	else {		// szükség van egy másik fajta timerre is, ennek lejártakor automatikusan hívódik az OnTimer function -- ha a Keilben leáll a kód és a kirajzolás után a 17ms-en belül történt még változás
		if (endTimerReset)		// azt is meg kell jeleníteni valahogy, mivel nincs CLK bit, ha 50ms-ig nem történik DrawLed hívás mert pl leállt a kód, akkor meghívódik még utoljára
		{						// ha egyszer belép az else ágba, akkor minden belépéskor reseteli a timert, nyilván ha 50ms alatt nem jött reset akkor hívódik az OnTimer fuction
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

void CngsledsDlg::OnBnClickedCheck1()		//a bit/portArrayExternal 8-15ig vannak a gombok "bitjei" eltárolva
{
	//buttons[0] = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	btnTemp.set(Filekez::bitArrayExt[8]);		// ha az elsõ gombról tudjuk hogy pl 7. bitre lett configolva, akkor ez egy ideiglenes 8bites változó 7. bitjét beseteli, egy átalakítás történik lényegében
	if (((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()) {		//gomb állapot vizsgálata
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[8]], 0, btnTemp.to_ulong());	//az elsõ gombhoz tartozó port 0-4ig megadja, ez alapján lesz kiválasztva melyik porthoz tartozó SFR értékre írjon 0-t
	}
	else {
		Agsi.WriteSFR(Filekez::sfraddress[Filekez::portArrayExt[8]], btnTemp.to_ulong(), btnTemp.to_ulong());	// a to_ulong egy sima castolás, bitsetet csak úgy nem lehet kiküldeni
	}
	btnTemp.reset(Filekez::bitArrayExt[8]);	// az elõbb besetelt bitet vissza teszi 0-ba
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
