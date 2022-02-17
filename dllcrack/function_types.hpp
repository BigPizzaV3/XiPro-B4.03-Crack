#pragma once
#include "common.hpp"

namespace functions
{
	using login_t = bool(*)(std::string username, std::string password, std::string code, std::string client_version);
	using heartbeat_t = bool(*)(int time, std::string loginAuth);
	using getUserInfo_t = std::string(*)(const char* name);
}
