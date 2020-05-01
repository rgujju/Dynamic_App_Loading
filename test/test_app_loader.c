#include "unity.h"
#include "app_loader.h"
#include "FreeRTOS.h"
//#include "task.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;

//FAKE_VALUE_FUNC(TaskHandle_t, xTaskCreateStatic, TaskFunction_t, const char * const, uint32_t, void*, UBaseType_t, StackType_t, StaticTask_t);
FAKE_VALUE_FUNC(int, xTaskCreateStatic, int, const char * const, uint32_t, void*, UBaseType_t, StackType_t, StaticTask_t);

uint32_t sys = 0xB6B6B6B6;

#define TEST_MAGIC_NEG "test_tinf_neg.tinf"
#define TEST_MAGIC_POS "test_tinf_pos.tinf"

void setUp(void) {
}

void tearDown(void) {
    // clean stuff up here
}


// Test app_name, version, etc

void test_MagicNegative(void) {
    uint8_t buffer[200];
    FILE* ptr;
    ptr = fopen(TEST_MAGIC_NEG, "rb"); // r for read, b for binary
    if(ptr == NULL) {
        TEST_FAIL_MESSAGE(TEST_MAGIC_NEG " not present in directory");
    }
    fread(buffer, sizeof(buffer), 1, ptr);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(-1, LoadApp(buffer), "Magic check passed when it shouldn't have");
    fclose(ptr);
}

void test_MagicPositive(void) {
    uint8_t buffer[200];
    FILE* ptr;
    ptr = fopen(TEST_MAGIC_POS, "rb"); // r for read, b for binary
    if(ptr == NULL) {
        TEST_FAIL_MESSAGE(TEST_MAGIC_POS " not present in directory");
    }
    fread(buffer, sizeof(buffer), 1, ptr);
    TEST_ASSERT_EQUAL_INT8_MESSAGE(0, LoadApp(buffer), "Magic check failed when it shouldn't have");
    fclose(ptr);
}

int main(void) {
    UNITY_BEGIN();
    //RUN_TEST(test_MagicNegative);
    RUN_TEST(test_MagicPositive);
    return UNITY_END();
}
