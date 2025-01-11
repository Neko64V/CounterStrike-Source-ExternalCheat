#pragma once
#include <Windows.h>

struct Globals
{
    // System
    bool g_Run = true;
    bool g_ShowMenu = false;

    // GameData
    HWND g_GameHwnd;
    RECT g_GameRect{};
    POINT g_GamePoint{};

    // Visual
    bool g_ESP      = true;
    bool g_ESP_Team = false;
    bool g_ESP_Box  = true;
    bool g_ESP_BoxFilled = false;
    bool g_ESP_Line      = false;
    bool g_ESP_Skeleton = true;
    bool g_ESP_HealthBar = true;
    bool g_ESP_Distance = true;
    bool g_ESP_Name      = true;
    int g_ESP_BoxType    = 0;

    // Misc
    bool g_BunnyHop = true;

    // System
    bool g_StreamProof = false;
    bool g_Crosshair = false;
    int g_CrosshairSize = 3;
    int g_CrosshairType = 0;

    // Key
    int g_MenuKey = VK_INSERT;
};

extern Globals g;
