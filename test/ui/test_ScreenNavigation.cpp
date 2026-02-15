#include <gtest/gtest.h>

#define private public
#include "ui/ScreenNavigation.hpp"
#undef private

extern uint8_t screenLoadAnimCalled;
extern uint8_t screenLoadCalled;
extern uint8_t addEventCbCalled;

static uint8_t screenCreatedIndex   = 0;
static uint8_t screenDeletedIndex   = 0;
static uint8_t populateCalled       = 0;
static uint8_t onPostPopulateCalled = 0;

class GenericScreen : public AbstractScreen {
   public:
    GenericScreen() : AbstractScreen() {
        screenCreatedIndex++;
        index_m = screenCreatedIndex;
    }

    ~GenericScreen() override {
        screenDeletedIndex = index_m;
        AbstractScreen::~AbstractScreen();
    }

    void populate() override {
        populateCalled++;
    }

    void onPostPopulate() override {
        onPostPopulateCalled++;
    }

    void onScreenLoaded() override {
    }

    uint8_t index_m;
};

class GenericScreen2 : public GenericScreen {};

ScreenNavigation::ScreenInstanciator instanciator  = ScreenNavigation::createInstance<GenericScreen>;
ScreenNavigation::ScreenInstanciator instanciator2 = ScreenNavigation::createInstance<GenericScreen2>;

TEST(ScreenNavigation, instance) {
    ScreenNavigation *instance = ScreenNavigation::instance();
    EXPECT_EQ(instance, ScreenNavigation::instance());
    EXPECT_EQ(instance, ScreenNavigation::instance());

    EXPECT_TRUE(instanciator != nullptr);
    EXPECT_TRUE(instanciator2 != nullptr);
}

TEST(ScreenNavigation, navigateTo) {
    ScreenNavigation nav;

    EXPECT_EQ(nav.getTopInstanciator(), nullptr);
    screenDeletedIndex = 255;
    nav.navigateTo<GenericScreen>();
    EXPECT_EQ(nav.stackIndex_m, 1);
    EXPECT_EQ(nav.screenStack_m[0], instanciator);
    EXPECT_EQ(nav.getTopInstanciator(), instanciator);
    EXPECT_EQ(static_cast<GenericScreen *>(nav.top())->index_m, 1);
    EXPECT_EQ(screenCreatedIndex, 1);
    EXPECT_EQ(screenDeletedIndex, 255);

    nav.navigateTo<GenericScreen2>();
    EXPECT_EQ(nav.stackIndex_m, 2);
    EXPECT_EQ(nav.screenStack_m[0], instanciator);
    EXPECT_EQ(nav.screenStack_m[1], instanciator2);
    EXPECT_EQ(nav.getTopInstanciator(), instanciator2);
    EXPECT_EQ(static_cast<GenericScreen *>(nav.top())->index_m, 2);
    EXPECT_EQ(screenCreatedIndex, 2);
    EXPECT_EQ(screenDeletedIndex, 1);

    nav.navigateTo<GenericScreen>();
    EXPECT_EQ(nav.stackIndex_m, 3);
    EXPECT_EQ(nav.screenStack_m[0], instanciator);
    EXPECT_EQ(nav.screenStack_m[1], instanciator2);
    EXPECT_EQ(nav.screenStack_m[2], instanciator);
    EXPECT_EQ(nav.getTopInstanciator(), instanciator);
    EXPECT_EQ(static_cast<GenericScreen *>(nav.top())->index_m, 3);
    EXPECT_EQ(screenCreatedIndex, 3);
    EXPECT_EQ(screenDeletedIndex, 2);

    // full
    nav.navigateTo<GenericScreen>();
    EXPECT_EQ(nav.stackIndex_m, 3);
    EXPECT_EQ(nav.screenStack_m[0], instanciator);
    EXPECT_EQ(nav.screenStack_m[1], instanciator2);
    EXPECT_EQ(nav.screenStack_m[2], instanciator);
    EXPECT_EQ(nav.getTopInstanciator(), instanciator);
    EXPECT_EQ(static_cast<GenericScreen *>(nav.top())->index_m, 3);
    EXPECT_EQ(screenCreatedIndex, 3);
    EXPECT_EQ(screenDeletedIndex, 2);
}

TEST(ScreenNavigation, navigateTo_clear) {
    ScreenNavigation nav;
    nav.navigateTo<GenericScreen>();
    nav.navigateTo<GenericScreen>();
    nav.navigateTo<GenericScreen>(NavigationTransition::NORMAL, true);

    EXPECT_EQ(nav.stackIndex_m, 1);
    EXPECT_EQ(nav.screenStack_m[0], instanciator);
    EXPECT_EQ(nav.screenStack_m[1], nullptr);
    EXPECT_EQ(nav.screenStack_m[2], nullptr);
}

TEST(ScreenNavigation, back) {
    ScreenNavigation nav;
    // no back
    EXPECT_EQ(nav.stackIndex_m, 0);
    nav.back();
    EXPECT_EQ(nav.stackIndex_m, 0);

    // no back
    nav.navigateTo<GenericScreen>();
    populateCalled = 0;

    nav.back();
    EXPECT_EQ(populateCalled, 0);

    nav.navigateTo<GenericScreen>();
    EXPECT_EQ(populateCalled, 1);
    EXPECT_EQ(nav.stackIndex_m, 2);
    nav.back();
    EXPECT_EQ(populateCalled, 2);
    EXPECT_EQ(nav.stackIndex_m, 1);

    nav.navigateTo<GenericScreen>();
    EXPECT_EQ(populateCalled, 3);
    EXPECT_EQ(nav.stackIndex_m, 2);
    nav.back();
    EXPECT_EQ(populateCalled, 4);
    EXPECT_EQ(nav.stackIndex_m, 1);
}

TEST(ScreenNavigation, navigate) {
    ScreenNavigation nav;
    GenericScreen *screen = new GenericScreen();

    nav.stackIndex_m     = 0;
    screenLoadAnimCalled = 0;
    screenLoadCalled     = 0;
    populateCalled       = 0;
    onPostPopulateCalled = 0;
    addEventCbCalled     = 0;

    EXPECT_EQ(screenLoadAnimCalled, 0);
    EXPECT_EQ(screenLoadCalled, 0);
    EXPECT_EQ(populateCalled, 0);
    EXPECT_EQ(onPostPopulateCalled, 0);
    nav.navigate(screen);
    EXPECT_EQ(addEventCbCalled, 0);
    EXPECT_EQ(screenLoadAnimCalled, 0);
    EXPECT_EQ(screenLoadCalled, 1);
    EXPECT_EQ(populateCalled, 1);
    EXPECT_EQ(onPostPopulateCalled, 1);

    nav.stackIndex_m = 1;
    nav.topScreen_m  = screen;
    nav.navigate(screen, NavigationTransition::NORMAL);
    EXPECT_EQ(addEventCbCalled, 1);
    EXPECT_EQ(screenLoadAnimCalled, 1);
    EXPECT_EQ(screenLoadCalled, 1);
    EXPECT_EQ(populateCalled, 2);
    EXPECT_EQ(onPostPopulateCalled, 2);

    nav.navigate(screen, NavigationTransition::TO_LEFT);
    EXPECT_EQ(addEventCbCalled, 2);
    EXPECT_EQ(screenLoadAnimCalled, 2);
    EXPECT_EQ(screenLoadCalled, 1);
    EXPECT_EQ(populateCalled, 3);
    EXPECT_EQ(onPostPopulateCalled, 3);

    nav.navigate(screen, NavigationTransition::TO_RIGHT);
    EXPECT_EQ(addEventCbCalled, 3);
    EXPECT_EQ(screenLoadAnimCalled, 3);
    EXPECT_EQ(screenLoadCalled, 1);
    EXPECT_EQ(populateCalled, 4);
    EXPECT_EQ(onPostPopulateCalled, 4);

    nav.navigate(screen, NavigationTransition::FADE_IN);
    EXPECT_EQ(addEventCbCalled, 4);
    EXPECT_EQ(screenLoadAnimCalled, 4);
    EXPECT_EQ(screenLoadCalled, 1);
    EXPECT_EQ(populateCalled, 5);
    EXPECT_EQ(onPostPopulateCalled, 5);
}
