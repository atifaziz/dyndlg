/**************************************************************************
	Dynamic Dialog Boxes Demo
	Header File for Dialogs Module
	Written by Atif Aziz
**************************************************************************/

	// Declarations

extern BOOL	g_fDefDlgEx;

	// Function prototypes

LRESULT	CommonDlg_DefProc(HWND, UINT, WPARAM, LPARAM);
void	ShowCategory(HWND, int, int, BOOL);
void	MoveCategory(HWND, int, int, int, int);
void	RemoveCategoryTextProps(HWND, int, int);
