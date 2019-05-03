//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	DOOM graphics stuff for NRF.
//


// NRFD-EXCLUDE: #include "icon.c"

#include <string.h> 

#include "doom_config.h"
#include "d_loop.h"
#include "deh_str.h"
#include "doomtype.h"
#include "i_input.h"
#include "i_joystick.h"
#include "i_system.h"
#include "i_timer.h"
#include "i_video.h"
#include "m_argv.h"
#include "m_config.h"
#include "m_misc.h"
#include "tables.h"
#include "v_diskicon.h"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"

#include "n_buttons.h"
#include "n_display.h"

#include "FT810.h"

static uint32_t pixel_format;

// palette

static uint32_t display_palette_loc;

// display has been set up?

static boolean initialized = false;

// disable mouse?

static boolean nomouse = false;
int usemouse = 1;

// Save screenshots in PNG format.

// int png_screenshots = 0;

// SDL video driver name

// char *video_driver = "";

// Window position:

// char *window_position = "center";

// SDL display number on which to run.

// int video_display = 0;

// Screen width and height, from configuration file.

// int window_width = SCREENWIDTH * 2;
// int window_height = SCREENHEIGHT_4_3 * 2;

// Fullscreen mode, 0x0 for SDL_WINDOW_FULLSCREEN_DESKTOP.

// int fullscreen_width = 0, fullscreen_height = 0;

// Maximum number of pixels to use for intermediate scale buffer.

// static int max_scaling_buffer_pixels = 16000000;

// Run in full screen mode?  (int type for config code)

// int fullscreen = true;

// Aspect ratio correction mode

// int aspect_ratio_correct = true;
// static int actualheight;

// Force integer scales for resolution-independent rendering

// int integer_scaling = false;

// VGA Porch palette change emulation

// const int vga_porch_flash = false;

// Force software rendering, for systems which lack effective hardware
// acceleration

// int force_software_renderer = false;

// Time to wait for the screen to settle on startup before starting the
// game (ms)

// const int startup_delay = 1000;

// Grab the mouse? (int type for config code). nograbmouse_override allows
// this to be temporarily disabled via the command line.

// const int grabmouse = true;
// const boolean nograbmouse_override = false;

// The screen buffer; this is modified to draw things to the screen

pixel_t I_VideoBuffer[320*200];

static uint32_t display_vbuffer_loc;

// If true, game is running as a screensaver

const boolean screensaver_mode = false;

// Flag indicating whether the screen is currently visible:
// when the screen isnt visible, don't render the screen

const boolean screenvisible = true;

// If true, we display dots at the bottom of the screen to 
// indicate FPS.

// static boolean display_fps_dots;

// If this is true, the screen is rendered but not blitted to the
// video buffer.

static boolean noblit;

// Callback function to invoke to determine whether to grab the 
// mouse pointer.

// static grabmouse_callback_t grabmouse_callback = NULL;

// Does the window currently have focus?

// static boolean window_focused = true;

// Window resize state.

// static boolean need_resize = false;
// static unsigned int last_resize_time;
// #define RESIZE_DELAY 500

// Gamma correction level to use

int usegamma = 0;

// Joystick/gamepad hysteresis
unsigned int joywait = 0;

void PrintVBuffer(void)
{
    pixel_t *vb = I_VideoBuffer;
    printf("%.2X %.2X %.2X %2X %.2X %.2X %.2X %2X\n", 
        vb[0], vb[1], vb[2], vb[3],
        vb[4], vb[5], vb[6], vb[7]); 
    printf("%.2X %.2X %.2X %2X %.2X %.2X %.2X %2X\n", 
        vb[1*SCREENWIDTH+0], vb[1*SCREENWIDTH+1], vb[1*SCREENWIDTH+2], vb[1*SCREENWIDTH+3],
        vb[1*SCREENWIDTH+4], vb[1*SCREENWIDTH+5], vb[1*SCREENWIDTH+6], vb[1*SCREENWIDTH+7]); 
}


void I_SetGrabMouseCallback(grabmouse_callback_t func)
{
    // grabmouse_callback = func;
}

// Set the variable controlling FPS dots.

void I_DisplayFPSDots(boolean dots_on)
{
    // display_fps_dots = dots_on;
}


void I_ShutdownGraphics(void)
{
    if (initialized)
    {
        initialized = false;
    }
}



//
// I_StartFrame
//
void I_StartFrame (void)
{
    // er?

}

static void I_ToggleFullScreen(void)
{
    //NRFD-EXCLUDE
}

void I_GetEvent(void)
{
    N_ldbg("NRFD-TODO: I_GetEvent\n");
}

//
// I_StartTic
//
void I_StartTic (void)
{
    N_ldbg("NRFD-TODO: I_StartTic\n");
    if (!initialized)
    {
        return;
    }
    N_ReadButtons();
/*
    I_GetEvent();

    if (usemouse && !nomouse)
    {
        I_ReadMouse();
    }

    if (joywait < I_GetTime())
    {
        I_UpdateJoystick();
    }
    */
}


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
    // what is this?
}

//
// I_FinishUpdate
//
void I_FinishUpdate (void)
{
    N_ldbg("NRFD-TODO: I_FinishUpdate\n");
    // PrintVBuffer();
    // static int lasttic;
    // int tics;
    // int i;

    // draws little dots on the bottom of the screen

    // NRFD-TODO
    // if (display_fps_dots)
    // {
    //     i = I_GetTime();
    //     tics = i - lasttic;
    //     lasttic = i;
    //     if (tics > 20) tics = 20;

    //     for (i=0 ; i<tics*4 ; i+=4)
    //         I_VideoBuffer[ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
    //     for ( ; i<20*4 ; i+=4)
    //         I_VideoBuffer[ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;
    // }

    // Draw disk icon before blit, if necessary.
    // NRFD_TODO: V_DrawDiskIcon();

    N_display_spi_wr(display_vbuffer_loc, SCREENWIDTH*SCREENHEIGHT, I_VideoBuffer);
    
    dl_start();

    dl(FT810_CLEAR_COLOR_RGB(0x00, 0x00, 0x00));
    dl(FT810_CLEAR(1,1,1));  // Clear color, stencil, tag
    dl(FT810_CLEAR_COLOR_RGB(0x00, 0x00, 0x00)); 
    dl(FT810_CLEAR(1,0,0));  // Clear color

    dl(FT810_BITMAP_HANDLE(0)) ;
    dl(FT810_BITMAP_LAYOUT(PALETTED8, SCREENWIDTH, SCREENHEIGHT)) ;
    dl(FT810_BITMAP_TRANSFORM_A((int)(256/2.0)));
    dl(FT810_BITMAP_TRANSFORM_E((int)(256/2.4)));
    dl(FT810_BITMAP_SIZE(NEAREST, BORDER, BORDER, 640&0x1FF, 480));
    dl(FT810_BITMAP_SIZE_H(640>>9, 0));
    dl(FT810_BITMAP_SOURCE(display_vbuffer_loc)) ;

    dl(FT810_COLOR_RGB(0xFF, 0xFF, 0xFF)); 

    dl(FT810_BEGIN(BITMAPS));
    {
        dl(FT810_VERTEX_TRANSLATE_X(80*16));
        // dl(FT810_VERTEX_TRANSLATE_Y(0*16));

        dl(FT810_BLEND_FUNC(ONE, ZERO));

        dl(FT810_COLOR_MASK(0,0,0,1));
        dl(FT810_PALETTE_SOURCE(display_palette_loc+3));
        dl(FT810_VERTEX2II(0, 0, 0, 0));

        dl(FT810_BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
        dl(FT810_COLOR_MASK(1,0,0,0));
        dl(FT810_PALETTE_SOURCE(display_palette_loc));
        dl(FT810_VERTEX2II(0, 0, 0, 0));

        dl(FT810_COLOR_MASK(0,1,0,0));
        dl(FT810_PALETTE_SOURCE(display_palette_loc+1));
        dl(FT810_VERTEX2II(0, 0, 0, 0));

        dl(FT810_COLOR_MASK(0,0,1,0));
        dl(FT810_PALETTE_SOURCE(display_palette_loc+2));
        dl(FT810_VERTEX2II(0, 0, 0, 0));
    }
    dl(FT810_END());

    dl_end();

    N_display_dlswap_frame();
    

    // Restore background and undo the disk indicator, if it was drawn.
    // NRFD-TODO: V_RestoreDiskBackground();
}


//
// I_ReadScreen
//
void I_ReadScreen (pixel_t* scr)
{
    printf("NRFD-TODO: I_ReadScreen\n");
    // memcpy(scr, I_VideoBuffer, SCREENWIDTH*SCREENHEIGHT*sizeof(*scr));
}


//
// I_SetPalette
//
void I_SetPalette (byte *doompalette)
{
    int i;
    N_ldbg("I_SetPalette %X\n", (unsigned int)(doompalette));

    // Convert Doom palette to FT810 palette
    uint8_t  display_pal[DISPLAY_PALETTE_SIZE];

    for (i=0; i<256; ++i)
    {
        // Zero out the bottom two bits of each channel - the PC VGA
        // controller only supports 6 bits of accuracy.

        uint8_t r = gammatable[usegamma][*doompalette++] & ~3;
        uint8_t g = gammatable[usegamma][*doompalette++] & ~3;
        uint8_t b = gammatable[usegamma][*doompalette++] & ~3;
        display_pal[i*4+0] = r;
        display_pal[i*4+1] = g;
        display_pal[i*4+2] = b;
        display_pal[i*4+3] = 0xFF;

    }
    N_display_spi_wr(display_palette_loc, DISPLAY_PALETTE_SIZE, display_pal);
}

/* NRFD-EXCLUDE
// Given an RGB value, find the closest matching palette index.

int I_GetPaletteIndex(int r, int g, int b)
{
    int best, best_diff, diff;
    int i;

    best = 0; best_diff = INT_MAX;

    for (i = 0; i < 256; ++i)
    {
        diff = (r - palette[i].r) * (r - palette[i].r)
             + (g - palette[i].g) * (g - palette[i].g)
             + (b - palette[i].b) * (b - palette[i].b);

        if (diff < best_diff)
        {
            best = i;
            best_diff = diff;
        }

        if (diff == 0)
        {
            break;
        }
    }

    return best;
}

    */

// 
// Set the window title
//

void I_SetWindowTitle(char *title)
{
    /* NRFD-EXCLUDE */
}


void I_GraphicsCheckCommandLine(void)
{
    int i;

    //!
    // @category video
    // @vanilla
    //
    // Disable blitting the screen.
    //

    noblit = M_CheckParm ("-noblit");

    // //!
    // // @category video 
    // //
    // // Don't grab the mouse when running in windowed mode.
    // //

    // nograbmouse_override = M_ParmExists("-nograbmouse");

    //!
    // @category video 
    //
    // Disable the mouse.
    //

    nomouse = M_CheckParm("-nomouse") > 0;
}



void I_InitGraphics(void)
{
    printf("I_InitGraphics\n");
    N_display_init();
    display_palette_loc = N_display_ram_alloc(DISPLAY_PALETTE_SIZE);
    display_vbuffer_loc = N_display_ram_alloc(SCREENWIDTH*SCREENHEIGHT);
    
    initialized = true;
}

// Bind all variables controlling video options into the configuration
// file system.
void I_BindVideoVariables(void)
{
    // NRFD-TODO:?
    // M_BindIntVariable("use_mouse",                 &usemouse);
    // M_BindIntVariable("fullscreen",                &fullscreen);
    // M_BindIntVariable("video_display",             &video_display);
    // M_BindIntVariable("aspect_ratio_correct",      &aspect_ratio_correct);
    // M_BindIntVariable("integer_scaling",           &integer_scaling);
    // M_BindIntVariable("vga_porch_flash",           &vga_porch_flash);
    // M_BindIntVariable("startup_delay",             &startup_delay);
    // M_BindIntVariable("fullscreen_width",          &fullscreen_width);
    // M_BindIntVariable("fullscreen_height",         &fullscreen_height);
    // M_BindIntVariable("force_software_renderer",   &force_software_renderer);
    // M_BindIntVariable("max_scaling_buffer_pixels", &max_scaling_buffer_pixels);
    // M_BindIntVariable("window_width",              &window_width);
    // M_BindIntVariable("window_height",             &window_height);
    // M_BindIntVariable("grabmouse",                 &grabmouse);
    // M_BindStringVariable("video_driver",           &video_driver);
    // M_BindStringVariable("window_position",        &window_position);
    M_BindIntVariable("usegamma",                  &usegamma);
    // M_BindIntVariable("png_screenshots",           &png_screenshots);
}
