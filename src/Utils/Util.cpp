#include "Util.h"

int lastNode = -1;

bool sdAvailable = false;

// List of operators available in the calculator
std::string tokenStr[MAXOP] = {"+", "-", "*", "/", "^", "sqrt", "ln", "exp", "sin", "cos", "tan", "sec", "csc", "cot", "asin", "acos", "atan", "(", ")", "=", "-", "NUM", "VAR"};
U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, SPI_RESET);

bool sample = false;
bool cursorMode = true; // Soild fill: true, border: false

// Keyboard matrix IO pins
byte ROW_MISO[ROWCNT] = {33, 32, 35, 34, 39};
byte COL_MOSI[COLCNT] = {13, 12, 14, 27, 26, 25};

bool HIDE_SIDEBAR = false;
bool SHOW_BATTERY_PERCENTAGE = false;

char keys[ROWCNT][COLCNT] = {
    {'i', '/', '*', '-', 'a', 'b'},
    {'7', '8', '9', '+', 'c', 'd'},
    {'4', '5', '6', '^', 'e', 'f'},
    {'1', '2', '3', 'e', 'g', 'h'},
    {'0', '.', 'f', 'd', 'i', 'j'}};

// Animation settings
AnimationType menuScroll = BOUNCE;
AnimationType cursorScroll = LINEAR;
AnimationType menuEntry = BOUNCE;
AnimationType sliderMove = BOUNCE;

std::string getKeyString(uint16_t x)
{
    switch (x)
    {
    case KEY_LEFT_CTRL:
        return "L CTRL";
    case KEY_LEFT_SHIFT:
        return "L SHIFT";
    case KEY_LEFT_ALT:
        return "L ALT";
    case KEY_LEFT_GUI:
        return "L GUI";
    case KEY_RIGHT_CTRL:
        return "R CTRL";
    case KEY_RIGHT_SHIFT:
        return "R SHIFT";
    case KEY_RIGHT_ALT:
        return "R ALT";
    case KEY_RIGHT_GUI:
        return "R GUI";

    case KEY_UP_ARROW:
        return "UP";
    case KEY_DOWN_ARROW:
        return "DOWN";
    case KEY_LEFT_ARROW:
        return "LEFT";
    case KEY_RIGHT_ARROW:
        return "RIGHT";
    case KEY_BACKSPACE:
        return "BKSP";
    case KEY_TAB:
        return "TAB";
    case KEY_RETURN:
        return "ENTER";
    case KEY_ESC:
        return "ESC";
    case KEY_INSERT:
        return "INS";
    case KEY_PRTSC:
        return "PRTSC";
    case KEY_DELETE:
        return "DEL";
    case KEY_PAGE_UP:
        return "PG UP";
    case KEY_PAGE_DOWN:
        return "PG DN";
    case KEY_HOME:
        return "HOME";
    case KEY_END:
        return "END";
    case KEY_CAPS_LOCK:
        return "CAPS LK";
    case KEY_F1:
        return "F1";
    case KEY_F2:
        return "F2";
    case KEY_F3:
        return "F3";
    case KEY_F4:
        return "F4";
    case KEY_F5:
        return "F5";
    case KEY_F6:
        return "F6";
    case KEY_F7:
        return "F7";
    case KEY_F8:
        return "F8";
    case KEY_F9:
        return "F9";
    case KEY_F10:
        return "F10";
    case KEY_F11:
        return "F11";
    case KEY_F12:
        return "F12";
    case KEY_F13:
        return "F13";
    case KEY_F14:
        return "F14";
    case KEY_F15:
        return "F15";
    case KEY_F16:
        return "F16";
    case KEY_F17:
        return "F17";
    case KEY_F18:
        return "F18";
    case KEY_F19:
        return "F19";
    case KEY_F20:
        return "F20";
    case KEY_F21:
        return "F21";
    case KEY_F22:
        return "F22";
    case KEY_F23:
        return "F23";
    case KEY_F24:
        return "F24";
    case KEY_LAYER_SWITCH:
        return "LAYER SWITCH";
    case KEY_EXP:
        return "exp";
    case KEY_LN:
        return "ln";
    case KEY_SIN:
        return "sin";
    case KEY_COS:
        return "cos";
    case KEY_TAN:
        return "tan";
    case KEY_ASIN:
        return "asin";
    case KEY_ACOS:
        return "acos";
    case KEY_ATAN:
        return "atan";
    case KEY_SEC:
        return "sec";
    case KEY_CSC:
        return "csc";
    case KEY_COT:
        return "cot";
    case KEY_SQRT:
        return "sqrt";
    case KEY_MODE_SWITCH:
        return "MODE SWITCH";
    case KEY_RPN_SWITCH:
        return "RPN SWITCH";
    case KEY_CLEAR:
        return "CLEAR";
    case KEY_SCALE_UP:
        return "SCALE UP";
    case KEY_SCALE_DOWN:
        return "SCALE DOWN";
    default:
        std::string res(1, char(x));
        return res;
    }
}

bool strIsOperator(std::string str)
{
    for (int i = 0; i < MAXOP; i++)
    {
        if (str == tokenStr[i])
            return true;
    }
    return false;
}

bool strIsNum(std::string str)
{
    for (char c : str)
    {
        if (!('0' <= c && c <= '9' || c == '.'))
        {
            return false;
        }
    }
    return true;
}

bool cmpLongDouble(long double a, long double b)
{
    if (fabsl(a - b) <= EPSILON)
        return true;
    return false;
}

int getPrecedenceToken(Token token)
{
    switch (token)
    {
    case EQUALS:
        return 0;
    case ADD:
    case SUB:
        return 1;
    case MUL:
    case DIV:
        return 2;
    case POW:
        return 3;
    case SIN:
    case COS:
    case TAN:
    case SEC:
    case CSC:
    case COT:
    case ASIN:
    case ACOS:
    case ATAN:
    case LN:
    case EXP:
    case SQRT:
    case NEGATIVE:
        return 4;
    default:
        return 5;
    }
}

Token convertToEnum(std::string s)
{
    if (s == "+")
        return ADD;
    if (s == "-")
        return SUB;
    if (s == "*")
        return MUL;
    if (s == "/")
        return DIV;
    if (s == "^")
        return POW;
    if (s == "sqrt")
        return SQRT;
    if (s == "ln")
        return LN;
    if (s == "exp")
        return EXP;
    if (s == "sin")
        return SIN;
    if (s == "cos")
        return COS;
    if (s == "tan")
        return TAN;
    if (s == "sec")
        return SEC;
    if (s == "csc")
        return CSC;
    if (s == "cot")
        return COT;
    if (s == "asin")
        return ASIN;
    if (s == "acos")
        return ACOS;
    if (s == "atan")
        return ATAN;
    if (s == "(")
        return LPARA;
    if (s == ")")
        return RPARA;
    if (s == "=")
        return EQUALS;
    if ('0' <= s[0] && s[0] <= '9' || s[0] == '.')
        return NUM;
    return VAR;
}

std::string convertToString(Token t)
{
    if (t == ADD)
        return "ADD";
    if (t == SUB)
        return "SUB";
    if (t == MUL)
        return "MUL";
    if (t == DIV)
        return "DIV";
    if (t == POW)
        return "POW";
    if (t == SQRT)
        return "SQRT";
    if (t == LN)
        return "LN";
    if (t == EXP)
        return "EXP";
    if (t == SIN)
        return "SIN";
    if (t == COS)
        return "COS";
    if (t == TAN)
        return "TAN";
    if (t == SEC)
        return "SEC";
    if (t == CSC)
        return "CSC";
    if (t == COT)
        return "COT";
    if (t == ASIN)
        return "ASIN";
    if (t == ACOS)
        return "ACOS";
    if (t == ATAN)
        return "ATAN";
    if (t == LPARA)
        return "LPARA";
    if (t == RPARA)
        return "RPARA";
    if (t == NUM)
        return "NUM";
    if (t == EQUALS)
        return "EQUALS";
    if (t == NEGATIVE)
        return "NEGATIVE";
    return "VAR";
}

std::string numToStr(long double x)
{
    float tmp = float(x);
    std::ostringstream ss;
    ss.precision(6);
    ss << tmp;

    return ss.str();
}

std::string numToStrRaw(long double x)
{
    float tmp = float(x);
    std::ostringstream ss;
    ss.precision(6);
    ss << tmp;

    return ss.str();
}

void performUpdate(Stream &updateSource, size_t updateSize)
{
    if (Update.begin(updateSize))
    {
        size_t written = Update.writeStream(updateSource);
        if (written == updateSize)
        {
            Serial.println("Written : " + String(written) + " successfully");
        }
        else
        {
            Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
        }
        if (Update.end())
        {
            Serial.println("OTA done!");
            if (Update.isFinished())
            {
                Serial.println("Update successfully completed. Rebooting.");
            }
            else
            {
                Serial.println("Update not finished? Something went wrong!");
            }
        }
        else
        {
            Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        }
    }
    else
    {
        Serial.println("Not enough space to begin OTA");
    }
}

// check given FS for valid update.bin and perform update if available
void updateFromFS(fs::FS &fs, std::string path)
{
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    u8g2.drawButtonUTF8(128, 32, U8G2_BTN_HCENTER | U8G2_BTN_BW1, 34, 2, 3, ("writing " + path + " to ROM...").c_str());
    u8g2.sendBuffer();

    File updateBin = fs.open(path.c_str());
    if (updateBin)
    {
        if (updateBin.isDirectory())
        {
            Serial.println(("Error, " + path + " is not a file").c_str());
            updateBin.close();
            return;
        }

        size_t updateSize = updateBin.size();

        if (updateSize > 0)
        {
            Serial.println("Try to start update");
            performUpdate(updateBin, updateSize);
        }
        else
        {
            Serial.println("Error, file is empty");
        }

        updateBin.close();
    }
    else
    {
        Serial.println("Could not load file from sd root");
    }
}

void rebootEspWithReason(std::string reason)
{
    Serial.println(reason.c_str());
    delay(100);
    ESP.restart();
}

void initFromFile()
{
    return;
    if (!SD.begin(4))
    {
        return;
    }

    File file = SD.open("/config.txt");

    if (!file)
    {
        Serial.println("could not open file!");
        return;
    }

    while (file.available())
    {
        Serial.write(file.read());
    }

    file.close();
}

const int ADC_PIN = 36;
const uint8_t ADC_SAMPLE_COUNT = 8;
float filtered_voltage = -1.0;
const float ALPHA = 0.15f;
const float BATTERY_DIVIDER_RATIO = 2.0f;

// Two-point linear calibration:
// raw 2.30V -> true 2.80V
// raw 3.50V -> true 4.20V
// 万用表测出来的，实际上不是线性的，但能用。
const float BATTERY_CALIBRATION_RAW_LOW = 2.30f;
const float BATTERY_CALIBRATION_TRUE_LOW = 2.80f;
const float BATTERY_CALIBRATION_RAW_HIGH = 3.50f;
const float BATTERY_CALIBRATION_TRUE_HIGH = 4.00f;

float calibrateBatteryVoltage(float rawVoltage)
{
    const float rawSpan = BATTERY_CALIBRATION_RAW_HIGH - BATTERY_CALIBRATION_RAW_LOW;
    if (fabsf(rawSpan) < 0.001f)
    {
        return rawVoltage;
    }

    const float scale = (BATTERY_CALIBRATION_TRUE_HIGH - BATTERY_CALIBRATION_TRUE_LOW) / rawSpan;
    const float offset = BATTERY_CALIBRATION_TRUE_LOW - (BATTERY_CALIBRATION_RAW_LOW * scale);
    return (rawVoltage * scale) + offset;
}

void initBatteryAdc()
{
    pinMode(ADC_PIN, INPUT);
    analogReadResolution(12);
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
    analogSetWidth(12);
    resetBatteryVoltageFilter();
}

void resetBatteryVoltageFilter()
{
    filtered_voltage = -1.0f;
}

float getBatteryVoltageRaw(uint8_t sampleCount)
{
    if (sampleCount == 0)
    {
        sampleCount = 1;
    }

    uint16_t minMv = UINT16_MAX;
    uint16_t maxMv = 0;
    uint32_t totalMv = 0;

    for (uint8_t i = 0; i < sampleCount; ++i)
    {
        const uint16_t currentMv = analogReadMilliVolts(ADC_PIN);
        totalMv += currentMv;
        if (currentMv < minMv)
        {
            minMv = currentMv;
        }
        if (currentMv > maxMv)
        {
            maxMv = currentMv;
        }
    }

    if (sampleCount > 2)
    {
        totalMv -= minMv;
        totalMv -= maxMv;
        sampleCount -= 2;
    }

    const float adcVoltage = static_cast<float>(totalMv) / static_cast<float>(sampleCount) / 1000.0f;
    return adcVoltage * BATTERY_DIVIDER_RATIO;
}

float getBatteryVoltagePrecise(uint8_t sampleCount)
{
    return calibrateBatteryVoltage(getBatteryVoltageRaw(sampleCount));
}

float getPrecisedBatteryVoltage()
{
    const float current_voltage = getBatteryVoltagePrecise(ADC_SAMPLE_COUNT);

    if (filtered_voltage < 0.0f)
    {
        filtered_voltage = current_voltage;
    }
    else
    {
        filtered_voltage = (ALPHA * current_voltage) + ((1.0f - ALPHA) * filtered_voltage);
    }

    return filtered_voltage;
}

volatile float global_battery_v = 4.2f;

// 后台电池采样任务
void batteryUpdateTask(void *pvParameters)
{
    for (;;)
    {
        // 在后台可以使用多次采样取平均，因为不会卡主线程
        float sum = 0;
        for (int i = 0; i < 24; i++)
        {
            sum += getPrecisedBatteryVoltage();
            vTaskDelay(pdMS_TO_TICKS(5)); // 使用非阻塞型延时，把 CPU 让给 UI 线程
        }

        global_battery_v = (sum / 24);
    }
}

// 供 UI 线程调用的接口，直接读取全局变量，任何情况下都应该使用这个函数。
float getBatteryVoltage()
{
    return global_battery_v;
}

void printHeapInfo()
{
    multi_heap_info_t heap_info;
    heap_caps_get_info(&heap_info, MALLOC_CAP_8BIT);

    Serial.print("Total heap size: ");
    Serial.println(heap_info.total_free_bytes + heap_info.total_allocated_bytes);

    Serial.print("Free heap size: ");
    Serial.println(heap_info.total_free_bytes);

    Serial.print("Allocated heap size: ");
    Serial.println(heap_info.total_allocated_bytes);

    Serial.print("Minimum free heap size: ");
    Serial.println(heap_info.minimum_free_bytes);
}

// 电压-电量查表，是由我手上设备测量得出的，仅供参考。
// 校准请参考 fuyu-calc-battery-discharge 项目。
const BatteryLUT battery_table[] = {
    {3.972, 100},
    {3.857, 90},
    {3.817, 80},
    {3.760, 70},
    {3.709, 60},
    {3.671, 50},
    {3.653, 40},
    {3.630, 30},
    {3.588, 20},
    {3.527, 10},
    {3.496, 5},
    {3.014, 0}};
const uint8_t table_size = sizeof(battery_table) / sizeof(BatteryLUT);
uint8_t getBatteryLevel(float voltage)
{
    if (voltage >= battery_table[0].voltage)
    {
        return 100;
    }
    if (voltage <= battery_table[table_size - 1].voltage)
    {
        return 0;
    }

    for (uint8_t i = 0; i < table_size - 1; i++)
    {
        if (voltage <= battery_table[i].voltage && voltage > battery_table[i + 1].voltage)
        {

            // 取出两个锚点的电压和百分比数值
            float v_high = battery_table[i].voltage;
            float v_low = battery_table[i + 1].voltage;
            float p_high = battery_table[i].percentage;
            float p_low = battery_table[i + 1].percentage;

            // 线性插值公式计算具体百分比
            float percentage = p_low + ((voltage - v_low) / (v_high - v_low)) * (p_high - p_low);

            return (uint8_t)round(percentage); // 四舍五入取整
        }
    }
    return 0;
}
std::string getBatteryPercentageStr()
{
    const float CHARGE_VOLTAGE = 4.5f;
    static uint8_t last_percentage = 255; 
    
    float voltage = getBatteryVoltage();
    
    if (voltage >= CHARGE_VOLTAGE)
    {
        last_percentage = 255; 
        return "CHG";
    }
    
    uint8_t current_percentage = getBatteryLevel(voltage);
    
    // 首次开机直接赋值
    if (last_percentage == 255)
    {
        last_percentage = current_percentage;
    }
    else
    {
        // 防抖核心逻辑：新旧电量差值必须大于等于 2% 才更新
        // 过滤掉微小波动
        int diff = abs((int)current_percentage - (int)last_percentage);
        if (diff >= 2) 
        {
            last_percentage = current_percentage;
        }
    }
    
    return std::to_string(last_percentage) + "%";
}