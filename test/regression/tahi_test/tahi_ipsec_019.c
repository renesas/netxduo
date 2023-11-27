#include "nx_api.h"
#if defined(NX_TAHI_ENABLE) && defined(FEATURE_NX_IPV6) && defined(NX_IPSEC_ENABLE)
#include "netx_tahi.h"



static char pkt1[] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x2c, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 
0x13, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x50, 0x61, 
0x64, 0x4c, 0x65, 0x6e, 0x20, 0x69, 0x73, 0x20, 
0x5a, 0x65, 0x72, 0x6f, 0x00, 0x3a, 0xba, 0x19, 
0x8b, 0x06, 0x67, 0x5d, 0x31, 0x3e, 0x5f, 0xd5, 
0x7c, 0x50 };

static char pkt2[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x30, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0x00, 
0x12, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x50, 0x61, 
0x64, 0x4c, 0x65, 0x6e, 0x20, 0x69, 0x73, 0x20, 
0x5a, 0x65, 0x72, 0x6f, 0x01, 0x02, 0x03, 0x04, 
0x04, 0x3a, 0x47, 0x50, 0x6a, 0xdd, 0xbe, 0xe3, 
0x26, 0xb5, 0xec, 0x8b, 0x07, 0xbe };

#if 0
static char pkt3[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x00, 0x0f, 0x87, 0x00, 
0x65, 0x50, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x00, 0x0f, 0x01, 0x01, 
0x00, 0x11, 0x22, 0x33, 0x44, 0x56 };
#endif

static char pkt4[] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x00, 0x0f, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x88, 0x00, 
0xe9, 0xda, 0xe0, 0x00, 0x00, 0x00, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x00, 0x0f, 0x02, 0x01, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f };

TAHI_TEST_SEQ tahi_ipsec_019[] = {
    {TITLE, "IPSEC-019", 9, 0, 0, 0, 0, 0},
    {WAIT, NX_NULL, 0, 5, 0, 0, 0, 0},

    {INJECT, &pkt1[0], sizeof(pkt1), 0, 0, 0, 0, 0},
    {INJECT, &pkt4[0], sizeof(pkt4), 0, 0, 0, 0, 0},  /* Send NA packet first.  */
    {D_CHECK, &pkt2[0], sizeof(pkt2), 3, NX_NULL, 0, 0, 0}, 

    {CLEANUP, NX_NULL, 0, 0, 0, 0, 0, 0},
    {DUMP, NX_NULL, 0, 0, 0, 0, 0, 0}
};

int tahi_ipsec_019_size = sizeof(tahi_ipsec_019) / sizeof(TAHI_TEST_SEQ);

#endif
