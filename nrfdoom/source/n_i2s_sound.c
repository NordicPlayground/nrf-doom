//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2008 David Flater
// Copyright(C) 2021 Nordic Semiconductor ASA
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
//  System interface for sound.
//

#include "doom_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "deh_str.h"
#include "i_sound.h"
#include "i_system.h"
#include "i_swap.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "z_zone.h"

#include "n_i2s.h"

#include "doomtype.h"

#define NUM_CHANNELS 16

static boolean use_sfx_prefix;

static boolean sound_initialized = false;

typedef struct __attribute__((packed)) mobj_s
{
    byte     *ptr;
    uint16_t  pos;
    uint16_t  len;
    uint8_t   left_vol;
    uint8_t   right_vol;
} channel_data_t;


channel_data_t channels[NUM_CHANNELS];

static boolean CacheSFX(sfxinfo_t *sfxinfo, int handle)
{
    int lumpnum;
    unsigned int lumplen;
    int samplerate;
    unsigned int length;
    byte *data;

    // need to load the sound

    lumpnum = sfxinfo->lumpnum;
    data = W_CacheLumpNum(lumpnum, PU_STATIC);
    lumplen = W_LumpLength(lumpnum);

    // Check the header, and ensure this is a valid sound

    if (lumplen < 8
     || data[0] != 0x03 || data[1] != 0x00)
    {
        // Invalid sound

        return false;
    }

    // 16 bit sample rate field, 32 bit length field

    samplerate = (data[3] << 8) | data[2];
    length = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];

    // If the header specifies that the length of the sound is greater than
    // the length of the lump itself, this is an invalid sound lump

    // We also discard sound lumps that are less than 49 samples long,
    // as this is how DMX behaves - although the actual cut-off length
    // seems to vary slightly depending on the sample rate.  This needs
    // further investigation to better understand the correct
    // behavior.

    if (length > lumplen - 8 || length <= 48)
    {
        return false;
    }

    // The DMX sound library seems to skip the first 16 and last 16
    // bytes of the lump - reason unknown.

    data += 16;
    length -= 32;

    // Setup channel
    // printf("CacheSFX: channel = %0d samplerate = %d, length = %d\n", handle, samplerate, length);

    channels[handle].ptr = data;
    channels[handle].pos = 0;
    channels[handle].len = length;


    // W_ReleaseLumpNum(lumpnum);

    return true;
}

static void GetSfxLumpName(sfxinfo_t *sfx, char *buf, size_t buf_len)
{
    // Linked sfx lumps? Get the lump number for the sound linked to.

    if (sfx->link != NULL)
    {
        sfx = sfx->link;
    }

    // Doom adds a DS* prefix to sound lumps; Heretic and Hexen don't
    // do this.

    if (use_sfx_prefix)
    {
        M_snprintf(buf, buf_len, "ds%s", DEH_String(sfx->name));
    }
    else
    {
        M_StringCopy(buf, DEH_String(sfx->name), buf_len);
    }
}


static void N_I2S_PrecacheSounds(sfxinfo_t *sounds, int num_sounds)
{
    printf("NRFD-TODO: N_I2S_PrecacheSounds?\n");
}


//
// Retrieve the raw data lump index
//  for a given SFX name.
//

static int N_I2S_GetSfxLumpNum(sfxinfo_t *sfx)
{
    char namebuf[9];

    GetSfxLumpName(sfx, namebuf, sizeof(namebuf));

    return W_GetNumForName(namebuf);
}

static void N_I2S_UpdateSoundParams(int handle, int vol, int sep)
{
    int left, right;

    // printf("N_I2S_UpdateSoundParams\n");

    if (!sound_initialized || handle < 0 || handle >= NUM_CHANNELS)
    {
        return;
    }

    left = ((254 - sep) * vol) / 127;
    right = ((sep) * vol) / 127;

    if (left < 0) left = 0;
    else if ( left > 255) left = 255;
    if (right < 0) right = 0;
    else if (right > 255) right = 255;

    channels[handle].left_vol = left;
    channels[handle].right_vol = right;

    // printf("N_I2S_UpdateSoundParams: %d %d\n", left, right);
}

static void ClearSoundOnChannel(int channel)
{
    // printf("Clear channel %0d\n", channel);
    channels[channel].ptr = NULL;
    channels[channel].len = 0;
    channels[channel].pos = 0;
    channels[channel].left_vol = 0;
    channels[channel].right_vol = 0;
}

//
// Starting a sound means adding it
//  to the current list of active sounds
//  in the internal channels.
// As the SFX info struct contains
//  e.g. a pointer to the raw data,
//  it is ignored.
// As our sound handling does not handle
//  priority, it is ignored.
// Pitching (that is, increased speed of playback)
//  is set, but currently not used by mixing.
//

static int N_I2S_StartSound(sfxinfo_t *sfxinfo, int channel, int vol, int sep, int pitch)
{
    // allocated_sound_t *snd;

    // printf("N_I2S_StartSound %.9s %d %d %d %d\n", sfxinfo->name, channel, vol, sep, pitch);

    if (!sound_initialized || channel < 0 || channel >= NUM_CHANNELS)
    {
        return -1;
    }

    // Clear a sound effect if there is already one playing
    // on this channel

    ClearSoundOnChannel(channel);

    // Get the sound data

    if (!CacheSFX(sfxinfo, channel))
    {
        printf("N_I2S_StartSound: Error caching SFX\n");
        return false;
    }

    // set separation, etc.

    N_I2S_UpdateSoundParams(channel, vol, sep);

    return channel;
}

static void N_I2S_StopSound(int handle)
{
    if (!sound_initialized || handle < 0 || handle >= NUM_CHANNELS)
    {
        return;
    }


    ClearSoundOnChannel(handle);
}


static boolean N_I2S_SoundIsPlaying(int handle)
{
    if (!sound_initialized || handle < 0 || handle >= NUM_CHANNELS)
    {
        return false;
    }

    // printf("N_I2S_SoundIsPlaying\n");

    return (channels[handle].ptr != NULL);
}

//
// Periodically called to update the sound system
//

static void N_I2S_UpdateSound(void)
{
    // printf("N_I2S_UpdateSound\n");

    int16_t *buf;
    int      buf_len;
    while (N_I2S_next_buffer(&buf_len, &buf)) {
        for (int i=0; i<buf_len/2; i++) {
            int16_t sample_l = 0;
            int16_t sample_r = 0;
            for (int j=0; j<NUM_CHANNELS; j++)
            {
                channel_data_t ch = channels[j];
                if (ch.ptr != NULL) {
                    int16_t sample = ch.ptr[ch.pos]-128;
                    channels[j].pos++;
                    if (channels[j].pos >= ch.len) {
                        ClearSoundOnChannel(j);
                    }
                    sample_l += sample*ch.left_vol;
                    sample_r += sample*ch.right_vol;
                }
            }
            buf[i*2] = sample_l;
            buf[i*2+1] = sample_r;
        }
    }

    N_I2S_process();
}

static void N_I2S_ShutdownSound(void)
{

}


static boolean N_I2S_InitSound(boolean _use_sfx_prefix)
{

    use_sfx_prefix = _use_sfx_prefix;

    printf("N_I2S_InitSound\n");

    for (int i=0; i<NUM_CHANNELS; i++) {
        ClearSoundOnChannel(i);
    }

    sound_initialized = true;

    return true;
}

static snddevice_t sound_sdl_devices[] =
{
    SNDDEVICE_SB,
    SNDDEVICE_PAS,
    SNDDEVICE_GUS,
    SNDDEVICE_WAVEBLASTER,
    SNDDEVICE_SOUNDCANVAS,
    SNDDEVICE_AWE32,
};

sound_module_t sound_i2s_module =
{
    sound_sdl_devices,
    arrlen(sound_sdl_devices),
    N_I2S_InitSound,
    N_I2S_ShutdownSound,
    N_I2S_GetSfxLumpNum,
    N_I2S_UpdateSound,
    N_I2S_UpdateSoundParams,
    N_I2S_StartSound,
    N_I2S_StopSound,
    N_I2S_SoundIsPlaying,
    N_I2S_PrecacheSounds,
};

