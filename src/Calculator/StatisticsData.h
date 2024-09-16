#ifndef STATISTICSDATA_H

#define STATISTICSDATA_H

#include "../UIElements/UIElement.h"
#include "../UIElements/Text.h"

class StatisticsData : public UIElement
{
    public:
        StatisticsData(int x, int y, int width, int height, int index, long double data);
        void activate();
        void draw();
        void update();
    private:
        Text index;
        Text data;
};

#endif