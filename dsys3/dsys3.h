#pragma once

#include <Windows.h> 
#include <iostream>
#include <utility>

int ds3_retrieve(const char* func, BOOL debug, BOOL recast);

using namespace std;

template <typename... Args>

void* ds3_run(BYTE syscall_id, BOOL debug, Args&&... args)
{
    BYTE stub[] = {
        0x4C, 0x8B, 0xD1,
        0xB8, syscall_id, 0x00, 0x00, 0x00, //mv scid
        0x0F, 0x05, //sc+ily carti
        0xC3
    };

    if (debug == TRUE and stub) {
        cout << "[DBG] shellcode built, stub size ";
        cout << sizeof(stub);
    };
    
    void* mem = VirtualAlloc(nullptr, sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!mem) return nullptr;

    if (debug == TRUE) {
        cout << "\n[DBG] alloc mem ";
        cout << sizeof(mem);
    };

    memcpy(mem, stub, sizeof(stub));

    using FnType = void* (*)(Args...);
    auto fn = reinterpret_cast<FnType>(mem);

    void* result = fn(forward<Args>(args)...);

    if (debug == TRUE) {
        cout << "\n[DBG] ran with result fn size ";
        cout << sizeof(result);
        cout << "\n";
    };

    VirtualFree(mem, 0, MEM_RELEASE);
    return result;
}
