#ifndef _MACROS
#define _MACROS
#include <stdio.h>

#define info(msg, ...) (printf("[i] " msg "\n", ##__VA_ARGS__))

#define error(msg, ...) (printf("[-] " msg "\n", ##__VA_ARGS__))

#define ok(msg, ...) (printf("[+] " msg "\n", ##__VA_ARGS__))
// ok aliases
#define success(msg, ...) (ok(msg, ##__VA_ARGS__))

#define warning(msg, ...) (printf("[*] " msg "\n", ##__VA_ARGS__))
#endif


// define DEBUG
#ifdef DEBUG
#include <stdio.h>
#define debug(msg, ...) (printf("[DEBUG] " msg "\n", ##__VA_ARGS__))
#else
#define debug(msg, ...) 
#endif