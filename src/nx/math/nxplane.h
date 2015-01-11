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
#ifndef __NX_PLANE_H__
#define __NX_PLANE_H__


#include "nx/math/nx3dmath.h"
#include "nx/math/nxintersect.h"

namespace nx
{

class NXPlane
{
public:
    NXPlane():
        direction(1.0f, 0.0f, 0.0f),
        offset(1.0f)
    {
    }

    /// Create a plane through: ax + by + cz + d = 0
    void set(const float a,
             const float b,
             const float c,
             const float d);

    /// Create a plane out of a normal and an offset
    void set(const glm::vec3& direction,
             const float offset)
    {
        this->direction = direction;
        this->offset = offset;
    }

    /// Create a plane out of a normal and a point
    void set(const glm::vec3& normal,
             const glm::vec3& point)
    {
        direction = normal;
        offset = glm::dot(normal, point);
    }

    /// Create a plane out of 3 points
    void set(const glm::vec3& p0,
             const glm::vec3& p1,
             const glm::vec3& p2);

    float distance(const glm::vec3& point) const
    {
        return glm::dot(direction, point) - offset;
    }


    glm::vec3 direction;
    float offset;
};

}
#endif
