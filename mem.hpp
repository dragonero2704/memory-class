#include <string>
#include <Windows.h>
#include <TlHelp32.h>

//#define DEBUG

#include "macros.h"
class Memory {
private:
	HANDLE hProcess = nullptr;
	DWORD PID = 0;
	std::uintptr_t baseAdress;
public:
	Memory() {
		hProcess = nullptr;
		PID = 0;
	}
	Memory(std::string processName) {
		PROCESSENTRY32 entry = { };
		entry.dwSize = sizeof(PROCESSENTRY32);

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE) {
			//error
			error("Error %lld: Invalid Handle", GetLastError());
			return;
		}

		if (!Process32First(snapshot, &entry)) {
			// no process
			error("Error %d: can't get processes", GetLastError());
			return;
		}
		//iterate through processes
		while (Process32Next(snapshot, &entry)) 
		{
			debug("Process %s", entry.szExeFile);
			if (!processName.compare(entry.szExeFile)) 
			{
				this->PID = entry.th32ProcessID;
				//get a process handle
				this->hProcess = OpenProcess(
				PROCESS_ALL_ACCESS,
				NULL,
				this->PID
				);
				if (this->hProcess == INVALID_HANDLE_VALUE) {
					error("Error %lld: Can't get target process handle", GetLastError());
					return;
				}
				break;
			}
		}
		debug("PID: %lld", this->PID);
		
		if (snapshot) CloseHandle(snapshot);
	}
	
	~Memory(){
		if (hProcess)
			CloseHandle(hProcess);
	}

	uintptr_t GetModuleByName(const std::string moduleName) {
		MODULEENTRY32 entry = {};
		entry.dwSize = sizeof(MODULEENTRY32);

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->PID);
		if (snapshot == INVALID_HANDLE_VALUE) {
			//error
			error("Error %lld: Invalid Handle", GetLastError());
			return-1;
		}

		if (!Module32First(snapshot, &entry)) {
			// no process
			error("Error %d: can't get modules", GetLastError());
			return -1;
		}
		std::uintptr_t res = 0;
		while (Module32Next(snapshot, &entry)) {
			debug("module %s", entry.szModule);
			if (!moduleName.compare(entry.szModule)) {
				res = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}
		if (snapshot) CloseHandle(snapshot);
		return res;
	}

	template<typename T>
	const T Read(const std::uintptr_t& address) {
		T value;
		if (!ReadProcessMemory(
			this->hProcess,
			reinterpret_cast<const void*>(address),
			&value,
			sizeof(T),
			NULL
		)) {
			//ReadProcessMemory failed
			error("Error %lld: read 0x%x address memory failed", GetLastError(), address);
		}
		return value;
	}

	template<typename T>
	void Write(const std::uintptr_t& address, const T& value) {
		if (!WriteProcessMemory(
			this->hProcess,
			reinterpret_cast<void*>(address),
			&value, 
			sizeof(T),
			NULL
		)) {
			//error
			error("Error %lld: ReadProcessMemory() failed", GetLastError());
			return;
		}
	}
};
