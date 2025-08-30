#pragma once

#include <lvgl.h>
#include <vector>
#include <string>
#include <cmath>

class CurvedLabel {
public:
    CurvedLabel();
    CurvedLabel(lv_obj_t *parent, int center_x, int center_y, int radius,
        float center_angle_deg = 0.0f,
        float letter_spacing_deg = 10.0f,
        bool inner = false);

    void setText(const std::string &text);
    void setRadius(int radius);
    void setCenterAngle(float center_angle_deg);
    void setLetterSpacing(float spacing_deg);
    void setCenter(int x, int y);
    void setInnerOrientation(bool inner);
    void setStyle(const lv_style_t *style);

    lv_obj_t *getContainer();

private:
    void updateLayout();

    lv_obj_t *container;
    std::vector<lv_obj_t *> letters;
    std::string currentText;

    int centerX, centerY;
    int radius;
    float centerAngleDeg;
    float letterSpacingDeg;
    bool innerOrientation;
    const lv_style_t *appliedStyle = nullptr;
};
