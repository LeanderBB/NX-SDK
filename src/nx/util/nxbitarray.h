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
#ifndef __NX_BITARRAY_H__
#define __NX_BITARRAY_H__

namespace nx
{

template <unsigned int ARRAYSIZE>
class NXBitArray
{
public:

    NXBitArray(bool fillWithOnes = false)
    {
        memset(_bits, (fillWithOnes) ? NX_U32_MAX : 0 ,
               sizeof(_bits));
    }

    NXBitArray(const NXBitArray<ARRAYSIZE>& rOther)
    {
        memcpy(_bits, rOther._bits , sizeof(_bits));
    }

    NXBitArray& operator =(const NXBitArray<ARRAYSIZE>& rOther)
    {
        memcpy(_bits, rOther._bits , sizeof(_bits));
        return *this;
    }

    /// Set bit to one, return previous value
    bool set(unsigned int index)
    {
        int bit_index = index * 0.125;
        int bit_offset = index % 8;
        NX_ASSERT(NX_BIT(bit_offset) <= NX_U8_MAX);
        bool previous = _bits[bit_index] & NX_BIT(bit_offset);
        _bits[bit_index] |= NX_BIT(bit_offset);
        return previous;
    }

    /// Set bit to 0, return previous value
    bool unset(unsigned int index)
    {
        int bit_index = index * 0.125;
        int bit_offset = index % 8;
        NX_ASSERT(NX_BIT(bit_offset) <= NX_U8_MAX);
        bool previous = _bits[bit_index] & NX_BIT(bit_offset);
        _bits[bit_index] &= ~NX_BIT(bit_offset);
        return previous;
    }

    bool isSet(unsigned int index) const
    {
        int bit_index = index * 0.125;
        int bit_offset = index % 8;
        NX_ASSERT(NX_BIT(bit_offset) <= NX_U8_MAX);
        return _bits[bit_index] & NX_BIT(bit_offset);
    }

    unsigned int size() const
    {
        return ARRAYSIZE;
    }

    bool operator == (const NXBitArray<ARRAYSIZE>& rOther) const
    {
        return memcmp(_bits, rOther._bits ,
                      sizeof(_bits)) == 0;
    }

    bool operator != (const NXBitArray<ARRAYSIZE>& rOther) const
    {
        return memcmp(_bits, rOther._bits ,
                      sizeof(_bits)) != 0;
    }
protected:
    NX_STATIC_ASSERT(ARRAYSIZE % 8 == 0, "Size must be divisible by 8")
    nx_u8 _bits[ARRAYSIZE/8];
};

}
#endif
