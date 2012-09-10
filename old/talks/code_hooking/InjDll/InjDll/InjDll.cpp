// InjDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "detours.h"
#include "pesupport.h"
#define SECURITY_WIN32
#include <sspi.h>
#include <Wincrypt.h>

HANDLE mutex;

typedef SECURITY_STATUS (WINAPI *EncryptMessage_t)(PCtxtHandle phContext, ULONG fQOP, PSecBufferDesc pMessage, ULONG MessageSeqNo);
EncryptMessage_t OrigEncryptMessage;

typedef SECURITY_STATUS (WINAPI *DecryptMessage_t)(PCtxtHandle phContext, PSecBufferDesc pMessage, ULONG MessageSeqNo, PULONG pfQOP
);
DecryptMessage_t OrigDecryptMessage;

typedef BOOL (WINAPI *CryptAcquireContext_t)(HCRYPTPROV* phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);
CryptAcquireContext_t OrigCryptAcquireContextA;
CryptAcquireContext_t OrigCryptAcquireContextW;

typedef FARPROC (WINAPI *GetProcAddress_t)(HMODULE hModule, LPCSTR lpProcName);
GetProcAddress_t OrigGetProcAddress;

typedef BOOL (WINAPI *CryptDeriveKey_t)(HCRYPTPROV hProv,ALG_ID Algid,HCRYPTHASH hBaseData,DWORD dwFlags,HCRYPTKEY* phKey);
CryptDeriveKey_t OrigCryptDeriveKey;

typedef BOOL (WINAPI *CPDeriveKey_t)(HCRYPTPROV hProv,ALG_ID Algid,HCRYPTHASH hBaseData, DWORD dwFlags, HCRYPTKEY* phKey);
CPDeriveKey_t OrigCPDeriveKey;


typedef SECURITY_STATUS (WINAPI *AcquireCredentialsHandle_t)(SEC_WCHAR* pszPrincipal, SEC_WCHAR* pszPackage, ULONG fCredentialUse,PLUID pvLogonID,SEC_WINNT_AUTH_IDENTITY *pAuthData, SEC_GET_KEY_FN pGetKeyFn,PVOID pvGetKeyArgument,PCredHandle phCredential,PTimeStamp ptsExpiry);
AcquireCredentialsHandle_t OrigAcquireCredentialsHandle;

typedef SECURITY_STATUS (WINAPI *InitializeSecurityContext_t)(PCredHandle phCredential, PCtxtHandle phContext,SEC_WCHAR* pszTargetName, ULONG fContextReq, ULONG Reserved1, ULONG TargetDataRep, PSecBufferDesc pInput, ULONG Reserved2,PCtxtHandle phNewContext, PSecBufferDesc pOutput, PULONG pfContextAttr, PTimeStamp ptsExpiry);
InitializeSecurityContext_t OrigInitializeSecurityContext;

typedef SECURITY_STATUS (WINAPI *AcceptSecurityContext_t)(PCredHandle phCredential, PCtxtHandle phContext, PSecBufferDesc pInput, ULONG fContextReq, ULONG TargetDataRep, PCtxtHandle phNewContext, PSecBufferDesc pOutput, ULONG SEC_FAR* pfContextAttr, PTimeStamp ptsExpiry);
AcceptSecurityContext_t OrigAcceptSecurityContext;

typedef LONG (WINAPI *RegOpenKeyExW_t)(HKEY hKey, LPCWSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult);
RegOpenKeyExW_t OrigRegOpenKeyExW;

typedef LONG (WINAPI *RegEnumValueW_t)(HKEY hKey, DWORD dwIndex, LPWSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
RegEnumValueW_t OrigRegEnumValueW;

typedef LONG (WINAPI *RegQueryValueExW_t)(HKEY hKey, LPWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
RegQueryValueExW_t OrigRegQueryValueExW;


typedef LONG (WINAPI *RegOpenKeyExA_t)(HKEY hKey, LPCSTR lpSubKey,DWORD ulOptions,REGSAM samDesired,PHKEY phkResult);
RegOpenKeyExA_t OrigRegOpenKeyExA;

typedef LONG (WINAPI *RegEnumValueA_t)(HKEY hKey, DWORD dwIndex, LPCSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
RegEnumValueA_t OrigRegEnumValueA;

typedef LONG (WINAPI *RegQueryValueExA_t)(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
RegQueryValueExA_t OrigRegQueryValueExA;

typedef HMODULE (WINAPI *LoadLibraryExW_t)(LPCWSTR lpFileName, HANDLE h, DWORD flags);
LoadLibraryExW_t OrigLoadLibraryExW;

typedef HMODULE (WINAPI *LoadLibraryExA_t)(LPCSTR lpFileName, HANDLE h, DWORD flags);
LoadLibraryExA_t OrigLoadLibraryExA;

typedef ULONG (WINAPI *LdrLoadDll_t)(IN PWSTR SearchPath OPTIONAL,
            IN ULONG LoadFlags,
            IN PUNICODE_STRING Name,
            OUT PVOID *BaseAddress OPTIONAL);
LdrLoadDll_t OrigLdrLoadDll;

typedef int (WINAPI *MAPILogonEx_t)(HWND hWnd,LPCSTR lpString,LPCWSTR lpWideCharStr,int,LPVOID lp);
MAPILogonEx_t OrigMAPILogonEx;


FILE *log;


void DumpSecBuffer(FILE *out, SecBuffer *pBuf)
{
	fprintf(out, " SecBuffer type %x:", pBuf->BufferType);
	for(unsigned i = 0; i < pBuf->cbBuffer; i++) {
		if(!(i % 16)) {
			fprintf(out, "\n  ");
			for(int j = 0; j < 16; j++) {
				fprintf(out, "%.2x ", ((unsigned char*)pBuf->pvBuffer)[i+j]);
			}
		}
		if(isprint(((char*)pBuf->pvBuffer)[i])) {
			fprintf(out, "%c", ((char*)pBuf->pvBuffer)[i]);
		} else {
			fprintf(out, ".");
		}
	}
	fprintf(out, "\n");
	
}

SECURITY_STATUS WINAPI NewEncryptMessage(
  PCtxtHandle phContext,
  ULONG fQOP,
  PSecBufferDesc pMessage,
  ULONG MessageSeqNo) {
	  fprintf(log, "Got EncryptMessage!\n");
#if 0
	  for(unsigned i = 0; i < pMessage->cBuffers; i++) {
		  if(pMessage->pBuffers[i].BufferType == SECBUFFER_DATA)
			  DumpSecBuffer(log, &pMessage->pBuffers[i]);
	  }
#endif
	  return OrigEncryptMessage(phContext, fQOP, pMessage, MessageSeqNo);
}

SECURITY_STATUS WINAPI NewDecryptMessage(
  PCtxtHandle phContext,
  PSecBufferDesc pMessage,
  ULONG MessageSeqNo,
  PULONG pfQOP) {
	SECURITY_STATUS stat;
	fprintf(log, "Got DecryptMessage!\n");
	stat = OrigDecryptMessage(phContext, pMessage, MessageSeqNo, pfQOP);

#if 0
	for(unsigned i = 0; i < pMessage->cBuffers; i++) {
		if(pMessage->pBuffers[i].BufferType == SECBUFFER_DATA)
		  DumpSecBuffer(log, &pMessage->pBuffers[i]);
	}
#endif
	return stat;
}


BOOL WINAPI NewCryptAcquireContextA(
  HCRYPTPROV* phProv,
  LPCTSTR pszContainer,
  LPCTSTR pszProvider,
  DWORD dwProvType,
  DWORD dwFlags) {
	fprintf(log, "Got CryptAcquireContextA(%x=%x, %x)\n", dwProvType, PROV_MS_EXCHANGE, dwFlags);
	
	return OrigCryptAcquireContextA(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
}

BOOL WINAPI NewCryptAcquireContextW(
  HCRYPTPROV* phProv,
  LPCTSTR pszContainer,
  LPCTSTR pszProvider,
  DWORD dwProvType,
  DWORD dwFlags) {
	fprintf(log, "Got CryptAcquireContextW(%x=%x, %x)\n", dwProvType, PROV_MS_EXCHANGE, dwFlags);
	
	return OrigCryptAcquireContextW(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
}


BOOL WINAPI NewCPDeriveKey(
  HCRYPTPROV hProv,
  ALG_ID Algid,
  HCRYPTHASH hBaseData,
  DWORD dwFlags,
  HCRYPTKEY* phKey) {
    fprintf(log, "Got CPDeriveKey\n");
	return OrigCPDeriveKey(hProv, Algid, hBaseData, dwFlags, phKey);
}

BOOL WINAPI NewCryptDeriveKey(
  HCRYPTPROV hProv,
  ALG_ID Algid,
  HCRYPTHASH hBaseData,
  DWORD dwFlags,
  HCRYPTKEY* phKey) {
  fprintf(log, "Got CryptDerive\n");
  return OrigCryptDeriveKey(hProv, Algid, hBaseData, dwFlags, phKey);
}

SECURITY_STATUS WINAPI NewInitializeSecurityContext(
  PCredHandle phCredential,
  PCtxtHandle phContext,
  SEC_WCHAR* pszTargetName,
  ULONG fContextReq,
  ULONG Reserved1,
  ULONG TargetDataRep,
  PSecBufferDesc pInput,
  ULONG Reserved2,
  PCtxtHandle phNewContext,
  PSecBufferDesc pOutput,
  PULONG pfContextAttr,
  PTimeStamp ptsExpiry) {
  SECURITY_STATUS stat;
  WaitForSingleObject(mutex, INFINITE);
  fprintf(log, "Got InitSecContext(%ws, %x, %x.{%x,%x})\n", pszTargetName, fContextReq, 
	  phCredential, phCredential->dwLower, phCredential->dwUpper);

  if(pInput) {
	  fprintf(log, "pInput: \n");
	  for(unsigned i = 0; i < pInput->cBuffers; i++) {
		  DumpSecBuffer(log, &pInput->pBuffers[i]);
	  }
  }
  stat = OrigInitializeSecurityContext(phCredential, phContext, pszTargetName, fContextReq, Reserved1,
	  TargetDataRep, pInput, Reserved2, phNewContext, pOutput, pfContextAttr, ptsExpiry);
  
  if(pOutput) {
	  fprintf(log, "pOutput: \n");
	  for(unsigned i = 0; i < pOutput->cBuffers; i++) {
		  DumpSecBuffer(log, &pOutput->pBuffers[i]);
	  }
  }
  
  ReleaseMutex(mutex);
  return stat;
}


SECURITY_STATUS WINAPI NewAcquireCredentialsHandle(
  SEC_WCHAR* pszPrincipal,
  SEC_WCHAR* pszPackage,
  ULONG fCredentialUse,
  PLUID pvLogonID,
  SEC_WINNT_AUTH_IDENTITY *pAuthData,
  SEC_GET_KEY_FN pGetKeyFn,
  PVOID pvGetKeyArgument,
  PCredHandle phCredential,
  PTimeStamp ptsExpiry) {
  SECURITY_STATUS stat;
  fprintf(log, "Got AcquireCredentialsHandle(%ws, %ws, %x.{%x, %x}\n", pszPrincipal, pszPackage,
	  phCredential, phCredential->dwLower, phCredential->dwUpper);
  stat = OrigAcquireCredentialsHandle(pszPrincipal, pszPackage, fCredentialUse, pvLogonID, pAuthData,
	  pGetKeyFn, pvGetKeyArgument, phCredential, ptsExpiry);
  fprintf(log, "   Post AcquireCredentialsHandle(%x, %x, %x)\n", phCredential, phCredential->dwLower, phCredential->dwUpper);
  return stat;
}

SECURITY_STATUS WINAPI NewAcceptSecurityContext( 
  PCredHandle phCredential, 
  PCtxtHandle phContext, 
  PSecBufferDesc pInput, 
  ULONG fContextReq, 
  ULONG TargetDataRep, 
  PCtxtHandle phNewContext, 
  PSecBufferDesc pOutput, 
  ULONG SEC_FAR* pfContextAttr, 
  PTimeStamp ptsExpiry) {
  fprintf(log, "Got AcceptSecContext\n");
  return OrigAcceptSecurityContext(phCredential, phContext, pInput, fContextReq, TargetDataRep, phNewContext, 
	  pOutput, pfContextAttr, ptsExpiry);
}

int WINAPI NewMAPILogonEx(HWND hWnd,LPCSTR lpString,LPCWSTR lpWideCharStr,int flags,LPVOID lpSession)
{
	fprintf(log, "MAPILogonEx(%s, %ws, %x)\n", lpString, lpWideCharStr, flags);
	return OrigMAPILogonEx(hWnd, lpString, lpWideCharStr, flags, lpSession);
}


BOOL second_stage = false;
void SecondStageHooks()
{
	HMODULE mod = GetModuleHandle(TEXT("secur32.dll"));
	HMODULE advMod = GetModuleHandle(TEXT("advapi32.dll"));
	HMODULE mapiMod = GetModuleHandle(TEXT("MSMAPI32.dll"));

	second_stage = true;

	if(!(OrigEncryptMessage = 
		(EncryptMessage_t)GetProcAddress(mod, "EncryptMessage"))) {
		fprintf(log, "No EncryptMessage!\n\n");
	}
	if(!(OrigDecryptMessage = 
		(DecryptMessage_t)GetProcAddress(mod, "DecryptMessage"))) {
		fprintf(log, "No DecryptMessage!\n\n");
	}
	if(!(OrigCryptAcquireContextA = 
		(CryptAcquireContext_t)GetProcAddress(advMod, "CryptAcquireContextA"))) {
		fprintf(log, "No CACA!\n\n");
	}
	if(!(OrigCryptAcquireContextW = 
		(CryptAcquireContext_t)GetProcAddress(advMod, "CryptAcquireContextW"))) {
		fprintf(log, "No CACW!\n\n");
	}
#if 0
	if(!(OrigCPDeriveKey =
		(CPDeriveKey_t)GetProcAddress(advMod, "CPDeriveKey"))) {
		fprintf(log, "No CPDerive!\n\n");
	}
#endif
	if(!(OrigCryptDeriveKey =
		(CryptDeriveKey_t)GetProcAddress(advMod, "CryptDeriveKey"))) {
		fprintf(log, "No CryptDerive!\n\n");
	}
	if(!(OrigInitializeSecurityContext =
		(InitializeSecurityContext_t)GetProcAddress(mod, "InitializeSecurityContextW"))) {
		fprintf(log, "No InitSecContext!\n\n");
	}
	if(!(OrigAcquireCredentialsHandle =
		(AcquireCredentialsHandle_t)GetProcAddress(mod, "AcquireCredentialsHandleW"))) {
		fprintf(log, "No AcquireCredHandle!\n\n");
	}
	if(!(OrigAcceptSecurityContext =
		(AcceptSecurityContext_t)GetProcAddress(mod, "AcceptSecurityContext"))) {
		fprintf(log, "No AcceptSecContext!\n\n");
	}
	if(!(OrigMAPILogonEx = 
		(MAPILogonEx_t)GetProcAddress(mapiMod, "MAPILogonEx"))) {
		fprintf(log, "No MAPILogonEx\n\n");
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)OrigEncryptMessage, NewEncryptMessage);
	DetourAttach(&(PVOID&)OrigDecryptMessage, NewDecryptMessage);
	DetourAttach(&(PVOID&)OrigCryptAcquireContextA, NewCryptAcquireContextA);
	DetourAttach(&(PVOID&)OrigCryptAcquireContextW, NewCryptAcquireContextW);
	DetourAttach(&(PVOID&)OrigCryptDeriveKey, NewCryptDeriveKey);
	DetourAttach(&(PVOID&)OrigInitializeSecurityContext, NewInitializeSecurityContext);
	DetourAttach(&(PVOID&)OrigAcquireCredentialsHandle, NewAcquireCredentialsHandle);
	DetourAttach(&(PVOID&)OrigAcceptSecurityContext, NewAcceptSecurityContext);
	DetourAttach(&(PVOID&)OrigMAPILogonEx, NewMAPILogonEx);
	DetourTransactionCommit();
}

ULONG NTAPI NewLdrLoadDll (IN PWSTR SearchPath OPTIONAL,
            IN ULONG LoadFlags,
            IN PUNICODE_STRING Name,
			OUT PVOID *BaseAddress OPTIONAL) {
	ULONG stat = OrigLdrLoadDll(SearchPath, LoadFlags, Name, BaseAddress);
	if(Name) {
		fprintf(log, "LdrLoadDll of %ws\n", Name->Buffer);
		if(GetModuleHandle(TEXT("secur32.dll")) && GetModuleHandle(TEXT("MSMAPI32.dll"))) {
			fprintf(log, "COOL! LdrLoadDll of %ws\n", Name->Buffer);
			WaitForSingleObject(mutex, INFINITE);
			if(!second_stage) {
				SecondStageHooks();
			}
			ReleaseMutex(mutex);
		}
	}
	return stat;
}


HMODULE WINAPI NewLoadLibraryExW(LPCWSTR lib, HANDLE h, DWORD flags)
{
	HMODULE ret = OrigLoadLibraryExW(lib,h,flags);
	if(lib && wcsstr(lib, L"secur32.dll")) {
		fprintf(log, "LoadLibW of %ws\n", lib);
		WaitForSingleObject(mutex, INFINITE);
		if(!second_stage) {
			SecondStageHooks();
		}
		ReleaseMutex(mutex);
	}
	return ret;
}

HMODULE WINAPI NewLoadLibraryExA(LPCSTR lib, HANDLE h, DWORD flags)
{
	HMODULE ret = OrigLoadLibraryExA(lib,h,flags);
	if(lib && strstr(lib, "secur32.dll")) {
		fprintf(log, "LoadLibW of %s\n", lib);
		WaitForSingleObject(mutex, INFINITE);
		if(!second_stage) {
			SecondStageHooks();
		}
		ReleaseMutex(mutex);
	}
	return ret;
}

LONG WINAPI NewRegOpenKeyExA(
  HKEY hKey,
  LPCSTR lpSubKey,
  DWORD ulOptions,
  REGSAM samDesired,
  PHKEY phkResult) {
	  //fprintf(log, "Open keyA: %s\n", lpSubKey);
	  if(lpSubKey) {
		  if(strstr(lpSubKey, "13dbb0c8aa05101a9bb000")) {
			fprintf(log, "Interesting keyA: %s\n", lpSubKey);
		  }
	  }
	  return OrigRegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

LONG WINAPI NewRegQueryValueExA(
  HKEY hKey,
  LPCSTR lpValueName,
  LPDWORD lpReserved,
  LPDWORD lpType,
  LPBYTE lpData,
  LPDWORD lpcbData) {
	  //fprintf(log, "Query keyA: %s\n", lpValueName);
	  if(lpValueName) {
		  if(strstr(lpValueName, "36606")) {
			fprintf(log, "Interesting Query keyA: %s\n", lpValueName);
		  }
	  }

	  return OrigRegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

LONG WINAPI NewRegEnumValueA(
  HKEY hKey,
  DWORD dwIndex,
  LPCSTR lpValueName,
  LPDWORD lpcValueName,
  LPDWORD lpReserved,
  LPDWORD lpType,
  LPBYTE lpData,
  LPDWORD lpcbData) {
	  LONG ret;
	  ret = OrigRegEnumValueA(hKey, dwIndex, lpValueName, lpcValueName, lpReserved, lpType, lpData,
		  lpcbData);
	  //fprintf(log, "Enum keyA: %s\n", lpValueName);
	  if(lpValueName) {
		  if(strstr(lpValueName, "36606")) {
			fprintf(log, "Interesting Enum keyA: %s\n", lpValueName);
		  }
	  }
	  return ret;
}


LONG WINAPI NewRegOpenKeyExW(
  HKEY hKey,
  LPCWSTR lpSubKey,
  DWORD ulOptions,
  REGSAM samDesired,
  PHKEY phkResult) {
	  //fprintf(log, "Open keyW: %ws\n", lpSubKey);
	  if(lpSubKey) {
		  if(wcsstr(lpSubKey, L"13dbb0c8aa05101a9bb000")) {
			fprintf(log, "Interesting keyW: %ws\n", lpSubKey);
		  }
	  }
	  return OrigRegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

void WINAPI AfxDumpStack(
   DWORD dwTarget = 0
);

LONG WINAPI NewRegQueryValueExW(
  HKEY hKey,
  LPWSTR lpValueName,
  LPDWORD lpReserved,
  LPDWORD lpType,
  LPBYTE lpData,
  LPDWORD lpcbData) {
	  //fprintf(log, "Query keyW: %ws\n", lpValueName);
	  if(lpValueName) {
		  if(wcsstr(lpValueName, L"36606")) {
			//AfxDumpStack();
			fprintf(log, "Interesting Query keyW: %ws\n", lpValueName);
		  }
	  }

	  return OrigRegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

LONG WINAPI NewRegEnumValueW(
  HKEY hKey,
  DWORD dwIndex,
  LPTSTR lpValueName,
  LPDWORD lpcValueName,
  LPDWORD lpReserved,
  LPDWORD lpType,
  LPBYTE lpData,
  LPDWORD lpcbData) {
	  LONG ret;
	  ret = OrigRegEnumValueW(hKey, dwIndex, lpValueName, lpcValueName, lpReserved, lpType, lpData,
		  lpcbData);
	  //fprintf(log, "Enum keyW: %ws\n", lpValueName);
	  if(lpValueName) {
		  if(wcsstr(lpValueName, L"36606")) {
			fprintf(log, "Interesting Enum keyW: %ws\n", lpValueName);
		  }
	  }
	  return ret;
}

void HookWithDetours()
{
	HMODULE advMod = GetModuleHandle(TEXT("advapi32.dll"));
	HMODULE kMod = GetModuleHandle(TEXT("kernel32.dll"));
	HMODULE ntMod = GetModuleHandle(TEXT("ntdll.dll"));

	if(!(OrigRegOpenKeyExA =
		(RegOpenKeyExA_t)GetProcAddress(advMod, "RegOpenKeyExA"))) {
		fprintf(log, "Damn. No RegOpenKeyExA\n\n");
	}
    if(!(OrigRegQueryValueExA =
		(RegQueryValueExA_t)GetProcAddress(advMod, "RegQueryValueExA"))) {
		fprintf(log, "Damn. No RegQueryValueExA\n\n");
	}
	if(!(OrigRegEnumValueA =
		(RegEnumValueA_t)GetProcAddress(advMod, "RegEnumValueA"))) {
		fprintf(log, "Damn. No RegEnumValueExA\n\n");
	}
	if(!(OrigRegOpenKeyExW =
		(RegOpenKeyExW_t)GetProcAddress(advMod, "RegOpenKeyExW"))) {
		fprintf(log, "Damn. No RegOpenKeyExW\n\n");
	}
    if(!(OrigRegQueryValueExW =
		(RegQueryValueExW_t)GetProcAddress(advMod, "RegQueryValueExW"))) {
		fprintf(log, "Damn. No RegQueryValueExW\n\n");
	}
	if(!(OrigRegEnumValueW =
		(RegEnumValueW_t)GetProcAddress(advMod, "RegEnumValueW"))) {
		fprintf(log, "Damn. No RegEnumValueExW\n\n");
	}
	if(!(OrigLoadLibraryExW =
		(LoadLibraryExW_t)GetProcAddress(kMod, "LoadLibraryExW"))) {
		fprintf(log, "Damn. No LoadLibW\n\n");
	}
	if(!(OrigLoadLibraryExA =
		(LoadLibraryExA_t)GetProcAddress(kMod, "LoadLibraryExA"))) {
		fprintf(log, "Damn. No LoadLibA\n\n");
	}
	if(!(OrigLdrLoadDll =
		(LdrLoadDll_t)GetProcAddress(ntMod, "LdrLoadDll"))) {
		fprintf(log, "Damn. No LdrLoadDll\n\n");
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)OrigRegOpenKeyExA, NewRegOpenKeyExA);
	DetourAttach(&(PVOID&)OrigRegQueryValueExA, NewRegQueryValueExA);
	DetourAttach(&(PVOID&)OrigRegEnumValueA, NewRegEnumValueA);
	DetourAttach(&(PVOID&)OrigRegOpenKeyExW, NewRegOpenKeyExW);
	DetourAttach(&(PVOID&)OrigRegQueryValueExW, NewRegQueryValueExW);
	DetourAttach(&(PVOID&)OrigRegEnumValueW, NewRegEnumValueW);
	DetourAttach(&(PVOID&)OrigLoadLibraryExW, NewLoadLibraryExW);
	DetourAttach(&(PVOID&)OrigLoadLibraryExA, NewLoadLibraryExA);
	DetourAttach(&(PVOID&)OrigLdrLoadDll, NewLdrLoadDll);
	DetourTransactionCommit();
}

FARPROC WINAPI NewGetProcAddress(
  HMODULE hModule,
  LPCSTR lpProcName) {
     FARPROC ret = 0;
	 if(lpProcName && !IsBadReadPtr(lpProcName, 4) 
		 && strcmp(lpProcName, "DecodePointer") != 0) {
		fprintf(log, "GetProc(%x %s)\n", hModule, lpProcName);
		fflush(log);
	 }

	 ret = OrigGetProcAddress(hModule, lpProcName);
	 return ret;
}

// XXX: Broken w/ outlook...
void HookWithIAT()
{
	HMODULE mod = GetModuleHandle(TEXT("kernel32.dll"));
	OrigGetProcAddress = 
		(GetProcAddress_t)GetProcAddress(mod, "GetProcAddress");

	if(!OrigGetProcAddress) {
		MessageBoxA(0, "Damn. No ProcAddress", "Error", MB_OK);
		fprintf(log, "No proc addr: %d\n", GetLastError());
		return;
	}
	HookAllModuleImports("kernel32.dll", OrigGetProcAddress, 
		NewGetProcAddress);
}

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		if(!(log = fopen("c:\\dev\\SM2k6\\InjDll.log", "w+"))) {
			MessageBoxA(0, "No log", "Sucks", MB_OK);
		}
		mutex = CreateMutex(NULL, FALSE, NULL);

#if 0
		__try {
			HookWithIAT();
		} __except(true) {
			fprintf(log, "Got IAT exception code: %x\n", GetExceptionCode());
		}
#endif
		__try {
			HookWithDetours();
		} __except (true) {
			fprintf(log, "Got detours exception code: %x\n", GetExceptionCode());
		}
		fprintf(log, "Ok Hook in\n");
		fflush(log);
		//MessageBoxA(0, "Hellllloooooooo nurse!!!", "Ok we're in!", MB_OK);
		//HideDll(L"detoured.dll");
		//HideDll(L"InjDll.dll");
	} else if(ul_reason_for_call == DLL_PROCESS_DETACH) {
		fclose(log);
	}

    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif