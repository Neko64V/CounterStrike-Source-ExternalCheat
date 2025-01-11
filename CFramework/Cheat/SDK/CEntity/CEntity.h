#pragma once
#include "../GameSDK/GameSDK.h"

class CEntity
{
public:
    uintptr_t address;

	// player
	Vector3 m_vecAbsOrigin;
	int m_iHealth;
	int m_iTeamNum;
	BYTE m_lifeState;
	uintptr_t m_dwBoneMatrix;
	std::string m_pName;

    // functions
	void Update();
	bool IsDead();
	bool IsDormant();

	uint32_t GetFlag();
	Vector3 vecMin();
	Vector3 vecMax();
	Matrix GetRgflCoordinateFrame();
	std::vector<Vector3> GetAllBones();
	Vector3 GetBonePosition(int BoneId); 
	std::string GetName(uintptr_t NameListPtr, int id);
};