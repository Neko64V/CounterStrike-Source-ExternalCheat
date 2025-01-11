#include "Cheat/FrameCore.h"
#include "Framework/Overlay/Overlay.h"

Overlay* overlay = new Overlay;
CFramework*  hax = new CFramework;

// ここにレンダリングしたいコンテンツ ( ESP, Menu and more )等を入れる.
void Overlay::OverlayUserFunction()
{
	if (g.g_BunnyHop)
		hax->BunnyHop();

	hax->RenderInfo();

	if (g.g_ESP)
		hax->RenderESP();

	if (g.g_ShowMenu)
		hax->RenderMenu();
}

// DEBUG時にはコンソールウィンドウを表示する.
#if _DEBUG
int main()
#else 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// Apexのウィンドウをベースにして初期化を行う
	if (!m.AttachProcess("Counter-Strike Source", InitMode::WINDOW_TITLE)) // 詳細は Memory/Memory.h を参照.
		return 1;

	// ベースアドレスを取得
	m.GetBaseAddress("client.dll", "engine.dll");

	// Overlay
	if (!overlay->InitOverlay("Counter-Strike Source", InitMode::WINDOW_TITLE)) // MemoryInitModeと同様.
		return 2;

	// スレッドを作成
	std::thread([&]() { hax->UpdateList(); }).detach(); // ESP/AIM用にプレイヤーのデータをキャッシュする.

	overlay->OverlayLoop();
	overlay->DestroyOverlay();
	m.DetachProcess();
	g.g_Run = false;
	delete hax, overlay;

	return 0;
}