#include <windows.h>
#include <iostream>
#include <utility>

#include "dsys3.h"

using namespace std;

template <typename... Args>

void* ds3_run(BYTE syscall_id, BOOL debug, Args&&... args)
{
    BYTE stub[] = {
        0x4C, 0x8B, 0xD1,
        0xB8, syscall_id, 0x00, 0x00, 0x00,
        0x0F, 0x05,
        0xC3
    };

    if (debug == TRUE and stub) {
        cout << "\n[DEBUG] shellcode built, stub size";
        cout << sizeof(stub);
    };

    void* mem = VirtualAlloc(nullptr, sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return nullptr;

    if (debug == TRUE) {
        cout << "\n[DEBUG] alloc mem ";
        cout << sizeof(mem);
    };

    memcpy(mem, stub, sizeof(stub));

    using FnType = void* (*)(Args...);
    auto fn = reinterpret_cast<FnType>(mem);

    void* result = fn(forward<Args>(args)...);

    if (debug == TRUE) {
        cout << "\n[DEBUG] result fn size ";
        cout << sizeof(result);
    };

    VirtualFree(mem, 0, MEM_RELEASE);
    return result;
}
