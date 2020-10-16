#ifndef OSMSCOUT_UTIL_MAGNIFICATION_H
#define OSMSCOUT_UTIL_MAGNIFICATION_H

/*
  This source is part of the libosmscout library
  Copyright (C) 2012  Tim Teulings

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

#include <osmscout/CoreImportExport.h>

#include <functional>
#include <ostream>
#include <string>
#include <unordered_map>

#include <osmscout/system/Compiler.h>
#include <osmscout/system/SystemTypes.h>

namespace osmscout {

  class OSMSCOUT_API MagnificationLevel CLASS_FINAL
  {
  private:
    uint32_t level = 0;

  public:
    MagnificationLevel() = default;

    inline explicit MagnificationLevel(uint32_t level) noexcept
      : level(level)
    {
    }

    inline MagnificationLevel(const MagnificationLevel& level) = default;

    inline uint32_t Get() const
    {
      return level;
    }

    inline MagnificationLevel& operator=(const MagnificationLevel& other) = default;

    inline MagnificationLevel& operator++()
    {
      ++level;

      return *this;
    }

    inline MagnificationLevel operator++(int)
    {
      ++level;

      return *this;
    }

    inline MagnificationLevel& operator+=(uint32_t increment)
    {
      level+=increment;

      return *this;
    }

    inline bool operator==(const MagnificationLevel& other) const
    {
      return level==other.level;
    }

    inline bool operator!=(const MagnificationLevel& other) const
    {
      return level!=other.level;
    }

    inline bool operator<(const MagnificationLevel& other) const
    {
      return level<other.level;
    }

    inline bool operator<=(const MagnificationLevel& other) const
    {
      return level<=other.level;
    }

    inline bool operator>=(const MagnificationLevel& other) const
    {
      return level>=other.level;
    }

    inline bool operator>(const MagnificationLevel& other) const
    {
      return level>other.level;
    }
  };

  inline std::ostream& operator<<(std::ostream& os,
                                  const MagnificationLevel& level)
  {
    os << level.Get();

    return os;
  }

  inline std::string operator+(const char* text,
                               const MagnificationLevel& level)
  {
    return std::string(text)+std::to_string(level.Get());
  }

  inline std::string operator+(const std::string& text,
                               const MagnificationLevel& level)
  {
    return text+std::to_string(level.Get());
  }
}

namespace std {
  template <>
  struct hash<osmscout::MagnificationLevel>
  {
    size_t operator()(const osmscout::MagnificationLevel& level) const
    {
      return hash<uint32_t>{}(level.Get());
    }
  };
}

namespace osmscout {
  class OSMSCOUT_API Magnification CLASS_FINAL
  {
  public:
    static MagnificationLevel magWorld;     //  0
    static MagnificationLevel magContinent; //  4
    static MagnificationLevel magState;     //  5
    static MagnificationLevel magStateOver; //  6
    static MagnificationLevel magCounty;    //  7
    static MagnificationLevel magRegion;    //  8
    static MagnificationLevel magProximity; //  9
    static MagnificationLevel magCityOver;  // 10
    static MagnificationLevel magCity;      // 11
    static MagnificationLevel magSuburb;    // 12
    static MagnificationLevel magDetail;    // 13
    static MagnificationLevel magClose;     // 14
    static MagnificationLevel magCloser;    // 15
    static MagnificationLevel magVeryClose; // 16
    static MagnificationLevel magBlock;     // 18
    static MagnificationLevel magStreet;    // 19
    static MagnificationLevel magHouse;     // 20

  private:
    double   magnification=1;
    uint32_t level=0;

  public:
    Magnification() = default;

    Magnification(const Magnification& other) = default;

    inline explicit Magnification(double magnification) noexcept
    {
      SetMagnification(magnification);
    }

    inline explicit Magnification(const MagnificationLevel& level) noexcept
    {
      SetLevel(level);
    }

    void SetMagnification(double magnification);

    void SetLevel(const MagnificationLevel& level);

    inline double GetMagnification() const
    {
      return magnification;
    }

    inline uint32_t GetLevel() const
    {
      return level;
    }

    inline Magnification& operator=(const Magnification& other)
    {
      if (this!=&other) {
        this->magnification=other.magnification;
        this->level=other.level;
      }

      return *this;
    }

    inline bool operator==(const Magnification& other) const
    {
      return magnification==other.magnification;
    }

    inline bool operator!=(const Magnification& other) const
    {
      return magnification!=other.magnification;
    }

    inline bool operator<(const Magnification& other) const
    {
      return magnification<other.magnification;
    }

    inline bool operator<=(const Magnification& other) const
    {
      return magnification<=other.magnification;
    }

    inline bool operator>=(const Magnification& other) const
    {
      return magnification>=other.magnification;
    }

    inline bool operator>(const Magnification& other) const
    {
      return magnification>other.magnification;
    }

    inline Magnification& operator++()
    {
      magnification*=2.0;
      level+=1;

      return *this;
    }

    inline Magnification operator++(int)
    {
      magnification*=2.0;
      level+=1;

      return *this;
    }
  };

  class OSMSCOUT_API MagnificationConverter
  {
  private:
    std::unordered_map<std::string,MagnificationLevel> stringToMagMap;
    std::unordered_map<MagnificationLevel,std::string> levelToStringMap;

  public:
    MagnificationConverter();

    bool Convert(const std::string& name,
                 Magnification& magnification);

    bool Convert(const MagnificationLevel& level,
                 std::string& name);
  };
}

#endif
