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
#include "nx/math/nxtransform.h"

namespace nx
{

NXTransform::NXTransform():
    _position(0.0f, 0.0f, 0.0f),
    _scale(1.0f, 1.0f, 1.0f),
    _rotation()
{
}

void
NXTransform::translate(const glm::vec3& translation)
{
    _position += translation;
}

void
NXTransform::rotate(const glm::quat& rotation)
{
    _rotation *= rotation;
}

void
NXTransform::identity()
{
    _position = glm::vec3(0.0f, 0.0f, 0.0f);
    _rotation = glm::quat();
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::mat4
NXTransform::toMatix() const
{
    //glm::mat4 result = glm::mat4_cast(_rotation);
    //result = glm::translate(result, _position);

    glm::mat4 result = glm::mat4_cast(_rotation);
    result[0][0] *= _scale.x;
    result[1][0] *= _scale.y;
    result[2][0] *= _scale.z;
    result[0][1] *= _scale.x;
    result[1][1] *= _scale.y;
    result[2][1] *= _scale.z;
    result[0][2] *= _scale.y;
    result[1][2] *= _scale.y;
    result[2][2] *= _scale.z;
    result[3][0] = _position.x;
    result[3][1] = _position.y;
    result[3][2] = _position.z;
    result[3][3] = 1.0f;

    return result;
}

glm::vec3
NXTransform::up() const
{
    return _rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3
NXTransform::forward() const
{
    return _rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3
NXTransform::right() const
{
    return _rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}


glm::mat4
NXTransform::toMatixInv() const
{
    glm::mat4 rotation_matrix = glm::mat4_cast(_rotation);
    glm::mat4 rotation_inv= glm::inverse(rotation_matrix);
    glm::mat4 trans_inv;
    trans_inv = glm::inverse(glm::translate(trans_inv, _position));
    return rotation_inv * trans_inv;
}

}
