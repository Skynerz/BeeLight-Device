#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "Event.hpp"

static uint8_t cbCalled = 0;
static uint8_t cbCalled2 = 0;
void dummyCallback(lv_event_t *e)
{
    cbCalled++;
}
void dummyCallback2(lv_event_t *e)
{
    cbCalled2++;
}

TEST(Event, functional)
{
    lv_init();
    lv_display_t * disp = lv_display_create(1, 1);
    Event *event = Event::instance();
    uint8_t testEventId = 79;
    auto obj = lv_obj_create(lv_screen_active());

    EXPECT_EQ(cbCalled, 0);
    EXPECT_EQ(cbCalled2, 0);

    event->registerEvent(testEventId);
    event->connect(obj, testEventId, &dummyCallback);

    EXPECT_EQ(cbCalled, 0);
    EXPECT_EQ(cbCalled2, 0);

    event->emit(testEventId);
    lv_timer_handler();

    EXPECT_EQ(cbCalled, 1);
    EXPECT_EQ(cbCalled2, 0);
}
