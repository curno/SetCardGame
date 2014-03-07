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
    // singleton
    static SoundPlayer &Instance()
    {
        static SoundPlayer SoundPlayer_;
        return SoundPlayer_;
    }

    void Play(int id)
    {
        ::PlaySound(MAKEINTRESOURCE(id), AfxGetApp()->m_hInstance, SND_RESOURCE | SND_ASYNC);
    }
};