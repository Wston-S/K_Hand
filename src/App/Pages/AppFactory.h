#ifndef APP_FACTORY_H
#define APP_FACTORY_H

#include "App/Utils/PageManager/PageManager.h"


class AppFactory : public PageFactory
{
public:
    virtual PageBase* CreatePage(const char *name);
};

#endif
