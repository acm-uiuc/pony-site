#include "stdafx.h"
#include "pesupport.h"
#include <windows.h>
#include <Tlhelp32.h>
#include <stdio.h>

VOID RtlInitUnicodeString(          PUNICODE_STRING DestinationString,
    PCWSTR SourceString
);

#define RVA_TO_VA(rva, base) (((DWORD)rva) + ((DWORD)base))
#define HINT_ORD (1<<31)

// Get the imports listed in the IAT and INT
BOOL WalkAndHookIXT(PVOID OldFunc, PVOID NewFunc, IMAGE_THUNK_DATA *pIAT)
{
	BOOL found = false;
	
	while(pIAT->u1.AddressOfData != 0) {
		if(pIAT->u1.Function == (DWORD)OldFunc) {
			DWORD old;
			fprintf(log, "Found match for %x. Replacing\n", pIAT->u1.Function);
			VirtualProtect(pIAT, sizeof(void *), PAGE_READWRITE, &old);
			pIAT->u1.Function = (DWORD)NewFunc;
			VirtualProtect(pIAT, sizeof(void *), old, &old);
			found = true;
		}
    
        pIAT++;
    }
    return found;
}

BOOL HookModule(IMAGE_DOS_HEADER *dosh, const char *fromDll, 
				PVOID OldFunc, PVOID NewFunc)
{
	BOOL found = false;
	IMAGE_NT_HEADERS *pnt = (IMAGE_NT_HEADERS *)((DWORD_PTR)dosh
            + (DWORD_PTR)dosh->e_lfanew);
    IMAGE_IMPORT_DESCRIPTOR *id;

    if(dosh->e_magic != IMAGE_DOS_SIGNATURE) {
        fprintf(log, "Magic number: %x vs %x\n", dosh->e_magic, IMAGE_DOS_SIGNATURE);
        return -1;
    }

    if(pnt->Signature != IMAGE_NT_SIGNATURE) {
        fprintf(log, "Invalid NT signature %x\n", pnt->Signature);
        return -1;
    }

    // get the RVA of the import descriptor from the image directory entires
    unsigned long ImportDescRVA =
        (pnt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    id = (IMAGE_IMPORT_DESCRIPTOR *)RVA_TO_VA(ImportDescRVA, dosh);

	while(id != 0 && (id->TimeDateStamp != 0 || id->Name != 0)) {
		char *name = (char *)RVA_TO_VA(id->Name, dosh);
		//msg("Id->Name RVA: %x\n", id->Name);
		if(stricmp(name, fromDll) == 0) {
			BOOL tmp = WalkAndHookIXT(OldFunc, NewFunc, 
				(IMAGE_THUNK_DATA *)RVA_TO_VA(id->FirstThunk, dosh));

			if(tmp) {
				fprintf(log, "Match for %x found in %s\n", OldFunc,
					name);
			}
		
			found = found || tmp;

		}
		id++;
	}
	return found;
}


BOOL HookAllModuleImports(const char *fromDll,
					PVOID OldFunc, PVOID NewFunc )
{
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;

  // Take a snapshot of all modules in the specified process.
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetCurrentProcessId() );
  if( hModuleSnap == INVALID_HANDLE_VALUE )
  {
    fprintf(log, "CreateToolhelp32Snapshot (of modules)" );
    return( FALSE );
  }

  // Set the size of the structure before using it.
  me32.dwSize = sizeof( MODULEENTRY32 );

  // Retrieve information about the first module,
  // and exit if unsuccessful
  if( !Module32First( hModuleSnap, &me32 ) )
  {
    CloseHandle( hModuleSnap );     // Must clean up the snapshot object!
    return( FALSE );
  }

//  Now walk the module list of the process,
//  and display information about each module
  do
  {
    fprintf(log, "     MODULE NAME:     %ws\n",             me32.szModule );
    fprintf(log, "     base address   = 0x%08X\n", (DWORD) me32.modBaseAddr );
  
	if(!HookModule((IMAGE_DOS_HEADER*)me32.modBaseAddr, fromDll, 
		OldFunc, NewFunc)) {
		fprintf(log, "Can't find %x in %ws\n", OldFunc, me32.szModule);
	}
  } while( Module32Next( hModuleSnap, &me32 ) );
  fprintf(log, "\n");

  // Do not forget to clean up the snapshot object.
  CloseHandle( hModuleSnap );
  return( TRUE );
}

PPEB GetPEB()
{
    PPEB dwPebBase = 0;
    // Return PEB address for current process
    // address is located at FS:0x30
    __asm {
        push eax
        mov eax, FS:[0x30]
        mov [dwPebBase], eax
        pop eax
    }

    return dwPebBase;
}

int RemoveDLL(PLIST_ENTRY Head, wchar_t *szDllToStrip, DWORD offset)
{
    PLDR_DATA_TABLE_ENTRY dllEntry;
    PLIST_ENTRY ptr = Head;

	fprintf(log, "Here1: %x\n", ptr);
	fflush(log);

    do {
		fprintf(log, "Here2: %x\n", ptr);
		fflush(log);
        ptr = ptr->Flink;
        dllEntry = (PLDR_DATA_TABLE_ENTRY)(&ptr[-offset]);
		if(!ptr) {
			fprintf(log, "Null ptr!\n");
			break;
		}
		if(!dllEntry->BaseDllName.Buffer || 
			!dllEntry->LoadCount) {
			fprintf(log, "Bad dll\n");
			continue;
		}
		fprintf(log, "Here3: %x\n", dllEntry);
		fflush(log);
        
		if( wcscmp(dllEntry->FullDllName.Buffer, szDllToStrip) == 0) {
			fprintf(log, "Found DLL match: %ws %ws %d\n", 
			 szDllToStrip, dllEntry->FullDllName.Buffer, offset);
			fflush(log);
            (ptr->Blink)->Flink = (ptr->Flink);
            (ptr->Flink)->Blink = (ptr->Blink);
		} else {
			fprintf(log, "No match: %ws %ws %d\n", 
				szDllToStrip, dllEntry->FullDllName.Buffer, offset);
			fflush(log);
		}

    } while(ptr->Flink != Head); 

	fprintf(log, "Done with DLL hide\n");
	fflush(log);
    return 1;
}

int HideDll(wchar_t *szDllName)
{
    PPEB peb = GetPEB();
    BOOL ret = 1;

    if(!peb)
        return 0;
    
    if(!peb->Ldr->Initialized) 
        return 0;


    ret = RemoveDLL(&peb->Ldr->ModuleListLoadOrder, szDllName, 0);
    ret &= RemoveDLL(&peb->Ldr->ModuleListMemoryOrder, szDllName, 1);
    ret &= RemoveDLL(&peb->Ldr->ModuleListInitOrder, szDllName, 2);
    return ret;
}


