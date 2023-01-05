#include "Account_Master.h"
#include "App/Utils/Filters/Filters.h"
#include "HAL/HAL.h"


static void onTimer(Account* account)
{
    static int cnt = 0;
    AccountSystem::MusicPlayer_Info_t info;

    switch (cnt)
    {
    case 0:        
        info.music = "Startup";
        break;
    
    case 1:        
        info.music = "Shutdown";
        break;

    case 2:        
        info.music = "Error";
        break;

    case 3:        
        info.music = "Connect";
        break;

    case 4:        
        info.music = "Disconnect";
        break;

    case 5:        
        info.music = "UnstableConnect";
        break;

    case 6:        
        info.music = "BattChargeStart";
        break;

    case 7:        
        info.music = "BattChargeEnd";
        break;
    
    case 8:        
        info.music = "DeviceInsert";
        break;

    case 9:        
        info.music = "DevicePullout";
        break;

    case 10:        
        info.music = "NoOperationWarning";
        break;
    
    default:
        break;
    }

    account->Notify("MusicPlayer", &info, sizeof(info));

    cnt++;
    if(cnt >= 11) cnt = 0;
}

static int onEvent(Account* account, Account::EventParam_t* param)
{
    if (param->event == Account::EVENT_TIMER)
    {
        //onTimer(account);
        return 0;
    }

    return 0;
}

ACCOUNT_INIT_DEF(Acttest)
{
    account->Subscribe("MusicPlayer");
    account->SetEventCallback(onEvent);
    account->SetTimerPeriod(2000);
}
