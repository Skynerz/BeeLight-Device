#include "UserInputScreen.hpp"
#include "ScreenNavigation.hpp"
#include <cstring>

void UserInputScreen::populate() {
    lv_obj_t* label = lv_label_create(obj());
    lv_label_set_text(label, title_m.c_str());
    lv_obj_set_style_text_font(label, &lv_font_montserrat_28, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -80);

    // scroll area
    inputArea_m = lv_label_create(obj());
    lv_obj_set_style_text_font(inputArea_m, &lv_font_montserrat_26, 0);
    lv_obj_align(inputArea_m, LV_ALIGN_CENTER, 0, 0);
    updateInputArea();

    lv_obj_t* scrollArea = lv_obj_create(obj());
    lv_obj_align(scrollArea, LV_ALIGN_CENTER, 0, 80);
    lv_obj_set_size(scrollArea, getScreenWidth() / 2, 30);
    lv_obj_set_scrollbar_mode(scrollArea, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(scrollArea, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(scrollArea, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flag(scrollArea, LV_OBJ_FLAG_SCROLL_MOMENTUM, true);

    // fill the scroll area
    for (uint8_t i = 0; i < strlen(inputs); i++) {
        lv_obj_t* inputLabel = lv_label_create(scrollArea);
        inputsLabel_m.push_back(inputLabel);
        lv_label_set_text(inputLabel, std::string(1, inputs[currentCharIndex_m + i]).c_str());
        lv_obj_align(inputLabel, LV_ALIGN_CENTER, i * kCharWidth - lv_obj_get_width(scrollArea) / 2, 0);
    }

    lv_obj_add_event_cb(scrollArea, &UserInputScreen::onScroll, LV_EVENT_SCROLL, this);
    selectCurrentChar(0);

    lv_obj_t* insertButton = lv_btn_create(obj());
    lv_obj_align_to(insertButton, scrollArea, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
    lv_obj_set_size(insertButton, 30, 30);
    lv_obj_t* insertLabel = lv_label_create(insertButton);
    lv_label_set_text(insertLabel, "+");
    lv_obj_align(insertLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(insertLabel, &lv_font_montserrat_20, 0);
    lv_obj_add_event_cb(insertButton, &UserInputScreen::onInsertCharClicked, LV_EVENT_CLICKED, this);

    lv_obj_t* removeButton = lv_btn_create(obj());
    lv_obj_align_to(removeButton, scrollArea, LV_ALIGN_OUT_LEFT_TOP, -10, 0);
    lv_obj_set_size(removeButton, 30, 30);
    lv_obj_t* removeLabel = lv_label_create(removeButton);
    lv_label_set_text(removeLabel, "-");
    lv_obj_align(removeLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(removeLabel, &lv_font_montserrat_20, 0);
    lv_obj_add_event_cb(removeButton, &UserInputScreen::onRemoveCharClicked, LV_EVENT_CLICKED, this);

    // footer
    lv_obj_t* resetButton = lv_btn_create(obj());
    lv_obj_align(resetButton, LV_ALIGN_CENTER, -30, 120);
    lv_obj_set_size(resetButton, 60, 30);
    lv_obj_t* resetLabel = lv_label_create(resetButton);
    lv_label_set_text(resetLabel, "Reset");
    lv_obj_align(resetLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(resetLabel, &lv_font_montserrat_20, 0);
    lv_obj_add_event_cb(resetButton, &UserInputScreen::onResetButtonClicked, LV_EVENT_CLICKED, this);

    lv_obj_t* submitButton = lv_btn_create(obj());
    lv_obj_align(submitButton, LV_ALIGN_CENTER, 30, 120);
    lv_obj_set_size(submitButton, 50, 30);
    lv_obj_t* submitLabel = lv_label_create(submitButton);
    lv_label_set_text(submitLabel, "OK");
    lv_obj_align(submitLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(submitLabel, &lv_font_montserrat_20, 0);
    lv_obj_add_event_cb(submitButton, &UserInputScreen::onValidButtonClicked, LV_EVENT_CLICKED, this);
}

void UserInputScreen::onTimerEvent() {
    // cursorDisplayed_m = !cursorDisplayed_m;
    // updateInputArea();
}

void UserInputScreen::onScroll(lv_event_t* event) {
    UserInputScreen* screen = static_cast<UserInputScreen*>(lv_event_get_user_data(event));
    lv_obj_t* scrollArea    = lv_event_get_current_target_obj(event);
    int32_t scrollX         = lv_obj_get_scroll_x(scrollArea);
    uint8_t charIndex       = (scrollX / kCharWidth);
    screen->selectCurrentChar(charIndex);
}

void UserInputScreen::onInsertCharClicked(lv_event_t* event) {
    UserInputScreen* screen = static_cast<UserInputScreen*>(lv_event_get_user_data(event));
    if(screen->inputText_m.length() < screen->maxInputLength_m) {
        screen->inputText_m += inputs[screen->currentCharIndex_m];
        screen->updateInputArea();
    }
}

void UserInputScreen::onRemoveCharClicked(lv_event_t* event) {
    UserInputScreen* screen = static_cast<UserInputScreen*>(lv_event_get_user_data(event));
    if(!screen->inputText_m.empty()) {
        screen->inputText_m.pop_back();
        screen->updateInputArea();
    }
}

void UserInputScreen::onResetButtonClicked(lv_event_t* event) {
    UserInputScreen* screen = static_cast<UserInputScreen*>(lv_event_get_user_data(event));
    screen->inputText_m = screen->defaultInputText_m;
    screen->updateInputArea();
}

void UserInputScreen::onValidButtonClicked(lv_event_t* event) {
    UserInputScreen* screen = static_cast<UserInputScreen*>(lv_event_get_user_data(event));
    screen->onSubmit();
    ScreenNavigation::instance()->back();
}

void UserInputScreen::selectCurrentChar(uint8_t charIndex) {
    currentCharIndex_m = charIndex;
    getLogger().debug("Char index:" + std::to_string(charIndex) + " char:" + std::string(1, inputs[charIndex]));
    for (uint8_t i = 0; i < inputsLabel_m.size(); i++) {
        if (i == charIndex) {
            lv_obj_set_style_text_font(inputsLabel_m[i], &lv_font_montserrat_28, 0);
        } else {
            lv_obj_set_style_text_font(inputsLabel_m[i], &lv_font_montserrat_20, 0);
        }
    }
}

void UserInputScreen::updateInputArea() {
    lv_label_set_text(inputArea_m, std::string(inputText_m + (cursorDisplayed_m ? "_" : " ")).c_str());
}
