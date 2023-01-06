#ifndef __STARTUPBAR_H
#define __STARTUPBAR_H

#include "../Page.h"

namespace Page
{

class StartUpView
{
public:
    void Create(lv_obj_t *root);
    void Delete();

public:
    struct
    {
        lv_obj_t* cont;
        lv_obj_t* labelLogo;

        lv_anim_timeline_t* anim_timeline;
    } ui;
};

}

#endif
