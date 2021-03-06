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
#ifndef __NX_OGLOBJ_H__
#define __NX_OGLOBJ_H__


#define NX_OGL_HDL_INVALID NX_U32_MAX
#include "nx/util/nxtlsharedptr.h"

namespace nx
{

typedef nx_u32 nxglhdl_t;
class NXOGLObj
{
public:

    NXOGLObj() :
        _oglhdl(NX_OGL_HDL_INVALID)
    {

    }

    virtual ~NXOGLObj()
    {
        NX_ASSERT(_oglhdl == NX_OGL_HDL_INVALID);
    }


    nxglhdl_t oglHdl() const
    {
        return _oglhdl;
    }

    bool oglHdlValid() const
    {
        return _oglhdl != NX_OGL_HDL_INVALID;
    }

protected:

    void oglHdlInvalidate()
    {
        _oglhdl = NX_OGL_HDL_INVALID;
    }

protected:
    nxglhdl_t _oglhdl;
};

typedef NXTLSharedPtr<NXOGLObj> NXOGLObjPtr_t;


}

#endif
