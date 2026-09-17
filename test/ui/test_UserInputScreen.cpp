#include <gtest/gtest.h>

#define protected public
#include "ui/UserInputScreen.hpp"
#undef protected

TEST(UserInputScreen, Context) {
    UserInputScreen::Context context;
    context.title = "Test title";
    context.initialText = "Initial text";
    context.maxInputLength = 10;
    context.onSubmit = [](const std::string& input) {
        // Do nothing
    };

    UserInputScreen screen;
    screen.setContext(&context);

    EXPECT_EQ(screen.title_m, "Test title");
    EXPECT_EQ(screen.inputText_m, "Initial text");
    EXPECT_EQ(screen.defaultInputText_m, "Initial text");
    EXPECT_EQ(screen.maxInputLength_m, 10);
    EXPECT_TRUE(screen.onSubmit_m != nullptr);
}

TEST(UserInputScreen, populate) {
    UserInputScreen screen;
    screen.populate();

    EXPECT_EQ(screen.inputsLabel_m.size(), strlen(UserInputScreen::inputs));
    EXPECT_EQ(screen.inputArea_m, (lv_obj_t*) 0x1234567);
    EXPECT_EQ(screen.currentCharIndex_m, 0);
}

TEST(UserInputScreen, selectCurrentChar) {
    UserInputScreen screen;
    screen.selectCurrentChar(5);
    EXPECT_EQ(screen.currentCharIndex_m, 5);

    screen.selectCurrentChar(10);
    EXPECT_EQ(screen.currentCharIndex_m, 10);
}
