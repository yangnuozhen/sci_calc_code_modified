#ifndef GRAPHIC_DRAWER_H
#define GRAPHIC_DRAWER_H
#include "../Animation/Animation.h"
#include "Expression.h"


class GraphicDrawer : public UIElement {
    public:
        GraphicDrawer(int x, int y, int width, int height);

        void init();
        void activate();
        void setExpression(std::string expression);
        void clearGraph();
        float getScale();
        void setScale(float scale);
        void draw();
        void update();
        void aniOut();
    private:
        std::string expression;
        Expression* expressionObject;
        float scale;
        bool hasExpression;
};
#endif