#include "Include/stdafx.h"

#include "Include/Web/HttpServer.h"

const ::std::wstring HttpServer::NameTag = TEXT("name");
const ::std::wstring HttpServer::ScoreTag = TEXT("score");
const ::std::wstring HttpServer::ElapsedTimeTag = TEXT("elapsed_time");
HttpServer::DataCallBack HttpServer::DataCallBackFunction = nullptr;
HttpServer::ErrorCallBack HttpServer::ErrorCallBackFunction = nullptr;