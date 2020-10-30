static const uint8_t vs_basic_dx9[317] =
{
	0x56, 0x53, 0x48, 0x06, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x8b, 0xef, 0x49, 0x01, 0x00, 0x0f, 0x75, // VSH........I...u
	0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01, // _modelViewProj..
	0x00, 0x00, 0x04, 0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, 0xfe, 0xff, 0x21, 0x00, // ..............!.
	0x43, 0x54, 0x41, 0x42, 0x1c, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, // CTAB....W.......
	0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, // ............P...
	0x30, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, // 0...........@...
	0x00, 0x00, 0x00, 0x00, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, // ....u_modelViewP
	0x72, 0x6f, 0x6a, 0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, // roj.............
	0x00, 0x00, 0x00, 0x00, 0x76, 0x73, 0x5f, 0x33, 0x5f, 0x30, 0x00, 0x4d, 0x69, 0x63, 0x72, 0x6f, // ....vs_3_0.Micro
	0x73, 0x6f, 0x66, 0x74, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, 0x53, 0x4c, 0x20, 0x53, 0x68, // soft (R) HLSL Sh
	0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x72, 0x20, 0x31, 0x30, // ader Compiler 10
	0x2e, 0x31, 0x00, 0xab, 0x1f, 0x00, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0f, 0x90, // .1..............
	0x1f, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x0f, 0x90, 0x1f, 0x00, 0x00, 0x02, // ................
	0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0f, 0xe0, 0x1f, 0x00, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x80, // ................
	0x01, 0x00, 0x0f, 0xe0, 0x05, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0f, 0x80, 0x01, 0x00, 0xe4, 0xa0, // ................
	0x01, 0x00, 0x55, 0x90, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0xe4, 0xa0, // ..U.............
	0x01, 0x00, 0x00, 0x90, 0x00, 0x00, 0xe4, 0x80, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x0f, 0x80, // ................
	0x02, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0xaa, 0x90, 0x00, 0x00, 0xe4, 0x80, 0x02, 0x00, 0x00, 0x03, // ................
	0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0xe4, 0x80, 0x03, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0x00, 0x02, // ................
	0x01, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0xe4, 0x90, 0xff, 0xff, 0x00, 0x00, 0x00,                   // .............
};
static const uint8_t vs_basic_dx11[516] =
{
	0x56, 0x53, 0x48, 0x06, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x8b, 0xef, 0x49, 0x01, 0x00, 0x0f, 0x75, // VSH........I...u
	0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x00, // _modelViewProj..
	0x00, 0x00, 0x04, 0x00, 0xd4, 0x01, 0x00, 0x00, 0x44, 0x58, 0x42, 0x43, 0x32, 0x9b, 0xdd, 0xb5, // ........DXBC2...
	0xa9, 0xb7, 0x22, 0xf0, 0xcf, 0x5e, 0x34, 0x2c, 0x72, 0xf0, 0x87, 0x00, 0x01, 0x00, 0x00, 0x00, // .."..^4,r.......
	0xd4, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, // ........,...|...
	0xd0, 0x00, 0x00, 0x00, 0x49, 0x53, 0x47, 0x4e, 0x48, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ....ISGNH.......
	0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ....8...........
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, // ............>...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ................
	0x07, 0x07, 0x00, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, // ....COLOR.POSITI
	0x4f, 0x4e, 0x00, 0xab, 0x4f, 0x53, 0x47, 0x4e, 0x4c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, // ON..OSGNL.......
	0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ....8...........
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, // ............D...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // ................
	0x0f, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, // ....SV_POSITION.
	0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0xab, 0xab, 0x53, 0x48, 0x44, 0x52, 0xfc, 0x00, 0x00, 0x00, // COLOR...SHDR....
	0x40, 0x00, 0x01, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x04, 0x46, 0x8e, 0x20, 0x00, // @...?...Y...F. .
	0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0xf2, 0x10, 0x10, 0x00, // ........_.......
	0x00, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0x72, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, // ...._...r.......
	0x67, 0x00, 0x00, 0x04, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // g.... ..........
	0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, // e.... ......h...
	0x01, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x08, 0xf2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ....8...........
	0x56, 0x15, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, // V.......F. .....
	0x01, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x0a, 0xf2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ....2...........
	0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x10, 0x10, 0x00, // F. .............
	0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x0a, // ....F.......2...
	0xf2, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, // ........F. .....
	0x02, 0x00, 0x00, 0x00, 0xa6, 0x1a, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, // ............F...
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, // ......... ......
	0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, // F.......F. .....
	0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0xf2, 0x20, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, // ....6.... ......
	0x46, 0x1e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x01, 0x00, 0x02, 0x05, 0x00, // F.......>.......
	0x01, 0x00, 0x40, 0x00,                                                                         // ..@.
};
