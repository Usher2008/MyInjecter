
// MyInjecterDlg.h: 头文件
//

#pragma once


// CMyInjecterDlg 对话框
class CMyInjecterDlg : public CDialogEx
{
// 构造
public:
	CMyInjecterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYINJECTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListCtrl m_list;
	afx_msg void OnBnClickedButton2();

	
};


void InjeceByRemoteThread(int pid, char* path);

typedef DWORD(WINAPI* PfnZwCreateThreadEx)(
	PHANDLE ThreadHandle,
	ACCESS_MASK DesiredAccess,
	LPVOID ObjectAttributes,
	HANDLE ProcessHandle,
	LPTHREAD_START_ROUTINE lpStartAddress,
	LPVOID lpParameter,
	ULONG CreateThreadFlags,
	SIZE_T ZeroBits,
	SIZE_T StackSize,
	SIZE_T MaximunStackSize,
	LPVOID pUnkown);

void InjeceByNtCreateThread(int pid, char* path);

typedef DWORD(WINAPI* pRtlCreateUserThread)(    //函数原型
	IN HANDLE                     ProcessHandle,
	IN PSECURITY_DESCRIPTOR     SecurityDescriptor,
	IN BOOL                     CreateSuspended,
	IN ULONG                    StackZeroBits,
	IN OUT PULONG                StackReserved,
	IN OUT PULONG                StackCommit,
	IN LPVOID                    StartAddress,
	IN LPVOID                    StartParameter,
	OUT HANDLE                     ThreadHandle,
	OUT LPVOID                    ClientID
	);

void InjeceByRTLCreateThread(int pid, char* path);
void InjeceByAPC(int pid, char* path);
void InjeceByRobThread(int pid, char* path);