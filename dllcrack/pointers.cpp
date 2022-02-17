#include "common.hpp"

#include "pointers.hpp"
#include "memory/all.hpp"


pointers::pointers()
{
	memory::pattern_batch main_batch;
	main_batch.add("login", "48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F1", [this](memory::handle ptr)
		{
			m_login = ptr.sub(0x15).as<functions::login_t>();
		});
	main_batch.add("heartbeat", "48 33 C4 48 89 85 ? ? ? ? 48 8B F2 48 89 54 24 ?", [this](memory::handle ptr)
		{
			m_heartbeat = ptr.sub(0x26).as<functions::heartbeat_t>();
		});	
	main_batch.add("getUserInfo", "48 89 44 24 ? 48 8B F1 48 89 4C 24 ?", [this](memory::handle ptr)
		{
			m_getUserInfo = ptr.sub(0x1C).as<functions::getUserInfo_t>();
		});	
	main_batch.add("username", "48 8D 4C 24 ? 48 8B 5C 24 ?", [this](memory::handle ptr)
		{
			m_username = ptr.add(0x38).rip().as<void*>();
		});
	main_batch.run(memory::module("Xi.dll"));

	g_pointers = this;
}

pointers::~pointers()
{
	g_pointers = nullptr;
}
