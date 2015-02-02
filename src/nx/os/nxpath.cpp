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
#include "nx/os/nxpath.h"


#if defined(NX_OS_UNIX)
#include <sys/time.h>
#include <sys/param.h>
#include <unistd.h>
#else
#include <time.h>
#endif
#include <sys/stat.h>
#if defined(NX_HAVE_DIRENT_H)
#include <dirent.h>
#elif defined(NX_OS_WIN)
#include "win/dirent.h"
#else
#error Could not find dirent.h
#endif
#include <time.h>
#if defined (NX_OS_UNIX)
#include <errno.h>
#endif

namespace nx
{
#if defined(NX_OS_UNIX)
const char* NXPath::sEnvSepStr = ":";
const char NXPath::sEnvSepChr = ':';
const char* NXPath::sPathSepStr = "/";
const char NXPath::sPathSepChr = '/';
#else
const char* NXPath::sEnvSepStr = ":";
const char NXPath::sEnvSepChr = ':';
const char* NXPath::sPathSepStr = "/";
const char NXPath::sPathSepChr = '/';
#endif

NXString
NXPath::cwd()
{
    NXString path;
    path.reserve(MAXPATHLEN);
    char cwd_path [MAXPATHLEN];
    char* ptr = ::getcwd(cwd_path, MAXPATHLEN);

    if (ptr)
    {
        return cwd_path;
    }
    else
    {
        nx::NXLogError("Failed to get cwd");
    }
    return path;
}

NXString
NXPath::realpath(const char* path)
{

    NXString result;
    if (NXPath::isAbsolute(path))
    {
        result = path;
    }
    else
    {
        result.reserve(PATH_MAX);
        char real_path[PATH_MAX];
        char *ptr = ::realpath(path, real_path);
        if (ptr)
        {
            result = real_path;
        }
        else
        {
            NXLogError("Failed to get realpath for path '%s'", path);
        }
    }
    return result;
}

bool
NXPath::chdir(const char* dir)
{
    return ::chdir(dir) == 0;
}


#if defined(NX_OS_UNIX)
#define NX_mkdir(dir) ::mkdir(dir, (S_IWUSR | S_IRUSR | S_IXUSR))
#elif defined (NX_OS_WIN)
#define NX_mkdir(dir) _mkdir(dir)
#else
#error Missing implementation of mkdir
#endif
static bool NXMakeDirRecursive(char *path,
                               char* offset)
{

    char* char_loc = strchr(offset, NXPath::sPathSepChr);
    if (char_loc)
    {
        *char_loc = '\0';
        errno = 0;
        if (NX_mkdir(path) == -1)
        {
            if (errno != EEXIST)
            {
                *char_loc = NXPath::sPathSepChr;
                return false;
            }
        }
        *char_loc= NXPath::sPathSepChr;
        return NXMakeDirRecursive(path, char_loc + 1);
    }
    else
    {
        if (NX_mkdir(path) == -1)
        {
            if (errno!=EEXIST)
            {
                return false;
            }
        }
        return true;
    }
}

#undef NX_mkdir

bool
NXPath::mkdir(const char* dir)
{
    NXString path(dir);
    return NXMakeDirRecursive(&path[0], 0);
}

bool
NXPath::rm(const char* path)
{
    return remove(path) == 0;
}

bool
NXPath::mv(const char* oldPath,
           const char* newPath)
{
    return rename(oldPath, newPath) == 0;
}

bool
NXPath::exists(const char* path)
{
    bool ret = false;
    if (path)
    {
        struct stat file_info;
        ret = (stat(path, &file_info) == 0);
    }
    return ret;
}

bool
NXPath::isFile(const char* path)
{
    bool ret = false;
    if (path)
    {
        struct stat file_info;
        ret = (stat(path, &file_info) == 0);
        if (ret)
        {
            ret = S_ISREG(file_info.st_mode);
        }
    }
    return ret;
}

bool
NXPath::isDirectory(const char* path)
{
    bool ret = false;
    if (path)
    {
        struct stat file_info;
        ret = (stat(path, &file_info) == 0);
        if (ret)
        {
            ret = S_ISDIR(file_info.st_mode);
        }
    }
    return ret;
}

bool
NXPath::isSymlink(const char* path)
{
    bool ret = false;
    if (path)
    {
        struct stat file_info;
        ret = (stat(path, &file_info) == 0);
        if (ret)
        {
            ret = S_ISLNK(file_info.st_mode);
        }
    }
    return ret;
}


bool
NXPath::isRelative(const char* path)
{
    return !NXPath::isAbsolute(path);
}

bool
NXPath::isAbsolute(const char* path)
{
    bool ret = false;
    if (path)
    {
#if defined(NX_OS_UNIX)
        ret = (path[0] == NXPath::sPathSepChr);
#else
        if(strlen(path) >=2)
        {
            ret = path[1] == ':';
        }
#endif
    }
    return ret;
}


void
NXPath::list(StringList_t& out,
             const char *path,
             const bool recursive)
{
    std::queue<NXString> dir_queue;
    NXString real_path;
    DIR* p_cur_dir;
    struct dirent *p_cur_dirent;

    real_path = NXPath::realpath(path);
    if (!real_path.size())
    {
        NXLogError("NXPath::List: Failed to get real path for \"%s\"\n", path);
        return;
    }

    dir_queue.push(real_path);
    while (!dir_queue.empty())
    {
        const NXString& cur_path = dir_queue.front();

        /* open dir */
        p_cur_dir = opendir(cur_path.c_str());

        if (!p_cur_dir)
        {
            NXLogError("NXPath::List: Failed to open dir \"%s\"\n", cur_path.c_str());
            return;
        }

        /* go through directory */
        while ((p_cur_dirent = readdir(p_cur_dir)))
        {
            /* skip . and .. */
            if (strcmp(".",p_cur_dirent->d_name) == 0 ||
                    strcmp("..", p_cur_dirent->d_name) == 0)
            {
                continue;
            }
            /* if it is a directory */

            if (p_cur_dirent->d_type == DT_REG)
            {
                NXString result = NXPath::join(cur_path.c_str(), p_cur_dirent->d_name);
                out.push_front(result);
            }
            else if (p_cur_dirent->d_type == DT_DIR && recursive)
            {
                NXString next_dir = NXPath::join(cur_path.c_str(), p_cur_dirent->d_name);
                out.push_front(next_dir);
                dir_queue.push(next_dir);
            }
        }
        closedir(p_cur_dir);
        dir_queue.pop();
    }
}

NXString
NXPath::join(const char* p1,
             const char* p2,
             const char sep)
{
    size_t p1_len = strlen(p1);
    size_t p2_len = strlen(p2);
    NXString result;
    result.reserve(p1_len + p2_len + 1);
    result = p1;
    if (result[p1_len - 1] != sep)
    {
        result += sep;
    }
    result += p2;
    return result;
}

NXString
NXPath::join(const NXString& p1,
             const NXString& p2,
             const char sep)
{
    size_t p1_len = p1.length();
    size_t p2_len = p2.length();
    NXString result;
    result.reserve(p1_len + p2_len + 1);
    result = p1;
    if (result[p1_len - 1] != sep)
    {
        result += sep;
    }
    result += p2;
    return result;
}

bool
NXPath::fileSize(size_t& out, const char* path)
{
    bool result = false;
    out = 0;
    if (path)
    {
        struct stat file_info;
        if ((stat(path, &file_info) == 0) && S_ISREG(file_info.st_mode))
        {
            out = file_info.st_size;
            result = true;
        }
    }
    return result;
}

const char*
NXPath::fileExtension(const char* path)
{
    const char* res = strrchr(path, '.');
    if (res)
    {
        res = res + 1;
    }
    return res;
}

}
