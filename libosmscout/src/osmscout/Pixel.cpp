/*
  This source is part of the libosmscout library
  Copyright (C) 2013  Tim Teulings

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

#include <osmscout/Pixel.h>

#include <sstream>

namespace osmscout {

  std::string Pixel::GetDisplayText() const
  {
    std::ostringstream stream;

    stream << x << "," << y;

    return stream.str();
  }

  Vertex2D operator-(const Vertex2D &a, const Vertex2D &b)
  {
    return Vertex2D(a.GetX()-b.GetX(),
                    a.GetY()-b.GetY());
  }

  Vertex2D operator+(const Vertex2D &a, const Vertex2D &b)
  {
    return Vertex2D(a.GetX()+b.GetX(),
                    a.GetY()+b.GetY());
  }

  Vertex2D operator*(const Vertex2D &a, double scale)
  {
    return Vertex2D(a.GetX() * scale,
                    a.GetY() * scale);
  }

}
