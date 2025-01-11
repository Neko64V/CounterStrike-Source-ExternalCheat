#include "Overlay.h"

bool Overlay::InitOverlay(const char* targetName, int mode)
{
    if (mode == WINDOW_TITLE || mode == WINDOW_CLASS) {
        m_hTargetWindow = mode == WINDOW_TITLE ? FindWindowA(NULL, targetName) : FindWindowA(targetName, NULL);

        if (!m_hTargetWindow) {
            MessageBoxA(nullptr, "Target window not found.", "Initialize Failed", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }
    }
    else if (mode == PROCESS) {
        m_hTargetWindow = GetTargetWindow(targetName);

        if (!m_hTargetWindow) {
            MessageBoxA(nullptr, "Target window not found.", "Initialize Failed", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }
    }
    else {
        MessageBoxA(nullptr, "Invalid initialize option.", "Initialize Failed", MB_TOPMOST | MB_ICONERROR | MB_OK);
        return false;
    }

    GetClassNameA(m_hTargetWindow, m_CTargetClass, sizeof(m_CTargetClass));

    return CreateOverlay();
}

// [+]オーバーレイのウィンドウをゲームのウィンドウの上やサイズに合わせるための処理です。
void Overlay::OverlayManager()
{
    // Window Check
    HWND CheckHwnd = FindWindowA(m_CTargetClass, NULL);
    if (!CheckHwnd) {
        g.g_Run = false;
        return;
    }

    // StreamProof
    DWORD Flag = NULL;
    GetWindowDisplayAffinity(m_hHwnd, &Flag);
    if (g.g_StreamProof && Flag == WDA_NONE)
        SetWindowDisplayAffinity(m_hHwnd, WDA_EXCLUDEFROMCAPTURE);
    else if (!g.g_StreamProof && Flag == WDA_EXCLUDEFROMCAPTURE)
        SetWindowDisplayAffinity(m_hHwnd, WDA_NONE);

    // Window Style Changer
    HWND ForegroundWindow = GetForegroundWindow();
    LONG TmpLong = GetWindowLong(m_hHwnd, GWL_EXSTYLE);

    // ImGuiのio->mouse_pos でフラグを変更せずともGUIを操作できるやつにしてもいい
    if (g.g_ShowMenu && m_lMenuFlag != TmpLong)
        SetWindowLong(m_hHwnd, GWL_EXSTYLE, m_lMenuFlag);
    else if (!g.g_ShowMenu && m_lDefaultFlag != TmpLong)
        SetWindowLong(m_hHwnd, GWL_EXSTYLE, m_lDefaultFlag);

    // ShowMenu Toggle
    if (GetKeyState(g.g_MenuKey) && !g.g_ShowMenu) {
        if (ForegroundWindow != m_hHwnd)
            SetForegroundWindow(m_hHwnd);

        g.g_ShowMenu = true;
    }
    else if (!GetKeyState(g.g_MenuKey) && g.g_ShowMenu) {
        if (ForegroundWindow != CheckHwnd)
            SetForegroundWindow(CheckHwnd);

        g.g_ShowMenu = false;
    }

    // Window Resizer
    RECT TmpRect{};
    POINT TmpPoint{};
    GetClientRect(CheckHwnd, &TmpRect);
    ClientToScreen(CheckHwnd, &TmpPoint);

    if (TmpRect.left != g.g_GameRect.left || TmpRect.bottom != g.g_GameRect.bottom || TmpRect.top != g.g_GameRect.top || TmpRect.right != g.g_GameRect.right || TmpPoint.x != g.g_GamePoint.x || TmpPoint.y != g.g_GamePoint.y)
    {
        g.g_GameRect = TmpRect;
        g.g_GamePoint = TmpPoint;
        SetWindowPos(m_hHwnd, nullptr, TmpPoint.x, TmpPoint.y, g.g_GameRect.right, g.g_GameRect.bottom, SWP_NOREDRAW);
    }
}

HWND Overlay::GetTargetWindow(const std::string processName)
{
    DWORD PID = NULL;
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    while (Process32Next(snapShot, &entry)) {
        if (!processName.compare(entry.szExeFile)) {
            PID = entry.th32ProcessID;
            break;
        }
    }

    CloseHandle(snapShot);

    HWND hwnd = GetTopWindow(NULL);
    do {
        if (GetWindowLong(hwnd, 8) != 0 || !IsWindowVisible(hwnd))
            continue;
        DWORD ProcessID;
        GetWindowThreadProcessId(hwnd, &ProcessID);
        if (PID == ProcessID)
            return hwnd;
    } while ((hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) != NULL);

    return NULL;
}
