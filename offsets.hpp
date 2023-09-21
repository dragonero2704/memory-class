#include <cstdint>

// offsets 

// method 1 !!! fails !!!
/*
dummy.exe + 0xEACE0 -> addr1 // THREADSTACK0
addr1 -> addr2   // THREADSTACK0
addr2 + 0x200 -> addr3 || addr2 + 0x20
addr3 + 0x8AC -> final adress!



namespace offsets {
	constexpr std::uintptr_t addr1_offset = 0xEACE0;
	constexpr std::uintptr_t addr2_offset = 0x0;
	constexpr std::uintptr_t addr3_offset = 0x200;
	constexpr std::uintptr_t addr3_offset_value = 0x8AC;

}*/

// method 2
/*
dummy.exe + 0xEA098 -> addr1
addr1 + 0x35C -> final address!
*/

namespace offsets2 {
	const std::uintptr_t addr1_offset = 0xEA098; // all variables section?
	const std::uintptr_t addr2_offset = 0x35C;
}
