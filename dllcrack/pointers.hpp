#pragma once
#include "common.hpp"
#include "function_types.hpp"

	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		functions::login_t m_login{};
		functions::heartbeat_t m_heartbeat{};
		functions::getUserInfo_t m_getUserInfo{};
		void* m_username{};
	};

	inline pointers* g_pointers{};
