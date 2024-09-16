#include "StatisticsUI.h"

StatisticsUI::StatisticsUI(int x, int y, int width, int height, InputBox *dataInputBox, Menu *dataMenu) : UIElement(x, y, width, height)
{
    this->dataInputBox = dataInputBox;
    this->dataMenu = dataMenu;
    this->resultMenu = nullptr;
    this->viewState = 0;
    this->mode = 0;
}

void StatisticsUI::init()
{
    this->dataInputBox->init();
    this->dataMenu->init();
}

void StatisticsUI::activate()
{
    this->dataMenu->activate();
    currentElement = this;
    bool viewState = 0;
    insertAnimation(new Animation(this->dataInputBox, INDENT, 0, 57, 500));
}

void StatisticsUI::enter()
{

    if (!isValidNumber(this->dataInputBox->getStr()))
    {
        state = true;
        curtimesyntax = millis();
        Serial.println("Not a long double value.");
        if (state)
        {
            if (millis() - curtimesyntax > 200)
            {
                state = false;
            }
            else
            {
                draw();
                u8g2.drawRFrame(70, 17, 82, 15, 2);
                const char *str = "LONG DOUBLE ONLY";
                u8g2.drawStr(70 + 82 / 2 - u8g2.getStrWidth(str) / 2, 17 + 12 / 2 + (u8g2.getAscent() - u8g2.getDescent()) / 2, str);
                u8g2.sendBuffer();
                delay(400);
                return;
            }
        }
    }

    char *end;
    long double value = std::strtold(this->dataInputBox->getStr().c_str(), &end);
    if (*end != '\0')
    {
        state = true;
        curtimesyntax = millis();
        Serial.println("Convert Failed.");
        if (state)
        {
            if (millis() - curtimesyntax > 200)
            {
                state = false;
            }
            else
            {
                draw();
                u8g2.drawRFrame(70, 17, 82, 15, 2);
                const char *str = "LONG DOUBLE ONLY";
                u8g2.drawStr(70 + 82 / 2 - u8g2.getStrWidth(str) / 2, 17 + 12 / 2 + (u8g2.getAscent() - u8g2.getDescent()) / 2, str);
                u8g2.sendBuffer();
                delay(400);
                return;
            }
        }
    }

    this->statistics.addData(value);
    StatisticsData *statisticsData = new StatisticsData(0, 0, this->dataMenu->getWidth() - 15, 12, this->statistics.getLastIndex() + 1, value);
    this->dataMenu->insertElement(statisticsData, statisticsData);
    this->dataInputBox->clearStr();
    this->dataMenu->scrollDown();
}

void StatisticsUI::draw()
{
    u8g2.drawStr(215, 24, ("Cur:" + calcLayout.getLayout().getName()).c_str());
    heap_caps_get_info(&heap_info, MALLOC_CAP_8BIT);
    u8g2.drawStr(x + this->dataMenu->getWidth() + 10, 40, ("Free Heap:" + numToStr(heap_info.total_free_bytes)).c_str());
    if (heap_info.total_free_bytes < 16000)
    {
        u8g2.drawStr(x + this->dataMenu->getWidth() + 10, 20, "!!MEMORY WARNING!!");
    }
    if (this->mode == 0)
    {
        this->dataMenu->draw();
        this->dataInputBox->update();
    }
    if (this->mode == 1)
    {
        this->dataMenu->update();
        this->dataInputBox->draw();
    }
}

void StatisticsUI::update()
{

    if (kb.getRisingEdgeKey() == std::make_pair(0, 0))
    {
        this->dataMenu->aniOut();
        insertAnimation(new Animation(this->dataInputBox, INDENTINV, 0, 64, 500));
        insertTmpAnimationPointer(this->dataInputBox);
        while (!tmpAnimationUI.empty())
        {
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 0, 210, 64);
            u8g2.setDrawColor(1);
            currentElement->draw();
            updateTmp();
            u8g2.sendBuffer();
            animateAll();
        }

        goBack();
    }
    else if (kb.getRisingEdgeKey() == std::make_pair(3, 3) && this->viewState == 0 && this->mode == 0 && this->dataInputBox->getStr() != "")
    {
        enter();
    }
    else if (kb.getKey(3, 3).getIsPressed() && kb.getKey(4, 0).getIsPressed() && this->viewState == 0 && this->dataMenu->getSize() != 0)
    {
        bool viewState = 1;
        calcLayout.setLayout(0);
        calculateAll();
    }
    else if (kb.getRisingEdgeKey() == std::make_pair(4, 5) && this->viewState == 0)
    {
        if (this->dataMenu->getSize() != 0)
            this->mode = !(this->mode);
    }
    draw();
}

bool StatisticsUI::isValidNumber(const std::string &str)
{
    // 正则表达式用于匹配有效的浮点数
    std::regex pattern(R"(^[-+]?\d*\.?\d+([eE][-+]?\d+)?$)");
    return std::regex_match(str, pattern);
}

void StatisticsUI::calculateAll()
{
    delete this->resultMenu;
    this->resultMenu = new Menu(this->x, this->y - 70, this->x, this->y, this->width, this->height, 5);
    this->resultMenu->init();
    this->resultMenu->setParent(this);
    long double sum = this->statistics.getSum();
    std::string sumDescribe = "求和 (Sum) = " + numToStr(sum) + "\n - 将所有数据相加得出的值。";
    this->resultMenu->insertElement(new Text("Sum = " + numToStr(sum)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, sumDescribe));

    long double mean = this->statistics.getMean();
    std::string meanDescribe = "平均值 (Mean/Average) = " + numToStr(mean) + "\n - 数据的平均值。";
    this->resultMenu->insertElement(new Text("Mean = " + numToStr(mean)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, meanDescribe));

    long double median = this->statistics.getMedian();
    std::string medianDescribe = "中位数 (Median) = " + numToStr(median) + "\n - 数据的中位数，表示一组数据中间的那个数。具体来说，中位数将数据分成两部分，其中一半的数据值小于或等于中位数，另一半的数据值大于或等于中位数。如果数据集中有奇数个数值，中位数就是中间的那个数；如果有偶数个数值，中位数则是中间两个数的平均值。\n 中位数可以有效地反映数据集的中心趋势，特别是在数据存在极端值时。";
    this->resultMenu->insertElement(new Text("Median = " + numToStr(median)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, medianDescribe));

    long double mode = this->statistics.getMode();
    std::string modeDescribe = "众数 (Mode) = " + numToStr(mode) + "\n - 数据中出现频率最高的数值。\n 如果存在多个众数，返回哪个看运气。";
    this->resultMenu->insertElement(new Text("Mode = " + numToStr(mode)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, modeDescribe));

    long double range = this->statistics.getRange();
    std::string rangeDescribe = "极差 (Range) = " + numToStr(range) + "\n - 将数据的最大值与最小值之差。";
    this->resultMenu->insertElement(new Text("Range = " + numToStr(range)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, rangeDescribe));

    long double variance = this->statistics.getVariance();
    std::string varianceDescribe = "方差 (Variance) = " + numToStr(variance) + "\n - 数据集中每个数据点与均值之间差值的平方的期望值。\n 用于量化数据集的离散程度或变异程度。它反映了数据点围绕其均值的分散程度。";
    this->resultMenu->insertElement(new Text("Variance = " + numToStr(variance)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, varianceDescribe));

    long double standardDeviation = this->statistics.getStandardDeviation();
    std::string SDDescribe = "标准差 (Standard Deviation) = " + numToStr(standardDeviation) + "\n - 方差的算术平方根。\n 用于衡量数据集的离散程度或变异程度。它是方差的平方根，提供了数据点与均值之间的平均距离的度量。标准差的单位与原数据相同，因此比方差更具直观性和解释性。";
    this->resultMenu->insertElement(new Text("StdDev = " + numToStr(standardDeviation)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, SDDescribe));

    long double q1 = this->statistics.getQ1();
    std::string q1Describe = "第一四分位数 (First Quartile) = " + numToStr(q1) + "\n - 通常记作Q1，是统计学中的一个重要分位数，用于描述数据集的分布情况。\n 它表示在有序数据集中有75%的数据点小于或等于这个值，25%的数据点大于或等于这个值。";
    this->resultMenu->insertElement(new Text("Q1 = " + numToStr(q1)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, q1Describe));

    long double q2 = this->statistics.getQ2();
    std::string q2Describe = "第二四分位数 (Second Quartile) = " + numToStr(q2) + "\n - 通常记作Q2，等价于中位数，用于描述数据集的分布情况。\n 它表示在有序数据集中有50%的数据点小于或等于这个值，另外50%的数据点大于或等于这个值。";
    this->resultMenu->insertElement(new Text("Q2 = " + numToStr(q2)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, q2Describe));

    long double q3 = this->statistics.getQ3();
    std::string q3Describe = "第三四分位数 (Third Quartile) = " + numToStr(q3) + "\n - 通常记作Q3，是统计学中的一个重要分位数，用于描述数据集的分布情况。\n 它表示在有序数据集中有25%的数据点小于或等于这个值，75%的数据点大于或等于这个值。";
    this->resultMenu->insertElement(new Text("Q3 = " + numToStr(q3)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, q3Describe));

    long double skewness = this->statistics.getSkewness();
    std::string skewnessDescribe = "偏度 (Skewness) = " + numToStr(skewness) + "\n - 偏度是用于衡量数据分布对称性的统计量。\n - 偏度为正值表示数据分布右偏，即数据右侧尾部较长；\n - 偏度为负值表示数据分布左偏，即数据左侧尾部较长；\n - 偏度为零表示数据分布对称。偏度可以帮助我们了解数据的分布形态及其是否存在偏斜。";
    this->resultMenu->insertElement(new Text("Skewness = " + numToStr(skewness)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, skewnessDescribe));

    long double kurtosis = this->statistics.getKurtosis();
    std::string kurtosisDescribe = "过剩峰度 (Excess Kurtosis) = " + numToStr(kurtosis) + "\n - 过剩峰度是衡量数据分布峰值尖锐程度的统计量。\n - 过剩峰度为正值表示数据分布比正态分布具有更尖锐的峰值；\n - 过剩峰度为负值表示数据分布比正态分布具有更平坦的峰值；\n - 过剩峰度为零表示数据分布的峰度与正态分布相同。过剩峰度可以帮助我们了解数据分布的尖峰平坦程度及其尾部特征。";
    this->resultMenu->insertElement(new Text("EKurtosis = " + numToStr(kurtosis)), new ChineseText(0, 70, this->x, this->y, this->width, this->height, true, kurtosisDescribe));

    this->dataMenu->aniOut();
    insertAnimation(new Animation(this->dataInputBox, INDENTINV, 0, 64, 500));
    insertTmpAnimationPointer(this->dataInputBox);
    this->resultMenu->setParent(this);
    this->resultMenu->activate();
}
