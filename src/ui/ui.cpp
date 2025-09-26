#include "ui.h"
#include <lvgl.h>
#include <string.h>
//#include "extra/libs/png/lv_png.h"

#include "CurvedLabel.h"
#include "Event.hpp"
#include "model/NavigationModel.hpp"

static lv_obj_t *timeLabel;

static CurvedLabel etaLabel;
static CurvedLabel edaLabel;

static lv_obj_t *directionLabel;
static lv_obj_t *directionDistanceLabel;

static lv_obj_t *connectionStateLabel;
static lv_obj_t *directionIcon;
static lv_img_dsc_t directionIconPng;
static uint8_t *iconCopy = nullptr;

static lv_style_t curvedLabelStyle;
static lv_style_t timeStyle;

static void init_styles() {

    // Current Time
    lv_style_init(&timeStyle);
    lv_style_set_text_color(&timeStyle, lv_color_hex(0x000000));
    lv_style_set_text_font(&timeStyle, &lv_font_montserrat_42);

    // Curved labels
    lv_style_init(&curvedLabelStyle);
    lv_style_set_text_color(&curvedLabelStyle, lv_color_hex(0x000000));
    lv_style_set_text_font(&curvedLabelStyle, &lv_font_montserrat_28);
}

static void updateCurrentTime(lv_event_t* event) {
    // lv_event_get_param(event);
    lv_label_set_text(timeLabel, NavigationModel::instance()->getCurrentTime().c_str());
}

static void updateDirectionDistanceLabel(lv_event_t* event) {
    lv_label_set_text(timeLabel, NavigationModel::instance()->getRemainingDistanceBeforeNextInstruction().c_str());
}

void ui_init() {
    auto navModel = NavigationModel::instance();
    // lv_scr_col lv_scr_act();

    init_styles();

    // Current Time 
    timeLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(timeLabel, navModel->getCurrentTime().c_str());
    Event::instance()->connect(timeLabel, NavigationModel::NavigationEvents::EVENT_CURRENT_TIME_UPDATED, &updateCurrentTime);
    lv_obj_align(timeLabel, LV_ALIGN_CENTER, 0, -150);
    lv_obj_add_style(timeLabel, &timeStyle, 0);

    // Estimated Time Arrival section curved label
    etaLabel = CurvedLabel(lv_scr_act(), 180, 180, 170, 220, 6, true);
    etaLabel.setText("12:34");
    etaLabel.setStyle(&curvedLabelStyle);

    // Estimated Distance Arrival section curved label
    edaLabel = CurvedLabel(lv_scr_act(), 180, 180, 170, -40, 6, true);
    edaLabel.setText("123km");
    edaLabel.setStyle(&curvedLabelStyle);

    // Direction Icon
    LV_IMAGE_DECLARE(beelight_logo_inv);
    lv_obj_t *directionIcon = lv_img_create(lv_scr_act());
    lv_img_set_src(directionIcon, &beelight_logo_inv);
    // lv_img_set_size_mode(directionIcon, LV_IMG_SIZE_MODE_REAL);
    lv_obj_set_size(directionIcon, 160, 160);
    // lv_img_set_zoom(directionIcon, 330);
    // lv_obj_align(directionIcon, LV_ALIGN_CENTER, 0, -40);

    // Direction label
    directionLabel = lv_label_create(lv_scr_act());
    lv_obj_set_width(directionLabel, 300);
    lv_label_set_long_mode(directionLabel, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(directionLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(directionLabel, "placeholder a little bit longer");
    lv_obj_set_style_text_font(directionLabel, &lv_font_montserrat_26, 0);
    lv_obj_align(directionLabel, LV_ALIGN_CENTER, 0, 70);

    // Distance next instruction
    directionDistanceLabel = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_align(directionDistanceLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(directionDistanceLabel, navModel->getRemainingDistanceBeforeNextInstruction().c_str());
    Event::instance()->connect(directionDistanceLabel, NavigationModel::NavigationEvents::EVENT_REMAINING_DISTANCE_UPDATED, &updateDirectionDistanceLabel);
    lv_obj_set_style_text_font(directionDistanceLabel, &lv_font_montserrat_30, 0);
    lv_obj_align(directionDistanceLabel, LV_ALIGN_CENTER, 0, 150);

    // Connection state label
    connectionStateLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(connectionStateLabel, "Disc.");
    lv_obj_set_style_text_align(connectionStateLabel, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(connectionStateLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(connectionStateLabel, LV_ALIGN_LEFT_MID, 10, 0);

    LV_IMAGE_DECLARE(img_jrbobdobbs);
    lv_obj_t * img;

    img = lv_image_create(lv_screen_active());
    lv_image_set_src(img, &img_jrbobdobbs);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}

void setTime(const String time) {
    lv_label_set_text(timeLabel, time.c_str());
}

void setEta(const String eta) {
    etaLabel.setText(eta.c_str());
}

void setEda(const String eda) {
    edaLabel.setText(eda.c_str());
}

void setArrivingTime(const String arrivingTime) {
    lv_label_set_text(timeLabel, arrivingTime.c_str());
}

void setDirection(const String direction) {

    lv_label_set_text(directionLabel, direction.c_str());
}

void setDirectionDistance(const String direction) {

    lv_label_set_text(directionDistanceLabel, direction.c_str());
}

// void copy_and_invert_colors_rgba(uint8_t* bufIn, uint8_t* bufOut, uint32_t px_cnt) {
//     for(uint32_t i = 0; i < px_cnt; i++) {
//         bufOut[4*i + 0] = 255 - bufIn[4*i + 0]; // R
//         bufOut[4*i + 1] = 255 - bufIn[4*i + 1]; // G
//         bufOut[4*i + 2] = 255 - bufIn[4*i + 2]; // B
//     }
// }

void setDirectionIcon(const uint8_t *iconData, size_t iconSize) {

    // Libérer l'ancien buffer si existant
    delete[] iconCopy;

    // Allouer et copier
    iconCopy = new uint8_t[iconSize];
    memcpy(iconCopy, iconData, iconSize);
    // copy_and_invert_colors_rgba((uint8_t*)iconData, iconCopy, iconSize / 4);

    // Remplir le descripteur
    //directionIconPng.header.always_zero = 0;

    directionIconPng.header.w = 0;
    directionIconPng.header.h = 0;
    directionIconPng.header.cf = LV_COLOR_FORMAT_RAW_ALPHA;
    directionIconPng.data_size = iconSize;
    directionIconPng.data = iconCopy;

    // DEBUG ----------------------------------------------------------------------
    // Serial.printf("Icon updated, size: %u bytes\n", directionIconPng.data_size);
    // for(uint16_t i = 0; i < directionIconPng.data_size; i++) {
    //     Serial.printf("%02X ", iconCopy[i]);
    // }
    // DEBUG ----------------------------------------------------------------------

    // Mise à jour dans le contexte LVGL
    lv_async_call([](void *) {
        lv_img_set_src(directionIcon, &directionIconPng);
    }, nullptr);
}

void setConnected(const bool connected) {
    lv_label_set_text(connectionStateLabel, connected ? "Conn." : "Disc.");
}

void clearData() {
    LV_IMAGE_DECLARE(beelight_logo_inv);
    etaLabel.setText("");
    edaLabel.setText("");
    lv_label_set_text(directionLabel, "");
    lv_label_set_text(directionDistanceLabel, "");
    lv_label_set_text(connectionStateLabel, "Disc.");
    lv_img_set_src(directionLabel, &beelight_logo_inv); 
}