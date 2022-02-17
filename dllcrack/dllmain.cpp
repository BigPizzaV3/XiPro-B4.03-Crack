#include<iostream>
#include<string>
#include <sstream>
#include <Windows.h>
#include"MinHook/MinHook.h"
#include <TlHelp32.h>
#include "hwd.h"
#ifdef _DEBUG
#pragma comment(lib,"MinHook/Debug/MinHook.lib")
#else
#pragma comment(lib,"MinHook/Release/MinHook.lib")
#endif
#include "pointers.hpp"
#include "hooking.hpp"
#include<chrono>
#include<thread>
void* NtProtectVirtualMemory = NULL;
HMODULE hm;

DWORD GetPID(const char* procName) {															// Itterates through every process and looks for a process who's executable name matches the char array passed to this function,
																						// then returns the process ID of that process
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	DWORD pID = NULL;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);				// Creates a snapshot of the currently running processes to itterate over
	//int i = 0; // To skip the first PUBG lite exe.
	if (Process32First(snapshot, &entry)) {												// Grabs the first process' information
		do {
			if (_stricmp(entry.szExeFile, procName) == 0) { 								// Compare the process file name to the only argument passed into the function																	
				pID = entry.th32ProcessID;												// If they are the same set the pID value to that process pID
				break;																	// and break out of the do while loop
			}
		} while (Process32Next(snapshot, &entry));										// Continue scanning the next process in the snapshot
	}

	CloseHandle(snapshot);																// Close the handle since we're done with it

	return pID;																			// Returns the pID

}

DWORD_PTR GetModuleBaseAddress(DWORD pID, const char* moduleName) {								// Itterates through the process with the provided process ID and returns the base address of the module provided

	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);
	DWORD_PTR baseAddress = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);					// Creates a snapshot of the process with the provided process ID

	if (Module32First(snapshot, &entry)) {												// Grabs the first modules information
		do {
			if (_stricmp(entry.szModule, moduleName) == 0) {							// Compares the module name to the argument passed into the function
				baseAddress = (DWORD_PTR)entry.modBaseAddr;									// if they are the same set the baseAddress variable to the base address of the module
				break;																	// and break out of the do while loop
			}
		} while (Module32Next(snapshot, &entry));										// continue scanning the next module in the snapshot
	}

	CloseHandle(snapshot);																// Close the handle since we're done with it
	return baseAddress;																	// Return the base Address
}

DWORD WINAPI crack(LPVOID)
{
	if (AllocConsole()) {
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
	}
	std::cout << u8R"kek(
		██╗░░██╗██╗░░░██╗░█████╗░███╗░░██╗░██████╗░███╗░░░███╗░█████╗░
		██║░░██║██║░░░██║██╔══██╗████╗░██║██╔════╝░████╗░████║██╔══██╗
		███████║██║░░░██║███████║██╔██╗██║██║░░██╗░██╔████╔██║██║░░██║
		██╔══██║██║░░░██║██╔══██║██║╚████║██║░░╚██╗██║╚██╔╝██║██║░░██║
		██║░░██║╚██████╔╝██║░░██║██║░╚███║╚██████╔╝██║░╚═╝░██║╚█████╔╝
		╚═╝░░╚═╝░╚═════╝░╚═╝░░╚═╝╚═╝░░╚══╝░╚═════╝░╚═╝░░░░░╚═╝░╚════╝░)kek"<<std::endl;
	std::cout << "欢迎使用(此版本仅支持B4.03以上版本,以下版本请使用老版补丁)" << std::endl;
	std::cout << "作者:荒陌 QQ群:260873883" << std::endl;
	std::cout << "等待注入Xi.dll" << std::endl;
	DWORD pid = GetPID("gta5.exe");

	uint64_t base_addr = NULL;
	while (base_addr == NULL)
	{
		Sleep(100);
		base_addr = GetModuleBaseAddress(pid, "Xi.dll");
	}

	//Patch vmp的hook的NtProtectVirtualMemory函数
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	if (ntdll!=NULL)
	{
		NtProtectVirtualMemory = GetProcAddress(ntdll, "NtProtectVirtualMemory");
		byte newBytes[] = { 0x4C,0x8B,0xD1,0xB8,0x50 };
		PDWORD O;
		if (VirtualProtect(NtProtectVirtualMemory, sizeof(newBytes) + 1, PAGE_EXECUTE_READWRITE, (PDWORD)&O) != 0)
		{
			std::cout << "VirtualProtect:OK" << std::endl;
			if (memcmp(NtProtectVirtualMemory, newBytes, sizeof(newBytes)) != 0)
			{
				memcpy(NtProtectVirtualMemory, newBytes, sizeof(newBytes));
				std::cout << "NtProtectVirtualMemory Patch:OK" << std::endl;
			}
		}
	}
	auto pointers_instance = std::make_unique<pointers>();
	std::cout << "特征码搜索"<<std::endl;

	auto hooking_instance = std::make_unique<hooking>();
	std::cout << "钩子安装" << std::endl;
	g_hooking->enable();


	while (true)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(500ms);
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hm = hModule;
		CreateThread(nullptr, 0, crack, nullptr, 0, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

