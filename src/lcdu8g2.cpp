#include "lcdu8g2.h"

#define CONVCHARX(col) (col * (char_option_.char_width + char_option_.width_space) + char_option_.col_offset)
#define CONVCHARY(row) ((row * (char_option_.char_height + char_option_.height_space)) \
+ char_option_.char_height + char_option_.row_offset)

void LiquidCrystalU8GLIB2::drawUnknownChar(uint8_t *ptr, uint8_t x_pos, uint8_t y_pos) {
  for(uint8_t y_off = 0; y_off < char_option_.char_height; y_off++) {
    uint8_t layer = ptr[char_option_.char_height - y_off - 1];
    for(uint8_t x_off = 0; x_off < char_option_.char_width; x_off++) {
      if(layer & (1 << (char_option_.char_width - x_off - 1))) {
        u8g2_.drawPixel(x_pos + x_off, y_pos - y_off);
      }
    }
  }
}

LiquidCrystalU8GLIB2::LiquidCrystalU8GLIB2(U8G2 &u8g2, const CharOption& char_option)
        : u8g2_(u8g2), char_option_(char_option) {
  u8g2_.setFont(char_option_.font);
  cols_ = u8g2.getWidth() / (char_option_.char_width + char_option_.width_space);
  rows_ = (u8g2.getHeight()) / (char_option_.char_height +char_option_.height_space);
  buffer_ = new uint8_t[cols_ * rows_];
  clear();
}

void LiquidCrystalU8GLIB2::setCharMap(CharMap *char_map) {
  char_map_ = char_map;
}

uint8_t LiquidCrystalU8GLIB2::cols() const {
  return cols_;
}

uint8_t LiquidCrystalU8GLIB2::rows() const {
  return rows_;
}

void LiquidCrystalU8GLIB2::flush() {
  char str_arr[2]{0, 0};
  uint8_t* alt_symbol;
  u8g2_.firstPage();
  do {
    for(uint8_t row = 0; row < rows_; row++) {
      uint8_t* cbuf = &buffer_[row * cols_];
      for (uint8_t col = 0; col < cols_; col++) {
        char ch = static_cast<char>(cbuf[col]);
        uint16_t x = CONVCHARX(col);
        uint16_t y = CONVCHARY(row);
        if(char_map_ != nullptr && (alt_symbol = char_map_->findReplace(ch)) != nullptr) {
          drawUnknownChar(alt_symbol, x, y);
        } else {
          str_arr[0] = ch;
          u8g2_.drawStr(x, y + 1, str_arr);
        }
        if(cursor_col_ == col && cursor_row_ == row && (flags_ & LCU8_FLAG_SHOW_CURSOR)) {
          u8g2_.drawLine(x, y, x + char_option_.char_width, y);
        }
      }
    }
  } while (u8g2_.nextPage());
}

void LiquidCrystalU8GLIB2::clear() {
  for(uint8_t row = 0; row < rows_; row++) {
    for (uint8_t col = 0; col < cols_; col++) {
      buffer_[row * cols_ + col] = 32; //space
    }
  }
  if(flags_ & LCU8_FLAG_AUTO_FLUSH) {
    flush();
  }
}

void LiquidCrystalU8GLIB2::print(const char *cstr) {
  print(String(cstr));
}

void LiquidCrystalU8GLIB2::print(const String &str) {
  if(cursor_row_ >= rows_) {
    return;
  }
  size_t len = str.length();
  size_t count = (len + cursor_col_ <= cols_) ? len : cols_ - cursor_col_;
  if(count == 0) {
    return;
  }
  String value = str.substring(0, count);
  uint8_t* cbuf = &buffer_[cursor_row_ * cols_ + cursor_col_];
  for (size_t idx = 0; idx < count; idx++) {
    cbuf[idx] = str[idx];
  }
  cursor_col_ += count;
  if(flags_ & LCU8_FLAG_AUTO_FLUSH) {
    flush();
  }
}

void LiquidCrystalU8GLIB2::setCursor(uint8_t col, uint8_t row) {
  cursor_col_ = col;
  cursor_row_ = row;
  if(flags_ & LCU8_FLAG_AUTO_FLUSH && flags_ & LCU8_FLAG_SHOW_CURSOR) {
    flush();
  }
}

void LiquidCrystalU8GLIB2::autoFlush(bool state) {
  flags_ = state ? (flags_ | LCU8_FLAG_AUTO_FLUSH) : (flags_ & ~LCU8_FLAG_AUTO_FLUSH);
}

void LiquidCrystalU8GLIB2::cursorVisible(bool state) {
  flags_ = state ? (flags_ | LCU8_FLAG_SHOW_CURSOR) : (flags_ & ~LCU8_FLAG_SHOW_CURSOR);
  if(flags_ & LCU8_FLAG_AUTO_FLUSH) {
    flush();
  }
}

CharMap::CharMap(const uint8_t &count) {
  count_ = count;
  char_arr_ = new uint8_t[count_];
  replace_char_ptr_arr_ = new uint8_t*[count_];
}

CharMap::~CharMap() {
  delete[] char_arr_;
  delete[] replace_char_ptr_arr_;
}

void CharMap::put(char ch, uint8_t* ptr) {
  if(avail() == 0) {
    return;
  }
  char_arr_[index_] = ch;
  replace_char_ptr_arr_[index_] = ptr;
  index_++;
}

uint8_t CharMap::avail() const {
  return count_ - index_;
}

uint8_t *CharMap::findReplace(char ch) {
  for(uint8_t idx = 0; idx < count_; idx++) {
    if(char_arr_[idx] == ch) {
      return replace_char_ptr_arr_[idx];
    }
  }
  return nullptr;
}