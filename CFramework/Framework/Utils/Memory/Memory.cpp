#include "Memory.h"

bool Memory::AttachProcess(const char* targetName, int mode)
{
    if (mode == WINDOW_TITLE || mode == WINDOW_CLASS) {
        // タイトルかクラス名でウィンドウハンドルを取得
        HWND TargetHwnd = mode == WINDOW_TITLE ? FindWindowA(NULL, targetName) : FindWindowA(targetName, NULL);

        // ウィンドウが存在しなかったら
        if (!TargetHwnd) {
            MessageBoxA(nullptr, "Please open CounterStrike: Source", "Initialize Failed", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        // PIDを取得
        GetWindowThreadProcessId(TargetHwnd, &m_dwPID);
    }
    else if (mode == PROCESS) {
        // 実行ファイル名から PROCESSENTRY32 の情報を取得
        PROCESSENTRY32 process = GetProcess(targetName);

        // PIDが0 == プロセスが実行されていなかったら
        if (process.th32ProcessID == 0) {
            MessageBoxA(nullptr, "Please open CounterStrike: Source", "Initialize Failed", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        m_dwPID = process.th32ProcessID;
    }
    else {
        // 無効な選択肢
        MessageBoxA(nullptr, "Invalid initialize option.", "Initialize Failed", MB_TOPMOST | MB_ICONERROR | MB_OK);
        return false;
    }

    // プロセスハンドルを取得
    m_hHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwPID);

    if (!m_hHandle) {
        MessageBoxA(nullptr, "Failed to get process handle", "Initialize Failed", MB_TOPMOST | MB_ICONERROR | MB_OK); // 管理者として実行するかアンチチートをバイパスする
        return false;
    }

    return true;
}

void Memory::GetBaseAddress(const char* target0, const char* target1)
{
    // ここだけ書き換えた方がいいかもね
    m_gClientBaseAddr = GetModuleBase(target0);
    m_gEngineBaseAddr = GetModuleBase(target1);
}

void Memory::DetachProcess()
{
    CloseHandle(m_hHandle);
}

uintptr_t Memory::GetModuleBase(const std::string moduleName)
{
    MODULEENTRY32 entry{};
    entry.dwSize = sizeof(MODULEENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwPID);

    while (Module32Next(snapShot, &entry))
    {
        if (!moduleName.compare(entry.szModule)) {
            CloseHandle(snapShot);
            return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
        }
    }

    CloseHandle(snapShot);

    return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
}

PROCESSENTRY32 Memory::GetProcess(const std::string processName)
{
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    while (Process32Next(snapShot, &entry))
    {
        if (!processName.compare(entry.szExeFile)) {
            CloseHandle(snapShot);
            return entry;
        }
    }

    CloseHandle(snapShot);

    return PROCESSENTRY32();
}

Memory m;
