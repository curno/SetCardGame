#pragma once

#include "stdafx.h"
#include "../Utils/GlobalConfiguration.h"
#include "../Rendering/Operation.h"

// this is a instance class contains web functions used in this game.
class HttpServer
{
    struct REQUEST_CONTEXT{
        HINTERNET   hConnect;       // Connection handle
        HINTERNET   hRequest;       // Resource request handle
        DWORD       dwSize;         // Size of the latest data block
        DWORD       dwTotalSize;    // Size of the total data
        LPSTR       lpBuffer;       // Buffer for storing read data

        ~REQUEST_CONTEXT() { delete lpBuffer; }
    };
    class InternetHandle
    {
    public:
        HINTERNET Handle;
        InternetHandle()
        {
            Handle = WinHttpOpen(TEXT("SETGAME"), WINHTTP_ACCESS_TYPE_NO_PROXY,
                WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, NULL/*WINHTTP_FLAG_ASYNC*/);
        }
        ~InternetHandle()
        {
            WinHttpCloseHandle(Handle);
        }
    };
    HttpServer() : DataCallBackFunction(nullptr), ErrorCallBackFunction(nullptr) { }
    HttpServer(const HttpServer &);
    HttpServer &Operation(const HttpServer &);
public:
    // singleton.
    static HttpServer &Instance()
    {
        static HttpServer Instance_;
        return Instance_;
    }
    // post username, score and elapsed_time to server.
    void Post(const CString &name, int score, long long elapsed_time);

    typedef void (*DataCallBack)(const ::std::string &data);
    typedef void(*ErrorCallBack)();
    DataCallBack DataCallBackFunction;
    ErrorCallBack ErrorCallBackFunction;

private:
    static const ::std::wstring NameTag;
    static const ::std::wstring ScoreTag;
    static const ::std::wstring ElapsedTimeTag;

    BOOL SendRequest(REQUEST_CONTEXT *cpContext, LPCWSTR szURL, HINTERNET session);

    void Cleanup(REQUEST_CONTEXT *cpContext);

};