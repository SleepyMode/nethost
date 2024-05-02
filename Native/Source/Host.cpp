
#include <Windows.h>
#include <print>
#include <tuple>
#include <vector>
#include "External/coreclr_delegates.h"
#include "External/hostfxr.h"
#include "Host.h"

#include "External/error_codes.h"


Host::Host()
{
	m_hostFxrInitializeForRuntimeConfigFn = nullptr;
	m_hostFxrGetRuntimeDelegateFn = nullptr;
	m_hostFxrCloseFn = nullptr;
}

bool Host::Initialize(const std::string& hostfxrPath)
{
	void* hostFxrHandle = LoadLibraryA(hostfxrPath.c_str());
	if (hostFxrHandle == nullptr)
	{
		std::println("[!] LoadLibrary to hostfxr failed.");
		return false;
	}

	const HMODULE moduleHandle = static_cast<HMODULE>(hostFxrHandle);
	m_hostFxrInitializeForRuntimeConfigFn = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(GetProcAddress(
		moduleHandle, "hostfxr_initialize_for_runtime_config"));
	m_hostFxrGetRuntimeDelegateFn = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(GetProcAddress(
		moduleHandle, "hostfxr_get_runtime_delegate"));
	m_hostFxrCloseFn = reinterpret_cast<hostfxr_close_fn>(GetProcAddress(
		moduleHandle, "hostfxr_close"));

	return m_hostFxrInitializeForRuntimeConfigFn != nullptr && m_hostFxrGetRuntimeDelegateFn != nullptr &&
		m_hostFxrCloseFn != nullptr;
}

void Host::RunTestModule(const string_t& root)
{
	int status = 0;
	const string_t configPath = root + L"TestModule.runtimeconfig.json";

	hostfxr_handle context = nullptr;
	status = m_hostFxrInitializeForRuntimeConfigFn(configPath.c_str(), nullptr, &context);
	if (!STATUS_CODE_SUCCEEDED(status) || !context)
	{
		std::println("[!] Failed to initialize .NET core from runtime config (status 0x{:X}).", static_cast<unsigned>(status));
		m_hostFxrCloseFn(context); // NOTE: I have no idea why this needs to be done if the init failed, but, who knows.
		return;
	}

	load_assembly_and_get_function_pointer_fn loadAssemblyAndGetFunctionPointerFn = nullptr;
	status = m_hostFxrGetRuntimeDelegateFn(context, hdt_load_assembly_and_get_function_pointer,
	                                       reinterpret_cast<void**>(&loadAssemblyAndGetFunctionPointerFn));
	if (!STATUS_CODE_SUCCEEDED(status))
	{
		std::println("[!] Failed to retrieve load_assembly_and_get_function_pointer delegate (status 0x{:X}).", static_cast<unsigned>(status));
		return;
	}

	m_hostFxrCloseFn(context);

	const string_t testModulePath = root + L"TestModule.dll";
	const char_t* type = L"TestModule.Module, TestModule";
	const char_t* method = L"Test";
	component_entry_point_fn managedTestFn = nullptr;

	// About to break a wall, here's some reasons why you should read through the documentation before coding:
	// 1. Making the managed Test function take no arguments (doesn't fit signature anymore! :D)
	// 2. Making the managed Test function return 'void' instead of 'int' (doesn't fit signature, I spent 3 hours here! :D)
	// don't make these two mistakes. I spent so long here. and I used JNI so I'm used to this but this is just a disaster.
	status = loadAssemblyAndGetFunctionPointerFn(testModulePath.c_str(), type, method, nullptr, nullptr,
	                                             reinterpret_cast<void**>(&managedTestFn));
	if (status != 0 || !managedTestFn)
	{
		std::println("[!] Failed to retrieve managed function TestModule::Module::Test (status 0x{:X}).", static_cast<unsigned>(status));
		return;
	}

	try
	{
		managedTestFn(nullptr, 0);
	}
	catch (...)
	{
		std::println("[!] Managed call failed.");
	}
}
