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
#ifndef __NX_PATH_H__
#define __NX_PATH_H__

namespace nx
{

class NXPath
{
public:

    static const char* sEnvSepStr;

    static const char sEnvSepChr;

    static const char* sPathSepStr;

    static const char sPathSepChr;

    static NXString cwd();

    static NXString realpath(const char* path);

    static bool chdir(const char* dir);

    static bool mkdir(const char* dir);

    static bool rm(const char* path);

    static bool mv(const char* oldPath,
                   const char* newPath);

    static bool exists(const char* path);

    static bool isFile(const char* path);

    static bool isDirectory(const char* path);

    static bool isSymlink(const char* path);

    static bool isRelative(const char* path);

    static bool isAbsolute(const char* path);

    static void list(StringList_t& out,
                     const char* path,
                     const bool recursive);

    static NXString join(const char* p1,
                         const char* p2,
                         const char sep = sPathSepChr);

    static NXString join(const NXString& p1,
                         const NXString& p2,
                         const char sep = sPathSepChr);

    /// return false on failure
    static bool fileSize(size_t &out, const char* path);

    static const char* fileExtension(const char* path);
};

}
#endif
