#include <ztest.h>
#include "../fff.h"
#include "app_loader/app_loader.h"

DEFINE_FFF_GLOBALS;

uint32_t sys = 0xA5A5A5A5;

#define TEST_MAGIC_NEG "test_tinf_neg.tinf"
#define TEST_MAGIC_POS "test_tinf_pos.tinf"

// Test app_name, version, etc

void test_MagicNegative(void) {
    uint8_t buffer[200];
    FILE* ptr;
    ptr = fopen(TEST_MAGIC_NEG, "rb"); // r for read, b for binary
    zassert_not_null(ptr,TEST_MAGIC_NEG " not present in directory");
    zassert_true(fread(buffer, sizeof(buffer), 1, ptr), "fread returned error");
    zassert_equal(-1, LoadApp(buffer), "Magic check passed when it shouldn't have");
    fclose(ptr);
}

void test_MagicPositive(void) {
    uint8_t buffer[200];
    FILE* ptr;
    ptr = fopen(TEST_MAGIC_POS, "rb"); // r for read, b for binary
    zassert_not_null(ptr,TEST_MAGIC_POS " not present in directory");
    zassert_true(fread(buffer, sizeof(buffer), 1, ptr), "fread returned error");
    zassert_equal(0, LoadApp(buffer), "Magic check failed when it shouldn't have");
    fclose(ptr);
}

void test_main(void) {
    ztest_test_suite(test_app_loader,
        ztest_unit_test(test_MagicNegative),
        ztest_unit_test(test_MagicPositive)
    );

    ztest_run_test_suite(test_app_loader);
}
