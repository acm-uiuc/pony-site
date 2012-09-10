#pragma once


#define CONTAINING_RECORD(address, type, field) \
  ((type *)(((ULONG_PTR)address) - (ULONG_PTR)(&(((type *)0)->field))))    


BOOL HookModule(IMAGE_DOS_HEADER *dosh, const char *fromDll, 
				PVOID OldFunc, PVOID NewFunc);

BOOL HookAllModuleImports(const char *fromDll,
					PVOID OldFunc, PVOID NewFunc );

int HideDll(wchar_t *szDllName);

#pragma pack(4)
typedef struct _UNICODE_STRING 
{
	USHORT  Length;
	USHORT  MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
#pragma pack()

#pragma pack(4)
typedef struct _PEB_LDR_DATA {
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY ModuleListLoadOrder;
	LIST_ENTRY ModuleListMemoryOrder;
	LIST_ENTRY ModuleListInitOrder;
        /*...*/
} PEB_LDR_DATA, *PPEB_LDR_DATA;


typedef struct _PEB {
	BOOLEAN              InheritedAddressSpace;
	BOOLEAN              ReadImageFileExecOptions;
	BOOLEAN              BeingDebugged;
	BOOL                 SpareBool; // alloc size
	HANDLE               Mutant;
	PVOID                ImageBaseAddress;
    PPEB_LDR_DATA        Ldr;
        /*...*/
} PEB, *PPEB;
#pragma pack()


typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
    /*...*/
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;


