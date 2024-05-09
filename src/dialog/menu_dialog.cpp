#include "lcdmenu.h"

using namespace lcdgui;

void MenuDialog::render(lcd_native_type lcd, LcdGuiService& service) {
  if(item_count_ == 0) {
    return;
  }
  const DisplayInfo& display_info = service.displayInfo();
  const uint8_t max_len = display_info.columns - 2;
  const uint8_t& rows = display_info.rows;
  uint8_t row = 0;
  lcd->clear();
  for(size_t idx = cursor_offset; idx < cursor_offset + rows; idx++) {
    if(idx >= item_count_) {
      break;
    }
    LcdBuffer item_buffer;
    menu_items_[idx]->build(item_buffer, max_len);
    item_buffer.resize(max_len);

    LcdBuffer line_buffer;
    line_buffer << (item_cursor_ == idx ? LCD_CHAR_ARROW_RIGHT : LCD_CHAR_SPACE);
    line_buffer << item_buffer;
    line_buffer << ((row == 0 && cursor_offset > 0) ? LCD_CHAR_ARROW_UP :
            (row == rows - 1 && idx < item_count_ - 1 ? LCD_CHAR_ARROW_DOWN : LCD_CHAR_SPACE));
    lcd->setCursor(0, row);
    lcd->print(line_buffer.str());
    row++;
  }
}

void MenuDialog::input(const uint8_t &input, LcdGuiService& service) {
  if(item_count_ == 0) {
    return;
  }
  const uint8_t& rows = service.displayInfo().rows;
  const uint8_t& first_row = cursor_offset;
  const uint8_t& last_row = cursor_offset + rows - 1;
  if(itemAt(item_cursor_)->input(input, this)) {
    return;
  }
  if(input_event_ != nullptr && input_event_(input)) {
    return;
  }
  if(input == LCD_INPUT_UP && item_cursor_ > 0) {
    if(first_row == item_cursor_) {
      cursor_offset--;
    }
    item_cursor_--;
    updateDisplay();
    return;
  }
  if(input == LCD_INPUT_DOWN && item_cursor_ < item_count_ - 1) {
    if(last_row == item_cursor_) {
      cursor_offset++;
    }
    item_cursor_++;
    updateDisplay();
    return;
  }
}


MenuDialog::~MenuDialog() {
  for(size_t idx = 0; idx < item_count_; idx++) {
    delete menu_items_[idx];
  }
  delete[] menu_items_;
}

MenuDialog::MenuDialog(MenuItem** menu_items, const size_t &count) {
  menu_items_ = menu_items;
  item_count_ = count;
}

void MenuDialog::inputEvent(input_event_t event) {
  input_event_ = event;
}

size_t MenuDialog::cursor() const {
  return item_cursor_;
}

void MenuDialog::cursor(const size_t &cursor) {
  item_cursor_ = cursor < item_count_ ? cursor : 0;
  updateDisplay();
}

MenuItem* MenuDialog::itemAt(const size_t &index) {
  return menu_items_[index];
}

size_t MenuDialog::itemCount() const {
  return item_count_;
}

gui_dialog MenuDialog::clone() {
  return new MenuDialog(menu_items_, item_count_);
}