#include "lcdmenu.h"

using namespace lcdgui;

lcdgui::OptionList::OptionList(String *arr, const uint8_t &count) {
  arr_ = arr;
  count_ = count;
}

String lcdgui::OptionList::option(const uint8_t &index) {
  return arr_[index];
}

String OptionList::option(const uint8_t &index) const {
  return { arr_[index] };
}

uint8_t lcdgui::OptionList::count() const {
  return count_;
}

String lcdgui::OptionList::operator[](const uint8_t &index) {
  return option(index);
}