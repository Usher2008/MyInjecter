// CDialog2.cpp: 实现文件
//

#include "pch.h"

// CDialog2 对话框

IMPLEMENT_DYNAMIC(CDialog2, CDialogEx)

CDialog2::CDialog2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODULE_DIALOG, pParent)
{

}

CDialog2::~CDialog2()
{
}

void CDialog2::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CDialog2, CDialogEx)
    ON_WM_CREATE()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDialog2 消息处理程序


int CDialog2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

    

	return 0;
}

BOOL CDialog2::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CRect rect;
    m_list.GetClientRect(&rect);
    int one = rect.Width() / 10;
    //base  = 0x % 08X"), (DWORD)me32.modBaseAddr);
    //MODULE NAME:     %s"), me32.szModule)
    // base size      = %d"), me32.modBaseSize);
    m_list.InsertColumn(0, TEXT("Addr"), LVCFMT_LEFT, one * 3);
    m_list.InsertColumn(1, TEXT("Size"), LVCFMT_LEFT, one * 2);
    m_list.InsertColumn(2, TEXT("Name"), LVCFMT_LEFT, one * 5);

    SetWindowTextW(g_PName);
    ShowModuleList(GetModuleList());

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDialog2::ShowModuleList(std::vector<MODULEENTRY32> me32)
{
    m_list.DeleteAllItems();
    if (me32.size() == 0)
    {
        return;
    }
    int ItemIndex = 0;
    for (auto i = me32.begin(); i != me32.end(); ++i)
    {
        CString strTEMP;

        m_list.InsertItem(ItemIndex, L"");
        m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        strTEMP.Format(L"%p", i->modBaseAddr);
        m_list.SetItemText(ItemIndex, 0, strTEMP);//addr

        strTEMP.Format(L"0x%08x", i->modBaseSize);
        m_list.SetItemText(ItemIndex, 1, strTEMP);//size

        strTEMP.Format(L"%s", i->szModule);
        m_list.SetItemText(ItemIndex, 2, strTEMP);//name
        ItemIndex++;
    }
}

std::vector<MODULEENTRY32> GetModuleList()
{
    std::vector<MODULEENTRY32> meList;
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, g_pid);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        ::MessageBox(0, L"Error Get the Process SnapShot\n", L"Error", 0);
        CloseHandle(hModuleSnap);
        return meList;
    }
    if (!Module32First(hModuleSnap, &me32))
    {
        ::MessageBoxW(0, TEXT("Module32First"), 0, 0);  // Show cause of failure 
        CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
        return meList;
    }
    do
    {
        meList.push_back(me32);
        //_tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
        //_tprintf(TEXT("\n     executable     = %s"), me32.szExePath);
        //_tprintf(TEXT("\n     process ID     = 0x%08X"), me32.th32ProcessID);
        //_tprintf(TEXT("\n     ref count (g)  =     0x%04X"), me32.GlblcntUsage);
        //_tprintf(TEXT("\n     ref count (p)  =     0x%04X"), me32.ProccntUsage);
        //_tprintf(TEXT("\n     base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
        //_tprintf(TEXT("\n     base size      = %d"), me32.modBaseSize);

    } while (Module32Next(hModuleSnap, &me32));
    return meList;
}


void CDialog2::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    EndDialog(IDCANCEL);
    CDialogEx::OnClose();
}



void CDialog2::OnOK()//啥也不做就OK了
{

}

void CDialog2::OnCancel()//同上
{

}