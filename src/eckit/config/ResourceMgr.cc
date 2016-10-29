/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/runtime/Main.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/filesystem/LocalPathName.h"
#include "eckit/log/Log.h"
#include "eckit/thread/Mutex.h"
#include "eckit/config/ResourceMgr.h"

#include <map>

namespace eckit {

static Mutex mutex;

// this should be a member of ResourceMgr
// it will be when I have tamed the xlC
// template instanciation mechanism

typedef std::map<ResourceQualifier, std::string> ResMap;

static ResMap resmap;
bool ResourceMgr::inited_ = false;

void ResourceMgr::reset()
{
    AutoLock<Mutex> lock(mutex);
    resmap.clear();
    inited_ = false;
}

// This has to be redone

static const char* skip_spaces(const char* p) {
    while (*p && isspace(*p)) p++;
    return p;
}

bool ResourceMgr::parse(const char* p)
{
    p = skip_spaces(p);

    if (*p == 0 || *p == '#') return true; // skip comments

    std::string s[3];
    int n = 0;

    for (;;)
    {
        const char *q = p;

        p = skip_spaces(p);
        while (*p && *p != ':' && *p != '.' && !isspace(*p) ) p++;

        int len = p - q;
        p = skip_spaces(p);

        s[n] = q; s[n].resize(len); n++;

        if (n == 3 || *p != '.') break;
        p++;
    }
    if (*p != ':')
        return false;
    else
    {
        switch (n)
        {
        case 1: s[2] = s[0]; s[0] = ""; break;
        case 2: s[2] = s[1]; s[1] = s[0]; s[0] = ""; break;
        case 3: break;
        }

        p++;
        p = skip_spaces(p);

        // Remove trailing blanks
        int l = strlen(p) - 1;
        while (l >= 0 && isspace(p[l])) l--;


        ResourceQualifier x(s[0], s[1], s[2]);

        std::string t = std::string(p, l + 1);
        resmap[x] = t;
    }
    return true;
}

void ResourceMgr::readConfigFile(const LocalPathName& file)
{
    // Read file ...

    //Log::info() << "ResourceMgr::readConfigFile(" << file << ")" << std::endl;

    int    cnt  = 0;

    std::ifstream in(file.localPath());
    char line[1024];

    while (in.getline(line, sizeof(line)))
    {
        cnt++;
        if (!parse(line))
        {
            Log::warning() << "Invalid line, file " << file
                           << " line " << cnt << std::endl;
            Log::warning() << line << std::endl;
        }
    }
}

void ResourceMgr::set(const std::string& name, const std::string& value)
{
    AutoLock<Mutex> lock(mutex);
    std::string s = name + ": " + value;
    if (!parse(s.c_str()))
        Log::warning() << "Failed to parse " << s << std::endl;
}

bool ResourceMgr::lookUp(const std::string& kind, const std::string& owner,
                         const std::string& name, std::string& result)
{

    AutoLock<Mutex> lock(mutex);

    if (!inited_)
    {
        inited_ = true;
        readConfigFile("~/etc/config/general");
        readConfigFile("~/etc/config/local");
        readConfigFile(std::string("~/etc/config/")
                       + Main::instance().name());
        readConfigFile(std::string("~/etc/config/")
                       + Main::instance().name() + ".local");
    }

    ResMap::iterator i = resmap.find(ResourceQualifier(kind, owner, name));

    if (i != resmap.end())
    {
        result = (*i).second;
        return true;
    }

    i = resmap.find(ResourceQualifier("", owner, name));

    if (i != resmap.end())
    {
        result = (*i).second;
        return true;
    }

    i = resmap.find(ResourceQualifier("", "", name));

    if (i != resmap.end())
    {
        result = (*i).second;
        return true;
    }

    return false;

}

//=======================================================================

ResourceQualifier::ResourceQualifier(const std::string& kind,
                                     const std::string& owner, const std::string& name):
    kind_(kind),
    owner_(owner),
    name_(name)
{
}

ResourceQualifier::ResourceQualifier(const ResourceQualifier& other)
{
    kind_  = other.kind_;
    owner_ = other.owner_;
    name_  = other.name_;
}

int ResourceQualifier::operator<(const ResourceQualifier& other) const
{
    // to be rewritten
    char buf1[1024];
    char buf2[1024];

    sprintf(buf1, "%s.%s.%s", owner_.c_str(), kind_.c_str(), name_.c_str());
    sprintf(buf2, "%s.%s.%s", other.owner_.c_str(), other.kind_.c_str(),
            other.name_.c_str());

    return strcmp(buf1, buf2) < 0;
}

}


