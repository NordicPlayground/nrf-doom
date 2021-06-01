
nRF-Doom
=======================================================

This is a port of `Doom (1993)`_ for `nRF5340`_, a dual-core low-power wireless SoC.

.. _nRF5340: https://www.nordicsemi.com/Products/Low-power-short-range-wireless/nRF5340
.. _Doom (1993): https://en.wikipedia.org/wiki/Doom_(1993_video_game)

Table of Contents
^^^^^^^^^^^^^^^^^^^^^^^

- `Status`_
- `Software`_
- `Hardware`_ 
- `Pin Mapping`_ 


Status
-------------------------------------------------------

======================= ================= ================================
 Feature                 Status            Comment
======================= ================= ================================
Load WAD from SD-card   Supported         Only small WADs supported as all data is moved to 8MB external flash
----------------------- ----------------- --------------------------------
Graphics                320x200 30-36fps  Resolution is hard-coded
----------------------- ----------------- --------------------------------
Sound effects           Supported         
----------------------- ----------------- --------------------------------
Demo playback           Supported         Only supports demos in WADs for now
----------------------- ----------------- --------------------------------
Demo recording          Not supported     No plans to support
----------------------- ----------------- --------------------------------
Config files            Not supported     No plans to support
----------------------- ----------------- --------------------------------
Music                   Not supported     Can probably not do on-chip MIDI synthesis, but external MIDI should be possible
----------------------- ----------------- --------------------------------
Multiplayer             Not supported     May try multiplayer over 802.15.4 radio at some point
----------------------- ----------------- --------------------------------
PWAD/Mods               Not supported     No plans to support
======================= ================= ================================

nRF-Doom has only been tested with shareware version of Doom 1 for now

======================= ================= ================================
 Game                    Status            Comment
======================= ================= ================================
Doom 1 Shareware        OK                
----------------------- ----------------- --------------------------------
Doom 1 Full Version     Not Tested        External flash too small, but should theoretically work
----------------------- ----------------- --------------------------------
Doom 2                  Not Tested        External flash too small. Should theoretically work, but may struggle with larger levels
======================= ================= ================================


Software
-------------------------------------------------------

The project is forked from `Chocolate Doom`_ version 3.0.0

.. _Chocolate Doom: https://www.chocolate-doom.org/wiki/index.php/Chocolate_Doom

Most changes are related to reducing memory usage, and replacing IO/Video/Sound interfaces with new ones for nRF5340 and external peripherals.

All the drivers/modules specific to this project is prefixed with n_ in the sourcecode. 

Summary of Changes
"""""""""""""""""""""

- All dependencies on SDL has been removed
- Many configuration options have been hard-coded
- Constant data has been marked as "const" to allow direct access from Flash NVM
- Data structures have been packed to reduce memory overhead
- Data structures have been modified to allow direct memory mapped access to WAD data, instead of copying it to new structures in RAM. Luckily the WADs have data stored in big endian format, which is the same format used by the ARM Cortex-M33 on nRF5340
- Debug printing over UART
- WAD loading through SD-Card over SPI
- Support for push buttons on the DevKit
- Modified game timer to use internal TIMER peripheral on nRF5340
- Video output over SPI to FT810 `display`_ driver
- PCM `sound`_ output over I2S to PCM5102 or MAX98357 DAC
- Support for custom wireless gamepad over Nordic proprietary radio 
- Hardfault handler
- Support for external QSPI flash memory for WAD data and composite texture data
- Code to pre-generate composite textures for all textures in the WAD 
- Removed code related to Hexen/Strife

Future improvements
"""""""""""""""""""""

It should be possible to add multiplayer support over Nordic proprietary or 802.15.4 radio.

It might be possible to use spare processing power and RAM on the Network MCU core to synthesize MIDI for music. But it might actually be more in spirit with the original version of Doom to use an external hardware MIDI synthesizer. 

Support for Bluetooth Keyboard and Mouse would be great, but the project will have to be ported to `Zephyr RTOS`_ / `nRF Connect SDK`_ first, which will likely be done as a different project or branch.

.. _Zephyr RTOS: https://www.zephyrproject.org/
.. _nRF Connect SDK: https://www.nordicsemi.com/Software-and-tools/Software/nRF-Connect-SDK

Hardware
-------------------------------------------------------

CPU
"""""""""""""""""""""""""

**Application Core**: Cortex-M33 @ 128Mhz

Used for all game logic, rendering, sound processing, etc.

**Network Core**: Cortex-M33 @ 64Mhz

Only used for gamepad support for now


Memory
"""""""""""""""""""""""""

======================= ================= ================================
 Memory                  Size              Usage
======================= ================= ================================
App Single-cycle RAM    256KiB            Main static memory and stack for game, video/sound buffers
----------------------- ----------------- --------------------------------
App Multi-cycle RAM     256KiB            Main heap memory for game
----------------------- ----------------- --------------------------------
App Flash NVM           1MiB              Game code and static data        
----------------------- ----------------- --------------------------------
Network RAM             64KiB             Only used for network firmware to communicate with gamepad
----------------------- ----------------- --------------------------------
Network Flash NVM       256KiB            Only used for network firmware to communicate with gamepad
----------------------- ----------------- --------------------------------
External QSPI flash     8MiB              Used for fast access to WAD, and pre-generated composite textures
----------------------- ----------------- --------------------------------
External SD-card        Variable          Used to transfer WAD from PC
======================= ================= ================================

The external QSPI flash is accessed through the Excecute-In-Place (XIP) functionality, so that the data can be accessed through a memory mapped region.

Display
""""""""""""""""""""""""

* **Display panel**: `4.3" IPS LCD - 800x480 <https://www.hotmcu.com/43-graphical-ips-lcd-touchscreen-800x480-spi-ft810-p-333.html>`_
* **Display driver**: `FT810 <https://www.ftdichip.com/old2020/Products/ICs/FT81X.html>`_  (`Datasheet <http://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT81x.pdf>`_)
* **Interface**: SPI, Max 30MHz 
* **Game Resolution**: 320x200 stretched to a 4:3 aspect ratio
* **Bits per pixel**: 8bit paletted
* **Required Bandwidth**: 15.36Mhz @ 30fps (per bit)

The display is driven over SPI at 32MHz. This is technically higher than the maximum spec listed in the datasheets, but tends to work fine, depending on the signal integratity of the connection. The frames are transferred as 320x200 paletted images (8 bit per pixel), and the FT810 takes care of palette lookup and scaling the image to 800x480

The display driver supports QSPI, but unfortunately there's not a general purpose QSPI interface on nRF5340


Sound
""""""""""""""""""""""""

nRF-Doom has been tested with two `I2S`_ DACs

`Texas Instruments PCM5102 <https://www.ti.com/product/PCM5102>`_

`Maxim Integrated MAX98357 <https://www.maximintegrated.com/en/products/analog/audio/MAX98357A.html>`_

The sound samples in Doom is 8-bit mono `PCM`_ with a sample rate of 11025Hz. The I2S DAC is driven with 16-bit stereo format at 10869.5Hz.

.. _I2S: https://en.wikipedia.org/wiki/I%C2%B2S
.. _PCM: https://en.wikipedia.org/wiki/Pulse-code_modulation


Gamepad
""""""""""""""""""""""""

The demo uses a gamepad module for the `BBC micro:bit`_. It can be found on eg. `AliExpress`_.

The Doom port communicates with the micro:bit gamepad over Nordic proprietary radio with a custom protocol. In addition to providing inputs for the game, the gamepad uses the LED-matrix on the micro:bit to show a represantation of the Doom-guys face.

The source code for the micro:bit gamepad can be found in the `microbit_ctrl`_ folder, and the corresponding source code for the Network MCU to communicate with the gamepad can be found under `nrfdoom_net`_, and the code for interfacing with the Doom event system is in `n_rjoy.c`_

.. _BBC micro:bit: https://microbit.org/

.. _AliExpress: https://www.aliexpress.com/item/32889301528.html

.. _microbit_ctrl: https://github.com/NordicPlayground/nrf-doom/tree/master/microbit_ctrl

.. _nrfdoom_net: https://github.com/NordicPlayground/nrf-doom/tree/master/nrfdoom_net

.. _n_rjoy.c: https://github.com/NordicPlayground/nrf-doom/blob/master/nrfdoom/source/n_rjoy.c


Pin Mapping
-------------------------------------------------------

======================= ================= ================================
 Peripheral              Function          nRF5340 Pin
======================= ================= ================================
Buttons                 Button 1          P0.23
----------------------- ----------------- --------------------------------
Buttons                 Button 2          P0.24
----------------------- ----------------- --------------------------------
Buttons                 Button 3          P0.8
----------------------- ----------------- --------------------------------
Buttons                 Button 4          P0.9
----------------------- ----------------- --------------------------------
LEDs                    LED 1             P0.28
----------------------- ----------------- --------------------------------
LEDs                    LED 2             P0.29
----------------------- ----------------- --------------------------------
LEDs                    LED 3             P0.30
----------------------- ----------------- --------------------------------
LEDs                    LED 4             P0.31
----------------------- ----------------- --------------------------------
UART Debug              TX                P0.20
----------------------- ----------------- --------------------------------
UART Debug              RX                P0.22
----------------------- ----------------- --------------------------------
QSPI Memory             SCK               P0.17
----------------------- ----------------- --------------------------------
QSPI Memory             CSN               P0.18
----------------------- ----------------- --------------------------------
QSPI Memory             IO0               P0.13
----------------------- ----------------- --------------------------------
QSPI Memory             IO1               P0.14
----------------------- ----------------- --------------------------------
QSPI Memory             IO2               P0.15
----------------------- ----------------- --------------------------------
QSPI Memory             IO3               P0.16
----------------------- ----------------- --------------------------------
SPI/SD card             SCK               P1.14
----------------------- ----------------- --------------------------------
SPI/SD card             MOSI              P1.13
----------------------- ----------------- --------------------------------
SPI/SD card             MISO              P1.15
----------------------- ----------------- --------------------------------
SPI/SD card             CS                P1.12
----------------------- ----------------- --------------------------------
SPI/FT810 Display       SCK               P0.06
----------------------- ----------------- --------------------------------
SPI/FT810 Display       MISO              P0.05
----------------------- ----------------- --------------------------------
SPI/FT810 Display       MOSI              P0.25
----------------------- ----------------- --------------------------------
SPI/FT810 Display       CS_N              P0.07
----------------------- ----------------- --------------------------------
SPI/FT810 Display       PD_N              P0.26
----------------------- ----------------- --------------------------------
I2S/PCM5102 DAC         SCK               P1.09
----------------------- ----------------- --------------------------------
I2S/PCM5102 DAC         BCK               P1.08
----------------------- ----------------- --------------------------------
I2S/PCM5102 DAC         DIN               P1.07
----------------------- ----------------- --------------------------------
I2S/PCM5102 DAC         LRCK              P1.06
======================= ================= ================================

