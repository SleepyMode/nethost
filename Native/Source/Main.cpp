
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <iostream>
#include <print>
#include <locale>
#include <codecvt>
#include <string>

#include "Host.h"

int main()
{
	std::println("[+] Welcome to Net Host or whatever this is!");
	std::println("[+] Just testing hosting .NET Core here because apparently that's a thing!");
	std::print("[+] Enter the path to hostfxr.dll: ");

	std::string hostFxrPath;
	std::getline(std::cin, hostFxrPath);

	if (!hostFxrPath.ends_with("\\hostfxr.dll"))
		hostFxrPath += "\\hostfxr.dll";
	std::println("Path: {}", hostFxrPath);

	Host host;
	if (!host.Initialize(hostFxrPath))
	{
		std::println("[!] Failed to initialize host.");
		return 1;
	}

	std::print("[+] Enter the path to the compiled test module root: ");

	std::string root;
	std::getline(std::cin, root);
	if (!root.ends_with("\\"))
		root += "\\";

	// Yes this is deprecated. But there's no replacement and I don't wanna use	MultiByteToWideChar.
	std::wstring_convert<std::codecvt_utf8_utf16<char_t>> converter;
	const string_t fixedRoot = converter.from_bytes(root);

	host.RunTestModule(fixedRoot);

	return 0;
}
