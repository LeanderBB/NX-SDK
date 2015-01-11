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

    void identity();

    void scale(const glm::vec3& scale);

    glm::mat4 toMatix() const;

    glm::mat4 toMatixInv() const;

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
}

#endif
