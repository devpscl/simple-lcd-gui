#ifndef SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
#define SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
#include "lcdcore.h"

namespace lcdgui {

#define RESULT_OPTION_YES     0x1
#define RESULT_OPTION_NO      0x2
#define RESULT_OPTION_OK      0x4

class GuiDialog {

 private:

  void setLcdService(LcdGuiService* service);

  friend class LcdGuiService;

 protected:
  LcdGuiService* lcd_gui_service = nullptr;

  virtual void render(lcd_native_type lcd, LcdGuiService& service);

  virtual void input(const uint8_t &input, LcdGuiService& service);

  friend class LcdGuiService;

 public:

  virtual ~GuiDialog();

  virtual gui_dialog clone();

  void close();

  void dispose();

  void updateDisplay();

};

class ResultDialog : public GuiDialog {
  String text_;
  uint8_t options_;
  result_choose_event_t choose_event_;
  uint8_t option_cursor_ = 0;

  uint8_t optionCount() const;

  uint8_t focusedOption() const;

 protected:
  void render(lcd_native_type lcd, LcdGuiService& service) override;

  void input(const uint8_t &input, LcdGuiService& service) override;

 public:

  ResultDialog(const String& text, uint8_t options, result_choose_event_t choose_event = nullptr);

  result_choose_event_t chooseEvent();

  uint8_t options() const;

  gui_dialog clone() override;

};

}

#endif //SIMPLE_LCD_GUI_SRC_LCDDIALOG_H_
