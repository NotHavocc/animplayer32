# AnimPlayer32 <img src="https://github.com/user-attachments/assets/69113c48-5157-4f38-9c8d-da646bc1642a" width="150" align="left">

![GitHub issues](https://img.shields.io/github/issues/NotHavocc/animplayer32)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/nothavocc/animplayer32/total)
![GitHub License](https://img.shields.io/github/license/nothavocc/animplayer32)
![GitHub Release](https://img.shields.io/github/v/release/NotHavocc/animplayer32)
![Static Badge](https://img.shields.io/badge/open_source-with_%3C3-blue)

A (sorta) GIF player for ESP32/arduino

## Showcase
![ezgif-6-591bf0e6e6-ezgif com-optimize](https://github.com/user-attachments/assets/fa6c964d-8322-4861-814a-60dc5a2bffa7)

## Hardware Dependencies
For this project you dont need really much stuff.
Stuff you need: 
- ESP32 (or any arduino compatible board that can handle JPG processing.)
- Display (any display supported by Bodmer's [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) works.)
- SD card slot (can be internal and external module)
- Wires (obviously, unless...)

## Software Dependencies
For the code to upload and work properly you will need some extra libraries:
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) , FS, [TJpg_Decoder](https://github.com/Bodmer/TJpg_Decoder), SD, SPIFFS

> [!NOTE]  
> FS and SPIFFS are included in the ESP32/ESP8266 core, if you dont have one of these boards you might have to install them seperately.

I have developed a software for automatically turning GIFS to frames used by this program. You will need it unless you will do it manually.
[animplayer32converter](https://github.com/NotHavocc/animplayer32converter)

## Setup

### Before flash
At the beginning of the code in the .ino file there is a special section where you can toggle different debug/optimisation settings. As of now they are:
```
SETTINGS FOR DEBUGGING AND GENERAL OPTIMIZATION
showFPS = show FPS counter at the top left of the screen
showFramePassed =  show what GIF frame its currently on
reset10thFrame = not very good optimisation method but im keeping it here, resets display (white screen) every 10th frame
resetAfterPlayback = SLIGHTLY better perfomance but at the end of the GIF a white screen flashes for a second
useLegacy =  use legacy frame counting mode (defined in code)
```

useLegacy will have more docummentation at PLACEHOLDER_LINK.

> [!IMPORTANT]  
> Wire the display and SD card module correctly, you can change the pins if you want to for the SD. The TFT has your config in the User_Setup.h file of the [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library. Dont forget to set up TFT_eSPI first.

### Flashing

After fiddling with the settings, it is time to flash. just use your ideal IDE to flash the code onto your board.

### Creating a GIF for AnimPlayer32

I have made a program for automatizing that, as mentioned. It is really simple, just follow what it says.

- Download the program

 Download it [here](https://github.com/NotHavocc/animplayer32converter).

- Input the path of your GIF, then press Enter.
![path input](https://github.com/user-attachments/assets/c773739f-cb5c-4771-9278-9881a38867d3)

- You will have to input the drive letter of the SD card, or any folder if you want to save it to your PC.

![drive input](https://github.com/user-attachments/assets/c945e1cc-ce44-4e72-b859-e7e92da6f6db)

- Input Y to confirm, N to exit.

![image](https://github.com/user-attachments/assets/4ee2c83a-cb8e-4a93-a159-8af0ada2b142)

- The program automatically converts the GIFs to .jpg format and renames them.

![image](https://github.com/user-attachments/assets/d2a32b2a-c534-4316-80ed-caa4b0e549f5)

- Results

![image](https://github.com/user-attachments/assets/76522883-3d18-4768-b2c9-d3ba19f8d021)

> [!WARNING]  
> GIFs have alpha transparency, just like PNGs. So some GIF frames can be corrupted after conversion, if it happens try again with a similar GIF.

## Other

### useLegacy

useLegacy is only useful for GIFs with many frames, as it will take long (depending on SD card and MCU speed) to count them all to make the program know how much frames to play.

- Set useLegacy as true in the code
  ![](https://github.com/user-attachments/assets/05257a7e-e2cf-43f4-80de-589bde01c95d)

- Comment the variable with 0 and uncomment the one where it says legacy. put the amount of frames the GIF has there\
 ![](https://github.com/user-attachments/assets/acbb5444-a733-4911-8dea-205b581ff79b)

After that frame counting will be instantaneous (as it doesnt need to go through the files to scan) , its unreliable if you hotswap GIFs through your SD card, and its only reccomended for large GIFs that will take a lot of time to count.

### Slight optimizations for large GIFs

You can put these settings as true to get slight perfomance increase.

![image](https://github.com/user-attachments/assets/7a271b5a-05d3-4316-b148-5ed13c703811)

> [!NOTE]
> Its only reliable for large GIFs, once again. But resetAfterPlayback can be a bit useful for lighter GIFs.

## Perfomance
These are my tests that I have tested on specific hardware. Any little thing can impact perfomance, even the GIF used. If you want, you can post your own perfomance results at the issues section and tag it as "Perfomance test", dont forget to write your board, SD card slot model, TFT driver, SD card itself, SPI speeds used and a video showing the FPS.
- My tests:
  ESP32, ST7735 display, random SD card slot, 128mb SD card and 80mhz SPI speed (not sure if some of that has gone to waste)\
  Max: 15-ish FPS\
  Min: 13-ish FPS\
- Community tests:
  coming soon, hopefully


  























  




