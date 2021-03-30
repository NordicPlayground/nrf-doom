
#define FT810_CMD_WAKEUP 0x00
#define FT810_CMD_STANDBY 0x41
#define FT810_CMD_SLEEP 0x42
#define FT810_CMD_PWRDOWN 0x43
#define FT810_CMD_CLKEXT 0x44
#define FT810_CMD_CLKINT 0x44
#define FT810_CMD_CLKSEL 0x61
#define FT810_CMD_RST 0x68

// #define LOW_FREQ_BOUND  58800000L//98% of 60Mhz

// #define FT81X_VERSION "1.0.4"

// #define ADC_DIFFERENTIAL     1UL
// #define ADC_SINGLE_ENDED     0UL
// #define ADPCM_SAMPLES        2UL
// #define ALWAYS               7UL
// #define BARGRAPH             11UL
#define BITMAPS              1UL

#define BORDER               0UL

// #define CMDBUF_SIZE          4096UL
// #define CMD_APPEND           4294967070UL
// #define CMD_BGCOLOR          4294967049UL
// #define CMD_BITMAP_TRANSFORM 4294967073UL
// #define CMD_BUTTON           4294967053UL
// #define CMD_CALIBRATE        4294967061UL
// #define CMD_CLOCK            4294967060UL
// #define CMD_COLDSTART        4294967090UL
// #define CMD_CRC              4294967043UL
// #define CMD_CSKETCH          4294967093UL
// #define CMD_DIAL             4294967085UL
// #define CMD_DLSTART          4294967040UL
// #define CMD_EXECUTE          4294967047UL
// #define CMD_FGCOLOR          4294967050UL
// #define CMD_GAUGE            4294967059UL
// #define CMD_GETMATRIX        4294967091UL
// #define CMD_GETPOINT         4294967048UL
// #define CMD_GETPROPS         4294967077UL
// #define CMD_GETPTR           4294967075UL
// #define CMD_GRADCOLOR        4294967092UL
// #define CMD_GRADIENT         4294967051UL
// #define CMD_HAMMERAUX        4294967044UL
// #define CMD_IDCT_DELETED     4294967046UL
// #define CMD_INFLATE          4294967074UL
// #define CMD_INTERRUPT        4294967042UL
// #define CMD_INT_RAMSHARED    4294967101UL
// #define CMD_INT_SWLOADIMAGE  4294967102UL
// #define CMD_KEYS             4294967054UL
// #define CMD_LOADIDENTITY     4294967078UL
// #define CMD_LOADIMAGE        4294967076UL
// #define CMD_LOGO             4294967089UL
// #define CMD_MARCH            4294967045UL
// #define CMD_MEDIAFIFO        4294967097UL
// #define CMD_MEMCPY           4294967069UL
// #define CMD_MEMCRC           4294967064UL
// #define CMD_MEMSET           4294967067UL
// #define CMD_MEMWRITE         4294967066UL
// #define CMD_MEMZERO          4294967068UL
// #define CMD_NUMBER           4294967086UL
// #define CMD_PLAYVIDEO        4294967098UL
// #define CMD_PROGRESS         4294967055UL
// #define CMD_REGREAD          4294967065UL
// #define CMD_ROMFONT          4294967103UL
// #define CMD_ROTATE           4294967081UL
// #define CMD_SCALE            4294967080UL
// #define CMD_SCREENSAVER      4294967087UL
// #define CMD_SCROLLBAR        4294967057UL
// #define CMD_SETBASE          4294967096UL
// #define CMD_SETBITMAP        4294967107UL
// #define CMD_SETFONT          4294967083UL
// #define CMD_SETFONT2         4294967099UL
// #define CMD_SETMATRIX        4294967082UL
// #define CMD_SETROTATE        4294967094UL
// #define CMD_SETSCRATCH       4294967100UL
// #define CMD_SKETCH           4294967088UL
// #define CMD_SLIDER           4294967056UL
// #define CMD_SNAPSHOT         4294967071UL
// #define CMD_SNAPSHOT2        4294967095UL
// #define CMD_SPINNER          4294967062UL
// #define CMD_STOP             4294967063UL
// #define CMD_SWAP             4294967041UL
// #define CMD_SYNC             4294967106UL
// #define CMD_TEXT             4294967052UL
// #define CMD_TOGGLE           4294967058UL
// #define CMD_TOUCH_TRANSFORM  4294967072UL
// #define CMD_TRACK            4294967084UL
// #define CMD_TRANSLATE        4294967079UL
// #define CMD_VIDEOFRAME       4294967105UL
// #define CMD_VIDEOSTART       4294967104UL

// #define DECR                 4UL
#define FT810_DLSWAP_DONE          0UL
#define FT810_DLSWAP_FRAME         2UL
#define FT810_DLSWAP_LINE          1UL

// #define EQUAL                5UL
// #define GEQUAL               4UL
// #define GREATER              3UL
// #define INCR                 3UL
// #define INT_CMDEMPTY         32UL
// #define INT_CMDFLAG          64UL
// #define INT_CONVCOMPLETE     128UL
// #define INT_G8               18UL
// #define INT_L8C              12UL
// #define INT_PLAYBACK         16UL
// #define INT_SOUND            8UL
// #define INT_SWAP             1UL
// #define INT_TAG              4UL
// #define INT_TOUCH            2UL
// #define INT_VGA              13UL
// #define INVERT               5UL
// #define KEEP                 1UL

// #define LEQUAL               2UL
// #define LESS                 1UL
// #define LINEAR_SAMPLES       0UL
// #define LINE_STRIP           4UL
#define BILINEAR             1UL
#define NEAREST              0UL
// #define NEVER                0UL
// #define NOTEQUAL             6UL
#define ZERO                 0UL
#define ONE                  1UL
#define DST_ALPHA            3UL
#define SRC_ALPHA            2UL
#define ONE_MINUS_DST_ALPHA  5UL
#define ONE_MINUS_SRC_ALPHA  4UL
// #define OPT_CENTER           1536UL
// #define OPT_CENTERX          512UL
// #define OPT_CENTERY          1024UL
// #define OPT_FLAT             256UL
// #define OPT_FULLSCREEN       8UL
// #define OPT_MEDIAFIFO        16UL
// #define OPT_MONO             1UL
// #define OPT_NOBACK           4096UL
// #define OPT_NODL             2UL
// #define OPT_NOHANDS          49152UL
// #define OPT_NOHM             16384UL
// #define OPT_NOPOINTER        16384UL
// #define OPT_NOSECS           32768UL
// #define OPT_NOTEAR           4UL
// #define OPT_NOTICKS          8192UL
// #define OPT_RIGHTX           2048UL
// #define OPT_SIGNED           256UL
// #define OPT_SOUND            32UL

#define L1                   1UL
#define L2                   17UL
#define L4                   2UL
#define L8                   3UL
#define PALETTED             8UL
#define PALETTED4444         15UL
#define PALETTED565          14UL
#define PALETTED8            16UL
#define RGB332               4UL
#define RGB565               7UL
#define TEXT8X8              9UL
#define TEXTVGA              10UL
#define ARGB1555             0UL
#define ARGB2                5UL
#define ARGB4                6UL

#define POINTS               2UL
#define LINES                3UL
#define RECTS                9UL
#define EDGE_STRIP_A         7UL
#define EDGE_STRIP_B         8UL
#define EDGE_STRIP_L         6UL
#define EDGE_STRIP_R         5UL

#define FT810_RAM_CMD              0x308000
#define FT810_RAM_DL               0x300000
#define FT810_RAM_G                0x000000
#define FT810_RAM_G_SIZE           1024*1024L
#define FT810_RAM_REG              0x302000
#define FT810_RAM_ROMSUB           0x30a000


// #define ROMFONT_TABLEADDRESS 3145724UL

#define FT810_REG_ANALOG              0x30216c
#define FT810_REG_ANA_COMP            0x302184
#define FT810_REG_BIST_EN             0x302174
#define FT810_REG_BUSYBITS            0x3020e8
#define FT810_REG_CLOCK               0x302008
#define FT810_REG_CMDB_SPACE          0x302574
#define FT810_REG_CMDB_WRITE          0x302578
#define FT810_REG_CMD_DL              0x302100
#define FT810_REG_CMD_READ            0x3020f8
#define FT810_REG_CMD_WRITE           0x3020fc
#define FT810_REG_CPURESET            0x302020
#define FT810_REG_CRC                 0x302178
#define FT810_REG_CSPREAD             0x302068
#define FT810_REG_CTOUCH_EXTENDED     0x302108
#define FT810_REG_CTOUCH_TOUCH0_XY    0x302124
#define FT810_REG_CTOUCH_TOUCH4_X     0x30216c
#define FT810_REG_CTOUCH_TOUCH4_Y     0x302120
#define FT810_REG_CTOUCH_TOUCH1_XY    0x30211c
#define FT810_REG_CTOUCH_TOUCH2_XY    0x30218c
#define FT810_REG_CTOUCH_TOUCH3_XY    0x302190
#define FT810_REG_CYA_TOUCH           0x302168
#define FT810_REG_DATESTAMP           0x302564
#define FT810_REG_DITHER              0x302060
#define FT810_REG_DLSWAP              0x302054
#define FT810_REG_FRAMES              0x302004
#define FT810_REG_FREQUENCY           0x30200c
#define FT810_REG_GPIO                0x302094
#define FT810_REG_GPIOX               0x30209c
#define FT810_REG_GPIOX_DIR           0x302098
#define FT810_REG_GPIO_DIR            0x302090
#define FT810_REG_HCYCLE              0x30202c
#define FT810_REG_HOFFSET             0x302030
#define FT810_REG_HSIZE               0x302034
#define FT810_REG_HSYNC0              0x302038
#define FT810_REG_HSYNC1              0x30203c
#define FT810_REG_ID                  0x302000
#define FT810_REG_INT_EN              0x3020ac
#define FT810_REG_INT_FLAGS           0x3020a8
#define FT810_REG_INT_MASK            0x3020b0
#define FT810_REG_MACRO_0             0x3020d8
#define FT810_REG_MACRO_1             0x3020dc
#define FT810_REG_MEDIAFIFO_READ      0x309014
#define FT810_REG_MEDIAFIFO_WRITE     0x309018
#define FT810_REG_OUTBITS             0x30205c
#define FT810_REG_PATCHED_ANALOG      0x302170
#define FT810_REG_PATCHED_TOUCH_FAULT 0x30216c
#define FT810_REG_PCLK                0x302070
#define FT810_REG_PCLK_POL            0x30206c
#define FT810_REG_PLAY                0x30208c
#define FT810_REG_PLAYBACK_FORMAT     0x3020c4
#define FT810_REG_PLAYBACK_FREQ       0x3020c0
#define FT810_REG_PLAYBACK_LENGTH     0x3020b8
#define FT810_REG_PLAYBACK_LOOP       0x3020c8
#define FT810_REG_PLAYBACK_PLAY       0x3020cc
#define FT810_REG_PLAYBACK_READPTR    0x3020bc
#define FT810_REG_PLAYBACK_START      0x3020b4
#define FT810_REG_PWM_DUTY            0x3020d4
#define FT810_REG_PWM_HZ              0x3020d0
#define FT810_REG_RENDERMODE          0x302010
#define FT810_REG_ROMSUB_SEL          0x3020f0
#define FT810_REG_ROTATE              0x302058
#define FT810_REG_SNAPFORMAT          0x30201c
#define FT810_REG_SNAPSHOT            0x302018
#define FT810_REG_SNAPY               0x302014
#define FT810_REG_SOUND               0x302088
#define FT810_REG_SPI_EARLY_TX        0x30217c
#define FT810_REG_SPI_WIDTH           0x302188
#define FT810_REG_SWIZZLE             0x302064
#define FT810_REG_TAG                 0x30207c
#define FT810_REG_TAG_X               0x302074
#define FT810_REG_TAG_Y               0x302078
#define FT810_REG_TAP_CRC             0x302024
#define FT810_REG_TAP_MASK            0x302028
#define FT810_REG_TOUCH_ADC_MODE      0x302108
#define FT810_REG_TOUCH_CHARGE        0x30210c
#define FT810_REG_TOUCH_DIRECT_XY     0x30218c
#define FT810_REG_TOUCH_DIRECT_Z1Z2   0x302190
#define FT810_REG_TOUCH_FAULT         0x302170
#define FT810_REG_TOUCH_MODE          0x302104
#define FT810_REG_TOUCH_OVERSAMPLE    0x302114
#define FT810_REG_TOUCH_RAW_XY        0x30211c
#define FT810_REG_TOUCH_RZ            0x302120
#define FT810_REG_TOUCH_RZTHRESH      0x302118
#define FT810_REG_TOUCH_SCREEN_XY     0x302124
#define FT810_REG_TOUCH_SETTLE        0x302110
#define FT810_REG_TOUCH_TAG           0x30212c
#define FT810_REG_TOUCH_TAG1          0x302134
#define FT810_REG_TOUCH_TAG1_XY       0x302130
#define FT810_REG_TOUCH_TAG2          0x30213c
#define FT810_REG_TOUCH_TAG2_XY       0x302138
#define FT810_REG_TOUCH_TAG3          0x302144
#define FT810_REG_TOUCH_TAG3_XY       0x302140
#define FT810_REG_TOUCH_TAG4          0x30214c
#define FT810_REG_TOUCH_TAG4_XY       0x302148
#define FT810_REG_TOUCH_TAG_XY        0x302128
#define FT810_REG_TOUCH_TRANSFORM_A   0x302150
#define FT810_REG_TOUCH_TRANSFORM_B   0x302154
#define FT810_REG_TOUCH_TRANSFORM_C   0x302158
#define FT810_REG_TOUCH_TRANSFORM_D   0x30215c
#define FT810_REG_TOUCH_TRANSFORM_E   0x302160
#define FT810_REG_TOUCH_TRANSFORM_F   0x302164
#define FT810_REG_TRACKER             0x309000
#define FT810_REG_TRACKER_1           0x309004
#define FT810_REG_TRACKER_2           0x309008
#define FT810_REG_TRACKER_3           0x30900c
#define FT810_REG_TRACKER_4           0x309010
#define FT810_REG_TRIM                0x302180
#define FT810_REG_VCYCLE              0x302040
#define FT810_REG_VOFFSET             0x302044
#define FT810_REG_VOL_PB              0x302080
#define FT810_REG_VOL_SOUND           0x302084
#define FT810_REG_VSIZE               0x302048
#define FT810_REG_VSYNC0              0x30204c
#define FT810_REG_VSYNC1              0x302050

#define FT810_REPEAT               1UL
#define FT810_REPLACE              2UL
#define FT810_TOUCHMODE_CONTINUOUS 3UL
#define FT810_TOUCHMODE_FRAME      2UL
#define FT810_TOUCHMODE_OFF        0UL
#define FT810_TOUCHMODE_ONESHOT    1UL
#define FT810_ULAW_SAMPLES         1UL
#define FT810_VERTEX2F(x,y) ((1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0))
#define FT810_VERTEX2II(x,y,handle,cell) ((2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0))
#define FT810_BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&4194303UL)<<0))
#define FT810_CLEAR_COLOR_RGB(red,green,blue) ((2UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define FT810_TAG(s) ((3UL<<24)|(((s)&255UL)<<0))
#define FT810_COLOR_RGB(red,green,blue) ((4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define FT810_BITMAP_HANDLE(handle) ((5UL<<24)|(((handle)&31UL)<<0))
#define FT810_CELL(cell) ((6UL<<24)|(((cell)&127UL)<<0))
#define FT810_BITMAP_LAYOUT(format,linestride,height) ((7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0))
#define FT810_BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((8UL<<24)|(((filter)&1UL)<<20)|(((wrapx)&1UL)<<19)|(((wrapy)&1UL)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0))
#define FT810_ALPHA_FUNC(func,ref) ((9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0))
#define FT810_STENCIL_FUNC(func,ref,mask) ((10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0))
#define FT810_BLEND_FUNC(src,dst) ((11UL<<24)|(((src)&7UL)<<3)|(((dst)&7UL)<<0))
#define FT810_STENCIL_OP(sfail,spass) ((12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0))
#define FT810_POINT_SIZE(size) ((13UL<<24)|(((size)&8191UL)<<0))
#define FT810_LINE_WIDTH(width) ((14UL<<24)|(((width)&4095UL)<<0))
#define FT810_CLEAR_COLOR_A(alpha) ((15UL<<24)|(((alpha)&255UL)<<0))
#define FT810_COLOR_A(alpha) ((16UL<<24)|(((alpha)&255UL)<<0))
#define FT810_CLEAR_STENCIL(s) ((17UL<<24)|(((s)&255UL)<<0))
#define FT810_CLEAR_TAG(s) ((18UL<<24)|(((s)&255UL)<<0))
#define FT810_STENCIL_MASK(mask) ((19UL<<24)|(((mask)&255UL)<<0))
#define FT810_TAG_MASK(mask) ((20UL<<24)|(((mask)&1UL)<<0))
#define FT810_BITMAP_TRANSFORM_A(a) ((21UL<<24)|(((a)&131071UL)<<0))
#define FT810_BITMAP_TRANSFORM_B(b) ((22UL<<24)|(((b)&131071UL)<<0))
#define FT810_BITMAP_TRANSFORM_C(c) ((23UL<<24)|(((c)&16777215UL)<<0))
#define FT810_BITMAP_TRANSFORM_D(d) ((24UL<<24)|(((d)&131071UL)<<0))
#define FT810_BITMAP_TRANSFORM_E(e) ((25UL<<24)|(((e)&131071UL)<<0))
#define FT810_BITMAP_TRANSFORM_F(f) ((26UL<<24)|(((f)&16777215UL)<<0))
#define FT810_SCISSOR_XY(x,y) ((27UL<<24)|(((x)&2047UL)<<11)|(((y)&2047UL)<<0))
#define FT810_SCISSOR_SIZE(width,height) ((28UL<<24)|(((width)&4095UL)<<12)|(((height)&4095UL)<<0))
#define FT810_CALL(dest) ((29UL<<24)|(((dest)&65535UL)<<0))
#define FT810_JUMP(dest) ((30UL<<24)|(((dest)&65535UL)<<0))
#define FT810_BEGIN(prim) ((31UL<<24)|(((prim)&15UL)<<0))
#define FT810_COLOR_MASK(r,g,b,a) ((32UL<<24)|(((r)&1UL)<<3)|(((g)&1UL)<<2)|(((b)&1UL)<<1)|(((a)&1UL)<<0))
#define FT810_CLEAR(c,s,t) ((38UL<<24)|(((c)&1UL)<<2)|(((s)&1UL)<<1)|(((t)&1UL)<<0))
#define FT810_VERTEX_FORMAT(frac) ((39UL<<24)|(((frac)&7UL)<<0))
#define FT810_BITMAP_LAYOUT_H(linestride,height) ((40UL<<24)|(((linestride)&3UL)<<2)|(((height)&3UL)<<0))
#define FT810_BITMAP_SIZE_H(width,height) ((41UL<<24)|(((width)&3UL)<<2)|(((height)&3UL)<<0))
#define FT810_PALETTE_SOURCE(addr) ((42UL<<24)|(((addr)&4194303UL)<<0))
#define FT810_VERTEX_TRANSLATE_X(x) ((43UL<<24)|(((x)&131071UL)<<0))
#define FT810_VERTEX_TRANSLATE_Y(y) ((44UL<<24)|(((y)&131071UL)<<0))
#define FT810_NOP() ((45UL<<24))
#define FT810_END() ((33UL<<24))
#define FT810_SAVE_CONTEXT() ((34UL<<24))
#define FT810_RESTORE_CONTEXT() ((35UL<<24))
#define FT810_RETURN() ((36UL<<24))
#define FT810_MACRO(m) ((37UL<<24)|(((m)&1UL)<<0))
#define FT810_DISPLAY() ((0UL<<24))

