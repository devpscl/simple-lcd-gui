#include "lcdmenu.h"

using namespace lcdgui;

void MenuDialog::render(LiquidCrystalGui& lcg) {
  const DisplayInfo& display_info = lcg.displayInfo();
  const uint8_t& rows = display_info.rows;
  for(uint8_t row = 0; row < rows; row++) {
    renderRow(row);
  }
}

void MenuDialog::input(LiquidCrystalGui& lcg, const uint8_t &input) {
  if(item_count_ == 0) {
    return;
  }
  const uint8_t& rows = lcg.displayInfo().rows;
  const uint8_t& first_row = cursor_offset_;
  const uint8_t& last_row = cursor_offset_ + rows - 1;
  if(itemAt(item_cursor_)->input(input, this)) {
    return;
  }
  if(input_event_ != nullptr && input_event_(input)) {
    return;
  }
  if(input == LCD_INPUT_UP && item_cursor_ > 0) {
    if(first_row == item_cursor_) {
      cursor_offset_--;
    }
    item_cursor_--;
    updateDisplay();
    return;
  }
  if(input == LCD_INPUT_DOWN && item_cursor_ < item_count_ - 1) {
    if(last_row == item_cursor_) {
      cursor_offset_++;
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

uint8_t MenuDialog::rowOf(const size_t& index) {
  if(lcg_instance == nullptr) {
    return - 1;
  }
  const DisplayInfo& display_info = lcg_instance->displayInfo();
  if(index < cursor_offset_ || index > (cursor_offset_ + display_info.rows)) {
    return -1;
  }
  return index - cursor_offset_;
}

size_t MenuDialog::indexOf(menu_item item) {
  for(size_t idx = 0; idx < item_count_; idx++) {
    if(menu_items_[idx] == item) {
      return idx;
    }
  }
  return -1;
}

void MenuDialog::renderRow(uint8_t row) {
  if(lcg_instance == nullptr) {
    return;
  }
  const uint8_t& cols = lcg_instance->columns();
  const uint8_t& rows = lcg_instance->rows();
  const uint8_t& max_len = cols - 2;
  size_t item_index = cursor_offset_ + row;
  LcdBuffer line_buffer;
  if(item_index >= item_count_) {
    line_buffer.space(cols);
  } else {
    LcdBuffer item_buffer;
    menu_items_[item_index]->build(item_buffer, max_len);
    item_buffer.resize(max_len);

    line_buffer << (item_cursor_ == item_index ? LCD_CHAR_ARROW_RIGHT : LCD_CHAR_SPACE);
    line_buffer << item_buffer;
    line_buffer << ((row == 0 && cursor_offset_ > 0) ? LCD_CHAR_ARROW_UP :
                    (row == rows - 1 && item_index < item_count_ - 1 ? LCD_CHAR_ARROW_DOWN : LCD_CHAR_SPACE));
  }
  lcg_instance->cursor(0, row);
  lcg_instance->print(line_buffer.str());
}

void MenuDialog::renderItem(const size_t& index) {
  renderRow(rowOf(index));
}

void MenuDialog::renderItem(menu_item item) {
  renderItem(indexOf(item));
}

void MenuDialog::inputEvent(input_event_t event) {
  input_event_ = event;
}

size_t MenuDialog::cursor() const {
  return item_cursor_;
}

void MenuDialog::cursor(const size_t &cursor) {
  if(lcg_instance == nullptr) {
    return;
  }
  const uint8_t& rows = lcg_instance->rows() - 1;
  item_cursor_ = cursor < item_count_ ? cursor : 0;
  cursor_offset_ = (rows > item_cursor_) ? 0 : item_cursor_ - rows;
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