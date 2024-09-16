#include "ChineseText.h"

ChineseText::ChineseText(int restX, int restY, int activeX, int activeY, int width, int height, bool hasBox, std::string str) : UIElement(restX, restY, width, height)
{
    this->hasBox = hasBox;
    this->str = std::move(str);
    this->restX = restX;
    this->restY = restY;
    this->activeX = activeX;
    this->activeY = activeY;
}
// Helper function to split UTF-8 string into lines
std::vector<std::string> splitTextIntoLines(const std::string &text, int charsPerLine)
{
    std::vector<std::string> lines;
    std::string currentLine;
    int charCount = 0;

    for (size_t i = 0; i < text.size();)
    {
        unsigned char c = text[i];
        int charLen = 1;

        if (c >= 0x80)
        {
            // UTF-8 multi-byte character
            if ((c & 0xE0) == 0xC0)
                charLen = 2; // 110xxxxx
            else if ((c & 0xF0) == 0xE0)
                charLen = 3; // 1110xxxx
            else if ((c & 0xF8) == 0xF0)
                charLen = 4; // 11110xxx
        }

        if (charCount + 1 > charsPerLine)
        {
            lines.push_back(currentLine);
            currentLine.clear();
            charCount = 0;
        }

        currentLine.append(text, i, charLen);
        i += charLen;
        charCount++;
    }

    if (!currentLine.empty())
    {
        lines.push_back(currentLine);
    }

    return lines;
}
void ChineseText::init()
{
    currentPage = 0;
}

void ChineseText::activate()
{
    currentElement = this;
    aniIn();
}

void ChineseText::draw()
{
    if (this->hasBox)
    {
        u8g2.drawRFrame(x, y, width, height, 1);
    }

    u8g2.setFont(u8g2_font_wqy12_t_gb2312);

    // Calculate characters per line
    int charWidth = u8g2.getMaxCharWidth();
    int charsPerLine = (width - 4) / charWidth; // 2 pixel padding on each side

    // Split text into lines
    auto lines = splitTextIntoLines(str, charsPerLine);

    // Calculate lines per page
    int lineHeight = u8g2.getMaxCharHeight();
    int linesPerPage = (height - 4) / lineHeight; // 2 pixel padding on top and bottom

    // Draw current page
    int startLine = currentPage * linesPerPage;
    int endLine = std::min(startLine + linesPerPage, static_cast<int>(lines.size()));

    for (int i = startLine; i < endLine; ++i)
    {
        u8g2.setCursor(x + 2, y + 15 + (i - startLine) * lineHeight);
        u8g2.print(lines[i].c_str());
    }

    u8g2.setFont(u8g2_font_profont10_mf);
    // u8g2.drawStr(this -> x, this -> y, this -> text.substr(0, min(int(this -> text.size()), this -> width / 5)).c_str());
}

void ChineseText::update()
{

    draw();
    if (kb.getRisingEdgeKey() == std::make_pair(1, 1))
    {
        prevPage();
    }
    if (kb.getRisingEdgeKey() == std::make_pair(3, 1))
    {
        nextPage();
    }
    if (kb.getRisingEdgeKey() == std::make_pair(0, 0))
    {
        aniOut();
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
}

void ChineseText::nextPage()
{
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    int charWidth = u8g2.getMaxCharWidth();
    int charsPerLine = (width - 4) / charWidth;

    // Split text into lines
    auto lines = splitTextIntoLines(str, charsPerLine);

    // Calculate lines per page
    int lineHeight = u8g2.getMaxCharHeight();
    int linesPerPage = (height - 4) / lineHeight;

    if ((currentPage + 1) * linesPerPage < lines.size())
    {
        ++currentPage;
    }
}

void ChineseText::prevPage()
{
    if (currentPage > 0)
    {
        --currentPage;
    }
}

void ChineseText::aniIn()
{
    insertAnimation(new Animation(this, INDENT, this->activeX, this->activeY, 500));
}

void ChineseText::aniOut()
{
    insertAnimation(new Animation(this, INDENT, this->restX, this->restY, 500));
    insertTmpAnimationPointer(this);
}