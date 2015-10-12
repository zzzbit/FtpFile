
// FtpFileDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CFtpFileDlg 对话框
class CFtpFileDlg : public CDialogEx
{
// 构造
public:
	CFtpFileDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FTPFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void ListContent();
	BOOL GetFile(CString strSourceName,CString strDesName);
	void OnRButtonDown(UINT nFlags, CPoint point);
	BOOL PutFile(CString strSourceName,CString strDesName);
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditFtp;
	CString m_strFtpSite;
	CEdit m_EditName;
	CString m_strName;
	CEdit m_EditPwd;
	CString m_strPwd;
	CButton m_BtnQuery;
	CButton m_BtnUpLoad;
	CButton m_BtnDownLoad;
	CButton m_BtnExit;
	afx_msg void OnBnClickedOk();
	CListBox m_ListFile;
	afx_msg void OnBnClickedCancel3();
	afx_msg void OnBnClickedCancel2();
};
