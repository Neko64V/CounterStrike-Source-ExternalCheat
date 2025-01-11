#pragma once
#include "../Framework/ImGui/imgui.h"
#include "../Framework/ImGui/imgui_impl_win32.h"
#include "../Framework/ImGui/imgui_impl_dx11.h"
#include "../Framework/ImGui/Custom.h"
#include "SDK/CEntity/CEntity.h"

class CFramework
{
public:
    // Render
	void RenderInfo();
	void RenderMenu();
	void RenderESP();

    // Misc
    void UpdateList();
    void BunnyHop();
private:
    CEntity local, *pLocal = &local;
    std::vector<CEntity> EntityList;

    // Colors
    ImColor ESP_Default  = { 1.f, 1.f, 1.f, 1.f };
    ImColor ESP_Team     = { 0.f, 1.f, 1.f, 1.f };
    ImColor ESP_Filled  = { 0.f, 0.f, 0.f, 0.2f };
    ImColor ESP_Shadow = { 0.f, 0.f, 0.f, 0.3f };
    ImColor CrosshairColor = { 0.f, 1.f, 0.f, 1.f };

    void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
    {
        ImGui::GetBackgroundDrawList()->AddLine(a, b, color, width);
    }
    void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
    {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2((int)x0, (int)y0), ImVec2((int)x1, (int)y1), color, rounding, rounding_corners_flags);
    }
    void HealthBar(float x, float y, float w, float h, int value, int v_max)
    {
        RectFilled(x, y, x + w, y + h, ImColor(0.f, 0.f, 0.f, 0.725f), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), ImColor(min(510 * (v_max - value) / 100, 255), min(510 * value / 100, 255), 25, 255), 0.0f, 0);
    }
    void Circle(ImVec2 pos, float fov_size, ImColor color)
    {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2((int)pos.x, (int)pos.y), fov_size, color, 100, 0);
    }
    void String(ImVec2 pos, ImColor color, const char* text)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2((int)pos.x, (int)pos.y), color, text, text + strlen(text), 1024, 0);
    }
    void StringEx(ImVec2 pos, ImColor color, float font_size, const char* text)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2((int)pos.x + 1, (int)pos.y + 1), ImColor(0.f, 0.f, 0.f, 1.f), text, text + strlen(text), 1024, 0);
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2((int)pos.x, (int)pos.y), color, text, text + strlen(text), 1024, 0);
    }
};