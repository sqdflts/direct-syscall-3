#include <iostream>
#include <windows.h>
#include "dsys3.h"
#define WIN32_LEAN_AND_MEAN

using namespace std;

int ds3_retrieve(const char* func, BOOL debug, BOOL recast)
{

    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) return 0;

    if (debug == TRUE) {
        cout << "[DBG] retrieving ";
        cout << func;
    }

    BYTE* addr = (BYTE*)GetProcAddress(ntdll, func);
    if (!addr) return 0;

    if (debug == TRUE) {
        cout << " > ";
        cout << static_cast<int>(addr[4]);
        cout << "\n";
    }

    if (recast == TRUE) {
        return static_cast<int>(addr[4]);
    }
    else {
        return addr[4];
    }
};