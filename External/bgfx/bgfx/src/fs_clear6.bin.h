static const uint8_t fs_clear6_glsl[374] =
{
	0x46, 0x53, 0x48, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x62, // FSH............b
	0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x02, // gfx_clear_color.
	0x08, 0x00, 0x00, 0x08, 0x00, 0x4c, 0x01, 0x00, 0x00, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, // .....L...uniform
	0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, //  vec4 bgfx_clear
	0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x38, 0x5d, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, // _color[8];.void 
	0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, // main ().{.  gl_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x5b, 0x30, 0x5d, 0x20, 0x3d, 0x20, 0x62, 0x67, 0x66, // ragData[0] = bgf
	0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x30, 0x5d, // x_clear_color[0]
	0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x5b, // ;.  gl_FragData[
	0x31, 0x5d, 0x20, 0x3d, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, // 1] = bgfx_clear_
	0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x31, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, // color[1];.  gl_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x5b, 0x32, 0x5d, 0x20, 0x3d, 0x20, 0x62, 0x67, 0x66, // ragData[2] = bgf
	0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x32, 0x5d, // x_clear_color[2]
	0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x5b, // ;.  gl_FragData[
	0x33, 0x5d, 0x20, 0x3d, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, // 3] = bgfx_clear_
	0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x33, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, // color[3];.  gl_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x5b, 0x34, 0x5d, 0x20, 0x3d, 0x20, 0x62, 0x67, 0x66, // ragData[4] = bgf
	0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x34, 0x5d, // x_clear_color[4]
	0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x5b, // ;.  gl_FragData[
	0x35, 0x5d, 0x20, 0x3d, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, // 5] = bgfx_clear_
	0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x35, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, // color[5];.  gl_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x5b, 0x36, 0x5d, 0x20, 0x3d, 0x20, 0x62, 0x67, 0x66, // ragData[6] = bgf
	0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x36, 0x5d, // x_clear_color[6]
	0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                                                             // ;.}...
};
static const uint8_t fs_clear6_spv[1437] =
{
	0x46, 0x53, 0x48, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x62, // FSH............b
	0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x12, // gfx_clear_color.
	0x07, 0x00, 0x00, 0x07, 0x00, 0x70, 0x05, 0x00, 0x00, 0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, // .....p.....#....
	0x00, 0x08, 0x00, 0x08, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, // ................
	0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53, // .............GLS
	0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, // L.std.450.......
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x00, // ................
	0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, // .....main....W..
	0x00, 0x59, 0x00, 0x00, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, // .Y...[...]..._..
	0x00, 0x61, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, // .a...c..........
	0x00, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0xf4, 0x01, 0x00, // ................
	0x00, 0x05, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, // .........main...
	0x00, 0x05, 0x00, 0x06, 0x00, 0x26, 0x00, 0x00, 0x00, 0x55, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, // .....&...Uniform
	0x42, 0x6c, 0x6f, 0x63, 0x6b, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x26, 0x00, 0x00, // Block........&..
	0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, // .....bgfx_clear_
	0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00, 0x28, 0x00, 0x00, // color........(..
	0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x57, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, // .........W...bgf
	0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x00, 0x00, 0x05, 0x00, 0x06, // x_FragData0.....
	0x00, 0x59, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, // .Y...bgfx_FragDa
	0x74, 0x61, 0x31, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, // ta1......[...bgf
	0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x32, 0x00, 0x00, 0x05, 0x00, 0x06, // x_FragData2.....
	0x00, 0x5d, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, // .]...bgfx_FragDa
	0x74, 0x61, 0x33, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x5f, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, // ta3......_...bgf
	0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x34, 0x00, 0x00, 0x05, 0x00, 0x06, // x_FragData4.....
	0x00, 0x61, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, // .a...bgfx_FragDa
	0x74, 0x61, 0x35, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x63, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, // ta5......c...bgf
	0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x36, 0x00, 0x00, 0x47, 0x00, 0x04, // x_FragData6..G..
	0x00, 0x25, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, // .%...........H..
	0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // .&.......#......
	0x00, 0x47, 0x00, 0x03, 0x00, 0x26, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .G...&.......G..
	0x00, 0x28, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .(...".......G..
	0x00, 0x28, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .(...!...0...G..
	0x00, 0x57, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .W...........G..
	0x00, 0x59, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .Y...........G..
	0x00, 0x5b, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .[...........G..
	0x00, 0x5d, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .]...........G..
	0x00, 0x5f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // ._...........G..
	0x00, 0x61, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, // .a...........G..
	0x00, 0x63, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, // .c..............
	0x00, 0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, // .....!..........
	0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, // ......... ......
	0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, // ................
	0x00, 0x23, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, // .#... .......+..
	0x00, 0x23, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x04, // .#...$..........
	0x00, 0x25, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x03, // .%.......$......
	0x00, 0x26, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x27, 0x00, 0x00, // .&...%... ...'..
	0x00, 0x02, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x27, 0x00, 0x00, // .....&...;...'..
	0x00, 0x28, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // .(...........)..
	0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // . .......+...)..
	0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x2b, 0x00, 0x00, // .*....... ...+..
	0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // .........+...)..
	0x00, 0x2e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // .........+...)..
	0x00, 0x31, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // .1.......+...)..
	0x00, 0x34, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // .4.......+...)..
	0x00, 0x37, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // .7.......+...)..
	0x00, 0x3a, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x29, 0x00, 0x00, // .:.......+...)..
	0x00, 0x3d, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // .=....... ...V..
	0x00, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // .........;...V..
	0x00, 0x57, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // .W.......;...V..
	0x00, 0x59, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // .Y.......;...V..
	0x00, 0x5b, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // .[.......;...V..
	0x00, 0x5d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // .].......;...V..
	0x00, 0x5f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // ._.......;...V..
	0x00, 0x61, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x56, 0x00, 0x00, // .a.......;...V..
	0x00, 0x63, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, // .c.......6......
	0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, // ................
	0x00, 0x05, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x7a, 0x00, 0x00, // .....A...+...z..
	0x00, 0x28, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, // .(...*...*...=..
	0x00, 0x08, 0x00, 0x00, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x7a, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, // .....{...z...A..
	0x00, 0x2b, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, // .+...|...(...*..
	0x00, 0x2e, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x7d, 0x00, 0x00, // .....=.......}..
	0x00, 0x7c, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, // .|...A...+...~..
	0x00, 0x28, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, // .(...*...1...=..
	0x00, 0x08, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, // .........~...A..
	0x00, 0x2b, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, // .+.......(...*..
	0x00, 0x34, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, // .4...=..........
	0x00, 0x80, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, // .....A...+......
	0x00, 0x28, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, // .(...*...7...=..
	0x00, 0x08, 0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, // .............A..
	0x00, 0x2b, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, // .+.......(...*..
	0x00, 0x3a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x85, 0x00, 0x00, // .:...=..........
	0x00, 0x84, 0x00, 0x00, 0x00, 0x41, 0x00, 0x06, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, // .....A...+......
	0x00, 0x28, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, // .(...*...=...=..
	0x00, 0x08, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, // .............>..
	0x00, 0x57, 0x00, 0x00, 0x00, 0x7b, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x59, 0x00, 0x00, // .W...{...>...Y..
	0x00, 0x7d, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, // .}...>...[......
	0x00, 0x3e, 0x00, 0x03, 0x00, 0x5d, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, // .>...].......>..
	0x00, 0x5f, 0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x61, 0x00, 0x00, // ._.......>...a..
	0x00, 0x85, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x63, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, // .....>...c......
	0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00, 0x00, 0x00, 0x70, 0x00,                   // .....8.....p.
};
static const uint8_t fs_clear6_dx9[238] =
{
	0x46, 0x53, 0x48, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x62, // FSH............b
	0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x12, // gfx_clear_color.
	0x08, 0x00, 0x00, 0x04, 0x00, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xfe, 0xff, 0x22, // ..............."
	0x00, 0x43, 0x54, 0x41, 0x42, 0x1c, 0x00, 0x00, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, // .CTAB....[......
	0xff, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x54, 0x00, 0x00, // .............T..
	0x00, 0x30, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, // .0...........D..
	0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, // .....bgfx_clear_
	0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0xab, 0xab, 0xab, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x04, // color...........
	0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x73, 0x5f, 0x33, 0x5f, 0x30, 0x00, // .........ps_3_0.
	0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, // Microsoft (R) HL
	0x53, 0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, // SL Shader Compil
	0x65, 0x72, 0x20, 0x31, 0x30, 0x2e, 0x31, 0x00, 0xab, 0x01, 0x00, 0x00, 0x02, 0x00, 0x08, 0x0f, // er 10.1.........
	0x80, 0x00, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0x00, 0x02, 0x01, 0x08, 0x0f, 0x80, 0x01, 0x00, 0xe4, // ................
	0xa0, 0x01, 0x00, 0x00, 0x02, 0x02, 0x08, 0x0f, 0x80, 0x02, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0x00, // ................
	0x02, 0x03, 0x08, 0x0f, 0x80, 0x03, 0x00, 0xe4, 0xa0, 0xff, 0xff, 0x00, 0x00, 0x00,             // ..............
};
static const uint8_t fs_clear6_dx11[625] =
{
	0x46, 0x53, 0x48, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x62, // FSH............b
	0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x12, // gfx_clear_color.
	0x08, 0x00, 0x00, 0x08, 0x00, 0x44, 0x02, 0x00, 0x00, 0x44, 0x58, 0x42, 0x43, 0x68, 0xe2, 0x88, // .....D...DXBCh..
	0x87, 0x2b, 0x8c, 0x92, 0xbc, 0x98, 0x11, 0xb6, 0x94, 0x5c, 0x76, 0x9a, 0x47, 0x01, 0x00, 0x00, // .+........v.G...
	0x00, 0x44, 0x02, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, // .D.......,...`..
	0x00, 0x24, 0x01, 0x00, 0x00, 0x49, 0x53, 0x47, 0x4e, 0x2c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, // .$...ISGN,......
	0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, // ..... ..........
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5f, // .............SV_
	0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x4f, 0x53, 0x47, 0x4e, 0xbc, 0x00, 0x00, // POSITION.OSGN...
	0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, // ................
	0x00, 0xb0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, // ................
	0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, // ................
	0x00, 0xb0, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, // ................
	0x00, 0x03, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, // ................
	0x00, 0xb0, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, // ................
	0x00, 0x05, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, // ................
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, // ................
	0x00, 0x53, 0x56, 0x5f, 0x54, 0x41, 0x52, 0x47, 0x45, 0x54, 0x00, 0xab, 0xab, 0x53, 0x48, 0x44, // .SV_TARGET...SHD
	0x52, 0x18, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, // R....@...F...Y..
	0x04, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, // .F. .........e..
	0x03, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, // .. ......e.... .
	0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x02, 0x00, 0x00, // .....e.... .....
	0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x03, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, // .e.... ......e..
	0x03, 0xf2, 0x20, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, // .. ......e.... .
	0x00, 0x05, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x06, 0x00, 0x00, // .....e.... .....
	0x00, 0x36, 0x00, 0x00, 0x06, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, // .6.... ......F. 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x06, 0xf2, 0x20, 0x10, // .........6.... .
	0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, // .....F. ........
	0x00, 0x36, 0x00, 0x00, 0x06, 0xf2, 0x20, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, // .6.... ......F. 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x06, 0xf2, 0x20, 0x10, // .........6.... .
	0x00, 0x03, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, // .....F. ........
	0x00, 0x36, 0x00, 0x00, 0x06, 0xf2, 0x20, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, // .6.... ......F. 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x06, 0xf2, 0x20, 0x10, // .........6.... .
	0x00, 0x05, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, // .....F. ........
	0x00, 0x36, 0x00, 0x00, 0x06, 0xf2, 0x20, 0x10, 0x00, 0x06, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, // .6.... ......F. 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, // .........>......
	0x00,                                                                                           // .
};
static const uint8_t fs_clear6_mtl[978] =
{
	0x46, 0x53, 0x48, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x62, // FSH............b
	0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x12, // gfx_clear_color.
	0x07, 0x00, 0x00, 0x07, 0x00, 0xa5, 0x03, 0x00, 0x00, 0x23, 0x69, 0x6e, 0x63, 0x6c, 0x75, 0x64, // .........#includ
	0x65, 0x20, 0x3c, 0x6d, 0x65, 0x74, 0x61, 0x6c, 0x5f, 0x73, 0x74, 0x64, 0x6c, 0x69, 0x62, 0x3e, // e <metal_stdlib>
	0x0a, 0x23, 0x69, 0x6e, 0x63, 0x6c, 0x75, 0x64, 0x65, 0x20, 0x3c, 0x73, 0x69, 0x6d, 0x64, 0x2f, // .#include <simd/
	0x73, 0x69, 0x6d, 0x64, 0x2e, 0x68, 0x3e, 0x0a, 0x0a, 0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x6e, // simd.h>..using n
	0x61, 0x6d, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x20, 0x6d, 0x65, 0x74, 0x61, 0x6c, 0x3b, 0x0a, // amespace metal;.
	0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x5f, 0x47, 0x6c, 0x6f, 0x62, 0x61, 0x6c, 0x0a, // .struct _Global.
	0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x62, 0x67, 0x66, // {.    float4 bgf
	0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x38, 0x5d, // x_clear_color[8]
	0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x20, 0x78, 0x6c, 0x61, // ;.};..struct xla
	0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x0a, 0x7b, 0x0a, 0x20, // tMtlMain_out.{. 
	0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, //    float4 bgfx_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, // ragData0 [[color
	0x28, 0x30, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, // (0)]];.    float
	0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x31, // 4 bgfx_FragData1
	0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x28, 0x31, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, //  [[color(1)]];. 
	0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, //    float4 bgfx_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x32, 0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, // ragData2 [[color
	0x28, 0x32, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, // (2)]];.    float
	0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x33, // 4 bgfx_FragData3
	0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x28, 0x33, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, //  [[color(3)]];. 
	0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, //    float4 bgfx_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x34, 0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, // ragData4 [[color
	0x28, 0x34, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, // (4)]];.    float
	0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x35, // 4 bgfx_FragData5
	0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x28, 0x35, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x20, //  [[color(5)]];. 
	0x20, 0x20, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x34, 0x20, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, //    float4 bgfx_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x36, 0x20, 0x5b, 0x5b, 0x63, 0x6f, 0x6c, 0x6f, 0x72, // ragData6 [[color
	0x28, 0x36, 0x29, 0x5d, 0x5d, 0x3b, 0x0a, 0x7d, 0x3b, 0x0a, 0x0a, 0x66, 0x72, 0x61, 0x67, 0x6d, // (6)]];.};..fragm
	0x65, 0x6e, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, // ent xlatMtlMain_
	0x6f, 0x75, 0x74, 0x20, 0x78, 0x6c, 0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x28, // out xlatMtlMain(
	0x63, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x20, 0x5f, 0x47, 0x6c, 0x6f, 0x62, 0x61, 0x6c, // constant _Global
	0x26, 0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x20, 0x5b, 0x5b, 0x62, 0x75, 0x66, 0x66, 0x65, // & _mtl_u [[buffe
	0x72, 0x28, 0x30, 0x29, 0x5d, 0x5d, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x78, 0x6c, // r(0)]]).{.    xl
	0x61, 0x74, 0x4d, 0x74, 0x6c, 0x4d, 0x61, 0x69, 0x6e, 0x5f, 0x6f, 0x75, 0x74, 0x20, 0x6f, 0x75, // atMtlMain_out ou
	0x74, 0x20, 0x3d, 0x20, 0x7b, 0x7d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, // t = {};.    out.
	0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x30, 0x20, 0x3d, // bgfx_FragData0 =
	0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, //  _mtl_u.bgfx_cle
	0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x30, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, // ar_color[0];.   
	0x20, 0x6f, 0x75, 0x74, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, //  out.bgfx_FragDa
	0x74, 0x61, 0x31, 0x20, 0x3d, 0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x2e, 0x62, 0x67, 0x66, // ta1 = _mtl_u.bgf
	0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x31, 0x5d, // x_clear_color[1]
	0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, // ;.    out.bgfx_F
	0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x32, 0x20, 0x3d, 0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, // ragData2 = _mtl_
	0x75, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, // u.bgfx_clear_col
	0x6f, 0x72, 0x5b, 0x32, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x62, // or[2];.    out.b
	0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x33, 0x20, 0x3d, 0x20, // gfx_FragData3 = 
	0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, // _mtl_u.bgfx_clea
	0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x33, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, // r_color[3];.    
	0x6f, 0x75, 0x74, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, // out.bgfx_FragDat
	0x61, 0x34, 0x20, 0x3d, 0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x2e, 0x62, 0x67, 0x66, 0x78, // a4 = _mtl_u.bgfx
	0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x34, 0x5d, 0x3b, // _clear_color[4];
	0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x46, 0x72, // .    out.bgfx_Fr
	0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x35, 0x20, 0x3d, 0x20, 0x5f, 0x6d, 0x74, 0x6c, 0x5f, 0x75, // agData5 = _mtl_u
	0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, // .bgfx_clear_colo
	0x72, 0x5b, 0x35, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x6f, 0x75, 0x74, 0x2e, 0x62, 0x67, // r[5];.    out.bg
	0x66, 0x78, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x44, 0x61, 0x74, 0x61, 0x36, 0x20, 0x3d, 0x20, 0x5f, // fx_FragData6 = _
	0x6d, 0x74, 0x6c, 0x5f, 0x75, 0x2e, 0x62, 0x67, 0x66, 0x78, 0x5f, 0x63, 0x6c, 0x65, 0x61, 0x72, // mtl_u.bgfx_clear
	0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x5b, 0x36, 0x5d, 0x3b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x72, // _color[6];.    r
	0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x6f, 0x75, 0x74, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00, 0x00, // eturn out;.}....
	0x70, 0x00,                                                                                     // p.
};
extern const uint8_t* fs_clear6_pssl;
extern const uint32_t fs_clear6_pssl_size;
