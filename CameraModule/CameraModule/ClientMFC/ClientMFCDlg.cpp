// ClientMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "ClientMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\include\TrackerPod.h"

// A set of points used for testing TrackerPod movement in a figure 8
POINT m_testPoints[] = { 
	{0,0}, {-10,-30}, {-30,-50}, {-50,-50}, {-70,-30}, 
	{-70,0}, {-70,30}, {-50,50}, {-30,50}, {-10,30},
	{10,-30}, {30,-50}, {50,-50}, {70,-30}, 
	{70,0},	{70, 30}, {50,50}, {30,50}, {10, 30}
};

// The main TrackerPod object
CTrackerPod *g_pTrackerPod = NULL;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientMFCDlg dialog

CClientMFCDlg::CClientMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientMFCDlg::IDD, pParent),
	m_bTesting(false),
	m_nCurrentTestPoint(0),
	m_bDragging(false)
{
	//{{AFX_DATA_INIT(CClientMFCDlg)
	m_bControlAll = FALSE;
	m_strTrackerpodInfo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hDragCursor = LoadCursor(NULL, IDC_SIZEALL);

}

CClientMFCDlg::~CClientMFCDlg()
{
	if(g_pTrackerPod)
		g_pTrackerPod->ReleaseTrackerPod();
	g_pTrackerPod = NULL;
}

void CClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientMFCDlg)
	DDX_Control(pDX, IDC_STATIC_DRAGHERE, m_wndDragHere);
	DDX_Control(pDX, IDC_CHECK_CONTROL_ALL, m_wndControlAll);
	DDX_Control(pDX, IDC_BUTTON_DOWN, m_wndDown);
	DDX_Control(pDX, IDC_BUTTON_GET_INFO, m_wndGetInfo);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_wndTest);
	DDX_Control(pDX, IDC_BUTTON_DO_COMMAND, m_wndDoCommand);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_HOME, m_wndZoomHome);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_OUT, m_wndZoomOut);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_IN, m_wndZoomIn);
	DDX_Control(pDX, IDC_BUTTON_RIGHT, m_wndRight);
	DDX_Control(pDX, IDC_BUTTON_LEFT, m_wndLeft);
	DDX_Control(pDX, IDC_BUTTON_HOME, m_wndHome);
	DDX_Control(pDX, IDC_BUTTON_UP, m_wndUp);
	DDX_Control(pDX, IDC_COMBO_CONTROL, m_wndControl);
	DDX_Control(pDX, IDC_LIST_DEVICES, m_wndDevices);
	DDX_Check(pDX, IDC_CHECK_CONTROL_ALL, m_bControlAll);
	DDX_Text(pDX, IDC_EDIT_TRACKERPOD_INFO, m_strTrackerpodInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientMFCDlg, CDialog)
	//{{AFX_MSG_MAP(CClientMFCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_HOME, OnButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, OnButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_HOME, OnButtonZoomHome)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, OnButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_DO_COMMAND, OnButtonDoCommand)
	ON_BN_CLICKED(IDC_BUTTON_GET_INFO, OnButtonGetInfo)
	ON_BN_CLICKED(ID_APP_ABOUT, OnAppAbout)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DEVICES, OnItemchangedListDevices)
	ON_MESSAGE(WM_POD_SERVICE_NOTIFICATION, OnWmPodServiceNotification)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK_CONTROL_ALL, OnCheckControlAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientMFCDlg message handlers

BOOL CClientMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	// Creating TrackerPod library object
	g_pTrackerPod = CTrackerPod::CreateTrackerPod();
	if(!g_pTrackerPod)
		return 0;

	// Populate the control combo box with a list of possible string commands that can be used to control a selected TrackerPod

	m_wndControl.AddString("led=on");
	m_wndControl.AddString("led=off");
	
	m_wndControl.AddString("motors_off");
	
	m_wndControl.AddString("buffering=on");
	m_wndControl.AddString("buffering=off");
	m_wndControl.AddString("flushbuffer");
	
	m_wndControl.AddString("pan_tilt_step_size=1");
	m_wndControl.AddString("pan_tilt_step_size=2");
	m_wndControl.AddString("pan_tilt_step_size=3");
	m_wndControl.AddString("pan_tilt_step_size=4");
	m_wndControl.AddString("pan_tilt_step_size=5");
	
	m_wndControl.AddString("zoom_speed=0");
	m_wndControl.AddString("zoom_speed=1");
	m_wndControl.AddString("zoom_speed=2");
	m_wndControl.AddString("zoom_speed=3");
	m_wndControl.AddString("zoom_speed=4");
	m_wndControl.AddString("zoom_speed=5");
	m_wndControl.AddString("zoom_speed=6");
	
	m_wndControl.AddString("zoom=in");
	m_wndControl.AddString("zoom=out");
	m_wndControl.AddString("zoom=stop");
	m_wndControl.AddString("zoom=home");
	
	m_wndControl.AddString("focus=far");
	m_wndControl.AddString("focus=near");
	
	m_wndControl.AddString("zoom_step_size=1");
	m_wndControl.AddString("zoom_step_size=2");
	m_wndControl.AddString("zoom_step_size=3");
	m_wndControl.AddString("zoom_step_size=4");
	m_wndControl.AddString("zoom_step_size=5");
	m_wndControl.AddString("zoom_step_size=6");
	m_wndControl.AddString("zoom_step_size=7");
	m_wndControl.AddString("zoom_step_size=8");
	m_wndControl.AddString("zoom_step_size=9");
	m_wndControl.AddString("zoom_step_size=10");
	
	m_wndControl.AddString("auto_focus=on");
	m_wndControl.AddString("auto_focus=off");
	
	m_wndControl.AddString("digital_zoom=on");
	m_wndControl.AddString("digital_zoom=off");
	
	m_wndControl.AddString("LANC_toggle_auto_focus");
	m_wndControl.AddString("LANC_toggle_record");
	m_wndControl.AddString("LANC_fader");
	m_wndControl.AddString("LANC_toggle_white_balance");
	m_wndControl.AddString("LANC_exposure");
	m_wndControl.AddString("LANC_shutter");
	m_wndControl.AddString("LANC_reset_white_balance");
	m_wndControl.AddString("LANC_stop");
	m_wndControl.AddString("LANC_pause");
	m_wndControl.AddString("LANC_play");
	m_wndControl.AddString("LANC_record");

	EnablePanTiltControls(FALSE);
	EnableZoomControls(FALSE);
	EnableOtherControls(FALSE);

	// Specify the windows that we want the library to send notification messages to. 
	// There can be up to 16 notification recipient windows
	g_pTrackerPod->UseNotification(m_hWnd, WM_POD_SERVICE_NOTIFICATION, 0);
	if (!g_pTrackerPod->initialize("EagletronPodConfigurationTool", false))
	{
		MessageBox("Unable to register client- Eagletron TrackerPod Service is not running?", "ERROR", MB_ICONERROR);
		return FALSE;
	}
	
	DWORD dwStyle = m_wndDevices.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_wndDevices.SetExtendedStyle(dwStyle);

	m_wndDevices.InsertColumn(0, "ID");
	m_wndDevices.InsertColumn(1, "Description");
	m_wndDevices.InsertColumn(2, "Status");
	m_wndDevices.InsertColumn(3, "Pan");
	m_wndDevices.InsertColumn(4, "Tilt");
	
	m_wndDevices.SetColumnWidth(0, 25);
	m_wndDevices.SetColumnWidth(1, 120);
	m_wndDevices.SetColumnWidth(2, 80);
	m_wndDevices.SetColumnWidth(3, 80);
	m_wndDevices.SetColumnWidth(4, 80);

	PopulateDeviceList();

	SetTimer(TIMER_QUERY_POS, 500, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientMFCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientMFCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CClientMFCDlg::EnablePanTiltControls( BOOL bEnable )
{
	m_wndUp.EnableWindow(bEnable);
	m_wndDown.EnableWindow(bEnable);
	m_wndLeft.EnableWindow(bEnable);
	m_wndRight.EnableWindow(bEnable);
	m_wndHome.EnableWindow(bEnable);
}

void CClientMFCDlg::EnableZoomControls( BOOL bEnable )
{
	m_wndZoomIn.EnableWindow(bEnable);
	m_wndZoomOut.EnableWindow(bEnable);
	m_wndZoomHome.EnableWindow(bEnable);
}

void CClientMFCDlg::EnableOtherControls( BOOL bEnable )
{
	m_wndControl.EnableWindow(bEnable);
	m_wndTest.EnableWindow(bEnable);
	m_wndGetInfo.EnableWindow(bEnable);
	m_wndDoCommand.EnableWindow(bEnable);
}

bool CClientMFCDlg::ParseValue(CString strSource, CString strKey, CString& strValue)
{
	int posStart, posEnd, count;
	
	posStart = strSource.Find(strKey + "=");
	if (posStart == -1)
		return false;
	posStart +=  strKey.GetLength() + 1;
	posEnd = strSource.Find("&", posStart);
	if (posEnd == -1)
		posEnd = strSource.GetLength();
	count = posEnd - posStart;
	
	strValue = strSource.Mid(posStart, count);
	
	return true;
}

void CClientMFCDlg::PopulateDeviceList()
{
	CString strExtendedName, strVersion, strID, strDescription;

	if(g_pTrackerPod->begin_enum_device())
	{
		while(true)
		{
			int devid = g_pTrackerPod->enum_next(0);
			if(devid < 0)
				break;
			
			
			char szDeviceInfo[1024];
			int nCount = 0;
			if (g_pTrackerPod->GetDeviceInfo(devid, szDeviceInfo))
			{
				CString strDeviceInfo = szDeviceInfo;
				
				if (ParseValue(strDeviceInfo, "extended_name", strExtendedName) &&
					ParseValue(strDeviceInfo, "version", strVersion))
				{
					strID.Format("%d", devid);
					strDescription.Format("%s (%s)", strExtendedName, strVersion);
					
					m_wndDevices.InsertItem(nCount, strID);
					m_wndDevices.SetItemText(nCount, 0, strID);
					m_wndDevices.SetItemText(nCount, 1, strDescription);
					m_wndDevices.SetItemText(nCount, 2, "Ok");
					m_wndDevices.SetItemData(nCount, devid);
					nCount++;
				}
			}
		}
		
	}
	
	m_wndControlAll.EnableWindow(m_wndDevices.GetItemCount() > 0);
}

void CClientMFCDlg::DoCommand(CString strCommand, CString strParam)
{
	CString strControl = strCommand;
	if (strParam != "")
		strControl += "=" + strParam;
	
	if (m_bControlAll)
	{
		long lDeviceUsed = g_pTrackerPod->GetDeviceInUse();
		int nCount = 1;
		
		if(g_pTrackerPod->begin_enum_device())
		{
			while(true)
			{
				int devid = g_pTrackerPod->enum_next(0);
				if(devid < 0)
					break;
				
				g_pTrackerPod->use_device(devid, false);
				g_pTrackerPod->SetBlocking(nCount == g_pTrackerPod->GetDeviceCount());   // Only the last one is blocking
				
				g_pTrackerPod->control((char*)(LPCTSTR)strControl);
				nCount++;
			}
		}
		
		g_pTrackerPod->use_device(lDeviceUsed, false);
		g_pTrackerPod->SetBlocking(true);
	}
	else
	{
		g_pTrackerPod->control((char*)(LPCTSTR)strControl);
	}
}

void CClientMFCDlg::OnTimer(UINT nIDEvent) 
{
	POINT ptMoveTo;
	int nPointCount = sizeof(m_testPoints) / sizeof(POINT);
	if (nIDEvent == TIMER_TEST)
	{
		if (m_nCurrentTestPoint == 0)
		{
			DoCommand("zoom", "in");
			DoCommand("zoom", "in");
		}
		if (m_nCurrentTestPoint == nPointCount / 2)
		{
			DoCommand("zoom", "out");
			DoCommand("zoom", "out");
		}
		
		ptMoveTo = m_testPoints[m_nCurrentTestPoint];
		
		CString strParam;
		strParam.Format("%d,%d", ptMoveTo.x, ptMoveTo.y);
		DoCommand("move_to", strParam);
		
		m_nCurrentTestPoint++;
		
		if (m_nCurrentTestPoint >= nPointCount)
			m_nCurrentTestPoint = 0;
	}
	else if (nIDEvent == TIMER_QUERY_POS)
	{
		for (int i = 0; i <= m_wndDevices.GetItemCount() - 1; i++)
		{
			long lDeviceId = m_wndDevices.GetItemData(i);
			CString strDeviceInfo;
			bool bResult = g_pTrackerPod->GetDeviceInfo(lDeviceId, strDeviceInfo.GetBuffer(1024));
			strDeviceInfo.ReleaseBuffer();
			
			if (bResult)
			{
				CString strX, strY;
				
				if (ParseValue(strDeviceInfo, "x", strX))
				{
					m_wndDevices.SetItemText(i, 3, strX);
				}
				else
				{
					m_wndDevices.SetItemText(i, 3, "parse err");
				}
				
				if (ParseValue(strDeviceInfo, "y", strY))
				{
					m_wndDevices.SetItemText(i, 4, strY);
				}
				else
				{
					m_wndDevices.SetItemText(i, 4, "parse err");
				}
				
			}
			else
			{
				m_wndDevices.SetItemText(i, 3, "err");
				m_wndDevices.SetItemText(i, 4, "err");
			}
		}
		
	}
	else
		CDialog::OnTimer(nIDEvent);
}

void CClientMFCDlg::OnButtonSelect() 
{ 
	// Display built-in TrackerPod selection dialog of the library
	g_pTrackerPod->use_device(-4);
}

void CClientMFCDlg::OnButtonLeft() 
{
	DoCommand("move_by", "1.0,0.0");

}

void CClientMFCDlg::OnButtonRight() 
{
	DoCommand("move_by", "-1.0,0.0");
}

void CClientMFCDlg::OnButtonUp() 
{
	DoCommand("move_by", "0.0,-1.0");
}

void CClientMFCDlg::OnButtonHome() 
{
	DoCommand("move_to", "0,0");
}

void CClientMFCDlg::OnButtonDown() 
{
	DoCommand("move_by", "0.0,1.0");
}

void CClientMFCDlg::OnButtonZoomIn() 
{
	DoCommand("zoom", "in");
}

void CClientMFCDlg::OnButtonZoomHome() 
{
	DoCommand("zoom", "home");
}

void CClientMFCDlg::OnButtonZoomOut() 
{
	DoCommand("zoom", "out");
}

void CClientMFCDlg::OnButtonTest() 
{
	if (m_bTesting)
	{
		m_wndTest.SetWindowText("Start Testing");
		m_bTesting = false;
		KillTimer(TIMER_TEST);
	}
	else
	{
		m_wndTest.SetWindowText("Stop Testing");
		SetTimer(TIMER_TEST, 300, NULL);
		m_bTesting = true;
	}

}

void CClientMFCDlg::OnButtonDoCommand() 
{
	int nIndex = m_wndControl.GetCurSel();
	
	if (nIndex != CB_ERR)
	{
		CString strCommand;
		m_wndControl.GetLBText(nIndex, strCommand);
		
		DoCommand((char*)(LPCTSTR)strCommand, "");
	}	
}

void CClientMFCDlg::OnButtonGetInfo() 
{
	char info[1024];
	if (!g_pTrackerPod->get_info(info))
		m_strTrackerpodInfo = "Cannot get trackerpod info";
	else
		m_strTrackerpodInfo = info;

	UpdateData(FALSE);
	
}

void CClientMFCDlg::OnAppAbout() 
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CClientMFCDlg::OnItemchangedListDevices(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;


	static int nLastDevID = -1;
	POSITION pos = m_wndDevices.GetFirstSelectedItemPosition();

	if (pos)
	{
		int nIndex = m_wndDevices.GetNextSelectedItem(pos);

		long lDevId = m_wndDevices.GetItemData(nIndex);
		
		if (nIndex != CB_ERR && !m_bControlAll && nLastDevID != lDevId)
		{
			g_pTrackerPod->use_device(lDevId);
			nLastDevID = lDevId;
		}
	}
		
}

LRESULT CClientMFCDlg::OnWmPodServiceNotification(WPARAM wParam, LPARAM lParam)
{
	int nIndexFound = -1;
	long lDeviceID;
	char szDeviceInfo[1024];
	CString strID,  strDescription, strStatus;
	int nIndexInUse = -1;

	long lDeviceInUse =  g_pTrackerPod->GetDeviceInUse();

	for (int i = 0; i < m_wndDevices.GetItemCount(); i++)
	{
		lDeviceID = m_wndDevices.GetItemData(i);
		if (lDeviceID == lParam)
		{
			nIndexFound = i;
		}
		if (lDeviceID == lDeviceInUse)
		{
			nIndexInUse = i;
		}
	}


	// Controls are available in the following cases:
	// 1. "Control All" is selected and there is 1 or more Pods
	// 2. The currently used device is on the list and responds properly
	int nDeviceCount = g_pTrackerPod->GetDeviceCount();
	CString strInfo;

	bool bDeviceInUseResponds = g_pTrackerPod->GetDeviceInfo(lDeviceInUse, strInfo.GetBuffer(1024));
	strInfo.ReleaseBuffer();

	bool bControlsAvailable = bDeviceInUseResponds || (nDeviceCount > 0 && m_bControlAll);

	EnablePanTiltControls(bControlsAvailable);
	EnableZoomControls(bControlsAvailable);
	EnableOtherControls(bControlsAvailable);

	KillTimer(TIMER_TEST);

	if (bControlsAvailable && m_bTesting)
	{
		SetTimer(TIMER_TEST, 300, NULL);
	}

	m_wndControlAll.EnableWindow(nDeviceCount > 0);


	if (wParam == 5 && nIndexFound != -1) // Removed
	{
		m_wndDevices.DeleteItem(nIndexFound);
		return 0;
	}

	if (lParam == -1)
	{
		POSITION pos = m_wndDevices.GetFirstSelectedItemPosition();
		
		if (pos)
		{
			nIndexInUse = m_wndDevices.GetNextSelectedItem(pos);
			m_wndDevices.SetItemState(nIndexInUse, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	if (wParam == 5)
	{
		return 0;
	}

	switch (wParam)
	{
	case 0:
		strStatus = "Plugged In";
		break;
	case 1:
		strStatus = "Initializing...";
		break;
	case 2:
		strStatus = "Ok";
		break;
	case 3:
		strStatus = "Failed";
		break;
	case 4:
		strStatus = "Resetting...";
		break;
	};



	if (wParam == 2 )
	{
		if (g_pTrackerPod->GetDeviceInfo(lParam, szDeviceInfo))
		{
			CString strDeviceInfo = szDeviceInfo;
			CString strExtendedName, strVersion;

			if (ParseValue(strDeviceInfo, "extended_name", strExtendedName) &&
				ParseValue(strDeviceInfo, "version", strVersion))
			{
				strDescription.Format("%s (%s)", strExtendedName, strVersion);
			}
		}
	}

	strID.Format("%d", lParam);
	if (nIndexFound == -1)
	{
		nIndexFound = m_wndDevices.GetItemCount();
		m_wndDevices.InsertItem(nIndexFound, strID);
	}
	else
	{
		m_wndDevices.SetItemText(nIndexFound, 0, strID);
	}

	m_wndDevices.SetItemText(nIndexFound, 1, strDescription);
	m_wndDevices.SetItemText(nIndexFound, 2, strStatus);

	m_wndDevices.SetItemData(nIndexFound, lParam);

	// Select trackerpod in use
	if (nIndexInUse != -1)
	{
		m_wndDevices.SetItemState(nIndexInUse, LVIS_SELECTED, LVIS_SELECTED);
	}


	return 0;
}

void CClientMFCDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	CRect rcDragArea;
	m_wndDragHere.GetClientRect(&rcDragArea);
	m_wndDragHere.ClientToScreen(&rcDragArea);
	
	if (rcDragArea.PtInRect(point))
	{
		m_hBackupCursor = GetCursor();
		SetCursor(m_hDragCursor);
		m_bDragging = true;
		m_ptPrevPos = point;
		SetCapture();
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CClientMFCDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		SetCursor(m_hBackupCursor);
		ReleaseCapture();
		m_bDragging = false;
	}
	

	CDialog::OnLButtonUp(nFlags, point);
}

void CClientMFCDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);

	if (m_bDragging)
	{
		float fDiffX = - (point.x - m_ptPrevPos.x) / 2.0;
		float fDiffY = (point.y - m_ptPrevPos.y) / 2.0;
		
		CPoint ptScreen = point;
		ClientToScreen(&ptScreen);
		
		
		int nSrcX = ::GetSystemMetrics(SM_CXSCREEN);
		int nScrY = ::GetSystemMetrics(SM_CYSCREEN);
		
		m_ptPrevPos = point;
		
		if(ptScreen.x == 0 || ptScreen.x == nSrcX - 1)
		{
			m_ptPrevPos.x = nSrcX / 2;
			m_ptPrevPos.y = ptScreen.y;
			SetCursorPos(m_ptPrevPos.x, m_ptPrevPos.y);
			ScreenToClient(&m_ptPrevPos);
		}
		
		
		if(ptScreen.y == 0 || ptScreen.y == nScrY - 1)
		{
			m_ptPrevPos.x = ptScreen.x;
			m_ptPrevPos.y = nScrY / 2;
			SetCursorPos(m_ptPrevPos.x, m_ptPrevPos.y);
			ScreenToClient(&m_ptPrevPos);
		}
		
		
		CString strParam;
		strParam.Format("%f,%f", fDiffX, fDiffY);
		
		DoCommand("move_by", strParam);
	}
	

	CDialog::OnMouseMove(nFlags, point);
}

void CClientMFCDlg::OnCheckControlAll() 
{
	UpdateData(TRUE);
	
	int nDeviceCount = m_wndDevices.GetItemCount();
	
	POSITION pos = m_wndDevices.GetFirstSelectedItemPosition();
	
	if (pos)
	{
		int nIndex = m_wndDevices.GetNextSelectedItem(pos);

	
		if (nDeviceCount)
		{
			bool bEnableControls = (m_bControlAll && nDeviceCount) || (!m_bControlAll && nIndex != CB_ERR);

			EnablePanTiltControls(bEnableControls);
			EnableZoomControls(bEnableControls);
			EnableOtherControls(bEnableControls);
			
			m_wndDevices.EnableWindow(!m_bControlAll);
		}
	}
}
