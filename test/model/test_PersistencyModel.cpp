#include <gtest/gtest.h>

#define private public
#include "PersistencyModel.hpp"

extern uint8_t persistency_init_called;
extern uint8_t persistency_deinit_called;
extern uint8_t persistency_write_called;
extern uint8_t persistency_read_called;

extern uint8_t persistency_write_return_value;
extern uint8_t persistency_read_return_value;
extern Key_t persistency_read_key;
extern Key_t persistency_write_key;

TEST(PersistencyModel, instance) {
    PersistencyModel *instance = PersistencyModel::instance();
    EXPECT_EQ(instance, PersistencyModel::instance());
    EXPECT_EQ(instance, PersistencyModel::instance());

    EXPECT_EQ(persistency_init_called, 1);
}

TEST(PersistencyModel, deviceName) {
    PersistencyModel *instance = PersistencyModel::instance();
    persistency_read_return_value = 1;
    persistency_write_called = 0;
    persistency_read_called = 0;
    instance->setDeviceName("test");
    EXPECT_EQ(persistency_write_called, 1);
    EXPECT_EQ(instance->getDeviceName(), "test");
    EXPECT_EQ(persistency_read_called, 1);

    persistency_read_return_value = 0;
    EXPECT_EQ(instance->getDeviceName(), PersistencyModel::kDefaultDeviceName);
    EXPECT_EQ(persistency_read_called, 2);
}

TEST(PersistencyModel, commissionningStatus) {
    PersistencyModel *instance = PersistencyModel::instance();
    persistency_read_return_value = 1;
    persistency_write_called = 0;
    persistency_read_called = 0;
    instance->setCommissioningStatus(1);
    EXPECT_EQ(persistency_write_called, 1);
    EXPECT_EQ(instance->getCommissioningStatus(), 1);
    EXPECT_EQ(persistency_read_called, 1);
    
    persistency_read_return_value = 0;
    EXPECT_EQ(instance->getCommissioningStatus(), 0);
    EXPECT_EQ(persistency_read_called, 2);
}