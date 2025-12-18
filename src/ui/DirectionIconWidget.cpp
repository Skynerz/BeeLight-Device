#include "DirectionIconWidget.hpp"

void DirectionIconWidget::init()
{
    icon_m = lv_image_create(lv_screen_active());
}

void DirectionIconWidget::setPosition(lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    lv_obj_align(icon_m, align, x_ofs, y_ofs);
}

void DirectionIconWidget::setScale(int32_t zoom)
{
    lv_image_set_scale(icon_m, zoom);
}

void DirectionIconWidget::setIcon(NavigationModel::InstructionIcon icon)
{
    auto it = icon_map_m.find(icon);
    if (it != icon_map_m.end())
    {
        lv_image_set_src(icon_m, it->second);
    }
    else
    {
        LV_IMAGE_DECLARE(img_jrbobdobbs);
        lv_image_set_src(icon_m, &img_jrbobdobbs);
    }
}
