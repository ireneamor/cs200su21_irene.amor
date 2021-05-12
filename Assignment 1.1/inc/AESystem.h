// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AESystem.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the system module
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_SYSTEM_H
#define AE_SYSTEM_H

// ---------------------------------------------------------------------------

// window related variables
extern HINSTANCE	ghAESysAppInstance;
extern HWND			ghAESysWin;
extern WNDCLASS		gAESysWinClass;
extern bool			gAESysWinExists;
extern const char*	gpAESysWinTitle;
extern const char*	gpAESysWinClassName;
extern bool			gAESysAppActive;
extern unsigned		gAESysWinWidth;
extern unsigned		gAESysWinHeight;

// ---------------------------------------------------------------------------

bool AESysInit(const char * title = NULL, int win_width = 800, int win_height = 600);
void AESysReset	();
void AESysUpdate();
void AESysExit	();
void AESysShowConsole();
void AESysHideConsole();
void AESysGameLoop();

// ---------------------------------------------------------------------------

void AESysFrameStart();
void AESysFrameEnd();

// ---------------------------------------------------------------------------

#endif // AE_SYSTEM_H

