﻿// SPDX-License-Identifier: GPL-2.0-or-later
/* defines_win32.h
 * Copyright (C) 2020 Feng Shun.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as 
 *   published by the Free Software Foundation, either version 3 of the
 *   License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License version 2 for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   version 2 along with this program. If not, see
 *   <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "includes_win32.h"
#include "defines_generic.h"

#define PXCH_DO_IN_CRITICAL_SECTION_RETURN_DWORD \
	DWORD dwReturn = 0; \
	int iLock; \
	HeapLock(GetProcessHeap()); \
	goto lock_critical_section_start; \
lock_after_critical_section: \
	HeapUnlock(GetProcessHeap()); \
	return dwReturn; \
 \
lock_critical_section_start: \
for (iLock = 0; ; iLock++) \
if (iLock > 0) goto lock_after_critical_section; \
else


#define PXCH_DO_IN_CRITICAL_SECTION_RETURN_VOID \
	int iLock; \
	HeapLock(GetProcessHeap()); \
	goto lock_critical_section_start; \
lock_after_critical_section: \
	HeapUnlock(GetProcessHeap()); \
	return; \
 \
lock_critical_section_start: \
for (iLock = 0; ; iLock++) \
if (iLock > 0) goto lock_after_critical_section; \
else


typedef HMODULE(WINAPI* FpGetModuleHandleW)(LPCWSTR);
typedef HMODULE(WINAPI* FpLoadLibraryW)(LPCWSTR);
typedef FARPROC(WINAPI* FpGetProcAddress)(HMODULE, LPCSTR);
typedef BOOL (WINAPI* FpFreeLibrary)(HMODULE);
typedef DWORD(WINAPI* FpGetLastError)(VOID);
typedef VOID (WINAPI* FpOutputDebugStringA)(LPCSTR);
typedef DWORD (WINAPI* FpGetCurrentProcessId)(VOID);
typedef int (__cdecl* FpwsprintfA)(char* const _Buffer, char const* const _Format,...);
typedef VOID (WINAPI* FpSleep)(DWORD);
typedef VOID (WINAPI* FpExitThread)(DWORD);
typedef BOOL (WINAPI* FpReleaseSemaphore)(HANDLE hSemaphore, DWORD lReleaseCount, LPDWORD lpPreviousCount);
typedef BOOL (WINAPI* FpCloseHandle)(HANDLE h);
typedef DWORD (WINAPI* FpWaitForSingleObject)(HANDLE hHandle, DWORD dwMilliseconds);

#pragma pack(push, 1)
typedef struct _PXCH_INJECT_REMOTE_DATA {
	PXCH_UINT32 dwSize;
	PXCH_UINT32 dwEverExecuted;

	PXCH_UINT32 dwParentPid;
	PXCH_UINT32 dwDebugDepth;

	CHAR szInitFuncName[PXCH_MAX_DLL_FUNC_NAME_BUFSIZE];
	CHAR szCIWCVarName[PXCH_MAX_DLL_FUNC_NAME_BUFSIZE];

	PXCH_UINT32 cbDebugOutputCharOffset;

	char chDebugOutputStepData[40];
	char chDebugOutputBuf[100];

	WCHAR szCygwin1ModuleName[PXCH_MAX_DLL_FILE_NAME_BUFSIZE];
	WCHAR szMsys2ModuleName[PXCH_MAX_DLL_FILE_NAME_BUFSIZE];
	WCHAR szHookDllModuleName[PXCH_MAX_DLL_FILE_NAME_BUFSIZE];

	PXCH_UINT32 dwLastError;
	PXCH_UINT64 qwSemaphore1;
	PXCH_UINT64 qwSemaphore2;

	PXCH_UINT32 dwZero;
	PXCH_UINT32 dwTemp;

	// pxchConfig must be the last member
	PROXYCHAINS_CONFIG pxchConfig;

} PXCH_INJECT_REMOTE_DATA;
#pragma pack(pop)

PXCH_DLL_API  extern BOOL g_bCurrentlyInWinapiCall;
PXCH_DLL_API  extern DWORD g_dwCurrentProcessIdForVerify;

