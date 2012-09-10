//------------------------------------------------------------------------------
//  <copyright from='2004' to='2005' company='WoWSharp.NET'>
//    Copyright (c) WoWSharp.NET. All Rights Reserved.
//
//    Please look in the accompanying license.htm file for the license that 
//    applies to this source code. (a copy can also be found at: 
//    http://www.wowsharp.net/license.htm)
//  </copyright>
//-------------------------------------------------------------------------------

#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "WoW!DllHideEng.h"
#include "WoW!PEBStruct.h"

int HideDll(char *szDllName, BOOL bHide)
{
	if(bHide)
		return (	WalkModuleList(LOAD_ORDER_TYPE, szDllName)
			&&	WalkModuleList(MEM_ORDER_TYPE, szDllName)
			&&	WalkModuleList(INIT_ORDER_TYPE, szDllName)	);

	return (	WalkModuleListAdd(LOAD_ORDER_TYPE, szDllName)
			&&	WalkModuleListAdd(MEM_ORDER_TYPE, szDllName)
			&&	WalkModuleListAdd(INIT_ORDER_TYPE, szDllName)	);
}

// Call GetPEB(0) to get PEB base address for current process
DWORD GetPEB(DWORD Pid)
{
	DWORD dwPebBase = FUNC_ERROR;
	if(!Pid)
	{
		// Return PEB address for current process
		// address is located at FS:0x30
		__asm 
		{
			push eax
			mov eax, FS:[0x30]
			mov [dwPebBase], eax
			pop eax
		}
	}

	return (DWORD)dwPebBase;
}

PLIST_ENTRY pUserModuleListLoadOrder;
PLIST_ENTRY pUserModuleListMemoryOrder;
PLIST_ENTRY pUserModuleListInitOrder;

// Walks one of the three modules double linked lists referenced by the PEB, adds old pointer back as first in list
int WalkModuleListAdd(char ModuleListType, char *szDllToStrip)
{
	PLIST_ENTRY pUserModuleListHead, pUserModuleListPtr;
		
	DWORD PebBaseAddr, dwOffset=0;
	PPEB_LDR_DATA pLdrData;
	
	PebBaseAddr = GetPEB(0);
	if(PebBaseAddr == FUNC_ERROR)
		return FUNC_ERROR;
	pLdrData=(PPEB_LDR_DATA)(DWORD *)(*(DWORD *)(PebBaseAddr + PEB_LDR_DATA_OFFSET)); // PEB.ProcessModuleInfo = PEB + 0x0C
	if(!pLdrData->Initialized) 
		return FUNC_ERROR;

	// Init chained list head and offset
	if(ModuleListType == LOAD_ORDER_TYPE)
	{
		// LOAD_ORDER_TYPE
		pUserModuleListHead = (PLIST_ENTRY)(&(pLdrData->ModuleListLoadOrder));
		pUserModuleListPtr = pUserModuleListLoadOrder;
		dwOffset = 0x0;
	} else if(ModuleListType == MEM_ORDER_TYPE)
	{
		// MEM_ORDER_TYPE
		pUserModuleListHead = (PLIST_ENTRY)(&(pLdrData->ModuleListMemoryOrder));
		pUserModuleListPtr = pUserModuleListMemoryOrder;
		dwOffset = 0x08;
	} else if(ModuleListType == INIT_ORDER_TYPE)
	{
		// INIT_ORDER_TYPE
		pUserModuleListHead = (PLIST_ENTRY)(&(pLdrData->ModuleListInitOrder));
		pUserModuleListPtr = pUserModuleListInitOrder;
		dwOffset = 0x10;
	}
	else return FUNC_ERROR;
	

	// Add this dll :
        pUserModuleListPtr->Blink = pUserModuleListHead;
        pUserModuleListPtr->Flink = (pUserModuleListHead->Flink);
        pUserModuleListHead->Flink = pUserModuleListPtr;
        (pUserModuleListPtr->Flink)->Blink = pUserModuleListPtr;

	return FUNC_SUCCESS;
}

DWORD whImageBase = 0;
DWORD whImageSize = 0;

// Walks one of the three modules double linked lists referenced by the PEB
int WalkModuleList(char ModuleListType, char *szDllToStrip)
{
	PLIST_ENTRY pUserModuleListHead, pUserModuleListPtr;
	PLIST_ENTRY *pHiddenModuleListPtr;

	int i;
	DWORD PebBaseAddr, dwOffset=0;
	DWORD ImageBase, ImageSize;
	PPEB_LDR_DATA pLdrData;
	PUNICODE_STRING pImageName;
	char szImageName[BUFMAXLEN]; // Non-unicode string
	
	PebBaseAddr = GetPEB(0);
	if(PebBaseAddr == FUNC_ERROR)
		return FUNC_ERROR;
	pLdrData=(PPEB_LDR_DATA)(DWORD *)(*(DWORD *)(PebBaseAddr + PEB_LDR_DATA_OFFSET)); // PEB.ProcessModuleInfo = PEB + 0x0C
	if(!pLdrData->Initialized) 
		return FUNC_ERROR;

	// Init chained list head and offset
	if(ModuleListType == LOAD_ORDER_TYPE)
	{
		// LOAD_ORDER_TYPE
		pUserModuleListHead = pUserModuleListPtr = (PLIST_ENTRY)(&(pLdrData->ModuleListLoadOrder));
		pHiddenModuleListPtr = &pUserModuleListLoadOrder;
		dwOffset = 0x0;
	} else if(ModuleListType == MEM_ORDER_TYPE)
	{
		// MEM_ORDER_TYPE
		pUserModuleListHead = pUserModuleListPtr = (PLIST_ENTRY)(&(pLdrData->ModuleListMemoryOrder));
		pHiddenModuleListPtr = &pUserModuleListMemoryOrder;
		dwOffset = 0x08;
	} else if(ModuleListType == INIT_ORDER_TYPE)
	{
		// INIT_ORDER_TYPE
		pUserModuleListHead = pUserModuleListPtr = (PLIST_ENTRY)(&(pLdrData->ModuleListInitOrder));
		pHiddenModuleListPtr = &pUserModuleListInitOrder;
		dwOffset = 0x10;
	}
	else return FUNC_ERROR;
	
	do
	{
		// Jump to next MODULE_ITEM structure
		pUserModuleListPtr = pUserModuleListPtr->Flink;
		pImageName = (PUNICODE_STRING)( ((DWORD)(pUserModuleListPtr)) + (LDR_DATA_PATHFILENAME_OFFSET-dwOffset));
		ImageBase = *(DWORD *)(((DWORD)(pUserModuleListPtr)) + (LDR_DATA_IMAGE_BASE-dwOffset));
		ImageSize = *(DWORD *)(((DWORD)(pUserModuleListPtr)) + (LDR_DATA_IMAGE_SIZE-dwOffset));

        //Convert string from unicode and to lower case :
		for(i=0; i < (pImageName->Length)/2 && i<BUFMAXLEN;i++) 
              szImageName[i] = LOWCASE(*( (pImageName->Buffer)+(i) ));
		szImageName[i] = '\0';

		if( strstr((char*)szImageName, szDllToStrip) != 0 )
		{
			// Hide this dll :
			// throw this module away (out of the double linked list)
           (pUserModuleListPtr->Blink)->Flink = (pUserModuleListPtr->Flink);
           (pUserModuleListPtr->Flink)->Blink = (pUserModuleListPtr->Blink);

			whImageBase = ImageBase;
			whImageSize = ImageSize;

		   *pHiddenModuleListPtr = pUserModuleListPtr;
		}
	}	while(pUserModuleListPtr->Flink != pUserModuleListHead); 

	return FUNC_SUCCESS;
}
