#include <Arduino.h>
#include <SPI.h>
#include <lcdgui.h>

#define PIN_CS 5

#define PIN_RS 13
#define PIN_EN 12
#define PIN_D4 14
#define PIN_D5 27
#define PIN_D6 26
#define PIN_D7 25

#define LCD_COLS 16
#define LCD_ROWS 2

using namespace lcdgui;

uint8_t last_keypad_input = 0;

LiquidCrystalGui lcd_main(PIN_RS, PIN_EN, PIN_D4, PIN_D5, PIN_D6,
                       PIN_D7, LCD_COLS, LCD_ROWS);

option_list toggle_option_list = OPTION_BUILDER("OFF", "ON");
option_list title_language_option_list = OPTION_BUILDER("EN", "DE");
option_list step_option_list = OPTION_BUILDER("1", "5", "10", "20", "50", "100");

const char* title_en = "This is a lcd";
const char* title_de = "Das ist ein lcd";

uint8_t step_count = 1;
int16_t x = 0;
int16_t y = 0;
int16_t z = 0;

void eventCounterChange(int16_t value, int8_t offset);
void eventChangeDisplayOption(uint8_t option);
void eventChangeTitleLangOption(uint8_t option);
void event3dChangeStepValue(uint8_t option);

void eventOpen3dDialog();
bool eventInput3dDialog(uint8_t input);

void event3dChangeXValue(int16_t value, int8_t offset);
void event3dChangeYValue(int16_t value, int8_t offset);
void event3dChangeZValue(int16_t value, int8_t offset);

void event3dReset();

menu_dialog dialog_main = MENU_BUILDER(
    DUMMY_MENU_ITEM(title_en),
    COUNTER_MENU_ITEM_EXTENDED("Ledcontrol", 0, 0, 1024, OptionItemStyle::Bracketed, eventCounterChange),
    OPTION_MENU_ITEM_EXTENDED("Display", toggle_option_list,
                              OptionItemStyle::Bracketed, eventChangeDisplayOption),
    OPTION_MENU_ITEM_EXTENDED("Title Lang", title_language_option_list,
                              OptionItemStyle::ColonSplit, eventChangeTitleLangOption),
    ACTION_MENU_ITEM("3D", eventOpen3dDialog)
);

menu_dialog dialog_3d = MENU_BUILDER(
    OPTION_MENU_ITEM_VA("Step", step_option_list, OptionItemStyle::ColonSplit, event3dChangeStepValue),
    COUNTER_MENU_ITEM_VA("X", x, 0, 8192, OptionItemStyle::ColonSplit, event3dChangeXValue),
    COUNTER_MENU_ITEM_VA("Y", y, 0, 8192, OptionItemStyle::ColonSplit, event3dChangeYValue),
    COUNTER_MENU_ITEM_VA("Z", z, 0, 8192, OptionItemStyle::ColonSplit, event3dChangeZValue),
    ACTION_MENU_ITEM_VA("Reset", event3dReset)
);

void eventCounterChange(int16_t value, int8_t offset) {
  analogWrite(A5, value); //change led
}

void eventChangeDisplayOption(uint8_t option) {
  if(option == 0) {
    lcd_main.nativeLcd()->display();
  } else {
    lcd_main.nativeLcd()->noDisplay();
  }
}

void eventChangeTitleLangOption(uint8_t option) {
  auto current_dialog = lcd_main.currentDialog<menu_dialog>(); // current menu
  auto title_item = current_dialog->itemAt<dummy_menu_item>(0); // first item
  title_item->text(option == 1 ? title_de : title_en);
  current_dialog->updateDisplay(); // update required for text change
}

void event3dChangeStepValue(uint8_t option) {
  switch (option) {
    case 0:
      step_count = 1;
      break;
    case 1:
      step_count = 5;
      break;
    case 2:
      step_count = 10;
      break;
    case 3:
      step_count = 20;
      break;
    case 4:
      step_count = 50;
      break;
    case 5:
      step_count = 100;
      break;
    default:break;
  }
}

void eventOpen3dDialog() {
  dialog_3d->cursor(0);
  lcd_main.openDialog(dialog_3d);
}

bool eventInput3dDialog(uint8_t input) {
  if(input == LCD_INPUT_BACK) {
    lcd_main.openDialog(dialog_main);
    return EVENT_CANCELLED;
  }
  return EVENT_SEND;
}

void event3dChangeXValue(int16_t value, int8_t offset) {
  int16_t new_value = (value - offset) + step_count * offset;
  auto current_dialog = lcd_main.currentDialog<menu_dialog>();
  auto item = current_dialog->itemAt<counter_menu_item>(1);
  item->value(new_value);
}

void event3dChangeYValue(int16_t value, int8_t offset) {
  int16_t new_value = (value - offset) + step_count * offset;
  auto current_dialog = lcd_main.currentDialog<menu_dialog>();
  auto item = current_dialog->itemAt<counter_menu_item>(2);
  item->value(new_value);
}

void event3dChangeZValue(int16_t value, int8_t offset) {
  int16_t new_value = (value - offset) + step_count * offset;
  auto current_dialog = lcd_main.currentDialog<menu_dialog>();
  auto item = current_dialog->itemAt<counter_menu_item>(3);
  item->value(new_value);
}

void event3dReset() {
  result_dialog dialog = RESULT_DIALOG("Are you sure?",RESULT_OPTION_YES | RESULT_OPTION_NO,
                      [](uint8_t option){
                        if(option == RESULT_OPTION_YES) {
                          auto x_item = GET_ITEM(dialog_3d, 1, counter_menu_item);
                          auto y_item = GET_ITEM(dialog_3d, 2, counter_menu_item);
                          auto z_item = GET_ITEM(dialog_3d, 3, counter_menu_item);
                          x_item->value(0);
                          y_item->value(0);
                          z_item->value(0);
                        }
                        lcd_main.openDialog(dialog_3d);
                        return DIALOG_DISPOSE;
  });
  lcd_main.openDialog(dialog);
}

void initKeypad();

void setup() {
  initKeypad();
  lcd_main.begin(true);
  lcd_main.openDialog(dialog_main);

  dialog_3d->inputEvent(eventInput3dDialog);
}

////////////////////////////////////
// End of important example code
////////////////////////////////////

void initKeypad() {
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  digitalWrite(PIN_CS, HIGH);
  pinMode(PIN_CS, OUTPUT);
}

uint8_t readKeypad() {
  digitalWrite(PIN_CS, LOW);
  delayMicroseconds(25);
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(PIN_CS, HIGH);
  return value;
}

void loop() {
  delay(100);
  uint8_t input = readKeypad();
  if(last_keypad_input == input) {
    return;
  }
  last_keypad_input = input;
  switch (input) {
    case 0x12: //Button up
      lcd_main.dispatchInput(LCD_INPUT_UP);
      break;
    case 0x21: //Button left
      lcd_main.dispatchInput(LCD_INPUT_LEFT);
      break;
    case 0x22: //Button next / ok
      lcd_main.dispatchInput(LCD_INPUT_NEXT);
      break;
    case 0x24: //Button right
      lcd_main.dispatchInput(LCD_INPUT_RIGHT);
      break;
    case 0x42: //Button down
      lcd_main.dispatchInput(LCD_INPUT_DOWN);
      break;
    case 0x81: //Button back / cancel
      lcd_main.dispatchInput(LCD_INPUT_BACK);
      break;
    default:return;
  }

}