#pragma once
#include "../../../Framework/Utils/Utils.h"

namespace offset
{
	// Main
	const uintptr_t dwEntityList    = 0x4D5AE4; // client.dll
	const uintptr_t dwLocalPlayer	= 0x4C88E8; // client.dll
	const uintptr_t dwNameList		= 0x50E368; // client.dll
	const uintptr_t dwViewMatrix	= 0x5B0D68; // engine.dll
	const uintptr_t levelname		= 0x47796D; // engine.dll
	const uintptr_t m_dwForceJump	= 0x4F5D24; // client.dll

	// Offsets
	const uintptr_t m_lifeState		= 0x93;		// BYTE
	const uintptr_t m_iHealth       = 0x94;		// int
	const uintptr_t m_iTeamNum      = 0x9C;		// int
	const uintptr_t m_bDormant		= 0x17E;	// bool
	const uintptr_t m_Collision		= 0x19C;	// this + Collision(m_vecMin/Max)
	const uintptr_t m_vecAbsOrigin  = 0x260;	// Vector3
	const uintptr_t m_rgflCoordinateFrame = 0x308;
	const uintptr_t m_fFlags		= 0x350;	// uint32_t/int
	const uintptr_t m_dwBoneMatrix	= 0x578;	// Pointer
	
	// m_Collision
	namespace Collision
	{
		const uintptr_t m_vecMin = 0x20;
		const uintptr_t m_vecMax = 0x2C;
	}
};

enum BoneID {
	PELVIS,
	LEFT_HIPS,
	LEFT_KNEE,
	LEFT_ANKLE,
	LEFT_FOOT,
	RIGHT_HIPS,
	RIGHT_KNEE,
	RIGHT_ANKLE,
	RIGHT_FOOT,
	STOMACH,
	BODY,
	CHEST,
	NECK,
	FACE,
	HEAD,
	LEFT_NECK,
	LEFT_SHOULDER,
	LEFT_ELBOW,
	LEFT_HAND,
	RIGHT_NECK = 28,
	RIGHT_SHOULDER,
	RIGHT_ELBOW,
	RIGHT_HAND
};

struct Bone
{
	float x;
	char junk0[0xC]{};
	float y;
	char junk1[0xC]{};
	float z;
	char junk2[0xC]{};
};

struct AllBone {
	Bone pos[32]{};
};

extern struct Bone;
extern bool Vec3_Empty(const Vector3& value);
extern bool WorldToScreen(Matrix ViewMatrix, RECT Size, Vector3 vIn, Vector2& vOut);