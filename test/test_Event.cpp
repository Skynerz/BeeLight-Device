#include <gtest/gtest.h>

#define private public
#include "Event.hpp"
#undef private

//STUBS
static uint8_t stub_send = 0;
static uint32_t event_last_id = 0;
static lv_event_code_t stub_event = LV_EVENT_ALL;
static void* stub_param = nullptr;
uint32_t lv_event_register_id(void)
{
    event_last_id ++;
    return event_last_id;
}

lv_result_t lv_obj_send_event(lv_obj_t * obj, lv_event_code_t event_code, void * param) {
    stub_send++;
    stub_event = event_code;
    stub_param = param;
    return LV_RESULT_OK;
}

static uint8_t stub_add = 0;
static lv_event_cb_t stub_event_cb = nullptr;
static void* stub_user_data = nullptr;
lv_event_dsc_t *lv_obj_add_event_cb(lv_obj_t *obj, lv_event_cb_t event_cb, lv_event_code_t filter, void *user_data)
{
    stub_add++;
    stub_event_cb = event_cb;
    stub_event = filter;
    stub_user_data = user_data;
    return nullptr;
}

// END STUBS

TEST(Event, instance) {
    Event* event1 = Event::instance();
    Event* event2 = Event::instance();
    EXPECT_EQ(event1, event2); // Both should point to the same instance
}

TEST(Event, registerAndGetEvent) {
    Event* event = Event::instance();
    EXPECT_EQ(event->registeredEvents_m.size(), 0);
    
    uint8_t testEventId = 1;
    event->registerEvent(testEventId);
    EXPECT_EQ(event->registeredEvents_m.size(), 1);
    EXPECT_TRUE(event->registeredEvents_m.find(testEventId) != event->registeredEvents_m.end());

    event->registerEvent(testEventId);
    EXPECT_EQ(event->registeredEvents_m.size(), 1);
    EXPECT_TRUE(event->registeredEvents_m.find(testEventId) != event->registeredEvents_m.end());
}

TEST(Event, getLvEvent) {
    Event* event = Event::instance();
    uint8_t testEventId = 2;

    EXPECT_EQ(event->getLvEvent(testEventId), nullptr);

    event->registerEvent(testEventId);
    auto lvEvent = event->getLvEvent(testEventId);
    EXPECT_NE(lvEvent, nullptr);
    EXPECT_EQ(lvEvent->first, event->registeredEvents_m[testEventId].first);
    EXPECT_EQ(lvEvent->second, event->registeredEvents_m[testEventId].second);
}

TEST(Event, emit) {
    Event* event = Event::instance();
    uint8_t testEventId = 3;

    stub_send = 0;
    event->emit(testEventId);
    EXPECT_EQ(stub_send, 0);

    event->registerEvent(testEventId);

    event->emit(testEventId);
    EXPECT_EQ(stub_send, 1);
    EXPECT_EQ(stub_event, event->registeredEvents_m[testEventId].first);
    EXPECT_EQ(stub_user_data, nullptr);

    uint8_t foo = 79;
    event->emit(testEventId, &foo);
    EXPECT_EQ(stub_send, 2);
    EXPECT_EQ(stub_event, event->registeredEvents_m[testEventId].first);
    EXPECT_EQ(stub_param, &foo);
}

void dummyCallback(lv_event_t *e) {}
TEST(Event, connect) {
    Event* event = Event::instance();
    uint8_t testEventId = 4;
    void* dummyObj;
    void* testData = (void*)0x12345678;

    stub_add = 0;
    event->connect((lv_obj_t*) dummyObj, testEventId, &dummyCallback);
    EXPECT_EQ(stub_add, 0);

    event->registerEvent(testEventId);

    dummyObj = (void*) 1;
    event->connect((lv_obj_t*) dummyObj, testEventId, &dummyCallback, testData);
    EXPECT_EQ(stub_add, 1);
    EXPECT_EQ(stub_event, event->registeredEvents_m[testEventId].first);
    EXPECT_EQ(stub_event_cb, &dummyCallback);
    EXPECT_EQ(stub_user_data, testData);
    EXPECT_EQ(event->registeredEvents_m[testEventId].second, dummyObj);
}
