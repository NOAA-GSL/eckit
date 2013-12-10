/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/exception/Exceptions.h"
#include "eckit/compat/StrStream.h"
#include "eckit/parser/Tokenizer.h"
#include "eckit/utils/Translator.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------


static unsigned long long multiplier(const char* p) {
    while(isspace(*p)) p++;

    if(*p && *(p+1))
    {
        if(towlower(*(p+1)) == 'b')
        {
            switch(towlower(*p)) {
                case 'k': return (1LL << 10);  break;
                case 'm': return (1LL << 20);  break;
                case 'g': return (1LL << 30);  break;
                case 't': return (1LL << 40);  break;
                case 'p': return (1LL << 50);  break;
                case 'e': return (1LL << 60);  break;
                //case 'z': return (1LL << 70);  break;
                //case 'y': return (1LL << 80);  break;
            }
        }
    }

    return 1;
}

std::string Translator<bool,std::string>::operator()(bool value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

bool Translator<std::string,bool>::operator()(const std::string& s)
{

    if(s == "no" || s == "off" || s == "false") return false;
    if(s == "yes"|| s == "on"  || s == "true")  return true;

    // Catter for ints
    return atoi(s.c_str());
}

std::string Translator<int,std::string>::operator()(int value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

std::string Translator<unsigned int,std::string>::operator()(unsigned int value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

int Translator<std::string,int>::operator()(const std::string& s)
{

    if(s == "no" || s == "off" || s == "false") return false;
    if(s == "yes"|| s == "on"  || s == "true")  return true;

    // Catter for ints
    char *more;
    int result =  strtol(s.c_str(),&more,10);
    return result * multiplier(more);
}

unsigned int Translator<std::string,unsigned int>::operator()(const std::string& s)
{

    if(s == "no" || s == "off" || s == "false") return false;
    if(s == "yes"|| s == "on"  || s == "true")  return true;

    // Catter for ints
    char *more;
    unsigned int result =  strtoul(s.c_str(),&more,10);
    return result * multiplier(more);
}

std::string Translator<long,std::string>::operator()(long value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

long Translator<std::string,long>::operator()(const std::string& s)
{
    char *more;
    long result =  strtol(s.c_str(),&more,10);
    return result * multiplier(more);
}

std::string Translator<double,std::string>::operator()(double value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

double Translator<std::string,double>::operator()(const std::string& s)
{
    return atof(s.c_str());
}

unsigned long Translator<std::string,unsigned long>::operator()(const std::string& s)
{
    char *more;
    unsigned long result =  strtoul(s.c_str(),&more,10);
    return result * multiplier(more);
}

std::string Translator<unsigned long,std::string>::operator()(unsigned long value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

    unsigned long long Translator<std::string,unsigned long long>::operator()
(const std::string& s)
{
    char *more;
    unsigned long long result =  strtoull(s.c_str(),&more,10);
    return result * multiplier(more);
}

std::string Translator<unsigned long long,std::string>::operator()(unsigned long long value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

long long Translator<std::string,long long>::operator()(const std::string& s)
{
    char *more;
    long long result =  strtoll(s.c_str(),&more,10);
    return result * multiplier(more);
}

std::string Translator<long long,std::string>::operator()(long long value)
{
    StrStream s;
    s << value << StrStream::ends;
    return std::string(s);
}

std::vector<std::string> Translator<std::string, std::vector<std::string> >::operator()(const std::string& s)
{
    std::vector<std::string> result;
    Tokenizer parse(", \t");

    parse(s,result);
    return result;
}

std::vector<long> Translator<std::string, std::vector<long> >::operator()(const std::string& s)
{
    std::vector<std::string> r;
    Tokenizer parse(", \t");

    parse(s,r);

    std::vector<long> result;
    for(size_t i = 0; i < r.size(); i++)
        result.push_back(Translator<std::string,long>()(r[i]));
    return result;
}

std::string Translator<std::vector<std::string>, std::string>::operator()(const std::vector<std::string>& v)
{
    std::string result;
    for(int i=0; i < v.size(); ++i)
    {
        if(i) result += ",";

        result += v[i];
    }
    return result;
}

std::set<std::string> Translator<std::string, std::set<std::string> >::operator()(const std::string& s)
{
    std::vector<std::string> v;
    Tokenizer parse(", \t");

    parse(s,v);

    std::set<std::string> result(v.begin(),v.end());
    return result;
}

std::string Translator<std::set<std::string>, std::string>::operator()(const std::set<std::string>& v)
{
    std::string result;
    for(std::set<std::string>::const_iterator i= v.begin(); i != v.end(); ++i)
    {
        if(result.length()) result += ",";
        result += *i;
    }
    return result;
}


char Translator<std::string,char>::operator()(const std::string& s)
{
    ASSERT(s.length() == 1);
    return s[0];
}

std::string Translator<char,std::string>::operator()(char c)
{
    std::string s;
    s = c;
    return s;
}

//-----------------------------------------------------------------------------

} // namespace eckit
