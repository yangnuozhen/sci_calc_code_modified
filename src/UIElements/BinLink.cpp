#include "BinLink.h"

BinLink::BinLink(std::string path) {
    this -> path = "/AnotherPrograms" + path;
}

BinLink::BinLink(std::string path, bool isRoot) {
    if (isRoot) {
        this -> path = path;
    }
    else {
        this -> path = "/AnotherPrograms" + path;
    }
    
}

void BinLink::activate() {
    updateFromFS(SD, this -> path);
    rebootEspWithReason(this -> path + " uploaded!");
}

void BinLink::draw() {
    
}

void BinLink::update() {

}