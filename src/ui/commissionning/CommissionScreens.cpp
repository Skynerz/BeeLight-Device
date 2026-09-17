
#include "CommissionScreens.hpp"

#include "BeelightApp.hpp"
#include "model/PersistencyModel.hpp"
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
    lv_obj_add_event_cb(nextButton, &CommissionStartScreen::onNextButtonClicked, LV_EVENT_CLICKED, this);
}

void CommissionStartScreen::onNextButtonClicked(lv_event_t* event) {
    CommissionStartScreen* screen = static_cast<CommissionStartScreen*>(lv_event_get_user_data(event));
    CommissionDeviceNameConfigScreen::Context context;
    context.title       = "BLE device name";
    context.initialText = "Beelight";
    context.onSubmit    = [screen](const std::string& input) {
        screen->getLogger().info("Device name set to " + input);
        PersistencyModel::instance()->setDeviceName(input);
        ScreenNavigation::instance()->navigateTo<CommissionConnWaitingScreen>(NavigationTransition::TO_LEFT, true);
    };
    ScreenNavigation::instance()->navigateTo<CommissionDeviceNameConfigScreen>(NavigationTransition::TO_LEFT, true,
                                                                               &context);
}

void CommissionConnWaitingScreen::populate() {
    lv_obj_t* title    = lv_label_create(obj());
    lv_label_set_text(title, "Waiting for BLE connection...");
    lv_obj_set_width(title, getScreenWidth() - 40);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -60);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_26, 0);
    
    Event::instance()->connect(obj(), BleEvents::EVENT_BLE_CONNECTED, [](lv_event_t*) {
        ScreenNavigation::instance()->navigateTo<CommissionFinishScreen>(NavigationTransition::TO_LEFT, true);
    });
    displayPinCode();
}

void CommissionConnWaitingScreen::onTimerEvent() {
    const auto com = BeelightApp::getComInstance();
    if ((getTick() > 0) && !is_advertising_started_m) {
        is_advertising_started_m = true;
        com->start_advertising();
    }
}

void CommissionConnWaitingScreen::displayPinCode() {
    const auto com = BeelightApp::getComInstance();
    uint32_t pin;
    if (com->get_pin(pin)) {
        std::string pinStr = std::to_string(pin);

        lv_obj_t* text = lv_label_create(obj());
        lv_label_set_text(text, "Please connect using this pin code:");
        lv_obj_set_width(text, getScreenWidth() - 40);
        lv_obj_align(text, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(text, &lv_font_montserrat_26, 0);

        lv_obj_t* pinCode = lv_label_create(obj());
        lv_label_set_text(pinCode, pinStr.c_str());
        lv_obj_set_width(pinCode, getScreenWidth() - 40);
        lv_obj_align(pinCode, LV_ALIGN_CENTER, 0, 80);
        lv_obj_set_style_text_align(pinCode, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(pinCode, &lv_font_montserrat_30, 0);
    }
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
                ScreenNavigation::instance()->navigateTo<Dashboard>(NavigationTransition::TO_LEFT, true);
            }
        },
        LV_EVENT_CLICKED, nullptr);

    PersistencyModel* persistency = PersistencyModel::instance();
    persistency->setCommissioningStatus(true);
}
