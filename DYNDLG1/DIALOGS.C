/**************************************************************************
	Dynamic Dialog Boxes Demo
	Dialogs Module
	Written by Atif Aziz
**************************************************************************/

#define STRICT

#include <windows.h>
#include <windowsx.h>
#include "winxext.h"
#include "dialogs.h"

	// Constants

#define SZ_TEXTPROP     TEXT("DynDlg_TextSave")

	// Declarations

BOOL            g_fDefDlgEx = FALSE;

	// Function prototypes

static void     GetDlgCtlRect(HWND, int, LPRECT);

// -----------------------------------------------------------------------
//  PURPOSE:
//      Common default window procedure for all dialog.
// -----------------------------------------------------------------------

LRESULT CommonDlg_DefProc(hwndDlg, uMessage, wParam, lParam)
HWND    hwndDlg;
UINT    uMessage;
WPARAM  wParam;
LPARAM  lParam;
{
  return DefDlgProcEx(hwndDlg, uMessage, wParam, lParam, &g_fDefDlgEx);
}

// -----------------------------------------------------------------------
//  PURPOSE:
//      Shows or hides controls whose ID fall within a given range. 
// -----------------------------------------------------------------------

void ShowCategory(hwndDlg, nFirstID, nLastID, fShow)
HWND    hwndDlg;
int     nFirstID;
int     nLastID;
BOOL    fShow;
{
  BOOL          fCtlText;
  int           i;
  HWND          hwndCtl;
  int           nLen;
  LPTSTR        lpszText;
  HLOCAL        hlocText;
  UINT          dwCtlCode;

  for (i = nFirstID; i <= nLastID; i++)
  {
    hwndCtl = GetDlgItem(hwndDlg, i);

    if (hwndCtl)
    {
      // Find out if the control is potential danger for mnemonic
      // conflicts. This is especially to get around a bug in Windows
      // which yields focus to hidden controls when they share
      // mnemonics with visible controls on a dialog box. 

      fCtlText = TRUE;

      dwCtlCode = (UINT) SendMessage(hwndCtl, WM_GETDLGCODE, 0, 0);

      if (dwCtlCode & DLGC_WANTCHARS)
	fCtlText = FALSE;

      if ((dwCtlCode & DLGC_STATIC) &&
	(GetWindowStyle(hwndCtl) & SS_NOPREFIX))
	fCtlText = FALSE;

      if (fShow)
      {
	// Restore the control's text?

	if (fCtlText)
	{
	  // Get the text from the property in which it was saved.

	  hlocText = GetProp(hwndCtl, SZ_TEXTPROP);

	  if (hlocText)
	  {
	    // Restore the text and then free the allocation for the
	    // text and the property.

	    lpszText = (LPTSTR) LocalLock(hlocText);
	    SetWindowText(hwndCtl, lpszText);
	    LocalUnlock(hlocText);
	    RemoveProp(hwndCtl, SZ_TEXTPROP);
	    LocalFree(hlocText);
	  }
	}

	// NOTE: We show a control after setting its text to avoid
	// unneccessary flicker due to re-draws.

	ShowWindow(hwndCtl, SW_SHOW);
      }
      else
      {
	// NOTE: We hide a control before setting its text to avoid
	// unneccessary flicker due to re-draws.

	ShowWindow(hwndCtl, SW_HIDE);

	// Save and destroy the control's text?

	if (fCtlText)
	{
	  // Get the length of the window text, make an equivalent
	  // allocation and then assign the handle to a property.

	  nLen = GetWindowTextLength(hwndCtl) + 1;
	  hlocText = LocalAlloc(LHND, nLen * sizeof(TCHAR));
	  SetProp(hwndCtl, SZ_TEXTPROP, hlocText);

	  if (hlocText)
	  {
	    lpszText = (LPTSTR) LocalLock(hlocText);
	    GetWindowText(hwndCtl, lpszText, nLen);
	    LocalUnlock(hlocText);

	    // Destroy the control's text.

	    SetWindowText(hwndCtl, TEXT(""));
	  }
	}
      }
    }
  }
}

// -----------------------------------------------------------------------
//  PURPOSE:
//      Moves controls whose ID fall within a given range from one
//      location to another. The displacement in location is measures
//      by using two controls on the dialog box as relative references.
// -----------------------------------------------------------------------

void MoveCategory(hwndDlg, nFirstID, nLastID, nSrcID, nDestID)
HWND    hwndDlg;
int     nFirstID;
int     nLastID;
int     nSrcID;
int     nDestID;
{
  int   i;
  RECT  rcSrc;
  RECT  rcDest;
  RECT  rcCtl;
  int   nDeltaX;
  int   nDeltaY;
  HWND  hwndCtl;

  // Get the source and destination rectangle coordinates

  GetDlgCtlRect(hwndDlg, nSrcID, &rcSrc);
  GetDlgCtlRect(hwndDlg, nDestID, &rcDest);

  // Calculate the offset by which to move each control

  nDeltaX = rcDest.left - rcSrc.left;
  nDeltaY = rcDest.top - rcSrc.top;

  for (i = nFirstID; i <= nLastID; i++)
  {
    hwndCtl = GetDlgItem(hwndDlg, i);

    if (hwndCtl)
    {
      GetDlgCtlRect(hwndDlg, i, &rcCtl);

      // Displace the control

      SetWindowPos(hwndCtl, NULL, rcCtl.left + nDeltaX,
	rcCtl.top + nDeltaY, 0, 0,
	SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
  }
}

// -----------------------------------------------------------------------
//  PURPOSE:
//      Gets a control's window coordinates relative its parent.
// -----------------------------------------------------------------------

static void GetDlgCtlRect(hwndDlg, nID, lprc)
HWND    hwndDlg;
int     nID;
LPRECT  lprc;
{
  GetWindowRect(GetDlgItem(hwndDlg, nID), lprc);
  MapWindowRect(NULL, hwndDlg, lprc);
}

// -----------------------------------------------------------------------
//  PURPOSE:
//      Clears the text save property and allocations for the given
//      range of controls. 
// -----------------------------------------------------------------------

void RemoveCategoryTextProps(hwndDlg, nFirstID, nLastID)
HWND    hwndDlg;
int     nFirstID;
int     nLastID;
{
  int           i;
  HWND          hwndCtl;
  HLOCAL        hlocText;

  for (i = nFirstID; i <= nLastID; i++)
  {
    hwndCtl = GetDlgItem(hwndDlg, i);

    if (hwndCtl)
    {
      hlocText = GetProp(hwndCtl, SZ_TEXTPROP);

      if (hlocText)
      {
	RemoveProp(hwndCtl, SZ_TEXTPROP);
	LocalFree(hlocText);
      }
    }
  }
}
