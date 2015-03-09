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
#ifndef __NX_TRANSFORM_H__
#define __NX_TRANSFORM_H__

#include "nx3dmath.h"

namespace nx
{
class NXTransform
{
public:
    NXTransform();

    void translate(const glm::vec3& translation);

    void rotate(const glm::quat& rotation);

    void rotatePrefix(const glm::quat& rotation);

    void identity();

    void scale(const glm::vec3& scale);

    glm::mat4 toMatrix() const;

    glm::mat4 toMatrixInv() const;

    glm::vec3 up() const;

    glm::vec3 forward() const;

    glm::vec3 right() const;

    const glm::vec3& position() const
    {
        return _position;
    }

    const glm::quat& rotation() const
    {
        return _rotation;
    }

    const glm::vec3& scale() const
    {
        return _scale;
    }

protected:
    glm::vec3 _position;
    glm::vec3 _scale;
    glm::quat _rotation;

};


NX_INLINE glm::quat
nxQuat(const float deg,
         const float x,
         const float y,
         const float z)
{
    return glm::angleAxis(glm::radians(deg), glm::vec3(x,y,z));
}

NX_INLINE glm::quat
nxQuat(const float deg,
         const glm::vec3& axis)
{
    return glm::angleAxis(glm::radians(deg), axis);
}

NX_INLINE glm::quat
nxQuat(const float degx,
                   const float degy,
                   const float degz)
{
    return glm::quat(glm::vec3(glm::radians(degx),
                               glm::radians(degy),
                               glm::radians(degz)));
}

NX_INLINE glm::quat
nxQuat(const glm::vec3& degs)
{
    return glm::quat(glm::vec3(glm::radians(degs.x),
                               glm::radians(degs.y),
                               glm::radians(degs.z)));
}

NX_INLINE glm::mat4
nxRotate(const glm::quat& quat)
{
    return glm::mat4_cast(quat);
}

NX_INLINE glm::mat4
nxRotate(const float deg,
         const float x,
         const float y,
         const float z)
{
   return nxRotate(nxQuat(deg, x, y, z));
}

NX_INLINE glm::mat4
nxRotate(const float deg,
         const glm::vec3& axis)
{
    return nxRotate(nxQuat(deg, axis));
}

NX_INLINE glm::mat4
nxRotate(const float degx,
                   const float degy,
                   const float degz)
{
    return nxRotate(nxQuat(degx, degy, degz));
}

NX_INLINE glm::mat4
nxRotate(const glm::vec3& degs)
{
    return nxRotate(nxQuat(degs));
}

NX_INLINE glm::mat4
nxTranslate(const glm::vec3& translate)
{
    return glm::translate(glm::mat4(), translate);
}

NX_INLINE glm::mat4
nxTranslate(const float x,
            const float y,
            const float z)
{
    return glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

NX_INLINE glm::mat4
nxScale(const glm::vec3& scale)
{
    return glm::scale(glm::mat4(), scale);
}

NX_INLINE glm::mat4
nxScale(const float x,
        const float y,
        const float z)
{
    return glm::scale(glm::mat4(), glm::vec3(x, y, z));
}

NX_INLINE glm::mat4
nxScale(const float v)
{
    return glm::scale(glm::mat4(), glm::vec3(v));
}



}

#endif
