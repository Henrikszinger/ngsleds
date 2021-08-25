// ngsleds.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "framework.h"
#include "ngsleds.h"
#include "ngsledsDlg.h"
#include "AGSI.h"
#include <filesystem>		// filesystem namespace, requires C++17

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CngsledsApp

BEGIN_MESSAGE_MAP(CngsledsApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


AGSICONFIG  AgsiConfig;
struct AGSIFUNCS   Agsi;

DWORD GetFunctionPointers(void) {           // get all function pointers

	Agsi.DefineSFR = (tAgsiDefineSFR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiDefineSFR");
	Agsi.DefineVTR = (tAgsiDefineVTR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiDefineVTR");
	Agsi.DeclareInterrupt = (tAgsiDeclareInterrupt)GetProcAddress(AgsiConfig.m_hInstance, "AgsiDeclareInterrupt");
	Agsi.SetWatchOnSFR = (tAgsiSetWatchOnSFR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiSetWatchOnSFR");
	Agsi.SetWatchOnVTR = (tAgsiSetWatchOnVTR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiSetWatchOnVTR");
	Agsi.SetWatchOnMemory = (tAgsiSetWatchOnMemory)GetProcAddress(AgsiConfig.m_hInstance, "AgsiSetWatchOnMemory");
	Agsi.CreateTimer = (tAgsiCreateTimer)GetProcAddress(AgsiConfig.m_hInstance, "AgsiCreateTimer");
	Agsi.DefineMenuItem = (tAgsiDefineMenuItem)GetProcAddress(AgsiConfig.m_hInstance, "AgsiDefineMenuItem");

	Agsi.WriteSFR = (tAgsiWriteSFR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiWriteSFR");
	Agsi.ReadSFR = (tAgsiReadSFR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiReadSFR");
	Agsi.WriteVTR = (tAgsiWriteVTR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiWriteVTR");
	Agsi.ReadVTR = (tAgsiReadVTR)GetProcAddress(AgsiConfig.m_hInstance, "AgsiReadVTR");
	Agsi.SetSFRReadValue = (tAgsiSetSFRReadValue)GetProcAddress(AgsiConfig.m_hInstance, "AgsiSetSFRReadValue");
	Agsi.ReadMemory = (tAgsiReadMemory)GetProcAddress(AgsiConfig.m_hInstance, "AgsiReadMemory");
	Agsi.WriteMemory = (tAgsiWriteMemory)GetProcAddress(AgsiConfig.m_hInstance, "AgsiWriteMemory");
	Agsi.GetLastMemoryAddress = (tAgsiGetLastMemoryAddress)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetLastMemoryAddress");
	Agsi.IsSimulatorAccess = (tAgsiIsSimulatorAccess)GetProcAddress(AgsiConfig.m_hInstance, "AgsiIsSimulatorAccess");
	Agsi.SetTimer = (tAgsiSetTimer)GetProcAddress(AgsiConfig.m_hInstance, "AgsiSetTimer");
	Agsi.GetStates = (tAgsiGetStates)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetStates");
	Agsi.GetProgramCounter = (tAgsiGetProgramCounter)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetProgramCounter");
	Agsi.IsInInterrupt = (tAgsiIsInInterrupt)GetProcAddress(AgsiConfig.m_hInstance, "AgsiIsInInterrupt");
	Agsi.IsSleeping = (tAgsiIsSleeping)GetProcAddress(AgsiConfig.m_hInstance, "AgsiIsSleeping");
	Agsi.StopSimulator = (tAgsiStopSimulator)GetProcAddress(AgsiConfig.m_hInstance, "AgsiStopSimulator");
	Agsi.TriggerReset = (tAgsiTriggerReset)GetProcAddress(AgsiConfig.m_hInstance, "AgsiTriggerReset");
	Agsi.UpdateWindows = (tAgsiUpdateWindows)GetProcAddress(AgsiConfig.m_hInstance, "AgsiUpdateWindows");
	Agsi.HandleFocus = (tAgsiHandleFocus)GetProcAddress(AgsiConfig.m_hInstance, "AgsiHandleFocus");
	Agsi.GetExternalClockRate = (tAgsiGetExternalClockRate)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetExternalClockRate");
	Agsi.GetInternalClockRate = (tAgsiGetInternalClockRate)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetInternalClockRate");
	Agsi.GetClockFactor = (tAgsiGetClockFactor)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetClockFactor");
	Agsi.Message = (tAgsiMessage)GetProcAddress(AgsiConfig.m_hInstance, "AgsiMessage");
	Agsi.GetTargetKey = (tAgsiGetTargetKey)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetTargetKey");
	Agsi.SetTargetKey = (tAgsiSetTargetKey)GetProcAddress(AgsiConfig.m_hInstance, "AgsiSetTargetKey");
	Agsi.GetSymbolByName = (tAgsiGetSymbolByName)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetSymbolByName");
	Agsi.GetSymbolByValue = (tAgsiGetSymbolByValue)GetProcAddress(AgsiConfig.m_hInstance, "AgsiGetSymbolByValue");

	if ((Agsi.DefineSFR == NULL) ||
		(Agsi.DefineVTR == NULL) ||
		(Agsi.DeclareInterrupt == NULL) ||
		(Agsi.SetWatchOnSFR == NULL) ||
		(Agsi.SetWatchOnVTR == NULL) ||
		(Agsi.SetWatchOnMemory == NULL) ||
		(Agsi.CreateTimer == NULL) ||
		(Agsi.DefineMenuItem == NULL) ||

		(Agsi.WriteSFR == NULL) ||
		(Agsi.ReadSFR == NULL) ||
		(Agsi.WriteVTR == NULL) ||
		(Agsi.ReadVTR == NULL) ||
		(Agsi.SetSFRReadValue == NULL) ||
		(Agsi.ReadMemory == NULL) ||
		(Agsi.WriteMemory == NULL) ||
		(Agsi.GetLastMemoryAddress == NULL) ||
		(Agsi.IsSimulatorAccess == NULL) ||
		(Agsi.SetTimer == NULL) ||
		(Agsi.GetStates == NULL) ||
		(Agsi.GetProgramCounter == NULL) ||
		(Agsi.IsInInterrupt == NULL) ||
		(Agsi.IsSleeping == NULL) ||
		(Agsi.StopSimulator == NULL) ||
		(Agsi.TriggerReset == NULL) ||
		(Agsi.UpdateWindows == NULL) ||
		(Agsi.HandleFocus == NULL) ||
		(Agsi.GetExternalClockRate == NULL) ||
		(Agsi.GetInternalClockRate == NULL) ||
		(Agsi.GetClockFactor == NULL) ||
		(Agsi.Message == NULL) ||
		(Agsi.GetTargetKey == NULL) ||
		(Agsi.SetTargetKey == NULL) ||
		(Agsi.GetSymbolByName == NULL) ||
		(Agsi.GetSymbolByValue == NULL)) {

		return(FALSE);
	}

	return(1);
}


// CngsledsApp construction

CngsledsApp::CngsledsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	Filekez::Get();
}


// The one and only CngsledsApp object

CngsledsApp theApp;


// CngsledsApp initialization

BOOL CngsledsApp::InitInstance()
{
	CWinApp::InitInstance();

	/*
	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CngsledsDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}
	
	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	//return FALSE;
	*/

	return TRUE;
}


DWORD DefineAllSFR(void) {
	BOOL ret = TRUE;
	return(ret);
}

DWORD DefineAllVTREG(void) {         // declare all virtual registers
	BOOL ret = TRUE;
	return(ret);
}

DWORD DefineAllInterrupts(void) {   // declare all interrupts
	return(TRUE);
}

//AGSITIMER Timer;


DWORD DefineAllWatches(void) {      // define all Watches
	BOOL ret = TRUE;

	//ret &= Agsi.SetWatchOnSFR(P1, OnChange, AGSIWRITE);
	//ret &= Agsi.SetWatchOnSFR(P4, OnChange, AGSIWRITE);

	for (char x = 0; x < 8; x++){  // set watch on ports only for the leds.  buttons dont need watch -- x is not 0-15
		ret &= Agsi.SetWatchOnSFR(Filekez::sfraddress[Filekez::portArrayExt[x]], OnChange, AGSIREADWRITE);
	}

	/*
	for (char x = 0; x <= 4; x++) {
		if (Filekez::portExt[x] != 0) {
			ret &= Agsi.SetWatchOnSFR(Filekez::sfraddress[x], OnChange, AGSIWRITE);
		}
	}
	*/

	return(ret);
}

DWORD DefineAllMenuEntries(void) {  // declare all peripheral-menu entries and dialogs

	if (!Agsi.DefineMenuItem(&PeriMenu)) return(FALSE);

	return(TRUE);
}


DWORD ResetPeripheral(void) {       // reset all SFR's of this peripheral
	BOOL ret = TRUE;
	return(ret);
}

extern "C" DWORD AGSIEXPORT AgsiEntry(DWORD nCode, void *vp) {
	DWORD CpuType;

	switch (nCode) {
	case AGSI_CHECK:
		CpuType = *((DWORD *)vp);
		if (CpuType == 8051) return(1);  // This driver supports the 8051 family of microcontrollers 
		else                 return(0);  // Other microcontrollers are not supported by the driver
		break;

	case AGSI_INIT:                    // Declare all SFR's, VTREG's, Watches and Interrupts here
		AgsiConfig = *((AGSICONFIG *)vp);
		//    AgsiConfig.m_hInstance;          // this pointer is used to get the function addresses of uVision
		//    AgsiConfig.m_pszProjectPath;     // Path to application e.g. C:\KEIL\C51\EXAMPLES\HELLO
		//    AgsiConfig.m_pszDevice;          // Simulated Device e.g. 52. This string is extracted out of the -p option.
		//    AgsiConfig.m_pszConfiguration;   // complete dialog DLL options e.g. -p52 -dmydll ...
		//    AgsiConfig.m_pszAppFile;         // name of loaded OMF file including path e.g. C:\KEIL\C51\EXAMPLES\HELLO\HELLO

		//if (!ReadSettings()) return(FALSE);

		if (!GetFunctionPointers()) return(FALSE);   // get all function pointers for Agsi calls
		if (!DefineAllSFR()) return(FALSE);          // define all special function registers
		if (!DefineAllVTREG()) return(FALSE);        // define all virtual registers
		if (!DefineAllInterrupts()) return(FALSE);   // define all interrupts
		if (!DefineAllWatches()) return(FALSE);      // define all watches
		if (!DefineAllMenuEntries()) return(FALSE);  // define all peripheral-menu entries and dialogs
		break;

	case AGSI_TERMINATE:               // Free all allocated memory, close all files ...
		break;

	case AGSI_RESET:                   // Reset all SFR's of this peripheral
		if (!ResetPeripheral()) return(FALSE);
		break;

	case AGSI_PREPLL:                  // Recalculate all peripherals before clock prescaler/PLL is set to a new value
		break;

	case AGSI_POSTPLL:                 // Recalculate all peripherals after clock prescaler/PLL is set to a new value
		break;
	}
	return(TRUE);       // return OK
}

void OnChange() // ezt hívja meg az AGSI watch, fontos hogy megvizsgáljuk hogy a pCPeriDialog ami lényegében maga a dialog ablak handlere, hogy létezik-e a dialog, ha nem, és meghívódik, crashel a Keil
{
	if (pCPeriDialog != NULL) {
		pCPeriDialog->WatchSFR();	//nem tudom ez így mennyire szép megoldás, vagy mennyire C++ vagy programozás hû úgy általánosságban, a Cngsledapp classban is lehet meg lehet írni a programot, talán úgy is lenne a helyes
	}
}
