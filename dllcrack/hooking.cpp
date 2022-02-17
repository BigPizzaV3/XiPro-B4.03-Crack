#include "common.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "pointers.hpp"
#include"MinHook/MinHook.h"
#include <thread>

hooking::hooking() :
	login_hook("Login", g_pointers->m_login, &hooks::login),
	heartbeat_hook("Heartbeat",g_pointers->m_heartbeat,&hooks::heartbeat),
	m_getUserInfo_hook("getUserInfo",g_pointers->m_getUserInfo,&hooks::getUserInfo),
	m_MessageBoxA_hook("MessageBoxA", MessageBoxA,&hooks::MessageBoxA)
{
	g_hooking = this;
}

hooking::~hooking()
{
	
	if (m_enabled)
		disable();

	g_hooking = nullptr;
}

void hooking::enable()
{
	login_hook.enable();
	heartbeat_hook.enable();
	m_getUserInfo_hook.enable();
	m_MessageBoxA_hook.enable();

	m_enabled = true;
}

void hooking::disable()
{
	m_enabled = false;

	login_hook.disable();
	heartbeat_hook.disable();
	m_getUserInfo_hook.disable();
	m_MessageBoxA_hook.disable();
}

minhook_keepalive::minhook_keepalive()
{
	MH_Initialize();
}

minhook_keepalive::~minhook_keepalive()
{
	MH_Uninitialize();
}

bool hooks::login(std::string username, std::string password, std::string code, std::string client_version)
{
	std::cout << "login username:"<<username<<" password:" <<password<<" code:"<<code<<" client_version:"<< client_version << std::endl;
	//return g_hooking->login_hook.get_original<functions::login_t>()(username,password,code,client_version);
	return true;
}

bool hooks::heartbeat(int time, std::string loginAuth)
{
	std::cout << "heartbeat time:" << time << " loginAuth:" << loginAuth  << std::endl;
	//return g_hooking->heartbeat_hook.get_original<functions::heartbeat_t>()(time,loginAuth);
	return true;
}
std::string hooks::getUserInfo(const char* name)
{
	//std::string res = g_hooking->m_getUserInfo_hook.get_original<functions::getUserInfo_t>()(name);
	std::cout << "getUserInfo name:" << name<<std::endl;
	//std::cout<< " return:" << res << std::endl;
	return "huangmoCrack";
}

int hooks::MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	std::cout << "MessageBoxA" << std::endl;
	while (true)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(500ms);
	}
	return 0;
}
