#include "FrameCore.h"

const int ReadCount = 64;

void CFramework::BunnyHop()
{
    if (GetAsyncKeyState(VK_SPACE))
    {
        int flag = pLocal->GetFlag();

        if (flag == 257 || flag == 263)
            m.Write<uint32_t>(m.m_gClientBaseAddr + offset::m_dwForceJump, 6);
    }
}

void CFramework::UpdateList()
{
    while (g.g_Run)
    {
        std::vector<CEntity> ent_list(32);

        char pMap[128]{};
        m.ReadString(m.m_gEngineBaseAddr + offset::levelname, pMap, sizeof(pMap));

        if (strlen(pMap) == 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1000));
            continue;
        }

        // NameList
        uintptr_t NameList = m.Read<uintptr_t>(m.m_gClientBaseAddr + offset::dwNameList) + 0x38;

        // Local
        pLocal->address = m.Read<uint32_t>(m.m_gClientBaseAddr + offset::dwLocalPlayer);

        if (pLocal->address == NULL) {
            std::this_thread::sleep_for(std::chrono::seconds(1000));
            continue;
        }

        // EntityList
        for (int i = 0; i < ReadCount; i++)
        {
            auto entity = m.Read<uint32_t>(m.m_gClientBaseAddr + offset::dwEntityList + (i * 0x10));

            if (entity != NULL && entity != pLocal->address)
            {
                CEntity p;
                p.address = entity;
                p.Update();

                // �`�F�b�N
                if (p.IsDead()) // �������Ă��邩
                    continue;
                else if (p.IsDormant()) // �x�~��� == �T�[�o�[����f�[�^�����ł��Ȃ���Ԃł͂Ȃ���
                    continue;
                else if (p.m_iTeamNum == 0 || p.m_iHealth <= 0) // �v���C���[���H
                    continue;

                p.m_pName = p.GetName(NameList, i);
                ent_list[i] = p;
            }
        }

        EntityList = ent_list;
        ent_list.clear();

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}