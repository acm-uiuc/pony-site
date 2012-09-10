// DllInjection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

// Detours 2.1 provies a CreateProcessWithDll but that
// actually modifies the imports of the PE header.
// Detours 1.5 was far nicer in this regard and even had a
// ContinueProcessWithDll funciton to inject on the fly.
// Microsoft probably crippled it for "security reasons", as well
// as adding the "detoured.dll" bullshit.
// Gotta hand it to them, censoring the code will work well... 
// Can't just rewrite that part. Oh wait, we did. It's right here:
void ManualInject(int pid, char *dll)
{
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	PVOID libname;

	if(!proc) {
		fprintf(stderr, "Failed to get handle: %d\n", GetLastError());
		return;
	}

	if(!(libname = VirtualAllocEx(proc, 0, 4096, MEM_COMMIT, 
		PAGE_EXECUTE_READWRITE))) {
		fprintf(stderr, "Failed to alloc: %d\n", GetLastError());
		return;
	}

	if(!WriteProcessMemory(proc, libname, (LPCVOID) dll, strlen(dll)+1, 0)) {
		fprintf(stderr, "Process memory write failed: %d\n", GetLastError());
		return;
	}

	HMODULE mod = GetModuleHandle(TEXT("Kernel32.dll"));
	PVOID LoadLib = GetProcAddress(mod, "LoadLibraryA");

	if(!LoadLib) {
		fprintf(stderr, "Can't load lib: %d\n", GetLastError());
		return;
	}

	DWORD thrid;
	HANDLE hthread = CreateRemoteThread(proc, NULL, 0, 
		(LPTHREAD_START_ROUTINE)LoadLib, libname, 0, &thrid);


	if(!hthread) {
		fprintf(stderr, "Can't create thread: %d\n", GetLastError());
		return;
	}

	printf("Dll %s successfully injected w/ thrid %d and start %p/%p!\n", 
		dll, thrid, LoadLib, mod);
}


int _tmain(int argc, _TCHAR* argv[])
{
	char buf[32];
	printf("Enter PID to inject to: ");
	fgets(buf, sizeof(buf), stdin);
	int pid = atoi(buf);
	ManualInject(pid, "c:\\dev\\SM2k6\\InjDll\\release\\detoured.dll");
	ManualInject(pid, "c:\\dev\\SM2k6\\InjDll\\release\\InjDll.dll");
	printf("Hit return to exit..");
	//fgets(buf, sizeof(buf), stdin);
	return 0;
}

