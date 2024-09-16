#include "GraphicCalc.h"

GraphicCalc::GraphicCalc(int x, int y, int width, int height, InputBox *expressionInput) : UIElement(x, y, width, height)
{
    this->expressionInput = expressionInput;
    this->graphicDrawer = new GraphicDrawer(x, y, width, height - 12);
}

void GraphicCalc::init()
{
    expressionInput->init();
}

void GraphicCalc::activate()
{
    currentElement = this;
    insertAnimation(new Animation(this->expressionInput, INDENT, 0, 57, 500));
    insertAnimation(new Animation(this->graphicDrawer, INDENT, 0, 0, 500));
}

void GraphicCalc::enter()
{
    graphicDrawer->setExpression(expressionInput->getStr());
}

void GraphicCalc::draw()
{
    this->expressionInput->draw();
    //this->graphicDrawer->draw();
    u8g2.drawStr(215, 24, (std::to_string(graphicDrawer -> getScale()) + "x").c_str());
}

void GraphicCalc::update()
{
    draw();
    this->expressionInput->update();
    this->graphicDrawer->update();
    std::string str = calcLayout.updateString();
    if (str == "CLEAR")
    {
        graphicDrawer->clearGraph();
        expressionInput->clearStr();
    }
    else if (str == "ESC")
    {
        this->graphicDrawer->aniOut();
        insertAnimation(new Animation(this->expressionInput, INDENTINV, 0, 64, 500));
        insertTmpAnimationPointer(this->expressionInput);
        while (!tmpAnimationUI.empty())
        {
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 210, 64);
            u8g2.setDrawColor(1);
            currentElement->draw();
            updateTmp();
            u8g2.sendBuffer();
            animateAll();
        }

        goBack();
    }
    else if (str == "ENTER" && this->expressionInput->getStr() != "")
    {
        enter();
    } else if (str == "SCALE UP"){
        graphicDrawer -> setScale(graphicDrawer -> getScale() + 1);
    } else if (str == "SCALE DOWN"){
        graphicDrawer -> setScale(graphicDrawer -> getScale() - 1);
    }
}