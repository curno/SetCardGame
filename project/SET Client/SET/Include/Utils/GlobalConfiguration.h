#pragma once

#include "stdafx.h"

// singleton class for global configurations of the game.
class GlobalConfiguration
{
private:
    GlobalConfiguration() { Init(); }
    GlobalConfiguration(const GlobalConfiguration &);
    GlobalConfiguration &operator=(const GlobalConfiguration &);

public:
    static GlobalConfiguration &Instance()
    {
        static GlobalConfiguration Instance_;
        return Instance_;
    }

    int WindowMinimumWidth;
    int WindowMinimumHeight;
    ::std::wstring HostName;
    bool MultiSample;

private:
    void Init()
    {
        WindowMinimumHeight = 600;
        WindowMinimumWidth = 900;
        HostName = TEXT("http://localhsfdost:80/python/submit.py");
        MultiSample = true;

        WIN32_FIND_DATA FindFileData;
        HANDLE handle = FindFirstFile(TEXT("conf.ini"), &FindFileData);
        int found = handle != INVALID_HANDLE_VALUE;
        if (found)
        {
            ::std::wifstream wif("conf.ini");
            while (!wif.eof())
            {
                ::std::wstring key;
                wif >> key; 
                #define LoadValue(Name) if (key == TEXT(#Name)) wif >> Name;
                LoadValue(WindowMinimumHeight)
                LoadValue(WindowMinimumWidth)
                LoadValue(HostName)
                LoadValue(MultiSample)
                #undef LoadValue
            }
        }
    }
};