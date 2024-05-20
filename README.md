# simple-lcd-gui
Simple-Lcd-Gui is a arduino library for creating user interfaces like menus on the lcd.

#### Features:
- Navigable menu system
- Different types of menu items such as counter, option selection and dummy
- Precast dialogs such as request dialog to choose between yes, no and ok
- Creation of own dialogs
- u8glib2 support
- i2c lcd support

## Fully Example
```c++
#include <Arduino.h>
#include <lcdgui.h>

using namespace lcdgui;

#define PIN_RS 13
#define PIN_EN 12
#define PIN_D4 14
#define PIN_D5 27
#define PIN_D6 26
#define PIN_D7 25

#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystalGui lcd_main(PIN_RS, PIN_EN, PIN_D4, PIN_D5, PIN_D6,
                          PIN_D7, LCD_COLS, LCD_ROWS);

option_list toggle_option_list = OPTION_BUILDER("ON", "OFF");

void eventItemAction();
void eventChangeDisplayOption(uint8_t option);

menu_dialog menu_root = MENU_BUILDER(
    DUMMY_MENU_ITEM_VA("I am a lcd"),
    ACTION_MENU_ITEM("Press me", eventItemAction),
    COUNTER_MENU_ITEM_VA("Counter", 10, -20, 20),
    OPTION_MENU_ITEM_VA("Display", toggle_option_list, OptionItemStyle::Bracketed, eventChangeDisplayOption)
);

void eventItemAction() {
  //do something
}

void eventChangeDisplayOption(uint8_t option) {
  if(option == 0) { // "on" selected
    lcd_main.nativeLcd()->display();
    return;
  }
  auto dialog = RESULT_DIALOG("Are you sure?", RESULT_OPTION_YES | RESULT_OPTION_NO, [](uint8_t o){
    if(o == RESULT_OPTION_YES) {
      lcd_main.nativeLcd()->noDisplay();
    }
    return DIALOG_DISPOSE; //delete dialog to free memory
  });
  menu_root->openChild(dialog); //Sets the refuge of dialog to the menu_root dialog
}

void setup() {
  lcd_main.begin();
  lcd_main.openDialog(menu_root);
}

void loop() {
  /*  Control the menu:
      LCD_INPUT_LEFT
      LCD_INPUT_RIGHT
      LCD_INPUT_UP,
      LCD_INPUT_DOWN,
      LCD_INPUT_BACK,
      LCD_INPUT_NEXT
   */
  lcd_main.dispatchInput(...);
}
```
More examples can you find <b>[here](https://github.com/APICodeYT/simple-lcd-gui/tree/master/examples)</b>
