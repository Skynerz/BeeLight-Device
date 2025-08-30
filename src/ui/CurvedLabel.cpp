#include "CurvedLabel.h"


CurvedLabel::CurvedLabel()
{

}

CurvedLabel::CurvedLabel(lv_obj_t *parent, int center_x, int center_y, int radius,
    float center_angle_deg, float letter_spacing_deg, bool inner)
    : centerX(center_x), centerY(center_y), radius(radius),
    centerAngleDeg(center_angle_deg), letterSpacingDeg(letter_spacing_deg),
    innerOrientation(inner)
{
    container = lv_obj_create(parent);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_clip_corner(container, false, 0);  // évite tout masquage
    lv_obj_set_size(container, 2 * (radius + 10), 2 * (radius + 10));  // zone assez large
    // lv_obj_set_pos(container, centerX - radius - 50, centerY - radius - 50);
}

void CurvedLabel::setText(const std::string &text)
{
    currentText = text;

    for (size_t i = 0; i < text.length(); ++i)
    {
        if (i >= letters.size())
        {
            lv_obj_t *lbl = lv_label_create(container);
            lv_label_set_text(lbl, "");
            if (appliedStyle)
            {
                lv_obj_add_style(lbl, (lv_style_t *)appliedStyle, 0);
            }
            letters.push_back(lbl);
        }
        char ch[2] = { text[i], '\0' };
        lv_label_set_text(letters[i], ch);
        lv_obj_clear_flag(letters[i], LV_OBJ_FLAG_HIDDEN);
    }

    for (size_t i = text.length(); i < letters.size(); ++i)
    {
        lv_obj_add_flag(letters[i], LV_OBJ_FLAG_HIDDEN);
    }

    updateLayout();
}

void CurvedLabel::setRadius(int r)
{
    radius = r;
    updateLayout();
}

void CurvedLabel::setCenter(int x, int y)
{
    centerX = x;
    centerY = y;
    updateLayout();
}

void CurvedLabel::setCenterAngle(float angle_deg)
{
    centerAngleDeg = angle_deg;
    updateLayout();
}

void CurvedLabel::setLetterSpacing(float spacing_deg)
{
    letterSpacingDeg = spacing_deg;
    updateLayout();
}

void CurvedLabel::setInnerOrientation(bool inner)
{
    innerOrientation = inner;
    updateLayout();
}

void CurvedLabel::setStyle(const lv_style_t *style)
{
    appliedStyle = style;
    for (auto lbl : letters)
    {
        if (style)
        {
            lv_obj_add_style(lbl, (lv_style_t *)style, 0);
        }
    }
}

lv_obj_t *CurvedLabel::getContainer()
{
    return container;
}

void CurvedLabel::updateLayout()
{
    size_t len = currentText.length();
    if (len == 0 || radius <= 0) return;

    float total_span = letterSpacingDeg * (len - 1);
    float start_angle = centerAngleDeg - total_span / 2;

    for (size_t i = 0; i < len; ++i)
    {
        float angle_deg = start_angle + i * letterSpacingDeg;
        float rad = angle_deg * M_PI / 180.0f;

        int x = centerX + radius * cos(rad);
        int y = centerY + radius * sin(rad);

        lv_obj_t *lbl = letters[i];
        lv_obj_set_pos(lbl, x, y);

        float tangent_angle = angle_deg + (innerOrientation ? 90 : -90);
        lv_obj_set_style_transform_angle(lbl, tangent_angle * 10, 0); // en 0.1°
        lv_obj_set_style_transform_pivot_x(lbl, 0, 0);
        lv_obj_set_style_transform_pivot_y(lbl, 0, 0);
    }
}
