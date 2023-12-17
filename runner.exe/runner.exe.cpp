// runner.exe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS 1
#include <string>
#include <iostream>
#include <windows.h>

#define SCECLog(...) printf(__VA_ARGS__)

#define PRIVATE_API static inline

typedef const char* SCEIString;

PRIVATE_API size_t SCE_file_getsize(SCEIString path)
{
    struct stat info = { 0 };
    if (0 == stat(path, &info)) return info.st_size;
    return 0;
}

PRIVATE_API std::string SCE_get_current_processName1()
{
    char tmp1[MAX_PATH];
    auto len = GetModuleFileNameA(NULL, tmp1, _countof(tmp1));
    return std::string(tmp1, len);
}

PRIVATE_API const char* SCE_strpbrk(const char* str1, const char* str2)
{
    return strpbrk(str1, str2);
}

PRIVATE_API std::string SCE_win_get_last_error()
{
    DWORD error = GetLastError();
    if (error)
    {
        LPVOID lpMsgBuf;
        DWORD bufLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
        if (bufLen)
        {
            LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
            while ('\r' == lpMsgStr[bufLen - 1] || '\n' == lpMsgStr[bufLen - 1]) bufLen -= 1;

            std::string result(lpMsgStr, bufLen);

            LocalFree(lpMsgBuf);
            SCECLog("%s(): %s\n", __FUNCTION__, result.c_str());
            return result;
        }
    }

    SCECLog("%s(): no error.\n", __FUNCTION__);
    return std::string();
}

int main(int argc, const char **argv)
{
    if (1 == argc)
    {
        /*
        * 不带参数运行时尝试从bin尾巴读取
        * runner.exe<CMDLINE: xxx\r\n><CONFIG: xxxx>
        */
        std::string args;
        {
            auto&& self = SCE_get_current_processName1();
            auto&& sz = SCE_file_getsize(self.c_str());
            if (sz <= 8192) return 0;

            args.resize(8192);
            auto&& f1 = fopen(self.c_str(), "rb");
            fseek(f1, sz - args.size(), SEEK_SET);
            fread(&args.front(), 1, args.size(), f1);
            fclose(f1);
        }

        auto p1 = args.rfind("CONFIG: ");
        if (std::string::npos != p1)
        {
            auto && config = args.substr(p1 + 8);

            char tmp1[MAX_PATH];
            snprintf(tmp1, _countof(tmp1), "%u", GetCurrentProcessId());
            SetEnvironmentVariableA("RUNNER_PID", tmp1);
            ExpandEnvironmentStringsA(R"(%TEMP%\config.%RUNNER_PID%.conf)", tmp1, _countof(tmp1));
            auto&& f2 = fopen(tmp1, "wb");
            fwrite(config.data(), 1, config.length(), f2);
            fclose(f2);

            SetEnvironmentVariableA("PROXYCHAINS_CONF_FILE", tmp1);
        }

        p1 = args.rfind("CMDLINE: ");
        if (std::string::npos != p1)
        {
            args = args.substr(p1 + 9);
            auto tmp1a = (char*)SCE_strpbrk(args.c_str(), "\r\n");
            if (tmp1a) *tmp1a = 0;

            STARTUPINFOA si = { 0 };
            si.cb = sizeof(si);
            PROCESS_INFORMATION pi = { 0 };
            printf("[EXEC] %s\n", args.c_str());
            if (CreateProcessA(NULL, &args.front(), NULL, NULL, false, CREATE_NEW_CONSOLE|CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
            {
                printf("[EXEC] pid: %u, tid: %u\n", pi.dwProcessId, pi.dwThreadId);
                //WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                return 0;
            }

            printf("[EXEC] failed to CreateProcess(), %s\n", SCE_win_get_last_error().c_str());
            return 0;
        }
    }


    return 0;
}

