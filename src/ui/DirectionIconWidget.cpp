#include "DirectionIconWidget.hpp"

#include "BeeLog.hpp"

DirectionIconWidget::DirectionIconWidget(lv_obj_t* parent) {
    icon_m = lv_image_create(parent);
}

void DirectionIconWidget::setPosition(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
    lv_obj_align(icon_m, align, x_ofs, y_ofs);
}

void DirectionIconWidget::setScale(int32_t zoom) {
    lv_image_set_scale(icon_m, zoom);
}

void DirectionIconWidget::setIcon(InstructionIcon::Values icon) {
    auto it = icon_map_m.find(icon);
    if (it != icon_map_m.end()) {
        BeeLog::debug("DirectionIconWidget", "new icon " + InstructionIcon::toString(icon));
        lv_image_set_src(icon_m, it->second);
    } else {
        LV_IMAGE_DECLARE(img_jrbobdobbs);
        lv_image_set_src(icon_m, &img_jrbobdobbs);
    }
}
