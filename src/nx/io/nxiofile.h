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

#ifndef __NX_IOFILE_H__
#define __NX_IOFILE_H__

namespace nx
{
class NXIOBase;

class NXIOFile : public NXIOBase
{
public:

    static NXIOFile* open(const char* path,
                          const nx_u32 flags);

    ~NXIOFile();

    size_t read(void* buffer,
                const size_t size);

    size_t write(const void* buffer,
                 const size_t size);

    bool seek(const size_t offset,
              const IOSeekOp seekOp);

    size_t tell() const;

    void close();

protected:
    NXIOFile(FILE* file,
             const size_t size,
             const nx_u32 state);

protected:
    FILE* _file;
};

}
#endif
