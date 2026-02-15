#include <gtest/gtest.h>
#include <sys/stat.h>

#include "common/port_persistency.h"

class TestPortPersistency : public ::testing::Test {
   public:
    void SetUp() override {
        unlink("beelight-device.conf");
        persistency_init();
    }

    void TearDown() override {
        persistency_deinit();
    }
};

TEST_F(TestPortPersistency, readNewFile) {
    uint8_t* read_status = nullptr;
    uint8_t len          = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(0), (void**) &read_status, &len), 0);
    EXPECT_TRUE(read_status == nullptr);
    EXPECT_EQ(len, 0);
}

TEST_F(TestPortPersistency, writeAndReadU8) {
    uint8_t status = 1;
    EXPECT_EQ(persistency_write(static_cast<const Key_t>(0), &status), 1);

    uint8_t* read_status = nullptr;
    uint8_t len          = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(0), (void**) &read_status, &len), 1);
    EXPECT_TRUE(read_status != nullptr);
    EXPECT_EQ(*read_status, status);
    EXPECT_EQ(len, sizeof(status));
}

TEST_F(TestPortPersistency, writeAndReadU32) {
    uint32_t test_value = 123456789;
    EXPECT_EQ(persistency_write(static_cast<const Key_t>(1), &test_value), 1);

    uint32_t* read_value = nullptr;
    uint8_t len          = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(1), (void**) &read_value, &len), 1);
    EXPECT_TRUE(read_value != nullptr);
    EXPECT_EQ(*read_value, test_value);
    EXPECT_EQ(len, sizeof(test_value));
}

TEST_F(TestPortPersistency, writeAndReadString) {
    const char* status = "test";
    EXPECT_EQ(persistency_write(static_cast<const Key_t>(2), status), 1);

    char* read_status = nullptr;
    uint8_t len       = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(2), (void**) &read_status, &len), 1);
    EXPECT_TRUE(read_status != nullptr);
    EXPECT_STREQ(read_status, status);
    EXPECT_EQ(len, strlen(status));
}

TEST_F(TestPortPersistency, writeAndReadNone) {
    uint8_t status = 1;
    EXPECT_EQ(persistency_write(KEY_COUNT, &status), 0);

    uint8_t* read_status = nullptr;
    uint8_t len          = 0;
    EXPECT_EQ(persistency_read(KEY_COUNT, (void**) &read_status, &len), 0);
    EXPECT_TRUE(read_status == nullptr);
    EXPECT_EQ(len, 0);
}

TEST_F(TestPortPersistency, reload) {
    uint8_t u8_value = 1;
    EXPECT_EQ(persistency_write(static_cast<const Key_t>(0), &u8_value), 1);
    uint32_t u32_value = 123456789;
    EXPECT_EQ(persistency_write(static_cast<const Key_t>(1), &u32_value), 1);
    const char* password = "test";
    EXPECT_EQ(persistency_write(static_cast<const Key_t>(2), password), 1);
    persistency_deinit();

    // cache empty
    uint8_t* read_status = nullptr;
    uint8_t len          = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(0), (void**) &read_status, &len), 0);
    EXPECT_TRUE(read_status == nullptr);
    EXPECT_EQ(len, 0);

    persistency_init();

    uint8_t* read_u8 = nullptr;
    len              = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(0), (void**) &read_u8, &len), 1);
    EXPECT_TRUE(read_u8 != nullptr);
    EXPECT_EQ(*read_u8, u8_value);
    EXPECT_EQ(len, sizeof(u8_value));

    uint32_t* read_u32 = nullptr;
    len                = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(1), (void**) &read_u32, &len), 1);
    EXPECT_TRUE(read_u32 != nullptr);
    EXPECT_EQ(*read_u32, u32_value);
    EXPECT_EQ(len, sizeof(u32_value));

    char* read_password = nullptr;
    len                 = 0;
    EXPECT_EQ(persistency_read(static_cast<const Key_t>(2), (void**) &read_password, &len), 1);
    EXPECT_TRUE(read_password != nullptr);
    EXPECT_STREQ(read_password, password);
    EXPECT_EQ(len, strlen(password));
}
