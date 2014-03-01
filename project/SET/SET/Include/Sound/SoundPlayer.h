#pragma once

#include "stdafx.h"

// this class plays sound effect for the game.
// it is a singleton.
class SoundPlayer
{
private:
    SoundPlayer() { } 
    SoundPlayer(const SoundPlayer &);
    SoundPlayer &operator= (const SoundPlayer &);
public:
    static SoundPlayer &Instance()
    {
        static SoundPlayer SoundPlayer_;
        return SoundPlayer_;
    }

    void Play(int id)
    {
        ::PlaySound(MAKEINTRESOURCE(id), AfxGetApp()->m_hInstance, SND_RESOURCE | SND_ASYNC);
        /*HRSRC rs = FindResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(id), TEXT("WAVE"));
        DWORD size = SizeofResource(AfxGetApp()->m_hInstance, rs);
        byte *data = new byte[size];
        HGLOBAL WINAPI LoadResource(
            _In_opt_  HMODULE hModule,
            _In_      HRSRC hResInfo
            );
        HGLOBAL h = LoadResource(AfxGetApp()->m_hInstance, rs);
        memcpy(data, h, sizeof(byte)* size);

        ::PlaySound((LPCTSTR)data, AfxGetApp()->m_hInstance, SND_MEMORY | SND_ASYNC | SND_NOSTOP);
        ::PlaySound((LPCTSTR)data, AfxGetApp()->m_hInstance, SND_MEMORY | SND_ASYNC | SND_NOSTOP);*/
    }
};