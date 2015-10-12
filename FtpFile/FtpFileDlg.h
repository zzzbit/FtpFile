
// FtpFileDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CFtpFileDlg �Ի���
class CFtpFileDlg : public CDialogEx
{
// ����
public:
	CFtpFileDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FTPFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
