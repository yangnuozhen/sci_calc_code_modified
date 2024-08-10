#include "UIMain.h"

std::string clipboard = "";

Keyboard kb;
bool angleMode = 0; // 0: radians, 1: degree
bool RPNMode = 0;
ESP32Time rtc;


Macro layout1[5][6] = {
    {Macro({KEY_ESC}),          Macro({'/'}), Macro({'*'}), Macro({'-'}),           Macro({KEY_LEFT_ALT, KEY_LEFT_GUI, 'c'}, "C_PATH"), Macro({KEY_LEFT_GUI, 'r'}, "CMD+R")},
    {Macro({'7'}),              Macro({'8'}), Macro({'9'}), Macro({'+'}),           Macro({KEY_LEFT_GUI, 'c'}, "COPY"),  Macro({KEY_LEFT_GUI, 'v'}, "PASTE")},
    {Macro({'4'}),              Macro({'5'}), Macro({'6'}), Macro({'^'}),           Macro({KEY_LEFT_GUI, 'x'}, "CUT"),   Macro({KEY_LEFT_GUI, 's'}, "SAVE")},
    {Macro({'1'}),              Macro({'2'}), Macro({'3'}), Macro({KEY_RETURN}),    Macro({KEY_LEFT_GUI, 'h'}, "HIDE"),  Macro({KEY_LEFT_GUI, 'n'}, "NEW")},
    {Macro({KEY_LAYER_SWITCH}), Macro({'0'}), Macro({'.'}), Macro({KEY_BACKSPACE}), Macro({KEY_F12}, "G_DEC"),                        Macro()}
};

Macro layout2[5][6] = {
    {Macro({KEY_ESC}),          Macro({'/'}),            Macro({'*'}),             Macro({'-'}),           Macro({KEY_LEFT_GUI, 'a'}, "CMD+A"), Macro({KEY_LEFT_GUI, 'r'}, "CMD+R")},
    {Macro({'7'}),              Macro({KEY_UP_ARROW}),   Macro({'9'}),             Macro({'+'}),           Macro({KEY_LEFT_GUI, 'c'}, "COPY"),  Macro({KEY_LEFT_GUI, 'v'}, "PASTE")},
    {Macro({KEY_LEFT_ARROW}),   Macro({'5'}),            Macro({KEY_RIGHT_ARROW}), Macro({'^'}),           Macro({KEY_LEFT_GUI, 'x'}, "CUT"),   Macro({KEY_LEFT_GUI, 's'}, "SAVE")},
    {Macro({'1'}),              Macro({KEY_DOWN_ARROW}), Macro({'3'}),             Macro({KEY_RETURN}),    Macro({KEY_LEFT_GUI, 'h'}, "HIDE"),  Macro({KEY_LEFT_GUI, 'n'}, "NEW")},
    {Macro({KEY_LAYER_SWITCH}), Macro({'0'}),            Macro({'.'}),             Macro({KEY_BACKSPACE}), Macro(),                        Macro()}
};

Macro photoshopLayer[5][6] = {
    {Macro({KEY_ESC}),          Macro({'/'}), Macro({'*'}), Macro({'-'}),           Macro({KEY_LEFT_GUI, 'a'}, "CMD+A"), Macro({KEY_LEFT_GUI, 'z'}, "UNDO")},
    {Macro({'7'}),              Macro({'8'}), Macro({'9'}), Macro({'+'}),           Macro({KEY_LEFT_GUI, 'c'}, "COPY"),  Macro({KEY_LEFT_GUI, 'v'}, "PASTE")},
    {Macro({KEY_LEFT_ARROW}),   Macro({'5'}),            Macro({KEY_RIGHT_ARROW}), Macro({'^'}),           Macro({KEY_LEFT_GUI, 'x'}, "CUT"),   Macro({KEY_LEFT_GUI, 's'}, "SAVE")},
    {Macro({'1'}),              Macro({KEY_DOWN_ARROW}), Macro({'3'}),             Macro({KEY_RETURN}),    Macro({KEY_LEFT_GUI, 't'}, "F_TRAN"),  Macro({KEY_LEFT_GUI, 'd'}, "DSEL")},
    {Macro({KEY_LAYER_SWITCH}), Macro({'0'}),            Macro({'.'}),             Macro({KEY_BACKSPACE}), Macro({'['}, "DEC_B"),                        Macro({']'}, "INC_B")}
};
Macro filmoraLayer[5][6] = {
    {Macro({KEY_ESC}),          Macro({'/'}), Macro({'*'}), Macro({'-'}),          Macro({KEY_LEFT_GUI, 'a'}, "CMD+A"), Macro({KEY_LEFT_GUI, 'z'}, "UNDO")},
    {Macro({'7'}),              Macro({'8'}), Macro({'9'}), Macro({'+'}),           Macro({KEY_LEFT_GUI, 'c'}, "COPY"),  Macro({KEY_LEFT_GUI, 'v'}, "PASTE")},
    {Macro({KEY_LEFT_ARROW}),   Macro({'5'}),            Macro({KEY_RIGHT_ARROW}), Macro({'^'}),           Macro({KEY_LEFT_GUI, 'x'}, "CUT"),   Macro({KEY_LEFT_GUI, 's'}, "SAVE")},
    {Macro({'1'}),              Macro({KEY_DOWN_ARROW}), Macro({'3'}),             Macro({KEY_RETURN}),    Macro({KEY_LEFT_GUI, 'b'}, "SPLIT"),  Macro({KEY_LEFT_GUI, 'i'}, "IMPORT")},
    {Macro({KEY_LAYER_SWITCH}), Macro({'0'}),            Macro({'.'}),             Macro({KEY_BACKSPACE}), Macro({KEY_LEFT_GUI, 'r'}, "SPEED"),                        Macro({'x'}, "SEL")}
};

Macro obsidianLayer[5][6] = {
    {Macro({KEY_ESC}),          Macro({'/'}), Macro({'*'}), Macro({'-'}),           Macro({KEY_LEFT_GUI, 'i'}, "ITAL"), Macro({KEY_LEFT_ALT, KEY_LEFT_GUI, 'l'}, "LINK")},
    {Macro({'7'}),              Macro({'8'}), Macro({'9'}), Macro({'+'}),           Macro({KEY_LEFT_GUI, 'b'}, "BOLD"),  Macro({KEY_LEFT_GUI, 'h'}, "HIGHL")},
    {Macro({'4'}),              Macro({'5'}), Macro({'6'}), Macro({'^'}),           Macro({'-', ' ', '[', ' ', ']', ' '}, "CHECK"),   Macro({KEY_LEFT_GUI, 's'}, "SAVE")},
    {Macro({'1'}),              Macro({'2'}), Macro({'3'}), Macro({KEY_RETURN}),    Macro({KEY_LEFT_ALT, KEY_LEFT_GUI, '1'}, "H1"),  Macro({KEY_LEFT_ALT, KEY_LEFT_GUI, '2'}, "H2")},
    {Macro({KEY_LAYER_SWITCH}), Macro({'0'}), Macro({'.'}), Macro({KEY_BACKSPACE}), Macro({KEY_LEFT_ALT, KEY_LEFT_GUI, '3'}, "H3"),                        Macro({KEY_LEFT_ALT, KEY_LEFT_GUI, '4'}, "H4")}
};


//Macro("Rewrite this paragraph so that it is more concise, academic, and professional:", "ACAD", true)
MacroPad macroPad({ 
    Layout("standard", layout1),
    Layout("layout 2", layout2),
    Layout("photoshop", photoshopLayer),
    Layout("editing", filmoraLayer),
    Layout("obsidian", obsidianLayer),
    //Layout("LLM", LLM)
});

MacropadUI macropadUI(&macroPad);



 
Macro layoutCalc1[5][6] = {
    {Macro({KEY_ESC}),          Macro({'/'}), Macro({'*'}), Macro({'-'}),           Macro({KEY_SIN}), Macro({KEY_SEC})},
    {Macro({'7'}),              Macro({'8'}), Macro({'9'}), Macro({'+'}),           Macro({KEY_COS}), Macro({KEY_CSC})},
    {Macro({'4'}),              Macro({'5'}), Macro({'6'}), Macro({'^'}),           Macro({KEY_TAN}), Macro({KEY_COT})},
    {Macro({'1'}),              Macro({'2'}), Macro({'3'}), Macro({KEY_RETURN}),    Macro({KEY_EXP}), Macro({KEY_RPN_SWITCH})},
    {Macro({KEY_LAYER_SWITCH}), Macro({'0'}), Macro({'.'}), Macro({KEY_BACKSPACE}), Macro({KEY_MODE_SWITCH}),     Macro({KEY_TAB})}
};

Macro layoutCalc2[5][6] = { 
    {Macro({KEY_ESC}),          Macro({'('}),            Macro({')'}),             Macro({'-'}),           Macro({KEY_ASIN}), Macro({KEY_SEC})},
    {Macro({'7'}),              Macro({KEY_UP_ARROW}),   Macro({'9'}),             Macro({'+'}),           Macro({KEY_ACOS}), Macro({KEY_CSC})},
    {Macro({KEY_LEFT_ARROW}),   Macro({'5'}),            Macro({KEY_RIGHT_ARROW}), Macro({KEY_SQRT}),      Macro({KEY_ATAN}), Macro({KEY_COT})},
    {Macro({'1'}),              Macro({KEY_DOWN_ARROW}), Macro({'3'}),             Macro({KEY_RETURN}),    Macro({KEY_LN}),   Macro({KEY_RPN_SWITCH})},
    {Macro({KEY_LAYER_SWITCH}), Macro({'0'}),            Macro({'.'}),             Macro({KEY_BACKSPACE}), Macro({KEY_MODE_SWITCH}),      Macro({KEY_TAB})}
};

InputBox inputBox(0, 0, 100, 12, 15);
Menu calcMenu(0, -70, 0, 0, 210, 48, 3);
InputBox expressionInput(0, 64, 210, 12, 42);
Checkbox checkbox1("Soild fill cursor", &cursorMode);
//Slider slider1("Testval", 0, 100);
Funstuff funstuff;


MacroPad calcLayout({
    Layout("1", layoutCalc1),
    Layout("2", layoutCalc2)
});

Menu menuSpecs(0, -70, 0, 0, 210, 64, 5, {
    new Text("Powered by an ESP32 WROOM-32E"), 
    new Text("Kailh choc switches"), 
    new Text("Designed by SHAO")
},
{
    nullptr,
    nullptr,
    nullptr
});


Menu menuSettings(0, -70, 0, 0, 210, 64, 5, {
    &checkbox1,
    new Text("Reload Bin"),
    new Text("Draw Chinese with Type 1"),
    new Text("Draw Chinese with Type 2"),
    new Text("Draw Chinese with Type 3"),
    new Text(("Version: " + VERSION).c_str()),
    new Text("Developed by SHAp256"),
    new Text("Modified & Installed by Aunt_nuozhen"),
    new Text("Owned by Aunt_nuozhen")
},
{
    &checkbox1,
    new BinLink("/main.bin"),
    new ChineseText(1),
    new ChineseText(2),
    new ChineseText(3),
    nullptr,
    nullptr,
    nullptr,
    nullptr
});



Calculator calcMain(0, 0, 210, 64, &calcMenu, &expressionInput);
CalculatorRPN calcRPN(0, 0, 210, 64);

Menu programMenu(0, -70, 0, 0, 210, 64, 4, {
    new Text("Chess"),
    new Text("Spacetrash"),
    new Text("Snake"),
    new Text("Tetris"),
    new Text("Catacombs of the damned"),
    new Text("Squario")
},
{
    new BinLink("/chess.bin"),
    new BinLink("/spacetrash.bin"),
    new BinLink("/snake.bin"),
    new BinLink("/tetris.bin"),
    new BinLink("/COTD.bin"),
    new BinLink("/squario.bin")
    
});

Menu stopwatchMenu(0, -70, 0, 0, 110, 64, 4);

StopwatchUI stopwatchUI(0, 0, 210, 64, &stopwatchMenu);


Menu mainMenu(-100, 0, 0, 0, 70, 64, 4, {
    new Text("Calculator"),
    new Text("RPN Calc"),
    new Text("Macropad"), 
    new Text("Programs"), 
    new Text("Stopwatch"),
    new Text("Settings")
},
{
    &calcMain, 
    &calcRPN,
    &macropadUI, 
    &programMenu, 
    &stopwatchUI,
    &menuSettings
});


UIElement* currentElement = &mainMenu;

void displayTitle() {
    if (currentElement == &mainMenu) {
        u8g2.drawStr(87, 15, ("Running Time: " + rtc.getTime("%T")).c_str());
        u8g2.setFont(u8g2_font_inb19_mf);
        u8g2.drawStr(78, 35, "SCI-CLAC");
        u8g2.setFont(u8g2_font_profont10_mf);
        u8g2.drawStr(90, 55, "Owned by Aunt_nuozhen");
        u8g2.setFont(u8g2_font_profont10_mf);
    }
    //struct tm timeinfo = rtc.getTimeStruct();
}