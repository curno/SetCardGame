#pragma once

#include "stdafx.h"
#include "../Utils/GlobalConfiguration.h"

// this is a static class contains web functions used in this game.
class HttpServer
{
    class InternetHandle
    {
    public:
        HINTERNET Handle;
        InternetHandle()
        {
            Handle = WinHttpOpen(TEXT("SETGAME"), WINHTTP_ACCESS_TYPE_NO_PROXY,
                WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, WINHTTP_FLAG_ASYNC);
        }
        ~InternetHandle()
        {
            WinHttpCloseHandle(Handle);
        }
    };
public:
    
    static void Post(const CString &name, int score, long long elapsed_time)
    {
        static InternetHandle Handle_;
        CString tmp = name;
        ::std::wstring name_wstring = tmp.GetBuffer(0);
        ::std::wostringstream woss;
        woss << GlobalConfiguration::Instance().HostName << TEXT("?") 
            << Name << TEXT("=") << name_wstring
            << TEXT("&") << Score << TEXT("=") << score 
            << TEXT("&") << ElapsedTime << TEXT("=") << elapsed_time;
        SendRequest(woss.str().c_str(), Handle_.Handle);
    }

private:
    static const ::std::wstring Name;
    static const ::std::wstring Score;
    static const ::std::wstring ElapsedTime;

    static BOOL SendRequest(LPCWSTR szURL, HINTERNET session)
    {
        WCHAR szHost[256];
        DWORD dwOpenRequestFlag = 0;
        URL_COMPONENTS urlComp;
        BOOL fRet = FALSE;

        // Initialize URL_COMPONENTS structure.
        ZeroMemory(&urlComp, sizeof(urlComp));
        urlComp.dwStructSize = sizeof(urlComp);

        // Use allocated buffer to store the Host Name.
        urlComp.lpszHostName = szHost;
        urlComp.dwHostNameLength = sizeof(szHost) / sizeof(szHost[0]);

        // Set non-zero lengths to obtain pointer a to the URL Path.
        // NOTE: If we threat this pointer as a null-terminated string,
        //       it gives us access to additional information as well. 
        urlComp.dwUrlPathLength = (DWORD)-1;

        // Crack HTTP scheme.
        urlComp.dwSchemeLength = (DWORD)-1;

        // Crack the URL.
        WinHttpCrackUrl(szURL, 0, 0, &urlComp);
        

        // Open an HTTP session.
        HINTERNET connection = WinHttpConnect(session, szHost,
            urlComp.nPort, 0);
        
        // Prepare OpenRequest flag
        dwOpenRequestFlag = (INTERNET_SCHEME_HTTPS == urlComp.nScheme) ?
        WINHTTP_FLAG_SECURE : 0;

       
        // Open a "GET" request.
        HINTERNET request = WinHttpOpenRequest(connection,
            TEXT("GET"), urlComp.lpszUrlPath,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            dwOpenRequestFlag);

             
        // Send the request.
        WinHttpSendRequest(request,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0, 0,
            NULL);

        return TRUE;
    }

};