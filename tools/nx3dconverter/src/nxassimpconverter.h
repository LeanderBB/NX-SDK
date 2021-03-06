//
// This file is part of the NX Project
//
// Copyright (c) 2015 Leander Beernaert
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
#ifndef __NX_ASSIMPCONVERTER_H__
#define __NX_ASSIMPCONVERTER_H__

#include <assimp/Importer.hpp>
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "nx3dconverter.h"

namespace nx
{
class NXIOBase;
enum ConverterBit
{
    kConverterPackComponentsBit = NX_BIT(0),
    kConverterExcludeBonesBit = NX_BIT(1),
    kConverterExcludeAnimationsBit = NX_BIT(2)
};


class NXAssimpConverter
{
public:

    static bool convert(NXInputStateVec_t& output,
                        const char* file,
                        const nx_u32 flags);


private:

};

}

#endif
