#include "CEntity.h"

void CEntity::Update()
{
	m_vecAbsOrigin = m.Read<Vector3>(address + offset::m_vecAbsOrigin);
	m_iHealth = m.Read<int>(address + offset::m_iHealth);
	m_iTeamNum = m.Read<int>(address + offset::m_iTeamNum);
	m_dwBoneMatrix = m.Read<uintptr_t>(address + offset::m_dwBoneMatrix);
}

bool CEntity::IsDead()
{
	return m.Read<BYTE>(address + offset::m_lifeState) > 0;
}

bool CEntity::IsDormant()
{
	return m.Read<bool>(address + offset::m_bDormant);
}

uint32_t CEntity::GetFlag()
{
	return m.Read<uint32_t>(address + offset::m_fFlags);
}

Vector3 CEntity::vecMin()
{
	return m.Read<Vector3>(address + offset::m_Collision + offset::Collision::m_vecMin) + this->m_vecAbsOrigin;
}

Vector3 CEntity::vecMax()
{
	return m.Read<Vector3>(address + offset::m_Collision + offset::Collision::m_vecMax) + this->m_vecAbsOrigin;
}

std::vector<Vector3> CEntity::GetAllBones()
{
	std::vector<Vector3> list;
	AllBone b = m.Read<AllBone>(m_dwBoneMatrix + 0xC), * bone = &b;

	for (int i = 0; i < 32; i++) {
		Vector3 bone_pos = Vector3(bone->pos[i].x, bone->pos[i].y, bone->pos[i].z);

		if(Vec3_Empty(bone_pos)) {
			list.clear();
			return list;
		}

		list.push_back(bone_pos);
	}

	return list;
}

Vector3 CEntity::GetBonePosition(int BoneId)
{
	Bone EntityBone = m.Read<Bone>(m_dwBoneMatrix + 0xC + (BoneId * 0x30));

	return Vector3(EntityBone.x, EntityBone.y, EntityBone.z);
}

Matrix CEntity::GetRgflCoordinateFrame()
{
	return m.Read<Matrix>(address + offset::m_rgflCoordinateFrame);
}

std::string CEntity::GetName(uintptr_t NameListPtr, int id)
{
	char pName[32]{};
	m.ReadString(NameListPtr + 0x140 * id, &pName, sizeof(pName));

	return std::string(pName);
}