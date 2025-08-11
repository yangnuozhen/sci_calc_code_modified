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
    isDrew = false;
    isDrawing = true;
    update();
    u8g2.sendBuffer();
    calcGraph();
}

void GraphicDrawer::clearGraph()
{
    hasExpression = false;
    isDrew = false;
    points.clear();
}
void GraphicDrawer::calcGraph()
{
    if (!hasExpression) return;

    points.clear();
    for (int x = 0; x < width; x++)
    {
        float fx = (x - 105) / scale;
        float fy = expressionObject->evaluateWithVariable("x", fx);
        int y = (this->y + height) / 2 - fy * scale;

        if (y >= this->y && y < this->y + height)
        {
            points.push_back(std::make_pair(x, y));
        }
    }
    isDrew = true;
    isDrawing = false;
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
    if (isDrawing){
        u8g2.drawStr(x + width / 2 - u8g2.getStrWidth("Drawing...") / 2, y + height / 2 + (u8g2.getAscent() - u8g2.getDescent()) / 2, "Drawing..."); // ps:这是一行可供参考的中央字符串绘制代码（
        return;
    }
    


    if (points.size() > 0)
    {
        // 绘制坐标轴
        u8g2.drawLine(x + 2, (y + height) / 2, x + width - 2, (y + height) / 2); // x轴
        u8g2.drawLine((x + width) / 2, y + 2, (x + width) / 2, y + height - 2);  // y轴
        for (const auto &point : points)
        {
            u8g2.drawPixel(point.first, point.second);
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
