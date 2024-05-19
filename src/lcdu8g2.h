#if not defined SIMPLE_LCD_GUI_SRC_LCDU8GLIB_H_ && __has_include("U8g2lib.h")
#define SIMPLE_LCD_GUI_SRC_LCDU8GLIB_H_
#include <U8g2lib.h>

#define CHAR_SIZE_5x7 CharOption{u8g2_font_6x12_tf, 5, 7, 1, 1, 0, 0}
#define CHAR_SIZE_6x9 CharOption{u8g2_font_7x13_tf, 6, 9, 1, 1, 0, 0}

#define LCU8_FLAG_AUTO_FLUSH 0x1
#define LCU8_FLAG_SHOW_CURSOR 0x2

struct CharOption {
  const uint8_t* font;
  uint8_t char_width;
  uint8_t char_height;
  uint8_t width_space;
  uint8_t height_space;
  int8_t col_offset;
  int8_t row_offset;
};

class CharMap {
 private:
  uint8_t* char_arr_;
  uint8_t** replace_char_ptr_arr_;
  uint8_t count_;
  uint8_t index_ = 0;

 public:

  explicit CharMap(const uint8_t& count);

  ~CharMap();

  void put(char ch, uint8_t* );

  uint8_t avail() const;

  uint8_t* findReplace(char ch);

};

class LiquidCrystalU8GLIB2 {
 private:
  U8G2& u8g2_;
  CharOption char_option_;
  uint8_t cols_;
  uint8_t rows_;
  uint8_t cursor_col_ = 0;
  uint8_t cursor_row_ = 0;
  uint8_t* buffer_;
  uint8_t flags_ = LCU8_FLAG_AUTO_FLUSH;
  CharMap* char_map_ = nullptr;

  void drawUnknownChar(uint8_t *ptr, uint8_t x_pos, uint8_t y_pos);

 public:
  explicit LiquidCrystalU8GLIB2(U8G2& u8g2, const CharOption& char_option = CHAR_SIZE_5x7);

  void setCharMap(CharMap* char_map);

  uint8_t cols() const;

  uint8_t rows() const;

  void flush();

  void clear();

  void print(const char* cstr);

  void print(const String& str);

  void setCursor(uint8_t col, uint8_t row);

  void autoFlush(bool state);

  void cursorVisible(bool state);

};

#endif //SIMPLE_LCD_GUI_SRC_LCDU8GLIB_H_
