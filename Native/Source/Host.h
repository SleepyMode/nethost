
#pragma once

#include "External/hostfxr.h"


using string_t = std::basic_string<char_t>;

class Host
{
public:
	Host();
	~Host() = default;

	bool Initialize(const std::string& hostfxrPath);
	void RunTestModule(const string_t& root);

private:
	hostfxr_initialize_for_runtime_config_fn m_hostFxrInitializeForRuntimeConfigFn;
	hostfxr_get_runtime_delegate_fn m_hostFxrGetRuntimeDelegateFn;
	hostfxr_close_fn m_hostFxrCloseFn;
};
