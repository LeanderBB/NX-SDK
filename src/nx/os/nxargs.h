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
#ifndef __NX_ARGS_H__
#define __NX_ARGS_H__

namespace nx
{

enum ArgFlag
{
    kArgFlagRequired = NX_BIT(0),
    kArgFlagSingleValue = NX_BIT(1),
    kArgFlagMultiValue = NX_BIT(2)
};



class NXArgs
{
public:
    NXArgs();

    bool add(const nx_u32 id,
             const char shortOpt,
             const char* longOpt,
             const char* help,
             const nx_u32 flags);


    /// @return Number of options pared. On error, it will return -1.
    int parse(const int argc,
               const char** argv);

    bool isSet(const nx_u32 id) const;

    /// Return nullptr if not found, otheriwise returns the value for this
    /// option and the number of values present in size.
    const char** get(const nx_u32 id,
                     int& size) const;

    void clear();

    void printHelp(const char* description = nullptr) const;

protected:

    class Option
    {
    public:
        Option(const nx_u32 id,
               const char shrtOpt,
               const char* lngOpt,
               const char* hlp,
               const nx_u32 flgs):
            name(id),
            longopt(lngOpt),
            help(hlp),
            value(nullptr),
            shortopt(shrtOpt),
            flags(flgs),
            size(-1)
        {
        }

        nx_u32 name;
        const char* longopt;
        const char* help;
        const char** value;
        const char shortopt;
        const nx_u32 flags;
        int size;

        bool operator < (const Option& other)
        {
            return shortopt < other.shortopt;
        }
    };

    const Option* findOption(const nx_u32 id) const;

    Option* findOptionByShort(const char c);

    Option* findOptionByLong(const char* str);

    bool checkDuplicate(const nx_u32 id,
                        const char shortop,
                        const char* longop) const;

protected:
    typedef std::vector<Option> OptionVec_t;

    int _nParsedOptions;
    int _nRequiredOptions;
    nx_u32 _maxOptionsLen;
    OptionVec_t _options;

};

}
#endif
