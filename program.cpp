#include <iostream>

#include "mem.hpp"

using namespace std;

int main(int argc, char** argv) 
{
	string targetProgram = "dummy.exe";
	uintptr_t targetAdress = 0x8d425ff69c;
	Memory mem(targetProgram);
	//cout << "Hello World!" << endl;
	cout << mem.Read<int>(targetAdress)<<endl;
	mem.Write<int>(targetAdress, 666);
	cout << mem.Read<int>(targetAdress)<<endl;

	return EXIT_SUCCESS;
}