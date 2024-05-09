#ifndef SIMPLE_LCD_GUI_SRC_LCDCORE_H_
#define SIMPLE_LCD_GUI_SRC_LCDCORE_H_

#include "lcdguidef.h"
#include <WString.h>

namespace lcdgui {

class LcdBuffer {
 protected:
  String buf_;
 public:

  typedef uint8_t size_type_;

  LcdBuffer();

  explicit LcdBuffer(const String &buf);

  LcdBuffer(const LcdBuffer &buffer);

  void append(const String &value);

  void space(size_type_ count = 1);

  void fill(char ch, size_type_ count);

  void reset();

  void substring(size_type_ start, size_type_ end);

  void resize(size_type_ len);

  void center(size_type_ max_len);

  size_type_ length() const;

  String str() const;

  LcdBuffer& operator<<(const char* arr);

  LcdBuffer &operator<<(const String &value);

  LcdBuffer &operator<<(const uint8_t &value);

  LcdBuffer &operator<<(const uint16_t &value);

  LcdBuffer &operator<<(const uint32_t &value);

  LcdBuffer &operator<<(const int8_t &value);

  LcdBuffer &operator<<(const int16_t &value);

  LcdBuffer &operator<<(const int32_t &value);

  LcdBuffer &operator<<(const float &value);

  LcdBuffer &operator<<(const double &value);

  LcdBuffer &operator<<(const char &value);

  LcdBuffer &operator<<(const bool &value);

  LcdBuffer &operator<<(const LcdBuffer &buffer);

};

class LcdGuiService {
  gui_dialog current_dialog_ = nullptr;
  lcd_native_type native_type_ = nullptr;
  DisplayInfo* display_info_;

 public:
  LcdGuiService(lcd_native_type native_type, const uint8_t& columns,
  const uint8_t& rows, const uint8_t& char_size = 0x00);

#if defined(LCD_DEFAULT_)
  LcdGuiService(uint8_t rs, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
  const uint8_t& columns, const uint8_t& rows, const uint8_t& char_size = 0x00);
#elif defined(LCD_I2C_)
  LcdGuiService(uint8_t lcd_addr, const uint8_t& columns, const uint8_t& rows,
             const uint8_t& char_size = 0x00);
#endif

  ~LcdGuiService();

  void begin(bool initialize_lcd = true);

  gui_dialog currentDialog();

  void openDialog(gui_dialog dialog);

  void closeDialog();

  void disposeDialog();

  void dispatchInput(const uint8_t& input);

  void updateDisplay();

  lcd_native_type nativeLcd() const;

  DisplayInfo displayInfo() const;

};


}

#endif //SIMPLE_LCD_GUI_SRC_LCDCORE_H_
