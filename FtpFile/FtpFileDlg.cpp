
// FtpFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FtpFile.h"
#include "FtpFileDlg.h"
#include "afxdialogex.h"
#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFtpFileDlg 对话框




CFtpFileDlg::CFtpFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFtpFileDlg::IDD, pParent)
	, m_strFtpSite(_T(""))
	, m_strName(_T(""))
	, m_strPwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFtpFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditFtp);
	DDX_Text(pDX, IDC_EDIT1, m_strFtpSite);
	DDX_Control(pDX, IDC_EDIT2, m_EditName);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Control(pDX, IDC_EDIT3, m_EditPwd);
	DDX_Text(pDX, IDC_EDIT3, m_strPwd);
	DDX_Control(pDX, IDOK, m_BtnQuery);
	DDX_Control(pDX, IDCANCEL2, m_BtnUpLoad);
	DDX_Control(pDX, IDCANCEL3, m_BtnDownLoad);
	DDX_Control(pDX, IDCANCEL, m_BtnExit);
	DDX_Control(pDX, IDC_LIST1, m_ListFile);
}

BEGIN_MESSAGE_MAP(CFtpFileDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFtpFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL3, &CFtpFileDlg::OnBnClickedCancel3)
	ON_BN_CLICKED(IDCANCEL2, &CFtpFileDlg::OnBnClickedCancel2)
END_MESSAGE_MAP()


// CFtpFileDlg 消息处理程序

BOOL CFtpFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_strFtpSite=_T("");
	m_strName=_T("");
	m_strPwd=_T("");
	UpdateData(FALSE);
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFtpFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFtpFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFtpFileDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	while(m_ListFile.GetCount()!=0){
		m_ListFile.DeleteString(0);
	}
	ListContent();
}

void CFtpFileDlg::ListContent()
{
	CInternetSession* pSession;
	CFtpConnection* pConnection;
	CFtpFileFind* pFileFind;
	CString strFileName;
	BOOL bContinue;
	pConnection = NULL;
	pFileFind = NULL;
	pSession = new CInternetSession(AfxGetAppName(),1,PRE_CONFIG_INTERNET_ACCESS);
	try{
		pConnection = pSession->GetFtpConnection(m_strFtpSite,m_strName,m_strPwd);
	}
	catch(CInternetException *e){
		e->Delete();
		pConnection = NULL;
		AfxMessageBox("连接失败！",MB_OK|MB_ICONSTOP);
	}
	if (pConnection!= NULL){
		pFileFind = new CFtpFileFind(pConnection);
		bContinue = pFileFind->FindFile(NULL);
		if (!bContinue){
			pFileFind->Close();
			pFileFind=NULL;
		}
		while(bContinue){
			bContinue = pFileFind->FindNextFile();
			strFileName = pFileFind->GetFileName();
			if (pFileFind->IsDirectory())
			{
				strFileName = "["+strFileName;
				strFileName += "]";
			}
			m_ListFile.AddString(strFileName);
		}
		if (pFileFind!=NULL){
			pFileFind->Close();
			pFileFind=NULL;
		}
	}
	delete pFileFind;
	if (pConnection!=NULL){
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
}



void CFtpFileDlg::OnBnClickedCancel3()
{
	UpdateData(TRUE);
	int nSel = m_ListFile.GetCurSel();
	CString strSourceName;
	m_ListFile.GetText(nSel,strSourceName);
	if (strSourceName.GetAt(0)!='['){
		CString strDesName;
		CFileDialog fdlg(FALSE);
		if (fdlg.DoModal()==IDOK){
			strDesName= fdlg.GetPathName();
			if (GetFile(strSourceName,strDesName)){
				AfxMessageBox("下载成功！",MB_OK|MB_ICONINFORMATION);
			}
			else{
				AfxMessageBox("下载失败！",MB_OK|MB_ICONSTOP);
			}
		}
		else{
			AfxMessageBox("请写入文件名！",MB_OK|MB_ICONSTOP);
		}
	}
	else{
		AfxMessageBox("不能下载目录，请重新选择！",MB_OK|MB_ICONSTOP);
	}
	m_BtnDownLoad.EnableWindow(TRUE);
	m_BtnUpLoad.EnableWindow(TRUE);
	m_BtnQuery.EnableWindow(TRUE);
	m_EditFtp.EnableWindow(TRUE);
	m_EditName.EnableWindow(TRUE);
	m_EditPwd.EnableWindow(TRUE);
}

BOOL CFtpFileDlg::GetFile( CString strSourceName,CString strDesName )
{
	CInternetSession* pSession;
	CFtpConnection *pConnection;
	pConnection = NULL;
	pSession = new CInternetSession(AfxGetAppName(),1,PRE_CONFIG_INTERNET_ACCESS);
	try{
		pConnection = pSession->GetFtpConnection(m_strFtpSite,m_strName,m_strPwd);
	}
	catch(CInternetException *e){
		e->Delete();
		pConnection = NULL;
		return FALSE;
	}
	if(pConnection != NULL){
		if (!pConnection->GetFile(strSourceName,strDesName)){
			pConnection->Close();
			delete pConnection;
			delete pSession;
			return FALSE;
		}
	}
	if (pConnection != NULL){
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return TRUE;
}

void CFtpFileDlg::OnRButtonDown( UINT nFlags, CPoint point )
{
	m_BtnDownLoad.EnableWindow(FALSE);
	m_BtnUpLoad.EnableWindow(TRUE);
	m_BtnQuery.EnableWindow(TRUE);
	m_EditFtp.EnableWindow(TRUE);
	m_EditName.EnableWindow(TRUE);
	m_EditPwd.EnableWindow(TRUE);
	CDialog::OnRButtonDown(nFlags,point);
}



void CFtpFileDlg::OnBnClickedCancel2()
{
	UpdateData(TRUE);
	m_BtnQuery.EnableWindow(FALSE);
	m_EditFtp.EnableWindow(FALSE);
	m_EditName.EnableWindow(FALSE);
	m_EditPwd.EnableWindow(FALSE);
	CString strSourceName,strDesName;
	CFileDialog fdlg(TRUE);
	if (fdlg.DoModal()==IDOK){
		strSourceName = fdlg.GetPathName();
		strDesName = fdlg.GetFileName();
		if (PutFile(strSourceName,strDesName)){
			AfxMessageBox("上传成功！",MB_OK|MB_ICONINFORMATION);
		}
		else{
			AfxMessageBox("上传失败！",MB_OK|MB_ICONSTOP);
		}
	}
	else{
		AfxMessageBox("请选择文件！",MB_OK|MB_ICONSTOP);
	}
	m_BtnUpLoad.EnableWindow(TRUE);
	m_BtnQuery.EnableWindow(TRUE);
	m_EditFtp.EnableWindow(TRUE);
	m_EditName.EnableWindow(TRUE);
	m_EditPwd.EnableWindow(TRUE);
}

BOOL CFtpFileDlg::PutFile( CString strSourceName,CString strDesName )
{
	CInternetSession* pSession;
	CFtpConnection *pConnection;
	pConnection = NULL;
	pSession = new CInternetSession(AfxGetAppName(),1,PRE_CONFIG_INTERNET_ACCESS);
	try{
		pConnection = pSession->GetFtpConnection(m_strFtpSite,m_strName,m_strPwd);
	}
	catch(CInternetException *e){
		e->Delete();
		pConnection = NULL;
		return FALSE;
	}
	if(pConnection != NULL){
		if (!pConnection->PutFile(strSourceName,strDesName)){
			pConnection->Close();
			delete pConnection;
			delete pSession;
			return FALSE;
		}
	}
	if (pConnection != NULL){
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return TRUE;
}
