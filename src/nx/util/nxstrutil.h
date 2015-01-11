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
#ifndef __NX_STRUTIL_H__
#define __NX_STRUTIL_H__

namespace nx
{

int NXStrCmpNoCase(const char* str1,
                   const char* str2);

bool NXStrIsAlpha(const char* str);

bool NXStrIsNumeric(const char* str);

bool NXStrIsUpper(const char* str);

bool NXStrIsLower(const char* str);

bool NXStrToBool(const char* str,
                 bool& out);

bool NXStrToU32(const char* str,
                nx_u32& out);

bool NXStrToI32(const char* str,
                nx_i32& out);

bool NXStrToU64(const char* str,
                nx_u64& out);

bool NXStrToI64(const char* str,
                nx_i64& out);

bool NXStrToFloat(const char* str,
                  float& out);

bool NXStrToDouble(const char* str,
                   double& out);

NXString NXStrWithFormat(const char* format,
                         ...);

void NXStrReplace(NXString& str,
                  const char src,
                  const char dst);
}

#endif // NXSTRUTIL_H
