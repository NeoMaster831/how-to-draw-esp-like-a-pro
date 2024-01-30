#include "Defines.h"

void Defines::init(LPCWSTR proc, LPCWSTR mod) {
	
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (hSnapshot == INVALID_HANDLE_VALUE || !Process32First(hSnapshot, &pe32)) {
		throw 0x3d000001;
	}

	do {
		if (!lstrcmpi(pe32.szExeFile, proc)) {
			this->pid = pe32.th32ProcessID;
			this->hProc = OpenProcess(PROCESS_ALL_ACCESS, false, this->pid);
			break;
		}
	} while (Process32Next(hSnapshot, &pe32));

	if (this->hProc == INVALID_HANDLE_VALUE || this->pid == -1) {
		throw 0x3d000002;
	}

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->pid);
	MODULEENTRY32 me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32);
	if (hSnapshot == INVALID_HANDLE_VALUE || !Module32First(hSnapshot, &me32)) {
		throw 0x3d000003;
	}

	do {
		if (!lstrcmpi(me32.szModule, mod)) {
			this->clientAddr = (uintptr_t)(me32.modBaseAddr);
		}
	} while (Module32Next(hSnapshot, &me32));

	if (this->clientAddr == -1) {
		throw 0x3d000004;
	}
}

Defines::Defines(LPCWSTR proc, LPCWSTR mod) {
	this->init(proc, mod);
}