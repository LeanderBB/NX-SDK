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
#include "nx/nxcore.h"
#include "nx/math/nxplane.h"

namespace nx
{

void
NXPlane::set(const float a,
             const float b,
             const float c,
             const float d)
{
    // normalize for cheap distance checks
    float lensq = a * a + b * b + c*c;

    // length of normal had better not be zero
    NX_ASSERT(!NX3DMath::isZero(lensq) && "must not be zero");

    // recover gracefully
    if (NX3DMath::isZero(lensq))
    {
        direction = glm::vec3(1.0f, 0.0f, 0.0f);
        offset = 0.0f;
    }
    else
    {
        float recip = glm::inversesqrt(lensq);
        direction = glm::vec3(a * recip, b * recip, c * recip);
        offset = d*recip;
    }
}

void
NXPlane::set(const glm::vec3& p0,
             const glm::vec3& p1,
             const glm::vec3& p2)
{
    glm::vec3 kU = p1 - p0;
    glm::vec3 kV = p2 - p0;
    glm::vec3 kW = glm::cross(kU, kV);

    // normalize for cheap distance checks
    float lensq = kW.x * kW.x + kW.y * kW.y + kW.z * kW.z;

    // length of normal had better not be zero
    NX_ASSERT(!NX3DMath::isZero(lensq) && "Must not be zero");

    // recover gracefully
    if (NX3DMath::isZero(lensq))
    {
        direction = glm::vec3(1.0f, 0.0f, 0.0f);
        offset = 0.0f;
    }
    else
    {
        float recip = 1.0f / lensq;
        direction = glm::vec3(kW.x * recip, kW.y * recip, kW.z * recip);
        offset = glm::dot(-direction, p0);
    }
}

}
