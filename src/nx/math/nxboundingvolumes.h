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
#ifndef __NX_BOUNDINGVOLUMES_H__
#define __NX_BOUNDINGVOLUMES_H__

#include "nx/math/nx3dmath.h"
#include "nx/math/nxintersect.h"
namespace nx
{


class NXSphere;
class NXRay;

class NXAABB
{
public:

    NXAABB():
        center(0.0f),
        extends(1.0f)
    {
    }

    void setMinMax(const glm::vec3& min,
                   const glm::vec3& max)
    {
        extends = (max + min) * 0.5f;
        center = (max - min) * 0.5f;
    }

    void set(const glm::vec3& center,
             const glm::vec3& extends)
    {
        this->extends = extends;
        this->center = center;
    }

    void minmax(glm::vec3& min,
                glm::vec3& max) const
    {
        min = center - extends;
        max = center + extends;
    }

    glm::vec3 min() const
    {
        return center - extends;
    }

    glm::vec3 max() const
    {
        return center + extends;
    }

    float distance(const glm::vec3& point) const;

    bool intersects(const NXAABB& other) const
    {
        return nxIntersects(*this, other);
    }

    bool intersects(const NXSphere& other) const
    {
        return nxIntersects(other, *this);
    }

    bool intersects(const NXRay& other) const
    {
        NXIntersectResult res = nxIntersectsWithRes(other, *this);
        return res.intersects;
    }

    bool contains(const NXAABB& other) const;
    bool contains(const NXSphere& other) const;
    bool contains(const glm::vec3& point) const;

    void merge(const NXAABB& other);
    void merge(const glm::vec3& point);


public:
    glm::vec3 center;
    glm::vec3 extends;
};



class NXSphere
{
public:
    NXSphere():
        center(0.0f),
        radius(1.0f)
    {
    }

    void set(const glm::vec3& center,
             const float radius)
    {
        this->center = center;
        this->radius = radius;
    }

    /// if it returns < 0, it means that the point is
    /// inside the sphere.
    float distance(const glm::vec3& point) const;

    bool intersects(const NXAABB& other) const
    {
        return nxIntersects(*this, other);
    }

    bool intersects(const NXSphere& other) const
    {
        return nxIntersects(*this, other);
    }

    bool intersects(const NXRay& other) const
    {
        NXIntersectResult res = nxIntersectsWithRes(other, *this);
        return res.intersects;
    }

    bool contains(const NXAABB& other) const;
    bool contains(const NXSphere& other) const;
    bool contains(const glm::vec3& other) const;

    void merge(const NXSphere& other);
    void merge(const glm::vec3& point);

public:

    glm::vec3 center;
    float radius;
};

}

#endif
