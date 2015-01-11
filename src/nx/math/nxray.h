//
// This file is part of the NX Project
//
// Copyright (c) 2014 Leander Beernaert
//
// NX Project is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// NX Project is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with NX. If not, see <http://www.gnu.org/licenses/>.
//
#ifndef __NX_RAY_H__
#define __NX_RAY_H__

#include "nx/math/nx3dmath.h"

namespace nx
{

class NXRay
{
public:
    NXRay():
        origin(0.0f),
        direction(0.0f, 0.0f, 1.0f),
        range()
    {
    }


    void set(const glm::vec3& origin,
             const glm::vec3& direction,
             const float range)
    {
        NX_ASSERT(range > 0);
        this->origin = origin;
        this->direction = direction;
        this->range = range;
    }

    void setRange(const float range)
    {
        NX_ASSERT(range > 0);
        this->range = range;
    }

    void setRangeInfinite()
    {
        range = std::numeric_limits<float>::max();
    }

    glm::vec3 pointAt(const float distance) const
    {
        return origin + (direction * distance);
    }


    glm::vec3 origin;
    glm::vec3 direction;
    float range;
};
}
#endif
