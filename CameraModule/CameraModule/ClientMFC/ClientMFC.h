// ClientMFC.h : main header file for the CLIENTMFC application
//

#if !defined(AFX_CLIENTMFC_H__C253BF75_1048_4544_8453_094DCB2A5961__INCLUDED_)
#define AFX_CLIENTMFC_H__C253BF75_1048_4544_8453_094DCB2A5961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CClientMFCApp:
// See ClientMFC.cpp for the implementation of this class
//

class CClientMFCApp : public CWinApp
{
public:
	CClientMFCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientMFCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CClientMFCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTMFC_H__C253BF75_1048_4544_8453_094DCB2A5961__INCLUDED_)
