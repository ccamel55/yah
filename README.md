# YAH (Yet Another Hook) 🎣

**⚠️ Note: This library is still very early in development.**

Portable function "hooking" (interception) library. 

Function interception is used to modify the execution flow
of an existing binary/process in memory. The method chosen to "intercept"
a function call depends on the requirements of the user. See 
[Interception Methods](#interception-methods) for more details. 

### Motivation

Existing solutions like [Detours](https://github.com/microsoft/Detours),
[Minhook](https://github.com/TsudaKageyu/minhook) and
[SafetyHook](https://github.com/cursey/safetyhook) only implement one "hooking"
method, only support one platform or only supports one ISA/architecture. This
means if your working on a cross-platform codebase, you need to include multiple
dependencies that effectively do the same thing and implement/integrate multiple
APIs. 

## Interception methods ⛓️‍💥

This library currently supports the interception methods listed in this section. 

### VFunc replacement 

|         | x86 | x86_64 (amd64) | Arm64 (aarch64) |
|---------|-----|----------------|-----------------|
| Windows | ✅   | ✅              | ✅ (untested)    |
| Linux   | ✅   | ✅              | ✅               |
| MacOS   | ❌   | ❌              | ❌               |

#### Description

VFunc replacement aka virtual function replacement assumes that the function
you want to intercept exists inside a virtual table. 

Nb: Virtual tables are not required by the C++ standard however most if not all compilers
used today implement virtual functions with a virtual table. 

Since a class's virtual table contains member function pointers in a contiguous section of memory
we can essentially treat the virtual table as an array of size `x` containing member function pointers.
By replacing the member function pointer stored at index `i` in the array, we can "intercept" said 
function at index `i`. 

### VTable replacement

|         | x86 | x86_64 (amd64) | Arm64 (aarch64) |
|---------|-----|----------------|-----------------|
| Windows | ✅   | ✅              | ✅ (untested)    |
| Linux   | ✅   | ✅              | ✅               |
| MacOS   | ❌   | ❌              | ❌               |

#### Description

VTable replacement works like VFunc replacement except instead of replacing a member function pointer,
we replace the virtual table pointer.  

We replace the original virtual table pointer with a pointer to a new table `vtable2` which itself is a 
copy of the original virtual table that has VFunc replacement applied. 

Nb: If RTTI is enabled, 8 bytes are reserved at the start of the vtable for type information.

### Trampoline (Inline)

**Note: Not implemented yet**

|         | x86 | x86_64 (amd64) | Arm64 (aarch64) |
|---------|-----|----------------|-----------------|
| Windows | ❌   | ❌              | ❌               |
| Linux   | ❌   | ❌              | ❌               |
| MacOS   | ❌   | ❌              | ❌               |

#### Description

If the function you want to intercept isn't a virtual member function, you need to use
trampoline hook (or some other kind of inline hooking).

As the same suggests, inline hooking works by inserting interception/redirection code
inside the compiled assembly (A unconditional `jmp` at the start of the function you 
wish to hook). 

The term "trampoline" is used because this variant of inline hooking does a `jmp` to a 
trampoline function which we write into executable memory during run time. 

**todo: finish description for trampoline hook**

Nb: Trampoline hook may not work on all functions. The function you want to hook must
have equal or more bytes than the initial redirection `asm` (`jmp`). The actual number
of bytes depends on architecture and ISA. 

## Dependencies 🧑‍🧒‍🧒

- [Catch2](https://github.com/catchorg/Catch2): test framework
- [Capstone](https://github.com/capstone-engine/capstone): disassembly framework
- [Keystone](https://github.com/keystone-engine/keystone): assembly framework