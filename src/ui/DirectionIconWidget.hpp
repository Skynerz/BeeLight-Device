#ifndef DIRECTION_ICON_WIDGET_HPP
#define DIRECTION_ICON_WIDGET_HPP

#include <map>

#include "model/NavigationModel.hpp"
#include "res/icons.h"

class DirectionIconWidget {
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
    void setIcon(InstructionIcon::Values icon);
    lv_obj_t *getObj() const {
        return icon_m;
    }

   private:
    lv_obj_t *icon_m{nullptr};
    std::map<InstructionIcon::Values, const lv_image_dsc_t *> icon_map_m{
        {InstructionIcon::Values::DEPART, &ic_depart},
        {InstructionIcon::Values::DESTINATION, &ic_destination},
        {InstructionIcon::Values::DESTINATION_LEFT, &ic_destination_left},
        {InstructionIcon::Values::DESTINATION_RIGHT, &ic_destination_right},
        {InstructionIcon::Values::FERRY_BOAT, &ic_ferry_boat},
        {InstructionIcon::Values::FERRY_TRAIN, &ic_ferry_train},
        {InstructionIcon::Values::FORK_LEFT, &ic_fork_left},
        {InstructionIcon::Values::FORK_RIGHT, &ic_fork_right},
        {InstructionIcon::Values::MERGE_UNSPECIFIED, &ic_merge},
        {InstructionIcon::Values::MERGE_LEFT, &ic_merge_left},
        {InstructionIcon::Values::MERGE_RIGHT, &ic_merge_right},
        {InstructionIcon::Values::ROUNDABOUT_CLOCKWISE, &ic_roundabout_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_COUNTERCLOCKWISE, &ic_roundabout_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_EXIT_CLOCKWISE, &ic_roundabout_exit_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_EXIT_COUNTERCLOCKWISE, &ic_roundabout_exit_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_LEFT_CLOCKWISE, &ic_roundabout_left_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_LEFT_COUNTERCLOCKWISE, &ic_roundabout_left_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_RIGHT_CLOCKWISE, &ic_roundabout_right_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_RIGHT_COUNTERCLOCKWISE, &ic_roundabout_right_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_SHARP_LEFT_CLOCKWISE, &ic_roundabout_sharp_left_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_SHARP_LEFT_COUNTERCLOCKWISE, &ic_roundabout_sharp_left_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_SHARP_RIGHT_CLOCKWISE, &ic_roundabout_sharp_right_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_SHARP_RIGHT_COUNTERCLOCKWISE, &ic_roundabout_sharp_right_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_SLIGHT_LEFT_CLOCKWISE, &ic_roundabout_slight_left_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_SLIGHT_LEFT_COUNTERCLOCKWISE, &ic_roundabout_slight_left_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_SLIGHT_RIGHT_CLOCKWISE, &ic_roundabout_slight_right_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_SLIGHT_RIGHT_COUNTERCLOCKWISE,
         &ic_roundabout_slight_right_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_STRAIGHT_CLOCKWISE, &ic_roundabout_straight_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_STRAIGHT_COUNTERCLOCKWISE, &ic_roundabout_straight_counterclockwise},
        {InstructionIcon::Values::ROUNDABOUT_U_TURN_CLOCKWISE, &ic_roundabout_u_turn_clockwise},
        {InstructionIcon::Values::ROUNDABOUT_U_TURN_COUNTERCLOCKWISE, &ic_roundabout_u_turn_counterclockwise},
        {InstructionIcon::Values::STRAIGHT, &ic_straight},
        {InstructionIcon::Values::TURN_LEFT, &ic_turn_left},
        {InstructionIcon::Values::TURN_RIGHT, &ic_turn_right},
        {InstructionIcon::Values::TURN_KEEP_LEFT, &ic_turn_sharp_left},
        {InstructionIcon::Values::TURN_KEEP_RIGHT, &ic_turn_sharp_right},
        {InstructionIcon::Values::TURN_SLIGHT_LEFT, &ic_turn_slight_left},
        {InstructionIcon::Values::TURN_SLIGHT_RIGHT, &ic_turn_slight_right},
        {InstructionIcon::Values::TURN_U_TURN_CLOCKWISE, &ic_turn_u_turn_clockwise},
        {InstructionIcon::Values::TURN_U_TURN_COUNTERCLOCKWISE, &ic_turn_u_turn_counterclockwise},
    };
};

#endif
