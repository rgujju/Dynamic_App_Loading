
#pragma once
#include "utilities.h"



typedef struct __attribute__((packed)) tinf_t {
    uint8_t magic[4]; // Will always be TINF
    uint8_t app_name[16]; 
    uint8_t major_version;
    uint8_t minor_version;
    uint16_t text_size; // In words
    uint16_t data_size; // In words
    uint16_t bss_size;  // In words 
    uint32_t got_entries;
    uint32_t bin[];
} tinf_t;


#define APP_INVALID -1
#define APP_OOM -2

int8_t LoadApp(const uint8_t* tinf_img); 
