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
#include "nx/io/nxfilemanager.h"
#include "nx/io/nxiobase.h"
#include "nx/io/nxiomemory.h"
#include "nx/os/nxpath.h"

static const char g_text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam euismod tempor sapien sit amet posuere. Donec porttitor nisl metus, in rutrum lacus interdum ac. Ut pharetra condimentum commodo. Pellentesque vel diam viverra, sagittis diam in, hendrerit dolor. Donec porttitor ante nec mauris hendrerit, sit amet egestas odio vestibulum. Curabitur malesuada lectus et ultrices lobortis. Duis faucibus, metus pulvinar tempus hendrerit, lacus risus malesuada justo, sit amet pellentesque sem ipsum at quam. Ut luctus pharetra pharetra.";
static const size_t sizeof_text_1 = sizeof(g_text) + 1;

#define TEST(a) if(!(a)) { nx::NXLogError ("Exp " #a " failed at %d", __LINE__);}

class CTestIO
{
public:
    CTestIO()
    {
    }
    ~CTestIO(){}

    bool setup()
    {
        if (!_fs.init())
        {
            return false;
        }
        nx::NXString cwd = nx::NXPath::cwd();
        return _fs.mount(cwd.c_str(), "TEST", false);
    }

    void testWriteReadOSFile()
    {
        nx::NXIOBase* p_io = _fs.open("$TEST$/tmpfile.txt", nx::kIOAccessModeWriteBit);
        TEST(p_io);
        if (!p_io) return;
        size_t sizeof_text = strlen(g_text);
        size_t bytes_written = p_io->write(g_text, sizeof_text);
        TEST(bytes_written == sizeof_text);
        TEST(!p_io->isError());
        delete p_io;
        char readbuffer[sizeof_text_1];
        p_io = _fs.open("$TEST$/tmpfile.txt", nx::kIOAccessModeReadBit | nx::kFileManagerOpenHintMemoryFileBit);
        TEST(p_io);
        size_t bytes_read = p_io->read(readbuffer,sizeof_text);
        TEST(bytes_read == sizeof_text);
        TEST(!p_io->isError());
        //TEST(!p_io->isEOF());
        readbuffer[bytes_read] = '\0';
        TEST(nx::NXStrCmpNoCase(g_text, readbuffer) == 0);
        delete p_io;
    }

    void testFSList()
    {
        nx::StringList_t list;
        _fs.list(list, "", false);
        for(auto it = list.begin(); it != list.end(); ++it)
        {
            nx::NXLog("Entry: %s", it->c_str());
        }
    }

    void testMemReadWrite()
    {
        size_t sizeof_text = strlen(g_text);
        char readbuffer[sizeof_text_1];
        nx::NXIOMemory* p_io(new nx::NXIOMemory(g_text, sizeof_text, nx::kIOAccessModeWriteBit | nx::kIOAccessModeReadBit));
        TEST(p_io);
        size_t bytes_read = p_io->read(readbuffer,sizeof_text);
        TEST(bytes_read == sizeof_text);
        TEST(!p_io->isError());
        TEST(p_io->isEOF());
        readbuffer[bytes_read] = '\0';
        TEST(nx::NXStrCmpNoCase(g_text, readbuffer) == 0);
        TEST(p_io->write("ABC", 2) == 0);
        TEST(p_io->buffer());
        delete p_io;
        p_io = new nx::NXIOMemory(sizeof_text + 1,  nx::kIOAccessModeWriteBit | nx::kIOAccessModeReadBit);
        TEST(p_io->write(g_text, sizeof_text) == sizeof_text);
        TEST(!p_io->isError());
        TEST(!p_io->isEOF());
        TEST(p_io->write("", 1) == 1);
        TEST(!p_io->isError());
        TEST(p_io->isEOF());
        const void* mem_buffer = p_io->buffer();
        TEST(mem_buffer);
        TEST(nx::NXStrCmpNoCase(g_text, static_cast<const char*>(mem_buffer)) == 0);
        delete p_io;
    }

    void destroy()
    {
        bool found = _fs.exists("$TEST$/tmpfile.txt");
        TEST(found);
        if (found)
        {
            TEST(_fs.remove("$TEST$/tmpfile.txt"));
        }

        _fs.shutdown();
    }

    virtual void run()
    {
        testWriteReadOSFile();
        testFSList();
        testMemReadWrite();
    }

public:
    nx::NXFileManager _fs;
};


int main()
{
    CTestIO t;
    if (!t.setup())
    {
        return EXIT_FAILURE;
    }
    t.run();
    t.destroy();
    return EXIT_SUCCESS;
}
