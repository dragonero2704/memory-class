#include <string>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include "macros.h"
class Memory {
private:
	HANDLE hProcess = nullptr;
	DWORD PID = 0;

	void init(const std::wstring& processName)
	{
		PROCESSENTRY32W entry = { };
		entry.dwSize = sizeof(PROCESSENTRY32W);

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE) {
			//error
			error("Error %ld: Invalid Handle", GetLastError());
			return;
		}

		if (!Process32FirstW(snapshot, &entry)) {
			// no process
			error("Error %d: can't get processes", GetLastError());
			return;
		}
		//iterate through processes
		while (Process32NextW(snapshot, &entry))
		{
			debug("Process %ls", entry.szExeFile);
			if (!processName.compare(entry.szExeFile))
			{
				this->PID = entry.th32ProcessID;
				//get a process handle
				this->hProcess = OpenProcess(
					PROCESS_ALL_ACCESS,
					FALSE,
					this->PID
				);
				if (this->hProcess == INVALID_HANDLE_VALUE) {
					error("Error %ld: Can't get target process handle", GetLastError());
					return;
				}
				break;
			}
		}
		debug("PID: %lld", this->PID);

		if (snapshot) CloseHandle(snapshot);
	}

public:
	const DWORD& ProcessId = PID;

	Memory() {
		hProcess = nullptr;
		PID = 0;
	}
	Memory(const std::wstring& processName)
	{
		init(processName);
	}

	Memory(const std::string& processName)
	{

		init(std::wstring(processName.begin(), processName.end()));
	}

	~Memory() {
		if (hProcess)
			CloseHandle(hProcess);
	}
	
	uintptr_t GetModuleByName(const std::wstring moduleName) {
		MODULEENTRY32W entry = {};
		entry.dwSize = sizeof(MODULEENTRY32W);

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->PID);
		if (snapshot == INVALID_HANDLE_VALUE) {
			//error
			error("Error %ld: Invalid Handle", GetLastError());
			return-1;
		}

		if (!Module32FirstW(snapshot, &entry)) {
			// no process
			error("Error %d: can't get modules", GetLastError());
			return -1;
		}
		std::uintptr_t res = 0;

		debug("module %ls", entry.szModule);
		if (!moduleName.compare(entry.szModule)) {
			//debug("entry.modBaseAddr: 0x%p", entry.modBaseAddr);
			res = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
			return res;
		}
		while (Module32NextW(snapshot, &entry)) {
			debug("module %ls", entry.szModule);
			if (!moduleName.compare(entry.szModule)) {
				res = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}
		if (snapshot) CloseHandle(snapshot);
		return res;
	}

	std::uintptr_t GetBaseAddress() {
		return reinterpret_cast<std::uintptr_t>(this->hProcess);
	}

	template<typename T>
	const T Read(const std::uintptr_t& address) {
		T value;
		SIZE_T bytesRead;
		if (!ReadProcessMemory(
			this->hProcess,
			reinterpret_cast<const void*>(address),
			reinterpret_cast<void*>(&value),
			sizeof(T),
			&bytesRead
		)) {
			//ReadProcessMemory failed
			error("Error %ld: read 0x%llx address memory failed", GetLastError(), address);
		}
		debug("Read %llu",bytesRead);
		return value;
	}

	template<typename T>
	bool Write(const std::uintptr_t& address, const T& value) {
		SIZE_T bytesRead;
		if (!WriteProcessMemory(
			this->hProcess,
			reinterpret_cast<void*>(address),
			&value,
			sizeof(T),
			&bytesRead
		)) {
			//error
			error("Error %ld: write 0x%llx failed", GetLastError(), address);
			return false;
		}
		debug("Read %llu",bytesRead);
		return true;
	}
};
