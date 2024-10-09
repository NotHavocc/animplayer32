#include <TJpg_Decoder.h>
#include "esp_task_wdt.h"


#define FS_NO_GLOBALS
#include <FS.h>
#ifdef ESP32
#include "SPIFFS.h"
#endif

#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define SD_CS 5

#include "SPI.h"
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
SPIClass hspi = SPIClass(HSPI);

SPISettings sdSpiSettings(80000000, MSBFIRST, SPI_MODE0);  // 80mhz, adjust if needed

int imageFrame = 0;
//int latestNumber = 3110; // legacy frame counter method, use for videos/gifs with many frames (new method is VERY slow for big gifs/videos), type in the amount of frames the gif has
int latestNumber;

// SETTINGS FOR DEBUGGING AND GENERAL OPTIMIZATION
#define preloadCount 3       // number of frames to preload
#define showFPS false             // set to false if you dont want to see the FPS counter on the screen
#define showFramePassed false     // show what GIF frame its currently on
#define reset10thFrame false      // not very good optimisation method but im keeping it here, resets display (white screen) every 10th frame
#define resetAfterPlayback false  // SLIGHTLY better perfomance but at the end of the GIF a white screen flashes for a second
#define useLegacy false           // use legacy frame counting mode (defined in code)
// SETTINGS FOR DEBUGGING AND GENERAL OPTIMIZATION

String preloadFrames[preloadCount];


bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
  if (y >= tft.height()) return 0;

  tft.pushImage(x, y, w, h, bitmap);

  return 1;
}


int findLargestNumber(fs::FS &fs, const char *dirname) {
  File root = fs.open(dirname);
  if (!root || !root.isDirectory()) {
    return -1;
  }

  File file;
  int maxNumber = -1;

  while (file = root.openNextFile()) {
    if (!file.isDirectory()) {
      String fileName = file.name();
      int fileNumber = fileName.toInt();

      if (fileNumber > maxNumber) {
        maxNumber = fileNumber;
      }
    }
    file.close();
  }

  return maxNumber;
}

void preloadNextFrames() {
  for (int i = 0; i < preloadCount; i++) {
    int nextFrame = imageFrame + i;
    if (nextFrame <= latestNumber) {
      preloadFrames[i] = "/" + String(nextFrame) + ".jpg";
    } else {
      preloadFrames[i] = "";
    }
  }
}


void setup() {
  hspi.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, SD_CS);

  tft.init();
  tft.setTextColor(0xFFFF, 0x0000);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setCursor(0, 0);
  tft.printf("Display started...");
  delay(200);
  tft.setCursor(0, 10);
  tft.printf("AnimPlayer32 beta 2");
  delay(100);

  SPI.beginTransaction(sdSpiSettings);

  if (!SD.begin(SD_CS, hspi)) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.printf("SD card not found!~");
    while (1) delay(0);
  }
  tft.setCursor(0, 20);
  tft.printf("SD initialized...");
  delay(100);
  tft.setTextColor(TFT_WHITE);
  if (useLegacy == false) {
    latestNumber = findLargestNumber(SD, "/");
  }
  tft.setTextSize(1);
  tft.setCursor(0, 30);
  tft.printf("%d frames", latestNumber);
  tft.setCursor(0, 40);
  tft.printf("%d Will preload", preloadCount, "frames");
  tft.setCursor(0, 50);
  tft.printf("Init complete...");

  delay(400);

  SPI.endTransaction();

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tft_output);

  preloadNextFrames();
}

void loop() {
  uint32_t startTime = millis();
  //TJpgDec.getSdJpgSize(&w, &h, "/" +  String(imageFrame) + ".jpg"); // debug (this STRONGLY impacts framerate/perfomance, dont turn on unless you know what your doing.)
  TJpgDec.drawSdJpg(0, 0, "/" + String(imageFrame) + ".jpg");
  imageFrame++;
  preloadNextFrames();

  if (imageFrame > latestNumber) {
    imageFrame = 1;
  }

  uint32_t frameTime = millis() - startTime;
  float fps = 1000.0 / frameTime;

  if (showFPS == true) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.printf("FPS: %.2f", fps);
  }
  if (showFramePassed == true) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 120);
    tft.printf("F: %d", imageFrame);
  }
  if (reset10thFrame == true) {
    if (imageFrame % 10 == 0) {
      tft.fillScreen(TFT_WHITE);
    }
  }
  if (resetAfterPlayback == true) {
    if (imageFrame > latestNumber) {
      tft.fillScreen(TFT_WHITE);
    }
  }

  if (imageFrame > latestNumber) {
    imageFrame = 1;
  }
  delay(1);
}
