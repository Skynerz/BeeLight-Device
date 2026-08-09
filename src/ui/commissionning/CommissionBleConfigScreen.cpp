#include "ui/commissionning/CommissionBleConfigScreen.hpp"
#include "ui/ScreenNavigation.hpp"
#include "model/PersistencyModel.hpp"
#include <string.h>

void CommissionBleConfigScreen::populate() {
    lv_obj_t* label = lv_label_create(obj());
    lv_label_set_text(label, "Set your BLE device name");
    lv_obj_set_width(label, getScreenWidth() - 40);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, 0);

    lv_obj_t* textArea = lv_textarea_create(obj());
    lv_obj_align(textArea, LV_ALIGN_CENTER, 0, 20);
    lv_obj_set_width(textArea, 200);
    lv_textarea_set_one_line(textArea, true);
    lv_textarea_set_text(textArea, PersistencyModel::instance()->getDeviceName().c_str());

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
                lv_obj_t* textArea = (lv_obj_t*) lv_event_get_user_data(event);
                const char* deviceName = lv_textarea_get_text(textArea);
                if (deviceName != nullptr && strlen(deviceName) > 0) {
                    PersistencyModel::instance()->setDeviceName(std::string(deviceName));
                    ScreenNavigation::instance()->navigateTo<CommissionFinishScreen>(NavigationTransition::TO_LEFT,
                                                                                    true);
                } else {
                    // Show an error message or keep the user on the same screen
                    // For simplicity, we'll just print a warning in the logs
                    BeeLog("CommissionBleConfigScreen").warn("Device name cannot be empty");
                }
            }
        }, LV_EVENT_ALL, textArea);

    lv_obj_t* keyboard = lv_keyboard_create(obj());
    lv_keyboard_set_map(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_obj_align(keyboard, LV_ALIGN_CENTER, 0, 120);
    lv_keyboard_set_textarea(keyboard, textArea);
}
