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
#ifndef __NX_3DMODELBUILDER_H__
#define __NX_3DMODELBUILDER_H__

#include "nx3dconverter.h"
#include <flatbuffers/flatbuffers.h>
#include "nx3dmodel_generated.h"

namespace nx
{

class NX3DModelBuilderHelper
{
public:
    NX3DModelBuilderHelper();

    ~NX3DModelBuilderHelper();

   bool build(const NXOutputState& output);

   bool write(const char* output);

private:
    NX_CPP_NO_COPY(NX3DModelBuilderHelper);

private:
    flatbuffers::FlatBufferBuilder _builder;

};

}
#endif
