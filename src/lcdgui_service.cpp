#include "lcdcore.h"
#include "lcddialog.h"

using namespace lcdgui;

LcdGuiService::LcdGuiService(lcd_native_type native_type,
                             const uint8_t &columns,
                             const uint8_t &rows,
                             const uint8_t &char_size) {
  native_type_ = native_type;
  display_info_ = new DisplayInfo{columns, rows, char_size};

}
#if defined(LCD_DEFAULT_)
LcdGuiService::LcdGuiService(uint8_t rs,
                             uint8_t en,
                             uint8_t d4,
                             uint8_t d5,
                             uint8_t d6,
                             uint8_t d7,
                             const uint8_t &columns,
                             const uint8_t &rows,
                             const uint8_t &char_size) {
  native_type_ = new LiquidCrystal(rs, en, d4, d5, d6, d7);
  display_info_ = new DisplayInfo{columns, rows, char_size};
}
#elif defined(LCD_I2C_)
LcdGuiService::LcdGuiService(uint8_t lcd_addr, const uint8_t& columns, const uint8_t& rows,
             const uint8_t& char_size) {
  native_type_ = new LiquidCrystal_I2C(lcd_addr, columns, rows);
  display_info_ = new DisplayInfo{columns, rows, char_size};
}
#endif

LcdGuiService::~LcdGuiService() {
  delete display_info_;
}

void LcdGuiService::begin(bool initialize_lcd) {
  if(initialize_lcd) {
#if defined(LCD_DEFAULT_)
    native_type_->begin(display_info_->columns,
                        display_info_->rows,
                        display_info_->char_size);
#elif defined(LCD_I2C_)
    native_type_->init();
    native_type_->backlight();
#endif
  }
  uint8_t arrow_up_data[] = LCD_DATA_ARROW_UP;
  uint8_t arrow_down_data[] = LCD_DATA_ARROW_DOWN;
  uint8_t arrow_right_data[] = LCD_DATA_ARROW_RIGHT;
  native_type_->createChar(LCD_CHAR_ARROW_UP, arrow_up_data);
  native_type_->createChar(LCD_CHAR_ARROW_DOWN, arrow_down_data);
  native_type_->createChar(LCD_CHAR_ARROW_RIGHT, arrow_right_data);
}

gui_dialog LcdGuiService::currentDialog() {
  return current_dialog_;
}

void LcdGuiService::openDialog(gui_dialog dialog) {
  current_dialog_ = dialog;
  if(current_dialog_ != nullptr) {
    current_dialog_->setLcdService(this);
  }
  updateDisplay();
}

void LcdGuiService::closeDialog() {
  current_dialog_ = nullptr;
  native_type_->clear();
}

void LcdGuiService::disposeDialog() {
  delete current_dialog_;
  current_dialog_ = nullptr;
  native_type_->clear();
}


void LcdGuiService::dispatchInput(const uint8_t &input) {
  if(current_dialog_ != nullptr) {
    current_dialog_->input(input, *this);
  }
}

void LcdGuiService::updateDisplay() {
  if(current_dialog_ != nullptr) {
    current_dialog_->render(native_type_, *this);
  }
}

lcd_native_type LcdGuiService::nativeLcd() const {
  return native_type_;
}

DisplayInfo LcdGuiService::displayInfo() const {
  return *display_info_;
}

