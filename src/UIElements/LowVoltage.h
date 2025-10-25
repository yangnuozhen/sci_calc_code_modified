#ifndef LOW_VOLTAGE_H
#define LOW_VOLTAGE_H

#include "../Animation/Animation.h"

class LowVoltage : public UIElement {
    public:
        LowVoltage();

        void activate();

        void draw();

        void update();
};

#endif