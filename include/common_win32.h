#pragma once

#include "defines_win32.h"
#include "common_generic.h"

#define MAX_ERROR_MESSAGE_BUFSIZE 256
#define MAX_FWPRINTF_BUFSIZE 256

extern wchar_t szErrorMessage[MAX_ERROR_MESSAGE_BUFSIZE];

PWCHAR FormatErrorToStr(DWORD dwError);
VOID StdWprintf(DWORD dwStdHandle, const WCHAR* fmt, ...);
VOID StdVwprintf(DWORD dwStdHandle, const WCHAR* fmt, va_list args);
VOID StdFlush(DWORD dwStdHandle);