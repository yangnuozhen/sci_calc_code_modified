#ifndef CHINESE_TEXT_H
#define CHINESE_TEXT_H

#include "../Animation/Animation.h"
#include <locale>
#include <codecvt>


class ChineseText : public UIElement {
    public:
        ChineseText(int restX, int restY, int activeX, int activeY, int width, int height, bool hasBox, std::string str);

        void init();
        void activate();
        void draw();
        void update();

    private:
        int restX, restY, activeX, activeY;
        bool hasBox;
        std::string str;
        int pos;
        int currentPage;
        void nextPage();
        void prevPage();
        void aniIn();
        void aniOut();
};

#endif