#pragma once

#include "stdafx.h"
#include "../Utils/GlobalConfiguration.h"
#include "../Rendering/Operation.h"

// this is a static class contains web functions used in this game.
class HttpServer
{
    struct REQUEST_CONTEXT{
        HINTERNET   hConnect;       // Connection handle
        HINTERNET   hRequest;       // Resource request handle
        DWORD       dwSize;         // Size of the latest data block
        DWORD       dwTotalSize;    // Size of the total data
        LPSTR       lpBuffer;       // Buffer for storing read data
        WCHAR       szMemo[256];    // String providing state information

    };
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

        REQUEST_CONTEXT *context = new REQUEST_CONTEXT;
        ZeroMemory(context, sizeof(REQUEST_CONTEXT));

        SendRequest(context, woss.str().c_str(), Handle_.Handle);
    }

    typedef void (*DataCallBack)(const ::std::string &data);
    typedef void(*ErrorCallBack)();
    static DataCallBack DataCallBackFunction;
    static ErrorCallBack ErrorCallBackFunction;

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

        WinHttpSetStatusCallback(request, HttpServer::CallBackFunction, WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS |
            WINHTTP_CALLBACK_FLAG_REDIRECT, NULL);

        // Send the request.
        WinHttpSendRequest(request,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0, 0,
            NULL);

        return TRUE;
    }

    static BOOL SendRequest(REQUEST_CONTEXT *cpContext, LPCWSTR szURL, HINTERNET session)
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
        if (!WinHttpCrackUrl(szURL, 0, 0, &urlComp))
        {
            goto cleanup;
        }

        // Open an HTTP session.
        cpContext->hConnect = WinHttpConnect(session, szHost,
            urlComp.nPort, 0);
        if (NULL == cpContext->hConnect)
        {
            goto cleanup;
        }

        // Prepare OpenRequest flag
        dwOpenRequestFlag = (INTERNET_SCHEME_HTTPS == urlComp.nScheme) ?
        WINHTTP_FLAG_SECURE : 0;

        // Open a "GET" request.
        cpContext->hRequest = WinHttpOpenRequest(cpContext->hConnect,
            L"GET", urlComp.lpszUrlPath,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            dwOpenRequestFlag);

        if (cpContext->hRequest == 0)
        {
            goto cleanup;
        }

        // Install the status callback function.
        WINHTTP_STATUS_CALLBACK pCallback = WinHttpSetStatusCallback(cpContext->hRequest,
            (WINHTTP_STATUS_CALLBACK)HttpServer::CallBackFunction,
            WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS |
            WINHTTP_CALLBACK_FLAG_REDIRECT,
            NULL);

        // note: On success WinHttpSetStatusCallback returns the previously defined callback function.
        // Here it should be NULL
        if (pCallback != NULL)
        {
            goto cleanup;
        }

        // Send the request.
        if (!WinHttpSendRequest(cpContext->hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
            WINHTTP_NO_REQUEST_DATA, 0, 0,
            (DWORD_PTR)cpContext))
        {
            goto cleanup;
        }

        fRet = TRUE;

    cleanup:

        if (fRet == FALSE)
        {
            // Cleanup handles.
            Cleanup(cpContext);
        }

        return fRet;
    }

    static VOID CALLBACK
        CallBackFunction(
        IN HINTERNET hInternet,
        IN DWORD_PTR dwContext,
        IN DWORD dwInternetStatus,
        IN LPVOID lpvStatusInformation OPTIONAL,
        IN DWORD dwStatusInformationLength
        )
    {
        REQUEST_CONTEXT *cpContext = static_cast<REQUEST_CONTEXT *>((void *)(dwContext));
        switch (dwInternetStatus)
        {
        case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
                
            // Prepare the request handle to receive a response.
            if (WinHttpReceiveResponse(cpContext->hRequest, NULL) == FALSE)
                Cleanup(cpContext);
            break;

        case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
            Header(cpContext);

            // Initialize the buffer sizes.
            cpContext->dwSize = 0;
            cpContext->dwTotalSize = 0;

            // Begin downloading the resource.
            if (QueryData(cpContext) == FALSE)
                Cleanup(cpContext);

        case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
            if (lpvStatusInformation == nullptr)
                break;
            cpContext->dwSize = *((LPDWORD)lpvStatusInformation);
            if (ReadData(cpContext) == FALSE)
                    Cleanup(cpContext);
            break;
        case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
            if (dwStatusInformationLength != 0)
            {
                if (DataCallBackFunction != nullptr)
                    DataCallBackFunction(std::string(cpContext->lpBuffer));
                // Check for more data.
                if (QueryData(cpContext) == FALSE)
                {
                    Cleanup(cpContext);
                }
            }
            break;
        case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
            if (ErrorCallBackFunction != nullptr)
                ErrorCallBackFunction();
            Cleanup(cpContext);
            break;
        }

    }

    static BOOL Header(REQUEST_CONTEXT *cpContext)
    {
        DWORD dwSize = 0;
        LPVOID lpOutBuffer = NULL;

        // Use HttpQueryInfo to obtain the size of the buffer.
        if (!WinHttpQueryHeaders(cpContext->hRequest,
            WINHTTP_QUERY_RAW_HEADERS_CRLF,
            WINHTTP_HEADER_NAME_BY_INDEX, NULL, &dwSize, WINHTTP_NO_HEADER_INDEX))
        {
            DWORD dwErr = GetLastError();
            if (dwErr != ERROR_INSUFFICIENT_BUFFER)
            {
                return FALSE;
            }
        }

        // Allocate memory for the buffer.
        lpOutBuffer = new WCHAR[dwSize];

        // Use HttpQueryInfo to obtain the header buffer.
        WinHttpQueryHeaders(cpContext->hRequest,
            WINHTTP_QUERY_RAW_HEADERS_CRLF,
            WINHTTP_HEADER_NAME_BY_INDEX, lpOutBuffer, &dwSize, WINHTTP_NO_HEADER_INDEX);

        // Free the allocated memory.
        delete[] lpOutBuffer;

        return TRUE;
    }

    static void Cleanup(REQUEST_CONTEXT *cpContext)
    {
        if (cpContext->hRequest)
        {
            WinHttpSetStatusCallback(cpContext->hRequest,
                NULL,
                NULL,
                NULL);

            WinHttpCloseHandle(cpContext->hRequest);
            cpContext->hRequest = NULL;
        }

        if (cpContext->hConnect)
        {
            WinHttpCloseHandle(cpContext->hConnect);
            cpContext->hConnect = NULL;
        }

        delete[] cpContext->lpBuffer;
        cpContext->lpBuffer = NULL;

        delete cpContext;
    }

    static BOOL QueryData(REQUEST_CONTEXT *cpContext)
    {
        // Chech for available data.
        if (WinHttpQueryDataAvailable(cpContext->hRequest, NULL) == FALSE)
            return FALSE;
        return TRUE;
    }

    static BOOL ReadData(REQUEST_CONTEXT *cpContext)
    {
        LPSTR buffer = cpContext->lpBuffer;
        cpContext->lpBuffer = new char[cpContext->dwSize + 1 + cpContext->dwTotalSize];

        
        if (buffer != nullptr)
        {
            CopyMemory(cpContext->lpBuffer, buffer, cpContext->dwTotalSize);
            delete[] buffer;
        }

        ZeroMemory(cpContext->lpBuffer + cpContext->dwTotalSize, cpContext->dwSize + 1);

        // Read the available data.
        if (WinHttpReadData(cpContext->hRequest, (LPVOID)(cpContext->lpBuffer + cpContext->dwTotalSize),
            cpContext->dwSize, NULL) == FALSE)
            return FALSE;

        cpContext->dwTotalSize += cpContext->dwSize + 1;

        return TRUE;
    }
};