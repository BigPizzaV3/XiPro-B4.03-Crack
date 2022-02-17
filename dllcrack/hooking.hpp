#pragma once
#include "common.hpp"
#include "detour_hook.hpp"

struct hooks
{
	static bool login(std::string username, std::string password, std::string code, std::string client_version);
	static bool heartbeat(int time, std::string loginAuth);
	static std::string getUserInfo(const char* name);
	static int MessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType);
};
struct minhook_keepalive
{
	minhook_keepalive();
	~minhook_keepalive();
};

class hooking
{
	friend hooks;
public:
	explicit hooking();
	~hooking();

	void enable();
	void disable();

private:
	bool m_enabled{};
	minhook_keepalive m_minhook_keepalive;

	detour_hook login_hook;
	detour_hook heartbeat_hook;
	detour_hook m_getUserInfo_hook;
	detour_hook m_MessageBoxA_hook;

};

inline hooking* g_hooking{};
