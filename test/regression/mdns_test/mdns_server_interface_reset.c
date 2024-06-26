
#include "nx_api.h"

#ifdef __PRODUCT_NETXDUO__

#include "netx_mdns_test.h"

/* Frame (287 bytes) */
static const unsigned char pkt1[287] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x11, 0x00, 0x01, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x9d, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xfd, /* ........ */
0x5b, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, /* [....... */
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x11, 0x53, 0x69, /* ......Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x0b, 0x41, 0x52, /* ......AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* t.local. */
0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, /* .......x */
0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x11, 0x53, /* .....B.S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .d...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x64, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* d...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

/* Frame (287 bytes) */
static const unsigned char pkt2[287] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x11, 0x00, 0x02, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x9c, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xfd, /* ........ */
0x5b, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, /* [....... */
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x11, 0x53, 0x69, /* ......Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x0b, 0x41, 0x52, /* ......AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* t.local. */
0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, /* .......x */
0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x11, 0x53, /* .....B.S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .d...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x64, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* d...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

/* Frame (287 bytes) */
static const unsigned char pkt3[287] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x11, 0x00, 0x03, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x9b, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xfd, /* ........ */
0x5b, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, /* [....... */
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x11, 0x53, 0x69, /* ......Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x0b, 0x41, 0x52, /* ......AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* t.local. */
0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, /* .......x */
0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x11, 0x53, /* .....B.S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .d...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x64, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* d...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

/* Frame (428 bytes) */
static const unsigned char pkt4[428] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x9e, 0x00, 0x04, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x0d, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x01, 0x8a, /* ........ */
0x40, 0x19, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* @....... */
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x0b, /* x.....B. */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* est.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x16, 0x0b, 0x41, 0x52, 0x4d, /* .x...ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x01, 0x40, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .@.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x21, /* local..! */
0x80, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x19, /* .....d.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0b, 0x41, /* .....P.A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* ..Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x10, 0x80, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x14, 0x08, /* ....d... */
0x70, 0x61, 0x70, 0x65, 0x72, 0x3d, 0x41, 0x34, /* paper=A4 */
0x0a, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, /* .version */
0x3d, 0x30, 0x31, 0x05, 0x5f, 0x68, 0x74, 0x74, /* =01._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x24, 0x11, /* ....d.$. */
0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, /* Simple W */
0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, /* eb Serve */
0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, /* r._http. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, /* al..Simp */
0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, /* le Web S */
0x65, 0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, /* erver._h */
0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ttp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x2f, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, /* /.....x. */
0x2b, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* +.Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x05, 0x00, /* ocal.... */
0x00, 0x80, 0x00, 0x40                          /* ...@ */
};

/* Frame (428 bytes) */
static const unsigned char pkt5[428] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x9e, 0x00, 0x05, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x0c, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x01, 0x8a, /* ........ */
0x40, 0x19, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* @....... */
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x0b, /* x.....B. */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* est.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x16, 0x0b, 0x41, 0x52, 0x4d, /* .x...ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x01, 0x40, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .@.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x21, /* local..! */
0x80, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x19, /* .....d.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0b, 0x41, /* .....P.A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* ..Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x10, 0x80, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x14, 0x08, /* ....d... */
0x70, 0x61, 0x70, 0x65, 0x72, 0x3d, 0x41, 0x34, /* paper=A4 */
0x0a, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, /* .version */
0x3d, 0x30, 0x31, 0x05, 0x5f, 0x68, 0x74, 0x74, /* =01._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x24, 0x11, /* ....d.$. */
0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, /* Simple W */
0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, /* eb Serve */
0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, /* r._http. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, /* al..Simp */
0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, /* le Web S */
0x65, 0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, /* erver._h */
0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ttp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x2f, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, /* /.....x. */
0x2b, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* +.Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x05, 0x00, /* ocal.... */
0x00, 0x80, 0x00, 0x40                          /* ...@ */
};

/* Frame (428 bytes) */
static const unsigned char pkt6[428] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x9e, 0x00, 0x06, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x0b, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x01, 0x8a, /* ........ */
0x40, 0x19, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* @....... */
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x0b, /* x.....B. */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* est.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x16, 0x0b, 0x41, 0x52, 0x4d, /* .x...ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x01, 0x40, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .@.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x21, /* local..! */
0x80, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x19, /* .....d.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0b, 0x41, /* .....P.A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* ..Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x10, 0x80, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x14, 0x08, /* ....d... */
0x70, 0x61, 0x70, 0x65, 0x72, 0x3d, 0x41, 0x34, /* paper=A4 */
0x0a, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, /* .version */
0x3d, 0x30, 0x31, 0x05, 0x5f, 0x68, 0x74, 0x74, /* =01._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x24, 0x11, /* ....d.$. */
0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, /* Simple W */
0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, /* eb Serve */
0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, /* r._http. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, /* al..Simp */
0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, /* le Web S */
0x65, 0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, /* erver._h */
0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ttp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x2f, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, /* /.....x. */
0x2b, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* +.Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x05, 0x00, /* ocal.... */
0x00, 0x80, 0x00, 0x40                          /* ...@ */
};

/* Frame (288 bytes) */
static const unsigned char pkt7[288] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x12, 0x00, 0x07, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x96, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xfe, /* ........ */
0x8f, 0x3f, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* .?...... */
0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x11, /* ......B. */
0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, /* Simple W */
0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, /* eb Serve */
0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, /* r._http. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, /* al..!... */
0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, /* ..P.ARMM */
0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, /* DNSTest. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, /* local..S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, /* l....... */
0x00, 0x00, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, /* .....pap */
0x65, 0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, /* er=A4.ve */
0x72, 0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31, /* rsion=01 */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, /* l....... */
0x00, 0x00, 0x00, 0x24, 0x11, 0x53, 0x69, 0x6d, /* ...$.Sim */
0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, 0x20, /* ple Web  */
0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, /* Server._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00  /* p.local. */
};

/* Frame (287 bytes) */
static const unsigned char pkt8[287] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x11, 0x00, 0x08, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x96, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xfd, /* ........ */
0x5b, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, /* [....... */
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x11, 0x53, 0x69, /* ......Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x0b, 0x41, 0x52, /* ......AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* t.local. */
0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, /* .......x */
0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x11, 0x53, /* .....B.S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .d...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x64, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* d...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

/* Frame (287 bytes) */
static const unsigned char pkt9[287] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x11, 0x00, 0x09, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x95, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xfd, /* ........ */
0x5b, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, /* [....... */
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x11, 0x53, 0x69, /* ......Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x0b, 0x41, 0x52, /* ......AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* t.local. */
0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, /* .......x */
0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x11, 0x53, /* .....B.S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .d...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x64, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* d...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

/* Frame (287 bytes) */
static const unsigned char pkt10[287] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x11, 0x00, 0x0a, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x94, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xfd, /* ........ */
0x5b, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, /* [....... */
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x11, 0x53, 0x69, /* ......Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0xff, 0x00, 0x01, 0x0b, 0x41, 0x52, /* ......AR */
0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, /* MMDNSTes */
0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* t.local. */
0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, /* .......x */
0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x11, 0x53, /* .....B.S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x64, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .d...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x64, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* d...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

/* Frame (428 bytes) */
static const unsigned char pkt11[428] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x9e, 0x00, 0x0b, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x06, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x01, 0x8a, /* ........ */
0x40, 0x19, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* @....... */
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x0b, /* x.....B. */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* est.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x16, 0x0b, 0x41, 0x52, 0x4d, /* .x...ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x01, 0x40, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .@.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x21, /* local..! */
0x80, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x19, /* .....d.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0b, 0x41, /* .....P.A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* ..Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x10, 0x80, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x14, 0x08, /* ....d... */
0x70, 0x61, 0x70, 0x65, 0x72, 0x3d, 0x41, 0x34, /* paper=A4 */
0x0a, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, /* .version */
0x3d, 0x30, 0x31, 0x05, 0x5f, 0x68, 0x74, 0x74, /* =01._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x24, 0x11, /* ....d.$. */
0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, /* Simple W */
0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, /* eb Serve */
0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, /* r._http. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, /* al..Simp */
0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, /* le Web S */
0x65, 0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, /* erver._h */
0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ttp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x2f, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, /* /.....x. */
0x2b, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* +.Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x05, 0x00, /* ocal.... */
0x00, 0x80, 0x00, 0x40                          /* ...@ */
};

/* Frame (428 bytes) */
static const unsigned char pkt12[428] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x9e, 0x00, 0x0c, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x05, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x01, 0x8a, /* ........ */
0x40, 0x19, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* @....... */
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x0b, /* x.....B. */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* est.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x16, 0x0b, 0x41, 0x52, 0x4d, /* .x...ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x01, 0x40, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .@.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x21, /* local..! */
0x80, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x19, /* .....d.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0b, 0x41, /* .....P.A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* ..Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x10, 0x80, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x14, 0x08, /* ....d... */
0x70, 0x61, 0x70, 0x65, 0x72, 0x3d, 0x41, 0x34, /* paper=A4 */
0x0a, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, /* .version */
0x3d, 0x30, 0x31, 0x05, 0x5f, 0x68, 0x74, 0x74, /* =01._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x24, 0x11, /* ....d.$. */
0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, /* Simple W */
0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, /* eb Serve */
0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, /* r._http. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, /* al..Simp */
0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, /* le Web S */
0x65, 0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, /* erver._h */
0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ttp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x2f, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, /* /.....x. */
0x2b, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* +.Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x05, 0x00, /* ocal.... */
0x00, 0x80, 0x00, 0x40                          /* ...@ */
};

/* Frame (428 bytes) */
static const unsigned char pkt13[428] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x01, 0x9e, 0x00, 0x0d, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0x04, 0x0a, 0x00, 0x00, 0x42, 0xe0, 0x00, /* .....B.. */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x01, 0x8a, /* ........ */
0x40, 0x19, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* @....... */
0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x41, /* .......A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x42, 0x0b, /* x.....B. */
0x41, 0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, /* ARMMDNST */
0x65, 0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* est.loca */
0x6c, 0x00, 0x00, 0x2f, 0x80, 0x01, 0x00, 0x00, /* l../.... */
0x00, 0x78, 0x00, 0x16, 0x0b, 0x41, 0x52, 0x4d, /* .x...ARM */
0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, /* MDNSTest */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x01, 0x40, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .@.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x21, /* local..! */
0x80, 0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x19, /* .....d.. */
0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x0b, 0x41, /* .....P.A */
0x52, 0x4d, 0x4d, 0x44, 0x4e, 0x53, 0x54, 0x65, /* RMMDNSTe */
0x73, 0x74, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* st.local */
0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* ..Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x10, 0x80, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x14, 0x08, /* ....d... */
0x70, 0x61, 0x70, 0x65, 0x72, 0x3d, 0x41, 0x34, /* paper=A4 */
0x0a, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, /* .version */
0x3d, 0x30, 0x31, 0x05, 0x5f, 0x68, 0x74, 0x74, /* =01._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x0c, 0x00, /* ocal.... */
0x01, 0x00, 0x00, 0x00, 0x64, 0x00, 0x24, 0x11, /* ....d.$. */
0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, /* Simple W */
0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, /* eb Serve */
0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, /* r._http. */
0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, /* _tcp.loc */
0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, 0x6d, 0x70, /* al..Simp */
0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, /* le Web S */
0x65, 0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, /* erver._h */
0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, /* ttp._tcp */
0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, /* .local.. */
0x2f, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, /* /.....x. */
0x2b, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, /* +.Simple */
0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, /*  Web Ser */
0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, 0x74, /* ver._htt */
0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, 0x6c, /* p._tcp.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x00, 0x05, 0x00, /* ocal.... */
0x00, 0x80, 0x00, 0x40                          /* ...@ */
};

static MDNS_SERVICE mdns_service = {"Simple Web Server", "_http._tcp", NX_NULL, "paper=A4;version=01", 100, 0, 0, 80, NX_MDNS_RR_SET_UNIQUE, 0};


MDNS_TEST_SEQ mdns_server_interface_reset[] = {
    {TITLE, "Server interface reset", 22, 0},
    {MDNS_SERVICE_ADD, (char*)&mdns_service, 0, 0},

    /* Check probing and announcement. */
    {MDNS_CHECK_DATA_V4, (char*)&pkt1[0], sizeof(pkt1), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt2[0], sizeof(pkt2), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt3[0], sizeof(pkt3), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt4[0], sizeof(pkt4), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt5[0], sizeof(pkt5), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt6[0], sizeof(pkt6), 2},

    /* Disable the interface. */
    {MDNS_INTERFACE_DISABLE, NX_NULL, 0, 0},
    {MDNS_CHECK_DATA_V4, (char*)&pkt7[0], sizeof(pkt7), 1},

    /* Enable the interface. */
    {MDNS_INTERFACE_ENABLE, NX_NULL, 0, 0},

    /* Check probing and announcement. */
    {MDNS_CHECK_DATA_V4, (char*)&pkt8[0], sizeof(pkt8), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt9[0], sizeof(pkt9), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt10[0], sizeof(pkt10), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt11[0], sizeof(pkt11), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt12[0], sizeof(pkt12), 1},
    {MDNS_CHECK_DATA_V4, (char*)&pkt13[0], sizeof(pkt13), 2},
};

int mdns_server_interface_reset_size = sizeof(mdns_server_interface_reset) / sizeof(MDNS_TEST_SEQ);

#endif /* __PRODUCT_NETXDUO__  */

