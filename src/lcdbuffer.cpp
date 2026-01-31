#include "../include/lcdcore.h"

using namespace lcdgui;

LcdBuffer::LcdBuffer() = default;

LcdBuffer::LcdBuffer(const String &buf) {
  buf_ = buf;
}

LcdBuffer::LcdBuffer(const LcdBuffer &buffer) {
  buf_ = buffer.buf_;
}

void LcdBuffer::append(const String &value) {
  buf_ += value;
}

void LcdBuffer::space(size_type_ count) {
  fill(' ', count);
}

void LcdBuffer::fill(char ch, size_type_ count) {
  char* arr = new char[count + 1];
  for(size_type_ idx = 0; idx < count; idx++) {
    arr[idx] = ch;
  }
  arr[count] = '\0';
  buf_.concat(arr);
  delete[] arr;
}

void LcdBuffer::reset() {
  buf_ = "";
}

void LcdBuffer::substring(size_type_ start, size_type_ end) {
  buf_ = buf_.substring(start, end);
}

void LcdBuffer::resize(size_type_ len) {
  if(buf_.length() > len) {
    substring(0, len);
    return;
  }
  auto remaining_len = len - length();
  fill(' ', remaining_len);
}

void LcdBuffer::center(size_type_ max_len) {
  if(length() >= max_len) {
    return;
  }
  size_type_ prev_space = (max_len / 2) - (length() / 2);
  String value = String(buf_);
  reset();
  space(prev_space);
  append(value);
}

LcdBuffer::size_type_ LcdBuffer::length() const {
  return buf_.length();
}

String LcdBuffer::str() const {
  return { buf_ };
}

LcdBuffer& LcdBuffer::operator<<(const char* arr) {
  append(String(arr));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const String &value) {
  append(value);
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const uint8_t &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const uint16_t &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const uint32_t &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const int8_t &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const int16_t &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const int32_t &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const float &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const double &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const char &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const bool &value) {
  append(String(value));
  return *this;
}

LcdBuffer& LcdBuffer::operator<<(const LcdBuffer &buffer) {
  append(buffer.buf_);
  return *this;
}