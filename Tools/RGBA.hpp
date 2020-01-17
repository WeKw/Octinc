/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_RGBA_HPP
#  define OCTINC_RGBA_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  if __cplusplus < 201103L && !defined(nullptr)
#    define nullptr NULL
#  endif

namespace Octinc {
static int parseHex(const std::string& str) {
    return static_cast<int>(strtoul(str.c_str(), nullptr, 16));
}

class RGBA {
   public:
    RGBA() : red(0), green(0), blue(0), alpha(255) {}
    RGBA(int red, int green, int blue, int alpha = 255)
        : red(red), green(green), blue(blue), alpha(alpha) {}

    bool hasAlpha() const { return alpha != 255; }

    RGBA operator=(std::string value) {
        static const std::regex reg(
            "^([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})?"
            "$");

        std::smatch match;

        if (std::regex_match(value, match, reg)) {
            red = parseHex(match[1].str());
            green = parseHex(match[2].str());
            blue = parseHex(match[3].str());

            if (match[4].matched) alpha = parseHex(match[4].str());
        } else
            throw std::runtime_error("Invalid color value");

        return *this;
    }

    friend std::istream& operator>>(std::istream& is, RGBA& rgba) {
        std::string value;
        is >> value;

        rgba = value;

        return is;
    }

   public:
    int red;
    int green;
    int blue;
    int alpha;
};
}  // namespace Octinc

#endif
