#include <iostream>
// #define DEBUG

#include "../memory.hpp"
#include "../macros.h"
#include "offsets.hpp"

// 0x00007FF74C280000 prefered image base of dummy.exe
int main(int argc, char** argv) 
{
	std::wstring targetProgram = L"dummy.exe";
	//uintptr_t targetAdress = 0x8d425ff69c;
	Memory mem(targetProgram);
	auto programBaseAdress = mem.GetModuleByName(L"dummy.exe");
	ok("%ls base address: 0x%p", targetProgram.c_str(), programBaseAdress);
	if(programBaseAdress == 0){
		error("Can't get \"%ls\" address", targetProgram.c_str());
		return EXIT_FAILURE;
	}
	info("Method 2");
	auto addr1 = mem.Read<std::uintptr_t>(programBaseAdress + offsets2::addr1_offset);
	ok("0x%p -> 0x%p", programBaseAdress + offsets2::addr1_offset, addr1);
	auto targetAdress = addr1+offsets2::addr2_offset;
	auto val = mem.Read<int>(targetAdress);
	ok("0x%p : %d", targetAdress, val);

	ok("Freezing variable 0x%p of \"%ls\" to value %d", targetAdress, targetProgram.c_str(), 666);
	while (mem.Write<int>(targetAdress, 666));
	
	return EXIT_SUCCESS;
}