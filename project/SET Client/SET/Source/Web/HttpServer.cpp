#include "Include/stdafx.h"

#include "Include/Web/HttpServer.h"

const ::std::wstring HttpServer::NameTag = TEXT("name");
const ::std::wstring HttpServer::ScoreTag = TEXT("score");
const ::std::wstring HttpServer::ElapsedTimeTag = TEXT("elapsed_time");

void HttpServer::Cleanup(REQUEST_CONTEXT *cpContext)
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
}

BOOL HttpServer::SendRequest(REQUEST_CONTEXT *cpContext, LPCWSTR szURL, HINTERNET session)
{
    ::std::string data;
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

    // Send the request.
    if (!WinHttpSendRequest(cpContext->hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0,
        (DWORD_PTR)cpContext))
    {
        goto cleanup;
    }
    WinHttpReceiveResponse(cpContext->hRequest, NULL);
    DWORD size;
    while (WinHttpQueryDataAvailable(cpContext->hRequest, &size))
    {
        if (size == 0)
            break;
        DWORD size2;
        char *buffer = new char[size + 1];
        ZeroMemory(buffer, size + 1);
        WinHttpReadData(cpContext->hRequest, buffer, size, &size2);
        data += buffer;
        delete[] buffer;

    }
    if (data.length() > 0)
    {
        if (DataCallBackFunction != nullptr)
            DataCallBackFunction(data);
    }
    fRet = TRUE;

cleanup:
    if (!fRet)
    {
        if (ErrorCallBackFunction != nullptr)
            ErrorCallBackFunction();
    }
    Cleanup(cpContext);

    return fRet;
}

void HttpServer::Post(const CString &name, int score, long long elapsed_time)
{
    static InternetHandle Handle_;
    CString tmp = name;
    ::std::wstring name_wstring = tmp.GetBuffer(0);
    ::std::wostringstream woss;
    woss << GlobalConfiguration::Instance().HostName << TEXT("?")
        << NameTag << TEXT("=") << name_wstring
        << TEXT("&") << ScoreTag << TEXT("=") << score
        << TEXT("&") << ElapsedTimeTag << TEXT("=") << elapsed_time;

    REQUEST_CONTEXT context;
    ZeroMemory(&context, sizeof(REQUEST_CONTEXT));

    SendRequest(&context, woss.str().c_str(), Handle_.Handle);
}
