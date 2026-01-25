#include "SplashScreen.hpp"

#include "ScreenNavigation.hpp"
#include "ui/Dashboard.hpp"

void SplashScreen::populate() {
    top_label_m = lv_label_create(obj());
    lv_obj_set_width(top_label_m, getScreenWidth());
    lv_obj_align(top_label_m, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_style_text_align(top_label_m, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(top_label_m, &lv_font_montserrat_26, 0);
    lv_label_set_text(top_label_m, "Beelight");

    icon_m.setPosition(LV_ALIGN_CENTER, 0, 0);
    icon_m.setScale(512);
    icon_m.setIcon(InstructionIcon::Values::UNKNOWN);

    status_label_m = lv_label_create(obj());
    lv_obj_set_width(status_label_m, getScreenWidth());
    lv_obj_align(status_label_m, LV_ALIGN_BOTTOM_MID, 0, -40);
    lv_obj_set_style_text_align(status_label_m, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(status_label_m, &lv_font_montserrat_26, 0);
    lv_label_set_text(status_label_m, "Loading...");
}

void SplashScreen::onTimerEvent() {
    tick_m++;
    if (tick_m > 2) {  // TBD
        ScreenNavigation::instance()->navigateTo<Dashboard>(NavigationTransition::TO_LEFT, true);
    }
}
