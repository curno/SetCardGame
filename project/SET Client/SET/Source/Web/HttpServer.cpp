#include "Include/stdafx.h"

#include "Include/Web/HttpServer.h"

const ::std::wstring HttpServer::Name = TEXT("name");
const ::std::wstring HttpServer::Score = TEXT("score");
const ::std::wstring HttpServer::ElapsedTime = TEXT("elapsed_time");
HttpServer::DataCallBack HttpServer::DataCallBackFunction = nullptr;