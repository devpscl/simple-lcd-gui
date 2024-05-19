#include "lcdcore.h"
#include "lcddialog.h"

using namespace lcdgui;

LiquidCrystalGui::LiquidCrystalGui(lcd_native_type native_type,
                             const uint8_t &columns,
                             const uint8_t &rows,
                             const uint8_t &char_size) {
  native_type_ = native_type;
  display_info_ = new DisplayInfo{columns, rows, char_size};

}
#if defined(LCD_DEFAULT_)
LiquidCrystalGui::LiquidCrystalGui(uint8_t rs,
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
LiquidCrystalGui::LiquidCrystalGui(uint8_t lcd_addr, const uint8_t& columns, const uint8_t& rows,
             const uint8_t& char_size) {
  native_type_ = new LiquidCrystal_I2C(lcd_addr, columns, rows);
  display_info_ = new DisplayInfo{columns, rows, char_size};
}
#endif

LiquidCrystalGui::~LiquidCrystalGui() {
  delete display_info_;
}

void LiquidCrystalGui::begin(bool initialize_lcd) {
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

gui_dialog LiquidCrystalGui::currentDialog() {
  return current_dialog_;
}

void LiquidCrystalGui::openDialog(gui_dialog dialog) {
  if(current_dialog_ != nullptr) {
    current_dialog_->disable(*this);
  }
  current_dialog_ = dialog;
  if(current_dialog_ != nullptr) {
    current_dialog_->setInstance(this);
  }
  current_dialog_->enable(*this);
  updateDisplay();
}

void LiquidCrystalGui::closeDialog() {
  if(current_dialog_ != nullptr) {
    current_dialog_->disable(*this);
  }
  current_dialog_ = nullptr;
  native_type_->clear();
}

void LiquidCrystalGui::disposeDialog() {
  if(current_dialog_ != nullptr) {
    current_dialog_->disable(*this);
  }
  delete current_dialog_;
  current_dialog_ = nullptr;
  native_type_->clear();
}


void LiquidCrystalGui::dispatchInput(const uint8_t &input) {
  if(current_dialog_ != nullptr) {
    current_dialog_->input(*this, input);
  }
}

void LiquidCrystalGui::updateDisplay() {
  if(current_dialog_ != nullptr) {
    current_dialog_->render(*this);
  }
}

lcd_native_type LiquidCrystalGui::nativeLcd() const {
  return native_type_;
}

DisplayInfo LiquidCrystalGui::displayInfo() const {
  return *display_info_;
}

void LiquidCrystalGui::cursor(uint8_t col, uint8_t row) {
  native_type_->setCursor(col, row);
}

void LiquidCrystalGui::print(const String &out) {
  native_type_->print(out);
}

void LiquidCrystalGui::clear() {
  native_type_->clear();
}

uint8_t LiquidCrystalGui::columns() {
  return display_info_->columns;
}

uint8_t LiquidCrystalGui::rows() {
  return display_info_->rows;
}

