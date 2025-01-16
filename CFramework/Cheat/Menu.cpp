#include "FrameCore.h"

// ImGui::Combo/ImGui::List等で使う文字列群
const char* BoxTypeList[] = { "2D Simple", "2D Cornered" };
const char* CrosshairList[] = { "Cross", "Circle" };
std::vector<std::string> MenuSelectList = { "Visual", "Misc", "System" };

// チートのメニュー
void CFramework::RenderMenu()
{
    // Setup
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImGuiStyle& style = ImGui::GetStyle();
    static float DefaultSpacing = style.ItemSpacing.y;
    style.ChildBorderSize = 1.f;
    static int Index = 0;
    static int BindingID = 0;

    ImGui::SetNextWindowBgAlpha(0.975f);
    ImGui::SetNextWindowSize(ImVec2(725.f, 450.f));
    ImGui::Begin("CounterStrike: Source [ EXTERNAL ]", &g.g_ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    //---// Clild 0 //-----------------------------------//
    ImGui::BeginChild("##SelectChild", ImVec2(150.f, ImGui::GetContentRegionAvail().y), false);

    ImGui::NewLine();

    for (int i = 0; i < MenuSelectList.size(); i++)
    {
        if (ImGui::CustomButton(MenuSelectList[i].c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 35.f), Index == i ? true : false))
            Index = i;
    }

    ImGui::EndChild();
    //---// Clild 0 End //-------------------------------//

    ImGui::SameLine();

    //---// Clild 1 //-----------------------------------//
    ImGui::BeginChild("##ContextChild", ImVec2(ImGui::GetContentRegionAvail()), false);

    //---// Left //--------------------------------------//
    ImGui::BeginChild("##LeftBase", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 16.f, ImGui::GetContentRegionAvail().y), false);

    switch (Index)
    {
    case 0: // visual
        ImGui::BeginChild("##LeftChild_0", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);

        ImGui::Text("Visual");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("ESP", &g.g_ESP);
        ImGui::Checkbox("Team ESP", &g.g_ESP_Team);

        ImGui::EndChild();

        ImGui::BeginChild("##LeftChild_0-1", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("ESP Options");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("Box", &g.g_ESP_Box);
        ImGui::Checkbox("BoxFilled", &g.g_ESP_BoxFilled);
        ImGui::Checkbox("Line", &g.g_ESP_Line);
        ImGui::Checkbox("Skeleton", &g.g_ESP_Skeleton);
        ImGui::Checkbox("HealthBar", &g.g_ESP_HealthBar);
        ImGui::Checkbox("Name", &g.g_ESP_Name);
        ImGui::Checkbox("Distance", &g.g_ESP_Distance);

        ImGui::EndChild();
        break;
    case 1: // misc
        ImGui::BeginChild("##LeftChild_1-0", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 4.f), true);

        ImGui::Text("Misc");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("BunnyHop", &g.g_BunnyHop);

        ImGui::EndChild();
        ImGui::BeginChild("##LeftChild_1-1", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::EndChild();
        break;
    case 2: // system
        ImGui::BeginChild("##LeftChild_2-0", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);

        ImGui::Text("System");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("StreamProof", &g.g_StreamProof);

        ImGui::EndChild();
        ImGui::BeginChild("##LeftChild_2-1", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("Crosshair");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Crosshair", &g.g_Crosshair);
        ImGui::CustomSliderInt("CrosshairSize", "##SizeCH", &g.g_CrosshairSize, 1, 10);
        ImGui::ColorEdit4("Color##C", &CrosshairColor.Value.x);
        ImGui::Combo("Type##C", &g.g_CrosshairType, CrosshairList, IM_ARRAYSIZE(CrosshairList));

        ImGui::EndChild();
        break;
    default:
        break;
    }

    ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::SameLine();

    //---// Right //--------------------------------------//
    ImGui::BeginChild("##RightChild", ImVec2(ImGui::GetContentRegionAvail()), false);

    ImGui::BeginChild("##RightChild_0", ImVec2(ImGui::GetContentRegionAvail()), true);

    switch (Index)
    {
    case 0: // visual
        ImGui::Text("ESP Setting");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Combo("BoxType", &g.g_ESP_BoxType, BoxTypeList, IM_ARRAYSIZE(BoxTypeList));

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("ESP Colors");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::BulletText("Main Color");
        ImGui::ColorEdit4("Default", &ESP_Default.Value.x);
        ImGui::ColorEdit4("Team", &ESP_Team.Value.x);

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::BulletText("Sub Color");
        ImGui::ColorEdit4("BoxFilled", &ESP_Filled.Value.x);
        break;
    case 1: // misc
        break;
    case 2: // system
    {

        // ExitButton
        float size = ImGui::GetContentRegionAvail().y - 30.f - ImGui::GetFontSize() - (style.ItemSpacing.y * 2);
        ImGui::SetCursorPosY(size);
        ImGui::Text("Exit");
        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::Button("Exit", ImVec2(ImGui::GetContentRegionAvail().x, 30.f)))
            g.g_Run = false;
    }   break;
    default:
        break;
    }

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::End();
}