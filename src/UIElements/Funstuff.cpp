#include "Funstuff.h"

Funstuff::Funstuff() : UIElement(0, 0, 256, 64)
{
    this->expression = "default";
}
Funstuff::Funstuff(std::string expression) : UIElement(0, 0, 256, 64)
{
    this->expression = expression;
}

void Funstuff::activate()
{
    currentElement = this;
    HIDE_SIDEBAR = true;
}

void Funstuff::drawDefaultSmile()
{
    u8g2.setFont(u8g2_font_crox5hb_tf);
    u8g2.setFontDirection(1);
    switch (currentSmile)
    {
    case 0:
        u8g2.drawUTF8(WIDTH / 2, HEIGHT / 2 - 20, " :)");
        break;
    case 1:
        u8g2.drawUTF8(WIDTH / 2, HEIGHT / 2 - 20, " :D");
        break;

    case 2:
        u8g2.drawUTF8(WIDTH / 2, HEIGHT / 2 - 20, " :P");
        break;

    case 3:
        u8g2.drawUTF8(WIDTH / 2, HEIGHT / 2 - 20, " :(");
        break;

    case 4:
        u8g2.drawUTF8(WIDTH / 2, HEIGHT / 2 - 20, " :o");
        break;

    case 5:
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_crox3hb_tf);
        u8g2.drawUTF8(WIDTH / 2 - 15, HEIGHT / 2, "^_^");
        break;

    case 6:
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_crox3hb_tf);
        u8g2.drawUTF8(WIDTH / 2 - 15, HEIGHT / 2, ">w<");
        break;
    case 7:
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_crox3hb_tf);
        u8g2.drawUTF8(WIDTH / 2 - 15, HEIGHT / 2, "O_o");
        break;

    case 8:
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_crox3hb_tf);
        u8g2.drawUTF8(WIDTH / 2 - 15, HEIGHT / 2, "o_O");
        break;
    case 9:
        u8g2.setFont(u8g2_font_fub42_tf);
        u8g2.setFontDirection(1);
        u8g2.drawUTF8(WIDTH / 2, HEIGHT / 2 - 40, " :)");
        u8g2.sendBuffer();
        break;
    default:
        this->currentSmile = 0;
        break;
    }
    u8g2.setFont(u8g2_font_profont10_mf);
    u8g2.setFontDirection(0);
}

void Funstuff::draw()
{
    if (this->expression == "default")
    {
        drawDefaultSmile();
    }
    drawDefaultSmile();
}

void Funstuff::update()
{
    draw();
    // Serial.println("vuuhfofhdoijfhdiovndiosjdoifjdiofhdoifjdsso");

    if (kb.getKey(0, 0).getIsPressed())
    {
        goBack();
        HIDE_SIDEBAR = false;
    }
    else if (kb.getRisingEdgeKey() == std::make_pair(3, 3))
    {
        currentSmile = random(0, 10);
    }
}