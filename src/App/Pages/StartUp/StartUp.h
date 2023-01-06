#ifndef __STARTUP_H
#define __STARTUP_H

#include "StartUpView.h"
#include "StartUPModel.h"

namespace Page
{
    
class Startup : PageBase
{
public:
    Startup();
    virtual ~Startup();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewDidUnload();

private:
    static void onTimer(lv_timer_t* timer);

    StartUpView View;
    StartUpModel Model;
};

} // namespace Page



#endif

