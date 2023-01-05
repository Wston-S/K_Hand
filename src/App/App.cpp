#include "App.h"
#include "HAL/HAL.h"
#include "Pages/AppFactory.h"
#include "Accounts/Account_Master.h"
#include "Utils/PageManager/PageManager.h"
#include "Resources/ResourcePool.h"
#include "Pages/StatusBar/StatusBar.h"

void App_Init()
{
    static AppFactory factory;
    static PageManager manager(&factory);

    Accounts_Init();
    Resource.Init();

    /*----------------------- Pages Init -----------------------*/
    //StatusBar::Init(lv_layer_top());

    //manager.Install("Startup", "Pages/Startup");

    //manager.SetGlobalLoadAnimType(PageManager::LOAD_ANIM_OVER_TOP, 500);
    //manager.Push("Pages/Startup");

    //ACCOUNT_SEND_NOTIFY_CMD(Storage, STORAGE_CMD_LOAD);
    //ACCOUNT_SEND_NOTIFY_CMD(SysConfig, SYSCONFIG_CMD_LOAD);

    //INIT_DONE();
}

void App_UnInit()
{
    // ACCOUNT_SEND_NOTIFY_CMD(SysConfig, SYSCONFIG_CMD_SAVE);
    // ACCOUNT_SEND_NOTIFY_CMD(Storage, STORAGE_CMD_SAVE);
    // ACCOUNT_SEND_NOTIFY_CMD(Recorder, RECORDER_CMD_STOP);
}