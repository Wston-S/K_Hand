#include "ResourcePool.h"

ResourcePool Resource;

extern "C"{

#define IMPORT_IMG(name) \
do{  \
    LV_IMG_DECLARE(img_src_##name)\
    Resource.Image_.AddResource(#name, (void*)&img_src_##name);\
}while (0)



static void Resource_Init()
{
    IMPORT_IMG(rb);
}
} /* extern "C" */

void ResourcePool::Init()
{
    lv_obj_remove_style_all(lv_scr_act());
    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());

    Font_.SetDefault((void*)&lv_font_montserrat_14);

    Resource_Init();
}