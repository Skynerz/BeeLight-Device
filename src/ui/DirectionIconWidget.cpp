#include "DirectionIconWidget.hpp"

void DirectionIconWidget::init()
{
    icon_m = lv_image_create(lv_screen_active());
}

void DirectionIconWidget::setPosition(lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    lv_obj_align(icon_m, LV_ALIGN_CENTER, 0, 0);
}

void DirectionIconWidget::setSize(int32_t w, int32_t h)
{
    lv_image_set_scale_x(icon_m, w);
    lv_image_set_scale_y(icon_m, h);
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
