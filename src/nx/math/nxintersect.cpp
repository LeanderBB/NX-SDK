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
#include "nx/math/nxintersect.h"

#include "nx/math/nxboundingvolumes.h"
#include "nx/math/nxplane.h"
#include "nx/math/nxray.h"

namespace nx
{

bool
nxIntersects(const NXSphere& sphere,
             const NXAABB& aabb)
{
    // Use splitting planes
    const glm::vec3& center = sphere.center;
    float radius = sphere.radius;
    const glm::vec3& min = aabb.min();
    const glm::vec3& max = aabb.max();

    // Arvo's algorithm
    float s, d = 0;
    for (int i = 0; i < 3; ++i)
    {
        if (center[i] < min[i])
        {
            s = center[i] - min[i];
            d += s * s;
        }
        else if(center[i] > max[i])
        {
            s = center[i] - max[i];
            d += s * s;
        }
    }

    return d <= radius * radius;
}


bool
nxIntersects(const NXPlane& plane,
             const NXAABB& aabb)
{
    // Calculate the distance between box centre and the plane
    float dist = plane.distance(aabb.center);

    // Calculate the maximise allows absolute distance for
    // the distance between box centre and plane
    float maxAbsDist = glm::abs(glm::dot(plane.direction, aabb.extends));

    if (dist < -maxAbsDist)
        return false;

    if (dist > +maxAbsDist)
        return false;

    return true;
}

bool
nxIntersects(const NXPlane& plane,
             const NXSphere& sphere)
{
    return glm::abs(plane.distance(sphere.center)) <= sphere.radius;
}


bool
nxIntersects(const NXAABB& aabb1,
             const NXAABB& aabb2)
{
    glm::vec3 dist(aabb1.center - aabb2.center);
    glm::vec3 sumHalfSizes(aabb1.extends + aabb2.extends);

    return (glm::abs(dist.x) <= sumHalfSizes.x) &&
            (glm::abs(dist.y) <= sumHalfSizes.y) &&
            (glm::abs(dist.z) <= sumHalfSizes.z);
}

bool
nxIntersects(const NXSphere& sphere1,
             const NXSphere& sphere2)
{
    glm::vec3 center_diff = sphere1.center - sphere2.center;
    float radius_sum = sphere1.radius + sphere2.radius;
    return (glm::dot(center_diff,center_diff) <= NX3DMath::squared(radius_sum));
}

NXIntersectResult
nxIntersectsWithRes(const NXRay& ray,
                    const NXPlane& plane)
{
    float denom = glm::dot(plane.direction, ray.direction);
    if (glm::abs(denom) < NX_FLT_EPSILON)
    {
        // Parallel
        return {false, 0.0f};
    }
    else
    {
        float nom = glm::dot(plane.direction, ray.origin) + plane.offset;
        float t = -(nom/denom);
        return {t >= 0.0f, t};
    }
}


NXIntersectResult
nxIntersectsWithRes(const NXRay& ray,
                    const NXAABB& aabb)
{
    float lowt = 0.0f;
    float t;
    bool hit = false;
    glm::vec3 hitpoint;
    const glm::vec3 min = aabb.min();
    const glm::vec3 max = aabb.max();
    const glm::vec3& rayorig = ray.origin;
    const glm::vec3& raydir = ray.direction;


    // Check origin inside first
    if (glm::all(glm::greaterThan(rayorig, min)) && glm::all(glm::lessThan(rayorig, max)))
    {
        return {true, 0};
    }

    // Check each face in turn, only check closest 3
    // Min x
    if (rayorig.x <= min.x && raydir.x > 0)
    {
        t = (min.x - rayorig.x) / raydir.x;

        // Substitute t back into ray and check bounds and dist
        hitpoint = rayorig + raydir * t;
        if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt))
        {
            hit = true;
            lowt = t;
        }
    }
    // Max x
    if (rayorig.x >= max.x && raydir.x < 0)
    {
        t = (max.x - rayorig.x) / raydir.x;

        // Substitute t back into ray and check bounds and dist
        hitpoint = rayorig + raydir * t;
        if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt))
        {
            hit = true;
            lowt = t;
        }
    }
    // Min y
    if (rayorig.y <= min.y && raydir.y > 0)
    {
        t = (min.y - rayorig.y) / raydir.y;

        // Substitute t back into ray and check bounds and dist
        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt))
        {
            hit = true;
            lowt = t;
        }
    }
    // Max y
    if (rayorig.y >= max.y && raydir.y < 0)
    {
        t = (max.y - rayorig.y) / raydir.y;

        // Substitute t back into ray and check bounds and dist
        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt))
        {
            hit = true;
            lowt = t;
        }
    }
    // Min z
    if (rayorig.z <= min.z && raydir.z > 0)
    {
        t = (min.z - rayorig.z) / raydir.z;

        // Substitute t back into ray and check bounds and dist
        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.y >= min.y && hitpoint.y <= max.y &&
                (!hit || t < lowt))
        {
            hit = true;
            lowt = t;
        }
    }
    // Max z
    if (rayorig.z >= max.z && raydir.z < 0)
    {
        t = (max.z - rayorig.z) / raydir.z;

        // Substitute t back into ray and check bounds and dist
        hitpoint = rayorig + raydir * t;
        if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.y >= min.y && hitpoint.y <= max.y &&
                (!hit || t < lowt))
        {
            hit = true;
            lowt = t;
        }
    }

    return {hit, lowt};

}

NXIntersectResult
nxIntersectsWithRes(const NXRay& ray,
                    const NXSphere& sphere,
                    const bool discardInside)
{
    const glm::vec3& raydir = ray.direction;
    // Adjust ray origin relative to sphere center
    const glm::vec3 rayorig = ray.origin - sphere.center;
    float radius = sphere.radius;

    // Check origin inside first
    if (glm::length_sqr(rayorig) <= radius*radius && discardInside)
    {
        return {true, 0.0f};
    }

    // Mmm, quadratics
    // Build coeffs which can be used with std quadratic solver
    // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
    float a = glm::dot(raydir,raydir);
    float b = 2 * glm::dot(rayorig,raydir);
    float c = glm::dot(rayorig,rayorig) - radius*radius;

    // Calc determinant
    float d = (b*b) - (4 * a * c);
    if (d < 0)
    {
        // No intersection
        return {false, 0.0f};
    }
    else
    {
        // BTW, if d=0 there is one intersection, if d > 0 there are 2
        // But we only want the closest one, so that's ok, just use the
        // '-' version of the solver
        float t = ( -b - glm::sqrt(d) ) / (2 * a);
        if (t < 0)
            t = ( -b + glm::sqrt(d) ) / (2 * a);
        return {true, t};
    }
}



}
