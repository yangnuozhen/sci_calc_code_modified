#include "Keyboard.h"

Key::Key() {

}

// Creates a Key object
Key::Key(int id) {
    this -> id = id;
    this -> status = NOT_PRESSED;
    this -> clickCnt = 0;
}

// Modifies the key object's properties
void Key::modifyKey(float statusTime, float startPress, float endPress, bool isPressed, KeyStatus status, int clickCnt) {
    this -> statusTime = statusTime;
    this -> startPress = startPress;
    this -> endPress = endPress;
    this -> isPressed = isPressed;
    this -> status = status;
    this -> clickCnt = clickCnt;
}

// Updates the key status and its click count (single press, double press, etc.)
void Key::updateKey(bool curState, float curtime) {
    if (curState) {
        if (!(this -> isPressed)) {
            this -> clickCnt++;
            this -> isPressed = true;
            this -> status = RISING_EDGE;
            this -> statusTime = 0;
            this -> startPress = curtime;
        }
        else if (this -> isPressed) {
            this -> status = PRESSED;
            this -> statusTime = curtime - startPress;
        }
    }
    else {
        if (!(this -> isPressed)) {
            this -> status = NOT_PRESSED;
            this -> statusTime = curtime - endPress;
            if (this -> statusTime > DELTA_TIME) {
                this -> clickCnt = 0;
            }
        }
        else if (this -> isPressed) {
            this -> isPressed = false;
            this -> status = FALLING_EDGE;
            this -> statusTime = 0;
            this -> endPress = curtime;
            this -> startPress = 0;
        }
    }
}

Key Key::getKey() {
    return *this;
}

bool Key::getIsPressed() {
    return this -> isPressed;
}

int Key::getClickCnt() {
    return this -> clickCnt;
}

float Key::getStatusTime() {
    return this -> statusTime;
}

KeyStatus Key::getStatus() {
    return this -> status;
}


Keyboard::Keyboard() {
    
}

// Initializes the keyboard IO pins
void Keyboard::init() {
    for (int i = 0; i < ROWCNT; i++) {
        pinMode(ROW_MISO[i], INPUT);
    }
    for (int i = 0; i < COLCNT; i++) {
        pinMode(COL_MOSI[i], OUTPUT);
        digitalWrite(COL_MOSI[i], LOW);
    }
}

// Updates the keyboard
void Keyboard::update() {
    float curtime = millis();
    if (curtime - this -> prevtime > REFRESH_DURATION) {
        sample = true;
        for (int i = 0; i < COLCNT; i++) {
            int curCol = COL_MOSI[i];
            for (int j = 0; j < ROWCNT; j++) {
                digitalWrite(curCol, HIGH);
                //delay(10);
                int curRow = ROW_MISO[j];
                //Serial.printf("Key[%d][%d]: [%d][%d] %d\n", j, i, curRow, curCol, digitalRead(curRow));
                this -> keys[j][i].updateKey(digitalRead(curRow), curtime);
                digitalWrite(curCol, LOW);  
            }
            //delay(10);
        }
    }
    this -> prevtime = curtime;
}


Key Keyboard::getKey(int row, int col) {
    return this -> keys[row][col];
}

// Gets the first rising edge key
std::pair <int, int> Keyboard::getRisingEdgeKey() {
    for (int i = 0; i < ROWCNT; i++) {
        for (int j = 0; j < COLCNT; j++) {
            if (this -> keys[i][j].getStatus() == RISING_EDGE) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

// Gets the first falling edge key
std::pair <int, int> Keyboard::getFallingEdgeKey() {
    for (int i = 0; i < ROWCNT; i++) {
        for (int j = 0; j < COLCNT; j++) {
            if (this -> keys[i][j].getStatus() == FALLING_EDGE) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

// Gets the first changed key
std::pair <int, int> Keyboard::getChangedKey() {
    for (int i = 0; i < ROWCNT; i++) {
        for (int j = 0; j < COLCNT; j++) {
            if (this -> keys[i][j].getStatus() == RISING_EDGE || this -> keys[i][j].getStatus() == FALLING_EDGE) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

// Prints the status of the keys for debugging
void Keyboard::printKeys() {
    for (int i = 0; i < ROWCNT; i++) {
        for (int j = 0; j < COLCNT; j++) {
            Serial.printf("Key[%d][%d]: isPressed: %d, statusTime: %lf\n", i, j, this -> keys[i][j].getIsPressed(), this -> keys[i][j].getStatusTime());
            std::string status;
            switch (this -> keys[i][j].getStatus()) {
                case NOT_PRESSED:
                    status = "N";
                    break;
                case PRESSED:
                    status = "P";
                    break;
                case RISING_EDGE:
                    status = "R";
                    break;
                case FALLING_EDGE:
                    status = "F";
                    break;
                default:
                    status = "N";
            }
            if (this -> keys[i][j].getIsPressed()) {
                u8g2.setDrawColor(0);
                u8g2.drawStr(j * 12, i * 12 + 12, status.c_str());
                u8g2.setDrawColor(1);
            }
            else {
                u8g2.drawStr(j * 12, i * 12 + 12, status.c_str());
            }
        }
    }
}
