#include "nx_api.h"
#if defined(NX_TAHI_ENABLE) && defined(FEATURE_NX_IPV6) && defined(NX_IPSEC_ENABLE)
#include "netx_tahi.h"


static char pkt1[] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x64, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x23, 
0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xf1, 0x71, 
0x71, 0x9c, 0x36, 0x13, 0x0d, 0xba, 0xbe, 0x4c, 
0xb0, 0x39, 0x12, 0xb8, 0x79, 0x14, 0x17, 0xfd, 
0xbc, 0x52, 0xb2, 0xb3, 0x37, 0x88, 0x65, 0xd3, 
0xbc, 0x2e, 0xf0, 0xa7, 0x1c, 0xe1, 0x67, 0x7b, 
0xe1, 0x7b, 0x3b, 0x8a, 0x56, 0x73, 0x7d, 0x9c, 
0x91, 0x05, 0xa6, 0xf8, 0xbb, 0x9b, 0xcc, 0xf7, 
0xc6, 0x59, 0xe4, 0xb5, 0x8c, 0x5d, 0x9c, 0x2e, 
0xbf, 0x6b, 0x6e, 0xe0, 0xb1, 0xe4, 0x10, 0xc6, 
0x01, 0xd1, 0x93, 0x9a, 0xa8, 0x35, 0x26, 0x02, 
0xfb, 0x6e, 0x7b, 0xcd, 0xac, 0x23, 0x45, 0xac, 
0xe8, 0x9b };

static char pkt2[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x11, 
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x64, 0x32, 0x40, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x3f, 0xfe, 
0x05, 0x01, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x59, 0x05, 
0xb5, 0x6d, 0x8d, 0x7d, 0x15, 0x7d, 0x8b, 0x97, 
0xb7, 0x42, 0x10, 0xb2, 0xba, 0x93, 0xea, 0x62, 
0x49, 0xa2, 0x82, 0x8e, 0x83, 0x50, 0xa7, 0x1e, 
0x81, 0x5f, 0xc9, 0x45, 0x47, 0x28, 0xd5, 0xbc, 
0x08, 0x7e, 0x2a, 0x23, 0x42, 0x05, 0x3f, 0x04, 
0x0e, 0x81, 0xfe, 0xf0, 0x5d, 0xb0, 0xda, 0x11, 
0xcf, 0xcf, 0xc5, 0x56, 0xeb, 0x2d, 0x97, 0x96, 
0x1a, 0xb9, 0x19, 0x6c, 0xde, 0x4b, 0x71, 0x8e, 
0xe1, 0xfb, 0xca, 0x48, 0x0a, 0x1f, 0xae, 0x35, 
0x33, 0x72, 0x32, 0x2b, 0x08, 0x38, 0x04, 0xa6, 
0x30, 0xaa, 0x3d, 0xce, 0x7f, 0x22, 0x89, 0x35, 
0xd9, 0xcf };

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

TAHI_TEST_SEQ tahi_ipsec_udp_026[] = {
    {TITLE, "IPSEC-UDP-026", 13, 0, 0, 0, 0, 0},
    {WAIT, NX_NULL, 0, 5, 0, 0, 0, 0},

    {INJECT, &pkt1[0], sizeof(pkt1), 0, 0, 0, 0, 0},
    {INJECT, &pkt4[0], sizeof(pkt4), 0, 0, 0, 0, 0},  /* Send NA packet first.  */
    {TD_CHECK, &pkt2[0], sizeof(pkt2), 3, NX_NULL, 0, 0, 0},

    {CLEANUP, NX_NULL, 0, 0, 0, 0, 0, 0},
    {DUMP, NX_NULL, 0, 0, 0, 0, 0, 0}
};

int tahi_ipsec_udp_026_size = sizeof(tahi_ipsec_udp_026) / sizeof(TAHI_TEST_SEQ);

#endif
