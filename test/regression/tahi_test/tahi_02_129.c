#include "nx_api.h"
#if defined(NX_TAHI_ENABLE) && defined(FEATURE_NX_IPV6)  

#include "netx_tahi.h"

#if 0
static char pkt1[] = {
0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, 
0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56 };

static char pkt2[] = {
0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, 
0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56 };

static char pkt3[] = {
0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, 
0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56 };
#endif

static char pkt4[] = {
0x33, 0x33, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x85, 0x00, 
0xad, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 
0x00, 0x11, 0x22, 0x33, 0x44, 0x56 };

static char pkt5[] = {
0x33, 0x33, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0xa0, 0xa0, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x30, 0x3a, 0xff, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0xa0, 0xa0, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 
0x41, 0xf3, 0x40, 0x00, 0x07, 0x08, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 
0x40, 0xc0, 0x00, 0x27, 0x8d, 0x00, 0x00, 0x09, 
0x3a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if 0
static char pkt6[] = {
0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, 
0x88, 0x85, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x01, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56 };

static char pkt7[] = {
0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, 
0x88, 0x85, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x01, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56 };
#endif

static char pkt8[] = {
0x33, 0x33, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0xa0, 0xa0, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x30, 0x3a, 0xff, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0xa0, 0xa0, 0xff, 0x02, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x86, 0x00, 
0x10, 0xac, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 
0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

TAHI_TEST_SEQ tahi_02_129[] = {
    {TITLE, "02-129", 6, 0},
    {WAIT, NX_NULL, 0, 5},

    {CHECK, &pkt4[0], sizeof(pkt4), 300},
    {INJECT, &pkt5[0], sizeof(pkt5), 0},
    {N_CHECK, (char *)RS, 0, 5},
    {WAIT, NX_NULL , 0, 3},

    {INJECT, &pkt8[0], sizeof(pkt8), 0},
    
    {CLEANUP, NX_NULL, 0, 0},
    {DUMP, NX_NULL, 0, 0}
};

int tahi_02_129_size = sizeof(tahi_02_129) / sizeof(TAHI_TEST_SEQ);

#endif /* NX_TAHI_ENABLE */
