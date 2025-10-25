#ifndef BIN_LINK_H
#define BIN_LINK_H

#include "../Animation/Animation.h"

class BinLink : public UIElement {
    public: 
        BinLink(std::string path);
        BinLink(std::string path, bool isRoot);

        void activate();

        void draw();

        void update();

    private:
        std::string path;
}; 

#endif