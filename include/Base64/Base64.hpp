#ifndef AI_BASE64_HPP
#define AI_BASE64_HPP

// Found at http://www.adp-gmbh.ch/cpp/common/base64.html

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

#endif