#include "lcddialog.h"

using namespace lcdgui;

void ResultDialog::render(lcd_native_type lcd, LcdGuiService &service) {
  LcdBuffer buffer;
  uint8_t c_pos = 0;
  if(options_ & RESULT_OPTION_OK) {
    if(c_pos == option_cursor_) {
      buffer << "<OK>";
    } else {
      buffer << " OK ";
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_YES) {
    if(c_pos > 0) {
      buffer.space();
    }
    if(c_pos == option_cursor_) {
      buffer << "<YES>";
    } else {
      buffer << " YES ";
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_NO) {
    if(c_pos > 0) {
      buffer.space();
    }
    if(c_pos == option_cursor_) {
      buffer << "<NO>";
    } else {
      buffer << " NO ";
    }
  }
  buffer.center(service.displayInfo().columns);
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(text_);
  lcd->setCursor(0, 1);
  lcd->print(buffer.str());
}

void ResultDialog::input(const uint8_t &input, LcdGuiService &service) {
  if(input == LCD_INPUT_LEFT && option_cursor_ > 0) {
    option_cursor_--;
    updateDisplay();
    return;
  }
  if(input == LCD_INPUT_RIGHT) {
    if(option_cursor_ < optionCount() - 1) {
      option_cursor_++;
      updateDisplay();
    }
    return;
  }
  if(input == LCD_INPUT_NEXT && choose_event_ != nullptr) {
    choose_event_(focusedOption());
  }
}

ResultDialog::ResultDialog(const String& text, uint8_t options, result_choose_event_t choose_event) {
  text_ = text;
  options_ = options;
  choose_event_ = choose_event;
}

result_choose_event_t ResultDialog::chooseEvent() {
  return choose_event_;
}

uint8_t ResultDialog::options() const {
  return options_;
}

gui_dialog ResultDialog::clone() {
  return new ResultDialog(text_, options_, choose_event_);
}

uint8_t ResultDialog::optionCount() const {
  return BIT_INUM(options_, RESULT_OPTION_OK)  +
         BIT_INUM(options_, RESULT_OPTION_YES) +
         BIT_INUM(options_, RESULT_OPTION_NO);
}

uint8_t ResultDialog::focusedOption() const {
  uint8_t c_pos = 0;
  if(options_ & RESULT_OPTION_OK) {
    if(c_pos == option_cursor_) {
      return c_pos;
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_YES) {
    if(c_pos == option_cursor_) {
      return c_pos;
    }
    c_pos++;
  }
  if(options_ & RESULT_OPTION_NO) {
    if(c_pos == option_cursor_) {
      return c_pos;
    }
  }
  return 0;
}

