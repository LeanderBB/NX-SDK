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
#ifndef __NX_3DMODELPOSTPROCESS_H__
#define __NX_3DMODELPOSTPROCESS_H__

#include "nx3dconverter.h"


namespace nx
{

enum PostProcessFlags
{
    kPostProcessFlagInterleaved = NX_BIT(0)
};

class NX3DModelPostProcess
{
public:

    NX3DModelPostProcess(const NXInputStateVec_t& input);

    ~NX3DModelPostProcess();

    bool buildOutputData(const nx_u32 flags = 0);

    const NXOutputState& output() const
    {
        return _output;
    }
private:
    NX_CPP_NO_COPY(NX3DModelPostProcess);

    bool validSeparateBuffers() const;

    bool validInterleavedBuffers() const;

    bool buildSeparate();

private:
    NXInputStateVec_t _input;
    NXOutputState _output;
};

}

#endif