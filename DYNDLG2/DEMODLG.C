/**************************************************************************
	Dynamic Dialog Boxes Demo
	Demo Dialog Module
	Written by Atif Aziz
**************************************************************************/

#define STRICT

#include <windows.h>
#include <windowsx.h>
#include "winxext.h"
#include "dyndlg.h"
#include "ids.h"

#ifdef __BORLANDC__
#pragma warn -eff	// Control API produces 'code has no effect'
#endif			// warnings under Borland C++, so turn it off.

	// Constants

#define IDS_TEXTFORMAT	0x1234

enum {
	IDCAT_FONT,
	IDCAT_TEXT,
	IDCAT_PATTERNS
};

	// Declarations

static DLGSECTION	dlgsec;
static HINSTANCE	hinst;
static BOOL		fDefDlgEx;

struct tagCATEGORYDEF
{
      int	nID;
      LPCTSTR	lpszName;
}
static catdef[] =
{
      { IDCAT_FONT,	TEXT("Font")		},
      { IDCAT_TEXT,	TEXT("Text")		},
      { IDCAT_PATTERNS,	TEXT("Patterns")	}
};

	// Variables to save state of category controls

static BOOL	fUnderline = FALSE;
static BOOL	fStrikeOut = FALSE;
static BOOL	fAutoText = FALSE;
static BOOL	fAutoSize = FALSE;
static BOOL	fShadow = FALSE;

	// Function prototypes

BOOL CALLBACK	DynDlgBox(HWND, UINT, WPARAM, LPARAM);

static LRESULT	DynDlgBox_DlgProc(HWND, UINT, WPARAM, LPARAM);
static BOOL	DynDlgBox_OnInitDialog(HWND, HWND, LPARAM);
static void	DynDlgBox_OnCommand(HWND, int, HWND, UINT);

static LRESULT	CommonDlg_DefProc(HWND, UINT, WPARAM, LPARAM);

static void	SaveCategoryState(HWND, int);
static void	RestoreCategoryState(HWND, int);

#ifdef __BORLANDC__
#pragma argsused        // Parameter 'hinstPrev' is not used.
#endif

int PASCAL WinMain(hinstA, hinstPrev, lpszCmdLine, nCmdShow)
HINSTANCE       hinstA;
HINSTANCE       hinstPrev;
LPSTR           lpszCmdLine;
int             nCmdShow;
{
  FARPROC	farproc;

  hinst = hinstA;

  farproc = MakeProcInstance((FARPROC) DynDlgBox, hinst);
  DialogBox(hinst, TEXT("TextFormat"), NULL, (DLGPROC) farproc);
  FreeProcInstance(farproc);

  return 0;
}

// -----------------------------------------------------------------------
//  PURPOSE:
//	Common default window procedure for all dialog.
// -----------------------------------------------------------------------

static LRESULT CommonDlg_DefProc(hwndDlg, uMessage, wParam, lParam)
HWND	hwndDlg;
UINT	uMessage;
WPARAM	wParam;
LPARAM	lParam;
{
  return DefDlgProcEx(hwndDlg, uMessage, wParam, lParam, &fDefDlgEx);
}

BOOL CALLBACK DynDlgBox(hwndDlg, uMessage, wParam, lParam)
HWND	hwndDlg;
UINT	uMessage;
WPARAM	wParam;
LPARAM	lParam;
{
  CheckDefDlgRecursion(&fDefDlgEx);
  return SetDlgMsgResult(hwndDlg, uMessage,
    DynDlgBox_DlgProc(hwndDlg, uMessage, wParam, lParam));
}

static LRESULT DynDlgBox_DlgProc(hwndDlg, uMessage, wParam, lParam)
HWND	hwndDlg;
UINT	uMessage;
WPARAM	wParam;
LPARAM	lParam;
{
  switch (uMessage)
  {
    HANDLE_MSG(hwndDlg, WM_INITDIALOG, DynDlgBox_OnInitDialog);
    HANDLE_MSG(hwndDlg, WM_COMMAND, DynDlgBox_OnCommand);

    default :
	return CommonDlg_DefProc(hwndDlg, uMessage, wParam, lParam);
  }
}

#ifdef __BORLANDC__
#pragma argsused	// Parameters 'hwndFocus' and 'lParam' not used.
#endif			

static BOOL DynDlgBox_OnInitDialog(hwndDlg, hwndFocus, lparam)
HWND	hwndDlg;
HWND	hwndFocus;
LPARAM	lparam;
{
  int	i;
  HWND	hwndProp;

  // Initialise the dialog section.

  InitDlgSection(&dlgsec, IDS_TEXTFORMAT, hwndDlg, IDC_FRAME_DIALOGBOX, NULL);

  // Set threshold so that a category is always discarded when the user
  // selects another.

  SetDlgSectionThreshold(&dlgsec, 1, 1);
  
  // Add the categories to the section and their names to the list box.

  hwndProp = GetDlgItem(hwndDlg, IDC_PROPERTY);

  for (i = 0; i < ARRAY_COUNT(catdef); i++)
  {
    ListBox_AddString(hwndProp, catdef[i].lpszName);
    AddDlgSectionCategory(&dlgsec, hinst, catdef[i].lpszName, catdef[i].nID,
      DSCF_DISCARDABLE, NULL);
  }

  // Select the first category and list box selection.

  SelectDlgSectionCategory(&dlgsec, IDCAT_FONT);
  ListBox_SetCurSel(hwndProp, 0);

  return TRUE;
}

#ifdef __BORLANDC__
#pragma argsused	// Parameters 'hwndCtl' and 'uNotifyCode' not used.
#endif			

static void DynDlgBox_OnCommand(hwndDlg, nID, hwndCtl, uNotifyCode)
HWND	hwndDlg;
int	nID;
HWND	hwndCtl;
UINT	uNotifyCode;
{
  int		nCurSel;
  int		nCatID;

  switch (nID)
  {
    case IDC_PROPERTY :

		if (uNotifyCode == LBN_SELCHANGE)
		{
		  nCurSel = ListBox_GetCurSel(hwndCtl);

		  if (nCurSel != LB_ERR)
		    SelectDlgSectionCategory(&dlgsec, catdef[nCurSel].nID);
                }
		break;

    case IDS_TEXTFORMAT :

		// Sections and categories don't have have window handles,
		// therefore the hwndCtl parameter of the WM_COMMAND is used
		// to signal the ID of the category being discarded or loaded.

		nCatID = (int) hwndCtl;

		switch (uNotifyCode)
		{
		  case DSN_DISCARDING :

			// Category is about to be discarded, so save the
			// that state of its controls.

			SaveCategoryState(hwndDlg, nCatID);
                        break;

		  case DSN_LOADED :

			// Category loaded, so restore the state of its
			// controls.

			RestoreCategoryState(hwndDlg, nCatID);
                        break;
		}

                break;

    case IDOK :
    case IDCANCEL :

		// Delete the dialog section (also deletes the categories
		// implicitly).

		DeleteDlgSection(&dlgsec);
		EndDialog(hwndDlg, nID == IDOK);
  }
}

static void SaveCategoryState(hwndDlg, nCatID)
HWND	hwndDlg;
int	nCatID;
{
  switch (nCatID)
  {
    case IDCAT_FONT :

		fStrikeOut = IsDlgButtonChecked(hwndDlg, IDC_STRIKEOUT);
		fUnderline = IsDlgButtonChecked(hwndDlg, IDC_UNDERLINE);
		break;

    case IDCAT_TEXT :

		fAutoText = IsDlgButtonChecked(hwndDlg, IDC_AUTOTEXT);
		fAutoSize = IsDlgButtonChecked(hwndDlg, IDC_AUTOSIZE);
		break;

    case IDCAT_PATTERNS :

		fShadow = IsDlgButtonChecked(hwndDlg, IDC_SHADOW);
                break;
  }
}

static void RestoreCategoryState(hwndDlg, nCatID)
HWND	hwndDlg;
int	nCatID;
{
  switch (nCatID)
  {
    case IDCAT_FONT :

		CheckDlgButton(hwndDlg, IDC_STRIKEOUT, fStrikeOut);
		CheckDlgButton(hwndDlg, IDC_UNDERLINE, fUnderline);
		break;

    case IDCAT_TEXT :

		CheckDlgButton(hwndDlg, IDC_AUTOTEXT, fAutoText);
		CheckDlgButton(hwndDlg, IDC_AUTOSIZE, fAutoSize);
		break;

    case IDCAT_PATTERNS:

		CheckDlgButton(hwndDlg, IDC_SHADOW, fShadow);
		break;
  }
}
