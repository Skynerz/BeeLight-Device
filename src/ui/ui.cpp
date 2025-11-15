#include "ui.h"
#include <lvgl.h>
#include <string.h>

#include "Event.hpp"
#include "model/NavigationModel.hpp"
#include "DirectionIconWidget.hpp"
#include "port.hpp"

#ifdef SIMULATOR
static lv_obj_t *screenBg;
#endif
static lv_obj_t *timeLabel;

static lv_obj_t *etaLabel;
static lv_obj_t *edaLabel;

static lv_obj_t *directionLabel;
static lv_obj_t *directionDistanceLabel;

static lv_obj_t *connectionStateLabel;
static DirectionIconWidget directionIcon;
static uint8_t *iconCopy = nullptr;

static lv_style_t curvedLabelStyle;
static lv_style_t timeStyle;

static void init_styles()
{

    // Current Time
    lv_style_init(&timeStyle);
    lv_style_set_text_color(&timeStyle, lv_color_hex(0x000000));
    lv_style_set_text_font(&timeStyle, &lv_font_montserrat_42);

    // Curved labels
    lv_style_init(&curvedLabelStyle);
    lv_style_set_text_color(&curvedLabelStyle, lv_color_hex(0x000000));
    lv_style_set_text_font(&curvedLabelStyle, &lv_font_montserrat_28);
}

static void updateCurrentTime(lv_event_t *event = nullptr)
{
    // lv_event_get_param(event);
    lv_label_set_text(timeLabel, NavigationModel::instance()->getCurrentTime().c_str());
}

static void updateEta(lv_event_t *event = nullptr)
{
    lv_arclabel_set_text(etaLabel, NavigationModel::instance()->getEstTimeBeforeArrival().c_str());
}

static void updateEda(lv_event_t *event = nullptr)
{
    lv_arclabel_set_text(edaLabel, NavigationModel::instance()->getEstDistanceBeforeArrival().c_str());
}

static void updateDirection(lv_event_t *event = nullptr)
{
    lv_label_set_text(directionLabel, NavigationModel::instance()->getNextInstruction().c_str());
}

static void updateDirectionDistanceLabel(lv_event_t *event = nullptr)
{
    lv_label_set_text(directionDistanceLabel, NavigationModel::instance()->getRemainingDistanceBeforeNextInstruction().c_str());
}

static void updateNextInstructionIcon(lv_event_t *event = nullptr)
{
    directionIcon.setIcon(NavigationModel::instance()->getNextInstructionIcon());
}

void ui_init()
{
    auto navModel = NavigationModel::instance();
    // lv_scr_col lv_scr_act();

    init_styles();
#ifdef SIMULATOR
    screenBg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screenBg, getScreenWidth(), getScreenHeight());
    lv_obj_set_style_bg_color(screenBg, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(screenBg, getScreenWidth() / 2, 0);
#endif
    // Current Time
    timeLabel = lv_label_create(lv_screen_active());
    updateCurrentTime();
    Event::instance()->connect(timeLabel, NavigationModel::NavigationEvents::EVENT_CURRENT_TIME_UPDATED, &updateCurrentTime);
    lv_obj_align(timeLabel, LV_ALIGN_CENTER, 0, -150);
    lv_obj_add_style(timeLabel, &timeStyle, 0);

    // Estimated Time Arrival section curved label
    etaLabel = lv_arclabel_create(lv_scr_act());
    lv_obj_set_size(etaLabel, getScreenWidth(), getScreenHeight());
    lv_obj_set_style_text_letter_space(etaLabel, 0, LV_PART_MAIN);
    lv_obj_add_style(etaLabel, &curvedLabelStyle, LV_PART_MAIN);
    lv_arclabel_set_angle_start(etaLabel, 40); // when center => - 180 ????
    lv_arclabel_set_radius(etaLabel, LV_PCT(80));
    lv_arclabel_set_recolor(etaLabel, true);
    lv_arclabel_set_text_vertical_align(etaLabel, LV_ARCLABEL_TEXT_ALIGN_CENTER);
    lv_arclabel_set_dir(etaLabel, LV_ARCLABEL_DIR_CLOCKWISE);
    lv_arclabel_set_text_horizontal_align(etaLabel, LV_ARCLABEL_TEXT_ALIGN_CENTER);
    lv_obj_center(etaLabel);
    updateEta();
    Event::instance()->connect(etaLabel, NavigationModel::NavigationEvents::EVENT_EST_TIME_ARRIVAL_UPDATED, &updateEta);

    // Estimated Distance Arrival section curved label
    edaLabel = lv_arclabel_create(lv_scr_act());
    lv_obj_set_size(edaLabel, getScreenWidth(), getScreenHeight());
    lv_obj_set_style_text_letter_space(edaLabel, 0, LV_PART_MAIN);
    lv_obj_add_style(edaLabel, &curvedLabelStyle, LV_PART_MAIN);
    lv_arclabel_set_angle_start(edaLabel, 140);
    lv_arclabel_set_radius(edaLabel, LV_PCT(80));
    lv_arclabel_set_recolor(edaLabel, true);
    lv_arclabel_set_text_vertical_align(edaLabel, LV_ARCLABEL_TEXT_ALIGN_CENTER);
    lv_arclabel_set_dir(edaLabel, LV_ARCLABEL_DIR_CLOCKWISE);
    lv_arclabel_set_text_horizontal_align(edaLabel, LV_ARCLABEL_TEXT_ALIGN_CENTER);
    lv_obj_center(edaLabel);
    updateEda();
    Event::instance()->connect(edaLabel, NavigationModel::NavigationEvents::EVENT_EST_DISTANCE_ARRIVAL_UPDATED, &updateEda);

    // Direction Icon
    directionIcon.init();
    directionIcon.setPosition(LV_ALIGN_CENTER, 0, -40);
    directionIcon.setSize(160, 160);
    updateNextInstructionIcon();
    Event::instance()->connect(directionIcon.getObj(), NavigationModel::NavigationEvents::EVENT_NEXT_INSTRUCTION_ICON_UPDATED, &updateNextInstructionIcon);

    // Direction label
    directionLabel = lv_label_create(lv_screen_active());
    lv_obj_set_width(directionLabel, 300);
    lv_label_set_long_mode(directionLabel, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(directionLabel, LV_TEXT_ALIGN_CENTER, 0);
    updateDirection();
    Event::instance()->connect(directionLabel, NavigationModel::NavigationEvents::EVENT_NEXT_INSTRUCTION_UPDATED, &updateDirection);
    lv_obj_set_style_text_font(directionLabel, &lv_font_montserrat_26, 0);
    lv_obj_align(directionLabel, LV_ALIGN_CENTER, 0, 70);

    // Distance next instruction
    directionDistanceLabel = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_align(directionDistanceLabel, LV_TEXT_ALIGN_CENTER, 0);
    updateDirectionDistanceLabel();
    Event::instance()->connect(directionDistanceLabel, NavigationModel::NavigationEvents::EVENT_REMAINING_DISTANCE_UPDATED, &updateDirectionDistanceLabel);
    lv_obj_set_style_text_font(directionDistanceLabel, &lv_font_montserrat_30, 0);
    lv_obj_align(directionDistanceLabel, LV_ALIGN_CENTER, 0, 150);

    // Connection state label
    connectionStateLabel = lv_label_create(lv_screen_active());
    lv_label_set_text(connectionStateLabel, "Disc.");
    lv_obj_set_style_text_align(connectionStateLabel, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(connectionStateLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(connectionStateLabel, LV_ALIGN_LEFT_MID, 10, 0);
}

// void copy_and_invert_colors_rgba(uint8_t* bufIn, uint8_t* bufOut, uint32_t px_cnt) {
//     for(uint32_t i = 0; i < px_cnt; i++) {
//         bufOut[4*i + 0] = 255 - bufIn[4*i + 0]; // R
//         bufOut[4*i + 1] = 255 - bufIn[4*i + 1]; // G
//         bufOut[4*i + 2] = 255 - bufIn[4*i + 2]; // B
//     }
// }

void setDirectionIcon(const uint8_t *iconData, size_t iconSize)
{

    // Libérer l'ancien buffer si existant
    delete[] iconCopy;

    // Allouer et copier
    iconCopy = new uint8_t[iconSize];
    memcpy(iconCopy, iconData, iconSize);
    // copy_and_invert_colors_rgba((uint8_t*)iconData, iconCopy, iconSize / 4);

    // Remplir le descripteur
    // directionIconPng.header.always_zero = 0;

    // directionIconPng.header.w = 0;
    // directionIconPng.header.h = 0;
    // directionIconPng.header.cf = LV_COLOR_FORMAT_RAW_ALPHA;
    // directionIconPng.data_size = iconSize;
    // directionIconPng.data = iconCopy;

    // DEBUG ----------------------------------------------------------------------
    // Serial.printf("Icon updated, size: %u bytes\n", directionIconPng.data_size);
    // for(uint16_t i = 0; i < directionIconPng.data_size; i++) {
    //     Serial.printf("%02X ", iconCopy[i]);
    // }
    // DEBUG ----------------------------------------------------------------------

    // Mise à jour dans le contexte LVGL
    // lv_async_call([](void *)
    //               { lv_img_set_src(directionIcon, &directionIconPng); }, nullptr);
}

void setConnected(const bool connected)
{
    lv_label_set_text(connectionStateLabel, connected ? "Conn." : "Disc.");
}
