#include "nx_api.h"
#if defined(NX_TAHI_ENABLE) && defined(FEATURE_NX_IPV6) && defined(NX_IPSEC_ENABLE)
#include "netx_tahi.h"



static char pkt1[] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x34, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x2f, 
0xed, 0xbd, 0xfa, 0xd7, 0x85, 0xd7, 0x5b, 0x9d, 
0xa8, 0x50, 0x6a, 0x45, 0xf8, 0xd3, 0x9d, 0x77, 
0x72, 0xdf, 0x1f, 0x0e, 0x9b, 0xf4, 0xb3, 0xa5, 
0x5a, 0xf3, 0x4c, 0xb1, 0x1c, 0x27, 0x84, 0xf8, 
0x1e, 0x3c };

static char pkt2[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x3c, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x5b, 
0x70, 0x30, 0xb4, 0x5d, 0xfd, 0x20, 0xc8, 0xc5, 
0x80, 0x58, 0xa2, 0x96, 0xa5, 0xe3, 0x66, 0x3d, 
0xca, 0xcf, 0xea, 0xac, 0x15, 0x46, 0x7e, 0x29, 
0x12, 0x6f, 0x2b, 0x99, 0xa3, 0x5e, 0x79, 0x1e, 
0xa1, 0xd9, 0x34, 0x3b, 0x7d, 0x5b, 0x84, 0xc7, 
0xd0, 0x3d, 0x2d, 0xd0, 0xdb, 0x20, 0xe5, 0x81, 
0x33, 0x13 };

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

static char pkt5[] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x34, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x2f, 
0xed, 0xbd, 0xfa, 0xd7, 0x85, 0xd7, 0x5b, 0x9d, 
0xa8, 0x50, 0x6a, 0x45, 0xf8, 0xd3, 0x9d, 0x77, 
0x72, 0xdf, 0x1f, 0x0e, 0x9b, 0xf4, 0x52, 0x80, 
0xaa, 0x15, 0x46, 0x2d, 0xd5, 0x7b, 0x9f, 0xc6, 
0x3f, 0x5a };

static char pkt6[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x3c, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x02, 0x78, 0x7f, 
0x8b, 0x1a, 0xd8, 0x59, 0x29, 0x50, 0x61, 0x10, 
0x51, 0x54, 0x2f, 0xdb, 0xdb, 0x75, 0xed, 0xb4, 
0xde, 0xda, 0xe4, 0xce, 0x9f, 0xf0, 0x33, 0x12, 
0xbd, 0x90, 0xea, 0xff, 0xfd, 0xf4, 0x43, 0x98, 
0x96, 0x50, 0x4e, 0x9c, 0xec, 0x2b, 0x31, 0xf2, 
0xeb, 0x63, 0x52, 0x89, 0x9d, 0xbc, 0x37, 0x67, 
0xf1, 0xad };

TAHI_TEST_SEQ tahi_ipsec_udp_004[] = {
    {TITLE, "IPSEC-UDP-004", 13, 0, 0, 0, 0, 0},
    {WAIT, NX_NULL, 0, 5, 0, 0, 0, 0},

    {INJECT, &pkt1[0], sizeof(pkt1), 0, 0, 0, 0, 0},
    {INJECT, &pkt4[0], sizeof(pkt4), 0, 0, 0, 0, 0},  /* Send NA packet first.  */
    {D_CHECK, &pkt2[0], sizeof(pkt2), 3, NX_NULL, 0, 0, 0},

    {DUMP, NX_NULL, 0, 0, 0, 0, 0, 0},
    {INJECT, &pkt5[0], sizeof(pkt5), 0, 0, 0, 0, 0},
    {D_CHECK, &pkt6[0], sizeof(pkt6), 3, NX_NULL, 0, 0, 0},

    {CLEANUP, NX_NULL, 0, 0, 0, 0, 0, 0},
    {DUMP, NX_NULL, 0, 0, 0, 0, 0, 0}
};

int tahi_ipsec_udp_004_size = sizeof(tahi_ipsec_udp_004) / sizeof(TAHI_TEST_SEQ);

#endif
