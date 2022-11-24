
// MyInjecterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyInjecter.h"
#include "MyInjecterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyInjecterDlg 对话框



CMyInjecterDlg::CMyInjecterDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MYINJECTER_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyInjecterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST3, m_list);
}

BEGIN_MESSAGE_MAP(CMyInjecterDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CMyInjecterDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CMyInjecterDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CMyInjecterDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON4, &CMyInjecterDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON5, &CMyInjecterDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON6, &CMyInjecterDlg::OnBnClickedButton2)
    ON_WM_CLOSE()
END_MESSAGE_MAP()




void CMyInjecterDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    EndDialog(IDCANCEL);
    CDialogEx::OnClose();
}



void CMyInjecterDlg::OnOK()//啥也不做就OK了
{

}

void CMyInjecterDlg::OnCancel()//同上
{
    
}

BOOL CMyInjecterDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    CRect rect;
    m_list.GetClientRect(&rect);
    int one = rect.Width() / 10;
    
    m_list.InsertColumn(0, TEXT("Index"), LVCFMT_LEFT, one);
    m_list.InsertColumn(1, TEXT("PID"), LVCFMT_LEFT, one * 2);
    m_list.InsertColumn(2, TEXT("Arc"), LVCFMT_LEFT, one);
    m_list.InsertColumn(3, TEXT("ProcessName"), LVCFMT_LEFT, one * 6);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyInjecterDlg::OnPaint()
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
HCURSOR CMyInjecterDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CMyInjecterDlg::OnBnClickedButton1()
{
    m_list.DeleteAllItems();

    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hProcessSanp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSanp == INVALID_HANDLE_VALUE)
    {
        MessageBox(L"Error Get the Process SnapShot\n");
        
    }

    BOOL bMore = Process32First(hProcessSanp, &pe32);
    int ItemIndex = 0;
    while (bMore)
    {
        CString strTEMP;

        m_list.InsertItem(ItemIndex, L"");
        m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        strTEMP.Format(L"%d", ItemIndex + 1);
        m_list.SetItemText(ItemIndex, 0, strTEMP);//index

        strTEMP.Format(L"%d/0x%x", pe32.th32ProcessID, pe32.th32ProcessID);
        m_list.SetItemText(ItemIndex, 1, strTEMP);//pid

        int a;
        HANDLE hpid = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
        IsWow64Process(hpid, &a);
        CloseHandle(hpid);
        strTEMP.Format(L"x%d", a ? 32 : 64);
        m_list.SetItemText(ItemIndex, 2, strTEMP);//Arc

        m_list.SetItemText(ItemIndex, 3, pe32.szExeFile);//FileName

        bMore = Process32Next(hProcessSanp, &pe32);
        ItemIndex++;
    }
    CloseHandle(hProcessSanp);

}

int g_CurArc;
void CMyInjecterDlg::OnBnClickedButton2()
{

    int mark = m_list.GetSelectionMark();
    int pid = _ttoi(m_list.GetItemText(mark, 1));
    g_CurArc = m_list.GetItemText(mark, 2).Compare(L"x86");
    if (!pid)
    {
        MessageBox(L"PID Error!");
        return;
    }
    char dllpath[256];
    GetDlgItemTextA(m_hWnd, IDC_MFCEDITBROWSE1, dllpath, 256);
    
    if (!strcmp(dllpath,""))
    {
        MessageBox(L"Have not choose dll");
    }
    int clickID = GetFocus()->GetDlgCtrlID();
    switch (clickID)
    {
        case IDC_BUTTON2:
            InjeceByRemoteThread(pid, dllpath);
            break;	
        case IDC_BUTTON3:
            InjeceByNtCreateThread(pid, dllpath);
            break;
        case IDC_BUTTON4:
            InjeceByRTLCreateThread(pid, dllpath);
            break;
        case IDC_BUTTON5:
            InjeceByAPC(pid, dllpath);
            break;
        case IDC_BUTTON6:
            InjeceByRobThread(pid, dllpath);
            break;
    }

    
}

void InjeceByRemoteThread(int pid, char* path)
{
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (hp)
    {
        PVOID mem = VirtualAllocEx(hp, 0, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (mem)
        {
            if (WriteProcessMemory(hp, mem, path, strlen(path), 0))
            {
                PVOID LoadLibraryABase = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryA");
                if (g_CurArc)
                {
                    LoadLibraryABase = (LPTHREAD_START_ROUTINE)system("loadLibrary_x86_address.exe");
                }
                HANDLE tid = CreateRemoteThread(hp, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryABase, mem, 0, 0);
                if (tid)
                {
                    CloseHandle(hp);
                }
                else
                {
                    MessageBox(0, L"CreateRemoteThread Error", L"Error", 0);
                }
            }
            else
            {
                MessageBox(0, L"WriteProcessMemory Error", L"Error", 0);
            }
        }
        else
        {
            MessageBox(0, L"VirtualAllocEx Error", L"Error", 0);
        }

    }
    else
    {
        MessageBox(0, L"OpenProcess Error", L"Error", 0);
    }
    return;


}

void InjeceByNtCreateThread(int pid, char* path)
{
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (hp)
    {
        PVOID mem = VirtualAllocEx(hp, 0, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (mem)
        {
            if (WriteProcessMemory(hp, mem, path, strlen(path), 0))
            {
                PVOID LoadLibraryABase = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryA");
                PfnZwCreateThreadEx ZwCreateThreadEx = (PfnZwCreateThreadEx)GetProcAddress(GetModuleHandle(L"ntdll"), "ZwCreateThreadEx");
                
                HANDLE hThreadHandle = NULL;
                ZwCreateThreadEx(&hThreadHandle, PROCESS_ALL_ACCESS, 0, hp, (LPTHREAD_START_ROUTINE)LoadLibraryABase, mem, 0, 0, 0, 0, 0);
                CloseHandle(hp);
            }
            else
            {
                MessageBox(0, L"WriteProcessMemory Error", L"Error", 0);
            }
        }
        else
        {
            MessageBox(0, L"VirtualAllocEx Error", L"Error", 0);
        }

    }
    else
    {
        MessageBox(0, L"OpenProcess Error", L"Error", 0);
    }
    return;
}

void InjeceByRTLCreateThread(int pid, char* path)
{
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (hp)
    {
        PVOID mem = VirtualAllocEx(hp, 0, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (mem)
        {
            if (WriteProcessMemory(hp, mem, path, strlen(path), 0))
            {
                PVOID LoadLibraryABase = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryA");
                pRtlCreateUserThread RtlCreateUserThread = (pRtlCreateUserThread)GetProcAddress(GetModuleHandle(L"ntdll"), "RtlCreateUserThread");

                RtlCreateUserThread(hp, 0, 0, 0, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryABase, mem, 0, 0);
                CloseHandle(hp);
            }
            else
            {
                MessageBox(0, L"WriteProcessMemory Error", L"Error", 0);
            }
        }
        else
        {
            MessageBox(0, L"VirtualAllocEx Error", L"Error", 0);
        }

    }
    else
    {
        MessageBox(0, L"OpenProcess Error", L"Error", 0);
    }
    return;
}

void InjeceByAPC(int pid, char* path)
{
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (hp)
    {
        PVOID mem = VirtualAllocEx(hp, 0, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (mem)
        {
            if (WriteProcessMemory(hp, mem, path, strlen(path), 0))
            {
                PVOID LoadLibraryABase = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryA");
                
                THREADENTRY32 te32;

                te32.dwSize = sizeof(THREADENTRY32);
                HANDLE hThreadSanp = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                if (hThreadSanp == INVALID_HANDLE_VALUE)
                {
                    MessageBox(0, L"Error Get the Process SnapShot\n", L"Error", 0);

                }

                BOOL bMore = Thread32First(hThreadSanp, &te32);
                while (bMore)
                {
                    if (pid == te32.th32OwnerProcessID)
                    {
                        HANDLE ht = OpenThread(THREAD_ALL_ACCESS, 0, te32.th32ThreadID);
                        if (ht)
                        {
                            QueueUserAPC((PAPCFUNC)LoadLibraryABase, ht, (ULONG_PTR)mem);
                            CloseHandle(ht);
                        }
                        else
                        {
                            MessageBox(0, L"OpenThread Error\n", L"Error", 0);
                        }
                        
                    }
                    bMore = Thread32Next(hThreadSanp, &te32);
                }
                CloseHandle(hp);
            }
            else
            {
                MessageBox(0, L"WriteProcessMemory Error", L"Error", 0);
            }
        }
        else
        {
            MessageBox(0, L"VirtualAllocEx Error", L"Error", 0);
        }

    }
    else
    {
        MessageBox(0, L"OpenProcess Error", L"Error", 0);
    }
    return;
}

void InjeceByRobThread(int pid, char* path)
{
    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (hp)
    {
        PVOID mem = VirtualAllocEx(hp, 0, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (mem)
        {
            if (WriteProcessMemory(hp, (PVOID)((DWORD64)mem + 512), path, strlen(path), 0))
            {
                PVOID LoadLibraryABase = GetProcAddress(GetModuleHandle(L"kernel32"), "LoadLibraryA");

                THREADENTRY32 te32;

                te32.dwSize = sizeof(THREADENTRY32);
                HANDLE hThreadSanp = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                if (hThreadSanp == INVALID_HANDLE_VALUE)
                {
                    MessageBox(0, L"Error Get the Process SnapShot\n", L"Error", 0);
                }

                BOOL bMore = Thread32First(hThreadSanp, &te32);
                while (bMore)
                {
                    if (pid == te32.th32OwnerProcessID)
                    {
                        break;
                    }
                    bMore = Thread32Next(hThreadSanp, &te32);
                }

                HANDLE ht = OpenThread(THREAD_ALL_ACCESS, 0, te32.th32ThreadID);
                if (ht)
                {
                    SuspendThread(ht);
                    CONTEXT context = {0};
                    context.ContextFlags = CONTEXT_ALL;
                    GetThreadContext(ht, &context);

                    unsigned char shell[0x100] = 
                    {
                        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, 0x9C, 
                        0x48, 0x83, 0xEC, 0x50, //sub rsp, 50
                        0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rcx,0
                        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rax,0
                        0xFF, 0xD0, 
                        0x48, 0x83, 0xC4, 
                        0x50, 0x9D, 0x41, 0x5F, 0x41, 0x5E, 0x41, 0x5D, 0x41, 0x5C, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5F, 0x5E, 0x5D, 0x5C, 0x5B, 0x5A, 0x59, 0x58, 
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,           
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00////jmp [$+12]
                    };
                    *(DWORD64*)(shell + 0x1F) = (DWORD64)mem + 512;
                    *(DWORD64*)(shell + 0x29) = (DWORD64)LoadLibraryABase;
                    *(DWORD64*)(shell + 0x56) = context.Rip;
                    if (!WriteProcessMemory(hp, mem, shell, sizeof(shell), 0))
                    {
                        MessageBox(0, L"WriteProcessMemory Error", L"Error", 0);
                    }

                    context.Rip = (DWORD64)mem;
                    SetThreadContext(ht, &context);
                    ResumeThread(ht);
                }
                else
                {
                    MessageBox(0, L"OpenThread Error\n", L"Error", 0);
                }
                CloseHandle(hp);
            }
            else
            {
                MessageBox(0, L"WriteProcessMemory Error", L"Error", 0);
            }
        }
        else
        {
            MessageBox(0, L"VirtualAllocEx Error", L"Error", 0);
        }

    }
    else
    {
        MessageBox(0, L"OpenProcess Error", L"Error", 0);
    }
    return;

}




