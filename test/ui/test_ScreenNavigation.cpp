#include <gtest/gtest.h>

#define private public
#include "ui/ScreenNavigation.hpp"
#undef private

static uint8_t index = 0;
class GenericScreen : public AbstractScreen {
public:
    GenericScreen() : index_m(index++)
    {
    }

    void populate() override
    {
    }

    uint8_t index_m;
};

TEST(ScreenNavigation, navigateTo) {
    ScreenNavigation nav;

    EXPECT_EQ(nav.stackIndex_m, 0);
    nav.navigateTo<GenericScreen>();
    EXPECT_EQ(nav.stackIndex_m, 0);
}