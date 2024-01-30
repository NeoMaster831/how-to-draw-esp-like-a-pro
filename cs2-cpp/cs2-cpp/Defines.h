#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <string>
#include <array>
#include <TlHelp32.h>

using namespace std;

class Defines
{
	// Defines - Defines of various functions and variables
	// Offsets

	const uint64_t localPlayer = 0x16D4F48;
	const uint64_t entityList = 0x17CE6A0;
	const uint64_t viewMatrix = 0x182CEA0;
	const uint64_t e_Health = 0x32C;
	const uint64_t e_Team = 0x3BF;
	const uint64_t e_Location = 0x1224;
	const uint64_t resX = 0x1831B6C, resY = resX + 4;
	const uint64_t e_PlayerPawn = 0x7EC;

	// Utilities

public:

	HANDLE hProc;
	int pid = -1;
	uintptr_t clientAddr = -1;
	void init(LPCWSTR proc, LPCWSTR mod);

	struct Matrix4x4 {
		float matrix[4][4];
	};

	struct Vector3 {
		float x, y, z;
	};

	template <class T>
	T RPM(uint64_t _where) {
		T ret; SIZE_T out = 0;
		ReadProcessMemory(this->hProc, (LPCVOID)_where, &ret, sizeof(T), &out);
		if (out != sizeof(T)) throw 0x3d000005;
		return ret;
	}

	Defines(LPCWSTR proc, LPCWSTR mod);

	uintptr_t getLocalPlayer() {
		return RPM<uintptr_t>(clientAddr + localPlayer);
	}

	uintptr_t getEntityList() {
		return RPM<uintptr_t>(clientAddr + entityList);
	}

	Matrix4x4 getViewMatrix() {
		return RPM<Matrix4x4>(clientAddr + viewMatrix);
	}

	pair<int, int> getResolution() {
		return {
			RPM<int>(clientAddr + resX),
			RPM<int>(clientAddr + resY)
		};
	}

	int getEntityHealth(uintptr_t entity) {
		return RPM<int>(entity + e_Health);
	}

	byte getEntityTeam(uintptr_t entity) {
		return RPM<byte>(entity + e_Team);
	}

	Vector3 getEntityLocation(uintptr_t entity) {
		Vector3 ret = RPM<Vector3>(entity + e_Location);
		//auto tmp = ret.y; ret.y = ret.z; ret.z = tmp;
		return ret;
	}

	int getPlayerPawn(uintptr_t entity) {
		return RPM<int>(entity + e_PlayerPawn);
	}
};

