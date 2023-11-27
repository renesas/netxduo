
#include "nx_api.h"
#if defined(NX_TAHI_ENABLE) && defined(FEATURE_NX_IPV6)  

#include "netx_tahi.h"

/* Frame (86 bytes) */
static char pkt1[86] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, /* .."3DV.. */
0x00, 0x00, 0x01, 0x00, 0x86, 0xdd, 0x60, 0x00, /* ......`. */
0x00, 0x00, 0x00, 0x20, 0x3c, 0xff, 0xfe, 0x80, /* ... <... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0xfe, 0x80, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x3c, 0x00, /* "..3DV<. */
0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x00, /* ......:. */
0x87, 0x04, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, /* ........ */
0x09, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, /* ........ */
0x03, 0x04, 0x05, 0x06, 0x07, 0x08              /* ...... */
};

/* Frame (86 bytes) */
static char pkt2[86] = {
0x33, 0x33, 0xff, 0x00, 0x01, 0x00, 0x00, 0x11, /* 33...... */
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, /* ... :... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x01, 0xff, 0x00, 0x01, 0x00, 0x87, 0x00, /* ........ */
0xab, 0x68, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* .h...... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x01, 0x01, /* ........ */
0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};

/* Frame (86 bytes) */
static char pkt3[86] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, /* .."3DV.. */
0x00, 0x00, 0x01, 0x00, 0x86, 0xdd, 0x60, 0x00, /* ......`. */
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, /* ... :... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0xfe, 0x80, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x88, 0x00, /* "..3DV.. */
0xad, 0x86, 0x60, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ..`..... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x02, 0x01, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x01, 0x00              /* ...... */
};

/* Frame (134 bytes) */
static char pkt4[134] = {
0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x11, /* ........ */
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
0x00, 0x00, 0x00, 0x50, 0x3a, 0xff, 0xfe, 0x80, /* ...P:... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xfe, 0x80, /* "..3DV.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x04, 0x02, /* ........ */
0xf9, 0xc6, 0x00, 0x00, 0x00, 0x32, 0x60, 0x00, /* .....2`. */
0x00, 0x00, 0x00, 0x20, 0x3c, 0xff, 0xfe, 0x80, /* ... <... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0xfe, 0x80, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x3c, 0x00, /* "..3DV<. */
0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x00, /* ......:. */
0x87, 0x04, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, /* ........ */
0x09, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, /* ........ */
0x03, 0x04, 0x05, 0x06, 0x07, 0x08              /* ...... */
};

/* Frame (86 bytes) */
static char pkt5[86] = {
0x33, 0x33, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, /* 33...... */
0x00, 0x00, 0x01, 0x00, 0x86, 0xdd, 0x60, 0x00, /* ......`. */
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, /* ... :... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0xff, 0x02, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x88, 0x00, /* ........ */
0xcd, 0x04, 0x20, 0x00, 0x00, 0x00, 0xfe, 0x80, /* .. ..... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x02, 0x01, /* ........ */
0x00, 0x11, 0x33, 0x77, 0x55, 0x11              /* ..3wU. */
};

/* Frame (70 bytes) */
static char pkt6[70] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, /* .."3DV.. */
0x00, 0x00, 0x01, 0x00, 0x86, 0xdd, 0x60, 0x00, /* ......`. */
0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0xfe, 0x80, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x80, 0x00, /* "..3DV.. */
0x09, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, /* ........ */
0x03, 0x04, 0x05, 0x06, 0x07, 0x08              /* ...... */
};

/* Frame (70 bytes) */
static char pkt7[70] = {
0x00, 0x11, 0x33, 0x77, 0x55, 0x11, 0x00, 0x11, /* ..3wU... */
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xfe, 0x80, /* "..3DV.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x81, 0x00, /* ........ */
0x08, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, /* ........ */
0x03, 0x04, 0x05, 0x06, 0x07, 0x08              /* ...... */
};

/* Frame (86 bytes) */
static char pkt8[86] = {
0x00, 0x11, 0x33, 0x77, 0x55, 0x11, 0x00, 0x11, /* ..3wU... */
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, /* ... :... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xfe, 0x80, /* "..3DV.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x87, 0x00, /* ........ */
0xa9, 0xec, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x01, 0x01, /* ........ */
0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};

/* Frame (86 bytes) */
static char pkt9[86] = {
0x00, 0x11, 0x33, 0x77, 0x55, 0x11, 0x00, 0x11, /* ..3wU... */
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, /* ... :... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xfe, 0x80, /* "..3DV.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x87, 0x00, /* ........ */
0xa9, 0xec, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x01, 0x01, /* ........ */
0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};

/* Frame (86 bytes) */
static char pkt10[86] = {
0x00, 0x11, 0x33, 0x77, 0x55, 0x11, 0x00, 0x11, /* ..3wU... */
0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
0x00, 0x00, 0x00, 0x20, 0x3a, 0xff, 0xfe, 0x80, /* ... :... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xfe, 0x80, /* "..3DV.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x87, 0x00, /* ........ */
0xa9, 0xec, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, /* ........ */
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0x01, 0x01, /* ........ */
0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};






TAHI_TEST_SEQ tahi_01_014[] = {
    {TITLE, "01-014", 6, 0},
    {WAIT, NX_NULL, 0, 5},

    {INJECT, &pkt1[0], sizeof(pkt1), 0},
    {CHECK, &pkt2[0], sizeof(pkt2), 5},
    {INJECT, &pkt3[0], sizeof(pkt3), 0},
    {CHECK, &pkt4[0], sizeof(pkt4), 5},

    {INJECT, &pkt5[0], sizeof(pkt5), 0},
    {INJECT, &pkt6[0], sizeof(pkt6), 0},
    {CHECK, &pkt7[0], sizeof(pkt7), 10},
    //{WAIT, NX_NULL, 0, 10},
    {CHECK, &pkt8[0], sizeof(pkt8), 10},
    {CHECK, &pkt9[0], sizeof(pkt9), 5},
    {CHECK, &pkt10[0], sizeof(pkt10), 5},

    {CLEANUP, NX_NULL, 0, 0},
    {CLEAN_HOP_LIMIT, NX_NULL, 0, 0},
    {DUMP, NX_NULL, 0, 0}
};

int tahi_01_014_size = sizeof(tahi_01_014) / sizeof(TAHI_TEST_SEQ);

#endif /* NX_TAHI_ENABLE */