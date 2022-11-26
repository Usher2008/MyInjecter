#pragma once
#include "pch.h"


// CDialog2 对话框

class CDialog2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDialog2)

public:
	CDialog2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialog2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODULE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ShowModuleList(std::vector<MODULEENTRY32>);
	void OnClose();
	void OnOK();
	void OnCancel();
	BOOL CDialog2::OnInitDialog();
	CListCtrl m_list;
};

std::vector<MODULEENTRY32> GetModuleList();