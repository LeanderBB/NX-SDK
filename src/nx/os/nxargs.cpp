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
#include "nx/os/nxargs.h"
#include <iostream>

namespace nx
{

NXArgs::NXArgs():
    _nParsedOptions(-1),
    _nRequiredOptions(0),
    _maxOptionsLen(0)
{
    clear();
}

bool
NXArgs::add(const nx_u32 id,
            const char shortOpt,
            const char* longOpt,
            const char* help,
            const nx_u32 flags)
{
    if(!checkDuplicate(id, shortOpt, longOpt))
    {
        _maxOptionsLen = std::max(_maxOptionsLen, (nx_u32) strlen(longOpt));
        Option opt(id, shortOpt, longOpt, help, flags);
        _options.push_back(opt);

        if (flags & kArgFlagRequired)
        {
            _nRequiredOptions++;
        }
        return true;
    }
    return false;
}


int
NXArgs::parse(const int argc,
              const char** argv)
{
    Option* p_cur_option = nullptr;
    int n_options = 0;
    int n_options_req = 0;
    for (int i = 1; i < argc;)
    {
        size_t len = strlen(argv[i]);

        if (!len)
        {
            std::cerr << "Invalid option at index " << i << "."<< std::endl;
            return -1;
        }



        if (p_cur_option)
        {
            // handle active object

            // check if it is the next argument
            if (argv[i][0] == '-')
            {
                // check if a value is already assigned

                if (p_cur_option->size <= 0 && (p_cur_option->flags & (kArgFlagSingleValue | kArgFlagMultiValue)))
                {
                    std::cerr << "Option (-"<< p_cur_option->shortopt
                              << ", --" << p_cur_option->longopt
                              << ") at requires at least one value. " << std::endl;
                    return -1;
                }
                p_cur_option = nullptr;
                continue;
            }

            // set value ptr , if not set before
            if (!p_cur_option->value)
            {
                p_cur_option->value = &argv[i];
                p_cur_option->size = 1;
            }
            else
            {
                if ((p_cur_option->flags & kArgFlagSingleValue) && p_cur_option->size == 1)
                {
                    std::cerr << "Option (-"<< p_cur_option->shortopt
                              << ", --" << p_cur_option->longopt
                              << ") only accepts one value." << std::endl;
                    return -1;
                }
                p_cur_option->size++;
            }
            ++i;
        }
        else
        {
            if (len == 1 || argv[i][0] != '-')
            {
                std::cerr << "Invalid option '" << argv[i] << "'."<< std::endl;
                return -1;
            }

            // check for short arg
            p_cur_option = findOptionByShort(argv[i][1]);

            // try to find long option
            if (!p_cur_option && len >2)
            {
                p_cur_option = findOptionByLong(argv[i] + 2);
            }

            if (!p_cur_option)
            {
                std::cerr << "Unknown option '" << argv[i] << "'."<< std::endl;
                return -1;
            }

            // mark found
            p_cur_option->size = 0;

            if (i + 1 >= argc && (p_cur_option->flags & (kArgFlagSingleValue | kArgFlagMultiValue)))
            {
                std::cerr << "Option (-"<< p_cur_option->shortopt
                          << ", --" << p_cur_option->longopt
                          << ") at requires at least one value. " << std::endl;
                return -1;
            }

            if (p_cur_option->flags & kArgFlagRequired)
            {
                n_options_req++;
            }

            if (!(p_cur_option->flags & (kArgFlagSingleValue | kArgFlagMultiValue)))
            {
                p_cur_option = nullptr;
            }
            ++i;
            n_options++;


        }
    }

    _nParsedOptions = n_options;
    if (n_options_req == _nRequiredOptions)
    {
        return n_options;
    }
    else
    {
        std::cerr << "Not all required options have been specified" << std::endl;
        return -1;
    }
}

bool
NXArgs::isSet(const nx_u32 id) const
{
    const Option* opt = findOption(id);
    return (opt) ? opt->size >= 0 : false;
}

const char **
NXArgs::get(const nx_u32 id,
            int &size) const
{
    const Option* opt = findOption(id);
    if(opt)
    {
        size = opt->size;
        return opt->value;
    }
    else
    {
        size = -1;
        return nullptr;
    }
}

void
NXArgs::clear()
{
    _options.clear();
    _nParsedOptions = -1;
    _nRequiredOptions = 0;
    _maxOptionsLen = 0;
    add(0, 'h', "help", "Prints this message", 0);

}

void
NXArgs::printHelp(const char *description) const
{
    if (description)
    {
        std::cout << std::endl;
        std::cout << description << std::endl;
        std::cout << std::endl;
    }

    std::cout << "Options: " << std::endl;
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        nx_u32 len = strlen(it->longopt);

        std::cout << "    -" << it->shortopt << ", --" << it->longopt;

        while(len < _maxOptionsLen)
        {
            std::cout << " ";
            ++len;
        }
        if (it->flags & kArgFlagRequired)
        {
            std::cout << " : [REQUIRED] ";
        }
        else
        {
            std::cout << " : [OPTIONAL] ";
        }
        std::cout << it->help << std::endl;
    }
    std::cout << std::endl;
}

const NXArgs::Option*
NXArgs::findOption(const nx_u32 id) const
{
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if (id == it->name)
        {
            return &(*it);
        }
    }
    return nullptr;
}

NXArgs::Option*
NXArgs::findOptionByShort(const char c)
{
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if (it->shortopt == c)
        {
            return &(*it);
        }
    }
    return nullptr;
}

NXArgs::Option*
NXArgs::findOptionByLong(const char* str)
{
    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if (strcmp(it->longopt, str) == 0)
        {
            return &(*it);
        }
    }
    return nullptr;
}

bool
NXArgs::checkDuplicate(const nx_u32 id,
                       const char shortop,
                       const char* longop) const
{

    for(auto it = _options.begin(); it != _options.end(); ++it)
    {
        if(it->name == id)
        {
            return true;
        }
        if (it->shortopt == shortop)
        {
            return true;
        }
        if (strcmp(it->longopt, longop) == 0)
        {
            return true;
        }
    }
    return false;
}

}
