// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#pragma once

#include <exception>
#include <string>
#include <string.h>

namespace dicomcodecs {
/// <summary>
/// dicomcodec exception class
/// </summary>
class dicom_codec_exception : public std::exception {
public:
  dicom_codec_exception(const char *codec, const char *reason)
      : codec_(codec), reason_(reason) {
    what_.resize(strlen(codec) + strlen(reason) + 4);
    sprintf((char *)what_.c_str(), "%s %s", codec, reason);
  }

  virtual ~dicom_codec_exception() throw() {}

  virtual const char *what() const throw() { return what_.c_str(); }

  const char *codec() const throw() { return codec_; }

  const char *reason() const throw() { return reason_; }

private:
  std::string what_;
  const char *codec_;
  const char *reason_;
};
} // namespace dicomcodecs
