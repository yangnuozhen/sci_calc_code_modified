#ifndef STATISTICS_UI_H
#define STATISTICS_UI_H

#include <regex>
#include <memory>
#include "Statistics.h"
#include "../UIElements/UIElement.h"
#include "../Animation/Animation.h"
#include "../UIElements/InputBox.h"
#include "../UIElements/Menu.h"
#include "../UIElements/ChineseText.h"
#include "StatisticsData.h"

class StatisticsUI : public UIElement
{
public:
    StatisticsUI(int x, int y, int width, int height, InputBox *dataInputBox, Menu *dataMenu);
    void activate();
    void init();
    void inputNum();
    void draw();
    void update();

private:
    Statistics statistics;
    InputBox *dataInputBox;
    Menu *dataMenu;
    Menu *resultMenu;
    bool viewState; // 0: input, 1: result
    bool mode;      // 0: input, 1: menu
    int curtimesyntax = 0;
    bool state = false;
    bool isValidNumber(const std::string &str);
    void calculateAll();
    multi_heap_info_t heap_info;
};

#endif