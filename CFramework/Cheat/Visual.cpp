#include "FrameCore.h"

// いちいち "ImColor(0.f, 0.f, 0.f, 1.f);" のように書くのはめんどうなのでここで定義
ImColor BLACK = ImColor(0.f, 0.f, 0.f, 1.f);
ImColor TEXT_COLOR = ImColor(1.f, 1.f, 1.f, 1.f);

void CFramework::RenderInfo()
{
    // ウォーターマーク的な.
    const auto str = "Counter-Strike: Source External - FPS: " + std::to_string((int)ImGui::GetIO().Framerate);
    String(ImVec2(5.f, 5.f), TEXT_COLOR, str.c_str());

    // Crosshair.
    if (g.g_Crosshair)
    {
        switch (g.g_CrosshairType)
        {
        case 0: {
            ImVec2 Center = ImVec2(g.g_GameRect.right / 2, g.g_GameRect.bottom / 2);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x - g.g_CrosshairSize, Center.y), ImVec2((Center.x + g.g_CrosshairSize) + 1, Center.y), CrosshairColor, 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x, Center.y - g.g_CrosshairSize), ImVec2(Center.x, (Center.y + g.g_CrosshairSize) + 1), CrosshairColor, 1);
        }   break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom / 2.f), g.g_CrosshairSize + 1, BLACK, NULL);
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom / 2.f), g.g_CrosshairSize, CrosshairColor, NULL);
            break;
        }
    }
}

void CFramework::RenderESP()
{
    // 必要な情報を取得.
    pLocal->Update();
    Matrix ViewMatrix = m.Read<Matrix>(m.m_gEngineBaseAddr + offset::dwViewMatrix);

    // ESP メインループ.
    for (auto& entity : EntityList)
    {
        CEntity* pEntity = &entity;
        pEntity->Update();

        // 距離を取得.
        const float pDistance = ((pLocal->m_vecAbsOrigin - pEntity->m_vecAbsOrigin).Length() * 0.01905f);

        // 各種チェック.
        if (!g.g_ESP_Team && pEntity->m_iTeamNum == pLocal->m_iTeamNum)
            continue;
        else if (pEntity->m_iHealth <= 1)
            continue;

        /* 頭と基準座標を取得する方が楽ではあるが….
       Vector2 pBase{}, pHead{};
       const Vector3 Head = pEntity->GetEntityBonePosition(BoneID::HEAD) + Vector3(0.f, 0.f, 12.f);
       if (!WorldToScreen(ViewMatrix, g.g_GameRect, pEntity->m_vecAbsOrigin, pBase) || !WorldToScreen(ViewMatrix, g.g_GameRect, Head, pHead))
           continue;
       */

        // m_Collisio ベースのサイズ取得.
        Vector3 min = pEntity->vecMin();
        Vector3 max = pEntity->vecMax() + Vector3(0.0f, 0.0f, 10.f);

        int left, top, right, bottom;
        Vector2 flb, brt, blb, frt, frb, brb, blt, flt;
        
        Vector3 points[] = { Vector3(min.x, min.y, min.z), Vector3(min.x, max.y, min.z), Vector3(max.x, max.y, min.z),
                    Vector3(max.x, min.y, min.z), Vector3(max.x, max.y, max.z), Vector3(min.x, max.y, max.z),
                    Vector3(min.x, min.y, max.z), Vector3(max.x, min.y, max.z) };

        Vector3 transformedPoints[8]{};
        
        for (int i = 0; i < 8; i++)
            transformedPoints[i] = points[i].TransformNormal(points[i], pEntity->GetRgflCoordinateFrame());

        if (!WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[3], flb) || !WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[5], brt) ||
            !WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[0], blb) || !WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[4], frt) ||
            !WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[2], frb) || !WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[1], brb) ||
            !WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[6], blt) || !WorldToScreen(ViewMatrix, g.g_GameRect, transformedPoints[7], flt))
            continue;

        Vector2 vec2_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };
        left = flb.x;
        top = flb.y;
        right = flb.x;
        bottom = flb.y;

        for (auto j = 1; j < 8; ++j)
        {
            if (left > vec2_array[j].x)
                left = vec2_array[j].x;
            if (bottom < vec2_array[j].y)
                bottom = vec2_array[j].y;
            if (right < vec2_array[j].x)
                right = vec2_array[j].x;
            if (top > vec2_array[j].y)
                top = vec2_array[j].y;
        }

        // サイズ算出.
        const int Height  = bottom - top;
        const int Width   = right - left;
        const int Center  = (right - left) / 2;
        const int bScale  = (right - left) / 3;

        // 色を決める.
        ImColor color = pLocal->m_iTeamNum == pEntity->m_iTeamNum ? ESP_Team : ESP_Default;

        // Line
        if (g.g_ESP_Line)
            DrawLine(ImVec2((float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom), ImVec2(right - (Width / 2.f), bottom), color, 1.f);

        // Box
        if (g.g_ESP_Box)
        {
            // BoxFilled
            if (g.g_ESP_BoxFilled)
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(left, top), ImVec2(right, bottom), ESP_Filled);

            // Shadow
            DrawLine(ImVec2(left - 1, top - 1), ImVec2(right + 2, top - 1), ESP_Shadow, 1.f);
            DrawLine(ImVec2(left - 1, top), ImVec2(left - 1, bottom + 2), ESP_Shadow, 1.f);
            DrawLine(ImVec2(right + 1, top), ImVec2(right + 1, bottom + 2), ESP_Shadow, 1.f);
            DrawLine(ImVec2(left - 1, bottom + 1), ImVec2(right + 1, bottom + 1), ESP_Shadow, 1.f);

            switch (g.g_ESP_BoxType)
            {
            case 0:
                DrawLine(ImVec2(left, top), ImVec2(right, top), color, 1.f);
                DrawLine(ImVec2(left, top), ImVec2(left, bottom), color, 1.f);
                DrawLine(ImVec2(right, top), ImVec2(right, bottom), color, 1.f);
                DrawLine(ImVec2(left, bottom), ImVec2(right + 1, bottom), color, 1.f);
                break;
            case 1:
                DrawLine(ImVec2(left, top), ImVec2(left + bScale, top), color, 1.f); // Top
                DrawLine(ImVec2(right, top), ImVec2(right - bScale, top), color, 1.f);
                DrawLine(ImVec2(left, top), ImVec2(left, top + bScale), color, 1.f); // Left
                DrawLine(ImVec2(left, bottom), ImVec2(left, bottom - bScale), color, 1.f);
                DrawLine(ImVec2(right, top), ImVec2(right, top + bScale), color, 1.f); // Right
                DrawLine(ImVec2(right, bottom), ImVec2(right + 1, bottom - bScale), color, 1.f);
                DrawLine(ImVec2(left, bottom), ImVec2(left + bScale, bottom), color, 1.f); // Bottom
                DrawLine(ImVec2(right, bottom), ImVec2(right - bScale, bottom), color, 1.f);
                break;
            }
        }

        // Skeleton
        if (g.g_ESP_Skeleton)
        {
            // 全てのBoneをvectorに格納
            std::vector<Vector3> BoneList = pEntity->GetAllBones();

            if (BoneList.size() == 32)
            {
                // 頭の円をレンダリング
                Vector2 pHead, pNeck;
                if (!WorldToScreen(ViewMatrix, g.g_GameRect, BoneList[HEAD], pHead) ||
                    !WorldToScreen(ViewMatrix, g.g_GameRect, BoneList[NECK], pNeck))
                    continue;

                Circle(ImVec2(pHead.x, pHead.y), pNeck.y - pHead.y, color);

                // 線を引くためのペアを作成する
                const Vector3 skeleton_list[][2] = {
                    { BoneList[NECK], BoneList[PELVIS] },
                    { BoneList[NECK], BoneList[LEFT_SHOULDER] },
                    { BoneList[LEFT_SHOULDER], BoneList[LEFT_ELBOW] },
                    { BoneList[LEFT_ELBOW], BoneList[LEFT_HAND] },
                    { BoneList[NECK], BoneList[RIGHT_SHOULDER] },
                    { BoneList[RIGHT_SHOULDER], BoneList[RIGHT_ELBOW] },
                    { BoneList[RIGHT_ELBOW], BoneList[RIGHT_HAND] },
                    { BoneList[PELVIS], BoneList[LEFT_KNEE] },
                    { BoneList[LEFT_KNEE], BoneList[LEFT_ANKLE] },
                    { BoneList[PELVIS], BoneList[RIGHT_KNEE] },
                    { BoneList[RIGHT_KNEE], BoneList[RIGHT_ANKLE] }
                };

                // WorldToScreenを行い各ペアをレンダリングする.
                for (int j = 0; j < 11; j++)
                {
                    Vector2 vOut0, vOut1;
                    if (!WorldToScreen(ViewMatrix, g.g_GameRect, skeleton_list[j][0], vOut0) ||
                        !WorldToScreen(ViewMatrix, g.g_GameRect, skeleton_list[j][1], vOut1))
                        break;

                    DrawLine(ImVec2(vOut0.x, vOut0.y), ImVec2(vOut1.x, vOut1.y), color, 1.f);
                }
            }

            BoneList.clear();
        }

        // Healthbar
        if (g.g_ESP_HealthBar) {
            HealthBar((left - 4), bottom, 2, -Height, pEntity->m_iHealth, 100);
        }

        // Distance
        if (g.g_ESP_Distance) {
            const auto distance_str = std::to_string((int)pDistance) + "m";
            StringEx(ImVec2(((left + Center) - ImGui::CalcTextSize(distance_str.c_str()).x / 2.f), bottom + 1.f), TEXT_COLOR, ImGui::GetFontSize(), distance_str.c_str());
        }

        // Name
        if (g.g_ESP_Name) {
            StringEx(ImVec2(((left + Center) - ImGui::CalcTextSize(pEntity->m_pName.c_str()).x / 2.f), top - ImGui::GetFontSize()), TEXT_COLOR, ImGui::GetFontSize(), pEntity->m_pName.c_str());
        }  
    }
}