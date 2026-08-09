
#include "CommissionScreens.hpp"
#include "ui/Dashboard.hpp"
#include "ui/ScreenNavigation.hpp"

void CommissionStartScreen::populate() {
    lv_obj_t* label = lv_label_create(obj());
    lv_label_set_text(label, "Welcome to Beelight!\nLet's get started with the configuration of the device.");
    lv_obj_set_width(label, getScreenWidth() - 40);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, 0);

    lv_obj_t* nextButton = lv_btn_create(obj());
    lv_obj_align(nextButton, LV_ALIGN_CENTER, 0, 80);
    lv_obj_set_size(nextButton, 120, 50);
    
    lv_obj_t* nextLabel = lv_label_create(nextButton);
    lv_label_set_text(nextLabel, "Next");
    lv_obj_center(nextLabel);
    lv_obj_add_event_cb(
        nextButton,
        [](lv_event_t* event) {
            if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
                ScreenNavigation::instance()->navigateTo<CommissionBleConfigScreen>(NavigationTransition::TO_LEFT,
                                                                                    true);
            }
        },
        LV_EVENT_ALL, nullptr);
}

void CommissionFinishScreen::populate() {
    lv_obj_t* label = lv_label_create(obj());
    lv_label_set_text(label, "Congratulations!\nYour device is now configured and ready to use.");
    lv_obj_set_width(label, getScreenWidth() - 40);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, 0);

    lv_obj_t* nextButton = lv_btn_create(obj());
    lv_obj_align(nextButton, LV_ALIGN_CENTER, 0, 80);
    lv_obj_set_size(nextButton, 120, 50);

    lv_obj_t* nextLabel = lv_label_create(nextButton);
    lv_label_set_text(nextLabel, "Next");
    lv_obj_center(nextLabel);
    lv_obj_add_event_cb(
        nextButton,
        [](lv_event_t* event) {
            if (lv_event_get_code(event) == LV_EVENT_CLICKED) {
                ScreenNavigation::instance()->navigateTo<Dashboard>(NavigationTransition::TO_LEFT,
                                                                                    true);
            }
        },
        LV_EVENT_ALL, nullptr);
}
