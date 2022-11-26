#include "pch.h"


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
    DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_edit);
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
//    ON_WM_DROPFILES()
ON_WM_DROPFILES()
//ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CMyInjecterDlg::OnEnChangeMfceditbrowse1)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CMyInjecterDlg::OnLvnItemchangedList3)
ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CMyInjecterDlg::OnNMRClickList3)
//ON_NOTIFY(HDN_ITEMCLICK, 0, &CMyInjecterDlg::OnHdnItemclickList3)
//ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CMyInjecterDlg::OnLvnColumnclickList3)
//ON_NOTIFY(NM_SETFOCUS, IDC_LIST3, &CMyInjecterDlg::OnNMSetfocusList3)
//ON_NOTIFY(NM_CLICK, IDC_LIST3, &CMyInjecterDlg::OnNMClickList3)
//ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST3, &CMyInjecterDlg::OnLvnOdcachehintList3)

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
int g_pid = 0;
void CMyInjecterDlg::OnBnClickedButton2()
{
    if (!g_pid)
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
            InjeceByRemoteThread(g_pid, dllpath);
            break;	
        case IDC_BUTTON3:
            InjeceByNtCreateThread(g_pid, dllpath);
            break;
        case IDC_BUTTON4:
            InjeceByRTLCreateThread(g_pid, dllpath);
            break;
        case IDC_BUTTON5:
            InjeceByAPC(g_pid, dllpath);
            break;
        case IDC_BUTTON6:
            InjeceByRobThread(g_pid, dllpath);
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

void CMyInjecterDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    WORD wNumFileDropped = DragQueryFile(hDropInfo, -1, NULL, 0);

    CString firstFile = L"";

    for (WORD x = 0; x < wNumFileDropped; x++) {
        WORD wPathnameSize = DragQueryFileA(hDropInfo, x, NULL, 0);
        char* npszFile = (char*)LocalAlloc(LPTR, wPathnameSize += 1);

        if (npszFile == NULL) {
            continue;
        }

        DragQueryFileA(hDropInfo, x, npszFile, wPathnameSize);
        if (firstFile == "") {
            firstFile = npszFile;
        }

        LocalFree(npszFile);

    }


    DragFinish(hDropInfo);

    m_edit.SetWindowTextW(firstFile);
    UpdateData(FALSE);
    CDialogEx::OnDropFiles(hDropInfo);
}

void ProcessMoudle()
{
    if (g_pid != 0)
    {
        CDialog2* dlg;
        dlg = new CDialog2();
        dlg->DoModal();
    }
    return;
    //CRect rc;
    //GetClientRect(m_hwnd,&rc);
    //dlg->Create(IDD_MODULE_DIALOG);
    //dlg->MoveWindow(&rc);
    //dlg->ShowWindow(sw_show);
    
}

void CMyInjecterDlg::OnNMRClickList3(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    int mark = m_list.GetSelectionMark();
    g_CurArc = m_list.GetItemText(mark, 2).Compare(L"x86");
    g_pid = _ttoi(m_list.GetItemText(mark, 1));
    g_PName = m_list.GetItemText(mark, 3);
    HMENU hMenu = ::CreatePopupMenu();
    HMENU hMenu2 = ::CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, 10001, _T("遍历模块"));
    AppendMenu(hMenu, MF_STRING, 10002, _T("菜单项2"));
    AppendMenu(hMenu2, MF_STRING, 10003, _T("菜单项1"));
    AppendMenu(hMenu2, MF_STRING, 10004, _T("菜单项1"));
    AppendMenu(hMenu, MF_POPUP, (LONG)hMenu2, _T("子菜单2"));
    
    
    CPoint pt;
    
    GetCursorPos(&pt);
    //获得当前鼠标位置

    UINT Cmd = (UINT)::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL);
    //弹出菜单

    switch(Cmd)//响应点击的菜单
    {
        case 10001:
            ProcessMoudle();
            break;
    }

    *pResult = 0;
}
CString g_PName = {};
void CMyInjecterDlg::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    int mark = m_list.GetSelectionMark();
    g_CurArc = m_list.GetItemText(mark, 2).Compare(L"x86");
    g_pid = _ttoi(m_list.GetItemText(mark, 1));
    g_PName = m_list.GetItemText(mark, 3);
    *pResult = 0;
}
