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
#ifndef __NX_3DMATH_H__
#define __NX_3DMATH_H__

// include glm
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ----------------------------------------------------------------------------
// Extensions to the GLM library
// lenghtSquared
// distanceSquared
// ----------------------------------------------------------------------------
namespace glm
{
template <typename T, precision P>
GLM_FUNC_QUALIFIER T length_sqr(glm::tvec2<T, P> const & v)
{
    GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'length' only accept floating-point inputs");
    T sqr = v.x * v.x + v.y * v.y;
    return sqr;
}

template <typename T, precision P>
GLM_FUNC_QUALIFIER T length_sqr(glm::tvec3<T, P> const & v)
{
    GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'length' only accept floating-point inputs");
    T sqr = v.x * v.x + v.y * v.y + v.z * v.z;
    return sqr;
}
}

// ----------------------------------------------------------------------------
// NX 3D Math
// ----------------------------------------------------------------------------

#define NX_FLT_EPSILON (std::numeric_limits<float>::epsilon())
#define NX_PI (3.1415926535897932384626433832795f)
#define NX_HALF_PI (1.5707963267948966192313216916398f)
#define NX_TWO_PI (6.283185307179586476925286766559f)

namespace nx
{

class NX3DMath
{
public:

/// Template equal function
template <typename T>
static NX_INLINE bool equal(const T a, const T b)
{
return a == b;
}

/// Template iszero function
template <typename T>
static NX_INLINE bool isZero(const T a)
{
return a == T(0);
}

template<typename T>
static NX_INLINE T squared(const T a)
{
    return a * a;
}

template <typename T>
static NX_INLINE bool isInf(const T a)
{
    return a == std::numeric_limits<T>::infinity();
}


};

/// Specialization of IsZero for floats
template <>
NX_INLINE bool NX3DMath::isZero<float>(const float a)
{
return fabsf(a) < NX_FLT_EPSILON;
}

/// Specialization of Equal for floats
template <>
NX_INLINE bool NX3DMath::equal<float>(const float a, const float b)
{
    return NX3DMath::isZero<float>(a - b);
}


}
#endif
