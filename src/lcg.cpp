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
#elif defined(LCD_U8G2_)

LiquidCrystalGui::LiquidCrystalGui(U8G2 &u8g2) {
  native_type_ = new LiquidCrystalU8GLIB2(u8g2, CHAR_SIZE_6x11);
  display_info_ = new DisplayInfo{native_type_->cols(), native_type_->rows(), 0};
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
#elif defined(LCD_U8G2_)
    native_type_->begin();
#endif
  }
#if defined(LCD_U8G2_)
  auto* arrow_up_data = new uint8_t[11]LCD_DATA_ARROW_UP_6x11;
  auto* arrow_down_data = new uint8_t[11]LCD_DATA_ARROW_DOWN_6x11;
  auto* arrow_right_data = new uint8_t[11]LCD_DATA_ARROW_RIGHT_6x11;
  auto* map = new CharMap(3);
  map->put(LCD_CHAR_ARROW_UP, arrow_up_data);
  map->put(LCD_CHAR_ARROW_DOWN, arrow_down_data);
  map->put(LCD_CHAR_ARROW_RIGHT, arrow_right_data);
  native_type_->setCharMap(map);
#else
  uint8_t arrow_up_data[] = LCD_DATA_ARROW_UP_5x8;
  uint8_t arrow_down_data[] = LCD_DATA_ARROW_DOWN_5x8;
  uint8_t arrow_right_data[] = LCD_DATA_ARROW_RIGHT_5x8;
  native_type_->createChar(LCD_CHAR_ARROW_UP, arrow_up_data);
  native_type_->createChar(LCD_CHAR_ARROW_DOWN, arrow_down_data);
  native_type_->createChar(LCD_CHAR_ARROW_RIGHT, arrow_right_data);

#endif
}

gui_dialog LiquidCrystalGui::currentDialog() const {
  return current_dialog_;
}

void LiquidCrystalGui::openDialog(gui_dialog dialog) {
  if(current_dialog_ != nullptr) {
    currentDialog()->dispatchDialogEvent(DIALOG_EVENT_CLOSE);
    current_dialog_->disable(*this);
  }
  current_dialog_ = dialog;
  if(current_dialog_ != nullptr) {
    current_dialog_->setInstance(this);
  }
  current_dialog_->enable(*this);
  currentDialog()->dispatchDialogEvent(DIALOG_EVENT_OPEN);
  updateDisplay();
}

void LiquidCrystalGui::closeDialog() {
  if(current_dialog_ != nullptr) {
    currentDialog()->dispatchDialogEvent(DIALOG_EVENT_CLOSE);
    current_dialog_->disable(*this);
  }
  current_dialog_ = nullptr;
  native_type_->clear();
}

void LiquidCrystalGui::disposeDialog() {
  if(current_dialog_ != nullptr) {
    currentDialog()->dispatchDialogEvent(DIALOG_EVENT_CLOSE);
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
#ifdef LCD_U8G2_
  native_type_->autoFlush(true);
#endif
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

void LiquidCrystalGui::print(const char *cstr) {
  native_type_->print(cstr);
}

void LiquidCrystalGui::print(const LcdBuffer &buffer) {
  native_type_->print(buffer.str());
}

void LiquidCrystalGui::clearRow(const uint8_t &row) {
  LcdBuffer buf;
  buf.space(display_info_->columns);
  native_type_->setCursor(0, row);
  native_type_->print(buf.str());
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

void LiquidCrystalGui::showCursor(bool state) {
#if defined(LCD_U8G2_)
  native_type_->cursorVisible(state);
#else
  if(state) {
    native_type_->cursor();
  } else {
    native_type_->noCursor();
  }
#endif

}

void LiquidCrystalGui::flags(const uint8_t &flags) {
#if defined(LCD_U8G2_)
  native_type_->cursorVisible(flags & FLAG_CURSOR_VISIBLE);
#else
  if(flags & FLAG_CURSOR_VISIBLE) {
    native_type_->cursor();
  } else {
    native_type_->noCursor();
  }
  if(flags & FLAG_CURSOR_BLINKING) {
    native_type_->blink();
  } else {
    native_type_->noBlink();
  }
  if(flags & FLAG_CURSOR_AUTO_SCROLL) {
    native_type_->autoscroll();
  } else {
    native_type_->noAutoscroll();
  }
#endif

}

