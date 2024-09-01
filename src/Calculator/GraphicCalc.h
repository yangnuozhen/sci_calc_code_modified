#ifndef GRAPHICCALC_H
#define GRAPHICCALC_H

#include "../Animation/Animation.h"
#include "../UIElements/InputBox.h"
#include "GraphicDrawer.h"
#include "ExpressionBlock.h"

class GraphicCalc : public UIElement
{
    public:
        GraphicCalc(int x, int y, int width, int height, InputBox* expressionInput);

        void init();
        void activate();

        void enter();
        void draw();
        void update();
    private:
        InputBox* expressionInput;
        GraphicDrawer* graphicDrawer;
};
#endif