#include "LowVoltage.h"
#include "../UIMain.h"
#include <string>

LowVoltage::LowVoltage()
{
}

void LowVoltage::activate()
{
    HIDE_SIDEBAR = true;
}
void LowVoltage::draw()
{
    u8g2.setFont(u8g2_font_profont12_mf);
    u8g2.drawStr(60, 25, "Low Voltage!");
    u8g2.drawStr(20, 45, "Please connect to the charger.");
    u8g2.drawStr(20, 65, "DO NOT TURN OFF or you will lose your work.");
    std::string volt = std::string("Voltage :") + std::to_string(getBatteryVoltage()) + "V";
    u8g2.drawStr(55, 85, volt.c_str());
}


void LowVoltage::update()
{
    draw();
    
}
