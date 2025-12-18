#ifndef DIRECTION_ICON_WIDGET_HPP
#define DIRECTION_ICON_WIDGET_HPP

#include "res/icons.h"
#include "model/NavigationModel.hpp"
#include <map>

class DirectionIconWidget
{
public:
    DirectionIconWidget() = default;
    void init();

    /**
     * @brief set icon position
     * @param align
     * @param x_ofs default 0
     * @param y_ofs default 0
     */
    void setPosition(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
    
    /**
     * @brief set icon scale
     * @param zoom
     * @note refers to lv_image_set_scale
     */
    void setScale(int32_t zoom);
    void setIcon(NavigationModel::InstructionIcon icon);
    lv_obj_t *getObj() const
    {
        return icon_m;
    }

private:
    lv_obj_t *icon_m{nullptr};
    std::map<NavigationModel::InstructionIcon, const lv_image_dsc_t *> icon_map_m
    {
        {NavigationModel::InstructionIcon::DEPART, &ic_depart},
        {NavigationModel::InstructionIcon::DESTINATION, &ic_destination},
        {NavigationModel::InstructionIcon::DESTINATION_LEFT, &ic_destination_left},
        {NavigationModel::InstructionIcon::DESTINATION_RIGHT, &ic_destination_right},
        {NavigationModel::InstructionIcon::FERRY_BOAT, &ic_ferry_boat},
        {NavigationModel::InstructionIcon::FERRY_TRAIN, &ic_ferry_train},
        {NavigationModel::InstructionIcon::FORK_LEFT, &ic_fork_left},
        {NavigationModel::InstructionIcon::FORK_RIGHT, &ic_fork_right},
        {NavigationModel::InstructionIcon::MERGE_UNSPECIFIED, &ic_merge},
        {NavigationModel::InstructionIcon::MERGE_LEFT, &ic_merge_left},
        {NavigationModel::InstructionIcon::MERGE_RIGHT, &ic_merge_right},
        {NavigationModel::InstructionIcon::ROUNDABOUT_CLOCKWISE, &ic_roundabout_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_COUNTERCLOCKWISE, &ic_roundabout_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_EXIT_CLOCKWISE, &ic_roundabout_exit_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_EXIT_COUNTERCLOCKWISE, &ic_roundabout_exit_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_LEFT_CLOCKWISE, &ic_roundabout_left_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_LEFT_COUNTERCLOCKWISE, &ic_roundabout_left_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_RIGHT_CLOCKWISE, &ic_roundabout_right_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_RIGHT_COUNTERCLOCKWISE, &ic_roundabout_right_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SHARP_LEFT_CLOCKWISE, &ic_roundabout_sharp_left_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SHARP_LEFT_COUNTERCLOCKWISE, &ic_roundabout_sharp_left_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SHARP_RIGHT_CLOCKWISE, &ic_roundabout_sharp_right_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SHARP_RIGHT_COUNTERCLOCKWISE, &ic_roundabout_sharp_right_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SLIGHT_LEFT_CLOCKWISE, &ic_roundabout_slight_left_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SLIGHT_LEFT_COUNTERCLOCKWISE, &ic_roundabout_slight_left_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SLIGHT_RIGHT_CLOCKWISE, &ic_roundabout_slight_right_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_SLIGHT_RIGHT_COUNTERCLOCKWISE, &ic_roundabout_slight_right_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_STRAIGHT_CLOCKWISE, &ic_roundabout_straight_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_STRAIGHT_COUNTERCLOCKWISE, &ic_roundabout_straight_counterclockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_U_TURN_CLOCKWISE, &ic_roundabout_u_turn_clockwise},
        {NavigationModel::InstructionIcon::ROUNDABOUT_U_TURN_COUNTERCLOCKWISE, &ic_roundabout_u_turn_counterclockwise},
        {NavigationModel::InstructionIcon::STRAIGHT, &ic_straight},
        {NavigationModel::InstructionIcon::TURN_LEFT, &ic_turn_left},
        {NavigationModel::InstructionIcon::TURN_RIGHT, &ic_turn_right},
        {NavigationModel::InstructionIcon::TURN_KEEP_LEFT, &ic_turn_sharp_left},
        {NavigationModel::InstructionIcon::TURN_KEEP_RIGHT, &ic_turn_sharp_right},
        {NavigationModel::InstructionIcon::TURN_SLIGHT_LEFT, &ic_turn_slight_left},
        {NavigationModel::InstructionIcon::TURN_SLIGHT_RIGHT, &ic_turn_slight_right},
        {NavigationModel::InstructionIcon::TURN_U_TURN_CLOCKWISE, &ic_turn_u_turn_clockwise},
        {NavigationModel::InstructionIcon::TURN_U_TURN_COUNTERCLOCKWISE, &ic_turn_u_turn_counterclockwise},
    };
};

#endif
