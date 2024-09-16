#include "StatisticsData.h"

StatisticsData::StatisticsData(int x, int y, int width, int height, int index, long double data) : UIElement(x, y, width, height)
{
    this->index.setText(numToStr(index));
    this->index.setWidth(110);
    this->data.setText(numToStr(data));
    this->data.init();
    this->doTransition = false;
}

void StatisticsData::activate()
{
}

void StatisticsData::draw()
{
    this->index.setX(this->x);
    this->index.setY(this->y);

    this->data.setX(this->x + width - data.getStr().size() * 5 - 5);
    this->data.setY(this->y);
    
    this->data.draw();
    this->index.draw();
    
}

void StatisticsData::update()
{
    draw();
    goBack();
}