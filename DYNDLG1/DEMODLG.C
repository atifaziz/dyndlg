/**************************************************************************
	Dynamic Dialog Boxes Demo
	Demo Dialog Module
	Written by Atif Aziz
**************************************************************************/

#define STRICT

#include <windows.h>
#include <windowsx.h>
#include "winxext.h"
#include "dialogs.h"
#include "ids.h"

#ifdef __BORLANDC__
#pragma warn -eff	// Control API produces 'code has no effect'
#endif			// warnings under Borland C++, so turn it off.

	// Category

typedef struct tagCATEGORY
{
      LPCTSTR	lpszName;
      int	nFrameID;
      int	nFirstID;
      int	nLastID;
}
CATEGORY;

static CATEGORY category[] =
		{
			{
				TEXT("Font"),
				IDC_FRAME_FONT,
				IDC_FONT_FIRST,
				IDC_FONT_LAST
			},
			{
				TEXT("Text"),
				IDC_FRAME_TEXT,
				IDC_TEXT_FIRST,
				IDC_TEXT_LAST
			},
			{
				TEXT("Patterns"),
				IDC_FRAME_PATTERNS,
				IDC_PATTERNS_FIRST,
				IDC_PATTERNS_LAST
			}
		};

static int	nCurProperty = 0;

	// Function prototypes

BOOL CALLBACK	DynDlgBox(HWND, UINT, WPARAM, LPARAM);

static LRESULT	DynDlgBox_DlgProc(HWND, UINT, WPARAM, LPARAM);
static BOOL	DynDlgBox_OnInitDialog(HWND, HWND, LPARAM);
static void	DynDlgBox_OnCommand(HWND, int, HWND, UINT);

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

  farproc = MakeProcInstance((FARPROC) DynDlgBox, hinstA);
  DialogBox(hinstA, TEXT("TextFormat"), NULL, (DLGPROC) farproc);
  FreeProcInstance(farproc);

  return 0;
}

BOOL CALLBACK DynDlgBox(hwndDlg, uMessage, wParam, lParam)
HWND	hwndDlg;
UINT	uMessage;
WPARAM	wParam;
LPARAM	lParam;
{
  CheckDefDlgRecursion(&g_fDefDlgEx);
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
    case WM_CTLCOLOR :
	return TRUE;
   
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

  hwndProp = GetDlgItem(hwndDlg, IDC_PROPERTY);

  for (i = 0; i < sizeof(category) / sizeof(CATEGORY); i++)
  {
    ListBox_AddString(hwndProp, category[i].lpszName);

    if (i != nCurProperty)
    {
      // If this is not the category to be initially shown, then hide it.

      ShowCategory(hwndDlg, category[i].nFirstID, category[i].nLastID, FALSE);
    }
    else
    {
      // Select the default in the category list box.

      ListBox_SetCurSel(hwndProp, i);
    }

    // Move the category controls from outside to the inside of the
    // dialog box.

    MoveCategory(hwndDlg, category[i].nFirstID, category[i].nLastID,
      category[i].nFrameID, IDC_FRAME_DIALOGBOX);
  }

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
  int		i;
  int		nCurSel;
  RECT		rc;

  switch (nID)
  {
    case IDC_PROPERTY :

		if (uNotifyCode == LBN_SELCHANGE)
		{

		// Select the corresponding category from the array based
		// on the current selection in the list box.

		  nCurSel = ListBox_GetCurSel(hwndCtl);

		  if (nCurSel != nCurProperty && nCurSel != LB_ERR)
		  {
		    // Avoid unnecessary screen refresh by suppressing
		    // redraws. This will optimise the display radically.

		    SetWindowRedraw(hwndDlg, FALSE);

		    // First hide the category currently on display.

		    ShowCategory(hwndDlg, category[nCurProperty].nFirstID,
		      category[nCurProperty].nLastID, FALSE);

		    nCurProperty = nCurSel;

		    // Now show the new category's controls.

		    ShowCategory(hwndDlg, category[nCurProperty].nFirstID,
		      category[nCurProperty].nLastID, TRUE);

		    // Set the redraw back on and invalidate the changes
                    // resulting from hiding and showing controls.

		    SetWindowRedraw(hwndDlg, TRUE);

		    GetWindowRect(GetDlgItem(hwndDlg, IDC_FRAME_DIALOGBOX),
		      &rc);
		    MapWindowRect(NULL, hwndDlg, &rc);
		    InvalidateRect(hwndDlg, &rc, TRUE);
		  }
		}
		break;

    case IDOK :
    case IDCANCEL :

		// Make sure we clean up the allocations made and properties
		// set by ShowCategory in order to overcome the Windows bug.

		for (i = 0; i < sizeof(category) / sizeof(CATEGORY); i++)
		  RemoveCategoryTextProps(hwndDlg, category[i].nFirstID,
		    category[i].nLastID);

		EndDialog(hwndDlg, nID == IDOK);
  }
}
