#include "StartupModel.h"

using namespace Page;

void StartUpModel::Init()
{
    account = new Account("StartupModel", AccountSystem::Broker(), 0, this);
}

void StartUpModel::DeInit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}

void StartUpModel::PlayMusic(const char* music)
{
    AccountSystem::MusicPlayer_Info_t info;
    info.music = music;
    account->Notify("MusicPlayer", &info, sizeof(info));
}
