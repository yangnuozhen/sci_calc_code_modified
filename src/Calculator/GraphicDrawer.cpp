#include "GraphicDrawer.h"

GraphicDrawer::GraphicDrawer(int x, int y, int width, int height) : UIElement(x, y, width, height)
{
    this->hasExpression = false;
    this->scale = 10.0;
}

void GraphicDrawer::init()
{
}

void GraphicDrawer::activate()
{
}

void GraphicDrawer::setExpression(std::string expression)
{
    bool state = false;
    int curtimesyntax = 0;
    if (!syntaxChecker.checkSyntax(expression))
    {
        state = true;
        curtimesyntax = millis();
    }
    if (state)
    {
        if (millis() - curtimesyntax > 200)
        {
            state = false;
        }
        else
        {
            draw();
            const char *str = "Error: Bad Expression";
            u8g2.drawStr(x + width / 2 - u8g2.getStrWidth(str) / 2, y + height / 2 + (u8g2.getAscent() - u8g2.getDescent()) / 2, str);
            u8g2.sendBuffer();
            delay(400);
            return;
        }
    }
    this->expression = expression;
    expressionObject = new Expression(expression);
    hasExpression = true;
}

void GraphicDrawer::clearGraph()
{
    hasExpression = false;
}
float GraphicDrawer::getScale()
{
    return scale;
}

void GraphicDrawer::setScale(float scale)
{
    this->scale = scale;
}

void GraphicDrawer::draw()
{
    u8g2.drawRFrame(x, y, width, height, 2);
    if (hasExpression)
    {
        // Draw graph

        // 绘制坐标轴
        u8g2.drawLine(x + 2, (y + height) / 2, x + width - 2, (y + height) / 2); // x轴
        u8g2.drawLine((x + width) / 2, y + 2, (x + width) / 2, y + height - 2);  // y轴
        for (int x = 0; x < width; x++)
        {
            // 将屏幕坐标转换为函数坐标
            float fx = (x - 128) / scale;                               // 假设x范围为[-12.8, 12.8]
            float fy = expressionObject->evaluateWithVariable("x", fx); // 计算y值
            int y = (this->y + height) / 2 - fy * scale;                // 将函数值转换为屏幕坐标，假设y范围为[-3.2, 3.2]

            // 绘制点
            if (y >= this->y && y < this->y + height)
            {
                /*                 Serial.println("Drawing");
                                Serial.println("fx = " + String(fx));
                                Serial.println("fy = " + String(fy));
                                Serial.println("-> scale = " + String(scale));
                                Serial.println("x = " + String(x));
                                Serial.println("y = " + String(y)); */
                u8g2.drawPixel(x, y);
            }
        }
    }
    else
    {
        const char *str = "Press ENTER To Draw";
        u8g2.drawStr(x + width / 2 - u8g2.getStrWidth(str) / 2, y + height / 2 + (u8g2.getAscent() - u8g2.getDescent()) / 2, str); // ps:这是一行可供参考的中央字符串绘制代码（
    }
}

void GraphicDrawer::update()
{
    draw();
}

void GraphicDrawer::aniOut()
{
    insertAnimation(new Animation(this, INDENTINV, x, -70, 500));
    insertTmpAnimationPointer(this);
}
