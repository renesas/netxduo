/* Frame (76 bytes) */
static const unsigned char pkt1[76] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x0c, /* ..^..... */
0x29, 0x01, 0xd4, 0x8d, 0x08, 0x00, 0x45, 0x00, /* ).....E. */
0x00, 0x3e, 0x00, 0x00, 0x40, 0x00, 0xff, 0x11, /* .>..@... */
0x90, 0xb2, 0x0a, 0x00, 0x00, 0x01, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x2a, /* .......* */
0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01                          /* .... */
};

/* Frame (255 bytes) */
static const unsigned char pkt2[255] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x00, 0xf1, 0x00, 0x08, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0xd9, 0x0a, 0x00, 0x00, 0x1f, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xdd, /* ........ */
0x19, 0x0d, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3c, /* .......< */
0x00, 0x24, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .$.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, /* local..S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x3c, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .<...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x3c, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* <...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

/* Frame (108 bytes) */
static const unsigned char pkt3[108] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x0c, /* ..^..... */
0x29, 0x01, 0xd4, 0x8d, 0x08, 0x00, 0x45, 0x00, /* ).....E. */
0x00, 0x5e, 0x00, 0x00, 0x40, 0x00, 0xff, 0x11, /* .^..@... */
0x90, 0x92, 0x0a, 0x00, 0x00, 0x01, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x4a, /* .......J */
0x2e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x0c, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x14, /* .....<.. */
0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, /* .Simple  */
0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, /* Web Serv */
0x65, 0x72, 0xc0, 0x0c                          /* er.. */
};

/* Frame (108 bytes) */
static const unsigned char pkt4[108] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x0c, /* ..^..... */
0x29, 0x01, 0xd4, 0x8d, 0x08, 0x00, 0x45, 0x00, /* ).....E. */
0x00, 0x5e, 0x00, 0x00, 0x40, 0x00, 0xff, 0x11, /* .^..@... */
0x90, 0x92, 0x0a, 0x00, 0x00, 0x01, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x4a, /* .......J */
0x2e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x0c, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x14, /* .....<.. */
0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, /* .Simple  */
0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, /* Web Serv */
0x65, 0x72, 0xc0, 0x0c                          /* er.. */
};

/* Frame (108 bytes) */
static const unsigned char pkt5[108] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x0c, /* ..^..... */
0x29, 0x01, 0xd4, 0x8d, 0x08, 0x00, 0x45, 0x00, /* ).....E. */
0x00, 0x5e, 0x00, 0x00, 0x40, 0x00, 0xff, 0x11, /* .^..@... */
0x90, 0x92, 0x0a, 0x00, 0x00, 0x01, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x4a, /* .......J */
0x2e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x0c, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x14, /* .....<.. */
0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, /* .Simple  */
0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, /* Web Serv */
0x65, 0x72, 0xc0, 0x0c                          /* er.. */
};

/* Frame (108 bytes) */
static const unsigned char pkt6[108] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x0c, /* ..^..... */
0x29, 0x01, 0xd4, 0x8d, 0x08, 0x00, 0x45, 0x00, /* ).....E. */
0x00, 0x5e, 0x00, 0x00, 0x40, 0x00, 0xff, 0x11, /* .^..@... */
0x90, 0x92, 0x0a, 0x00, 0x00, 0x01, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x4a, /* .......J */
0x2e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x0c, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x14, /* .....<.. */
0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, /* .Simple  */
0x57, 0x65, 0x62, 0x20, 0x53, 0x65, 0x72, 0x76, /* Web Serv */
0x65, 0x72, 0xc0, 0x0c                          /* er.. */
};

/* Frame (76 bytes) */
static const unsigned char pkt7[76] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x0c, /* ..^..... */
0x29, 0x01, 0xd4, 0x8d, 0x08, 0x00, 0x45, 0x00, /* ).....E. */
0x00, 0x3e, 0x00, 0x00, 0x40, 0x00, 0xff, 0x11, /* .>..@... */
0x90, 0xb2, 0x0a, 0x00, 0x00, 0x01, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0x2a, /* .......* */
0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01                          /* .... */
};

/* Frame (255 bytes) */
static const unsigned char pkt8[255] = {
0x01, 0x00, 0x5e, 0x00, 0x00, 0xfb, 0x00, 0x11, /* ..^..... */
0x22, 0x33, 0x44, 0x57, 0x08, 0x00, 0x45, 0x00, /* "3DW..E. */
0x00, 0xf1, 0x00, 0x09, 0x40, 0x00, 0xff, 0x11, /* ....@... */
0x8f, 0xd8, 0x0a, 0x00, 0x00, 0x1f, 0xe0, 0x00, /* ........ */
0x00, 0xfb, 0x14, 0xe9, 0x14, 0xe9, 0x00, 0xdd, /* ........ */
0x19, 0x0d, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x05, 0x5f, /* ......._ */
0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, /* http._tc */
0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, /* p.local. */
0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3c, /* .......< */
0x00, 0x24, 0x11, 0x53, 0x69, 0x6d, 0x70, 0x6c, /* .$.Simpl */
0x65, 0x20, 0x57, 0x65, 0x62, 0x20, 0x53, 0x65, /* e Web Se */
0x72, 0x76, 0x65, 0x72, 0x05, 0x5f, 0x68, 0x74, /* rver._ht */
0x74, 0x70, 0x04, 0x5f, 0x74, 0x63, 0x70, 0x05, /* tp._tcp. */
0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, /* local..S */
0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, /* imple We */
0x62, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, /* b Server */
0x05, 0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, /* ._http._ */
0x74, 0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, /* tcp.loca */
0x6c, 0x00, 0x00, 0x21, 0x80, 0x01, 0x00, 0x00, /* l..!.... */
0x00, 0x3c, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, /* .<...... */
0x00, 0x50, 0x0b, 0x41, 0x52, 0x4d, 0x4d, 0x44, /* .P.ARMMD */
0x4e, 0x53, 0x54, 0x65, 0x73, 0x74, 0x05, 0x6c, /* NSTest.l */
0x6f, 0x63, 0x61, 0x6c, 0x00, 0x11, 0x53, 0x69, /* ocal..Si */
0x6d, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x65, 0x62, /* mple Web */
0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x05, /*  Server. */
0x5f, 0x68, 0x74, 0x74, 0x70, 0x04, 0x5f, 0x74, /* _http._t */
0x63, 0x70, 0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c, /* cp.local */
0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, /* ........ */
0x3c, 0x00, 0x14, 0x08, 0x70, 0x61, 0x70, 0x65, /* <...pape */
0x72, 0x3d, 0x41, 0x34, 0x0a, 0x76, 0x65, 0x72, /* r=A4.ver */
0x73, 0x69, 0x6f, 0x6e, 0x3d, 0x30, 0x31        /* sion=01 */
};

