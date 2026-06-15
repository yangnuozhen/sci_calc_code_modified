
// #include <U8g2lib.h>
// #include <U8x8lib.h>

#include "UIMain.h"

BleKeyboard bleKeyboard("SCI-CALC", "SHAP", 100);

void appInit()
{
    u8g2.begin();
    u8g2.setFontMode(0);
    Serial.begin(115200);
    u8g2.setFont(u8g2_font_profont10_mf);
    u8g2.setFontPosCenter();
    u8g2.setFontMode(1);
    u8g2.enableUTF8Print();

    Serial.begin(115200);
    if (SD.begin(4))
    {
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE)
        {
            Serial.println("No SD_MMC card attached");
        }
        else
        {
            Serial.println("SD Card init success.");
            sdAvailable = true;
        }
    }

    pinMode(36, INPUT);
    initBatteryAdc();
    xTaskCreatePinnedToCore(batteryUpdateTask, "BatTask", 2048, NULL, 1, NULL, 0);
}

void setup()
{
    appInit();
    kb.init();
    bleKeyboard.begin();
    if (!sdAvailable)
    {
        mainMenu = noSdMainMenu;
    }
    else{
        mainMenu = hasSdMainMenu;
    }
    currentElement->init();
    u8g2.clearBuffer();
    currentElement->activate();
}

void loop()
{
    animateAll();
    bleKeyboard.setBatteryLevel(getBatteryLevel());
    u8g2.clearBuffer();
    kb.update();
    drawSidebar();
    currentElement->update();
    checkLowVoltage();
    updateTmp();
    u8g2.sendBuffer();
}