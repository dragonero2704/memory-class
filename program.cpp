#include <iostream>

// #define DEBUG

#include "mem.hpp"
#include "macros.h"
#include "offsets.hpp"

using namespace std;
// 0x00007FF74C280000 prefered image base of dummy.exe
int main(int argc, char** argv) 
{
	wstring targetProgram = L"dummy.exe";
	//uintptr_t targetAdress = 0x8d425ff69c;
	Memory mem(targetProgram);
	auto programBaseAdress = mem.GetModuleByName(L"dummy.exe");
	info("%ls base address: 0x%p", targetProgram.c_str(), programBaseAdress);
	/* fails
	info("Method 1");
	auto addr1 = mem.Read<uintptr_t>(programBaseAdress + offsets::addr1_offset);
	ok("%p -> %p", programBaseAdress + offsets::addr1_offset, addr1);
	auto addr2 = mem.Read<uintptr_t>(addr1 + offsets::addr2_offset);
	ok("%p -> %p", addr1 + offsets::addr2_offset, addr2);
	auto addr3 = mem.Read<uintptr_t>(addr2 + offsets::addr3_offset);
	ok("%p -> %p", addr2 + offsets::addr3_offset, addr3);
	auto val = mem.Read<int>(addr3 + offsets::addr3_offset_value);
	ok("%p : %d", addr3 + offsets::addr3_offset_value, val);
	*/
	info("Method 2");
	auto addr1 = mem.Read<uintptr_t>(programBaseAdress + offsets2::addr1_offset);
	ok("%p -> %p", programBaseAdress + offsets2::addr1_offset, addr1);
	auto targetAdress = addr1+offsets2::addr2_offset;
	auto val = mem.Read<int>(targetAdress);
	ok("%p : %d", targetAdress, val);

	
	while (true)
	{
		mem.Write<int>(targetAdress, 666);
	}
	
	

	return EXIT_SUCCESS;
}