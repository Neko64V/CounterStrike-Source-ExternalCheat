#pragma once
#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_impl_win32.h"
#include "..\ImGui\imgui_impl_dx11.h"
#include "..\Utils\Utils.h";
#include <dwmapi.h>
#include <thread>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class Overlay
{
private:
	// オーバーレイに登録する情報
	WNDCLASSEXA wc{};
	HWND m_hHwnd;
	char m_CTitle[32] = "The Overlay";
	char m_CClass[32] = "WND_CLS";
	LONG m_lMenuFlag = WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	LONG m_lDefaultFlag = WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	
	// ターゲットウィンドウに関する情報
	HWND m_hTargetWindow;
	char m_CTargetClass[128]{};
	RECT m_GameRect{};
	POINT m_GamePoint{};

	bool CreateOverlay();
	HWND GetTargetWindow(const std::string processName);	// 実行ファイル名からウィンドウハンドルを取得する
public:
	bool InitOverlay(const char* targetName, int InitMode);
	void OverlayUserFunction();
	void OverlayLoop();
	void DestroyOverlay();
	void OverlayManager();
};