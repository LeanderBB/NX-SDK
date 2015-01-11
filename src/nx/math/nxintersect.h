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
#ifndef __NX_INTERSECT_H__
#define __NX_INTERSECT_H__

namespace nx
{

class NXAABB;
class NXSphere;
class NXRay;
class NXPlane;


typedef struct
{
    bool intersects;
    float distance;
} NXIntersectResult;



bool nxIntersects(const NXSphere& sphere,
                  const NXAABB& aabb);


bool nxIntersects(const NXPlane& plane,
                  const NXAABB& aabb);

bool nxIntersects(const NXPlane& plane,
                  const NXSphere& sphere);


bool nxIntersects(const NXAABB& aabb1,
                  const NXAABB& aabb2);

bool nxIntersects(const NXSphere& sphere1,
                  const NXSphere& sphere2);

NXIntersectResult nxIntersectsWithRes(const NXRay& ray,
                                      const NXPlane& plane);


NXIntersectResult nxIntersectsWithRes(const NXRay& ray,
                                      const NXAABB& aabb);

NXIntersectResult nxIntersectsWithRes(const NXRay& ray,
                                      const NXSphere& sphere,
                                      const bool discardInside = false);

}

#endif
