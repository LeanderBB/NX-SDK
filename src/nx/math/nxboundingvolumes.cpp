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
#include "nx/math/nxboundingvolumes.h"
#include "nxray.h"


namespace nx
{

float
NXAABB::distance(const glm::vec3& point) const
{
    glm::vec3 dist(this->center - point);

    dist.x = glm::abs(dist.x) - extends.x;
    dist.y = glm::abs(dist.y) - extends.y;
    dist.z = glm::abs(dist.z) - extends.z;

    return std::max( std::min( std::min( dist.x, dist.y ), dist.z ), 1.0f );
}


bool
NXAABB::contains(const NXAABB& other) const
{
    glm::vec3 dist(this->center - other.center);

    return (glm::abs( dist.x ) + other.extends.x <= this->extends.x) &&
            (glm::abs( dist.y ) + other.extends.y <= this->extends.y) &&
            (glm::abs( dist.z ) + other.extends.z <= this->extends.z);
}

bool
NXAABB::contains(const NXSphere& other) const
{
    glm::vec3 dist(this->center - other.center);

    return (glm::abs( dist.x ) + other.radius <= this->extends.x) &&
            (glm::abs( dist.y ) + other.radius <= this->extends.y) &&
            (glm::abs( dist.z ) + other.radius <= this->extends.z);
}

bool
NXAABB::contains(const glm::vec3& point) const
{
    glm::vec3 dist(this->center - point);

    return (glm::abs(dist.x) <= this->extends.x) &&
            (glm::abs(dist.y) <= this->extends.y) &&
            (glm::abs(dist.z) <= this->extends.z);
}

void
NXAABB::merge(const NXAABB& other)
{
    glm::vec3 min, max;
    this->minmax(min, max);
    glm::vec3 other_max(other.center + other.extends);
    glm::vec3 other_min(other.center - other.extends);

    std::max(max.x, other_max.x);
    std::max(max.y, other_max.y);
    std::max(max.z, other_max.z);

    std::min(min.x, other_min.x);
    std::min(min.y, other_min.y);
    std::min(max.z, other_min.z);

    if(!NX3DMath::isInf(max.x) &&
            !NX3DMath::isInf(max.y) &&
            !NX3DMath::isInf(max.z))
    {
        center = ( max + min ) * 0.5f;
    }
    extends = ( max - min ) * 0.5f;

}

void
NXAABB::merge(const glm::vec3& point)
{
    glm::vec3 min, max;
    this->minmax(min, max);

    std::max(max.x, point.x);
    std::max(max.y, point.y);
    std::max(max.z, point.z);

    std::min(min.x, point.x);
    std::min(min.y, point.y);
    std::min(max.z, point.z);

    if(!NX3DMath::isInf(max.x) &&
            !NX3DMath::isInf(max.y) &&
            !NX3DMath::isInf(max.z))
    {
        center = ( max + min ) * 0.5f;
    }
    extends = ( max - min ) * 0.5f;
}



float
NXSphere::distance(const glm::vec3& point) const
{
    float len = glm::distance(center, point);
    return len - radius;
}


bool
NXSphere::contains(const NXAABB& other) const
{
    glm::vec3 dist(this->center - other.center);

    return (glm::abs( dist.x ) + other.extends.x <= radius) &&
            (glm::abs( dist.y ) + other.extends.y <= radius) &&
            (glm::abs( dist.z ) + other.extends.z <= radius);
}

bool
NXSphere::contains(const NXSphere& other) const
{
    float dist = glm::length_sqr(center - other.center);
    float radius_sqr = NX3DMath::squared(radius);
    float other_radius_sqr = NX3DMath::squared(other.radius);

    return (dist <= radius_sqr) && (dist + other_radius_sqr <= radius_sqr);
}

bool
NXSphere::contains(const glm::vec3& other) const
{
    return glm::length_sqr(center - other) <= NX3DMath::squared(radius);
}

void
NXSphere::merge(const NXSphere& other)
{
    glm::vec3 diff = other.center - center;
    float lengthSq = glm::length_sqr(diff);
    float radiusDiff = other.radius - radius;

    // Early-out
    if (NX3DMath::squared(radiusDiff) >= lengthSq)
    {
        // One fully contains the other
        if (radiusDiff <= 0.0f)
        {
            return; // no change
        }
        else
        {
            center = other.center;
            radius = other.radius;
            return;
        }
    }

    float length = glm::sqrt(lengthSq);
    float t = (length + radiusDiff) / (2.0f * length);
    center = center + diff * t;
    radius = 0.5f * (length + radius + other.radius);
}

void
NXSphere::merge(const glm::vec3& point)
{
    glm::vec3 diff = point - center;
    float radius_sqrd = NX3DMath::squared(radius);
    float distance_sqrd = glm::length_sqr(diff);

    if (radius_sqrd < distance_sqrd)
    {
        radius = glm::sqrt(distance_sqrd);
    }
}

}
