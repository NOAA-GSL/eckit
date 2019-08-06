/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <iostream>
#include <vector>

#include "eckit/exception/Exceptions.h"
#include "eckit/filesystem/URI.h"
#include "eckit/filesystem/URIManager.h"
#include "eckit/serialisation/Stream.h"
#include "eckit/utils/Tokenizer.h"


namespace eckit {

//----------------------------------------------------------------------------------------------------------------------

URI::URI() {}

URI::URI(const std::string& uri) {
    if (!uri.empty())
        parse(uri);

    if (scheme_.empty())
        scheme_ = "unix";
}

URI::URI(const URI uri, const std::string &scheme, const std::string &host, const int port):
    scheme_(scheme), user_(uri.user_), host_(host), port_(port), path_(uri.path_), queryValues_(uri.queryValues_), fragment_(uri.fragment_) {}

URI::URI(Stream &s) {
    std::string query;

    s >> scheme_;
    s >> user_;
    s >> host_;
    s >> port_;
    s >> path_;
    s >> query;
    s >> fragment_;

    parseQueryValues(query);
}

URI::~URI() {}

const std::string URI::query(std::string attribute) const {

    auto it = queryValues_.find(attribute);
    if (it != queryValues_.end())
        return it->second;
    return "";
}


void URI::parse(const std::string &uri) {

    std::string aux(uri);

    // get fragment start
    std::size_t fragmentStart = aux.find_last_of("#");
    if (fragmentStart != std::string::npos) {
        fragment_ = aux.substr(fragmentStart + 1);
        aux = aux.substr(0, fragmentStart);
    }

    // get query start
    std::size_t queryStart = aux.find_last_of("?");
    if (queryStart != std::string::npos) {
        std::string query = aux.substr(queryStart + 1);
        aux = aux.substr(0, queryStart);

        parseQueryValues(query);
    }

    std::size_t protocolEnd = aux.find(":");
    if (protocolEnd != std::string::npos) {
        scheme_ = aux.substr(0, protocolEnd);
        aux = aux.substr(protocolEnd + 1);
    }

    parseAuthority(aux);

    std::size_t pathStart = aux.find("/");
    if (pathStart != std::string::npos) {
        path_ = aux.substr(pathStart);
    } else {
        path_ = aux;
    }
}

void URI::parseAuthority(std::string &aux) {

    if (aux.rfind("//", 0) != 0) {
        port_ = -1;
        return;
    }

    aux = aux.substr(2);
    std::size_t userEnd = aux.find_last_of("@");
    if (userEnd != std::string::npos) {
        user_ = aux.substr(0, userEnd);
        aux = aux.substr(userEnd + 1);
    }
    std::size_t portStart = aux.find(":");
    if (portStart != std::string::npos) {
        ASSERT(portStart > 0);
        host_ = aux.substr(0, portStart);
        port_ = 0;
        int i=portStart+1;
        for (;i<aux.size() && std::isdigit(aux[i]);i++) {
            port_ = port_*10 + (aux[i] - '0');
        }
        aux = aux.substr(i);
    } else {
        port_ = -1;
        std::size_t hostEnd = aux.find("/");
        ASSERT(hostEnd != std::string::npos);
        host_ = aux.substr(0, hostEnd);
        aux = aux.substr(hostEnd);
    }
}

void URI::parseQueryValues(const std::string &query) {
    Tokenizer parse("&");
    Tokenizer splitValues("=");
    std::vector<std::string> attributeValuePairs;

    parse(query, attributeValuePairs);
    for (std::string &attributeValue : attributeValuePairs) {
        std::vector<std::string> s;
        splitValues(attributeValue, s);
        if (s.size() == 2) {
            queryValues_[s[0]] = s[1];
        }
    }
}

const void URI::query(std::string attribute, std::string value) {

}

bool URI::exists() const {
    ASSERT(!path_.empty());
    ASSERT(!scheme_.empty());
    return URIManager::lookUp(scheme_).exists(*this);
}

DataHandle* URI::newWriteHandle() const {
    ASSERT(!path_.empty());
    ASSERT(!scheme_.empty());
    return URIManager::lookUp(scheme_).newWriteHandle(*this);
}

DataHandle* URI::newReadHandle(const OffsetList& ol, const LengthList& ll) const {
    ASSERT(!path_.empty());
    ASSERT(!scheme_.empty());
    return URIManager::lookUp(scheme_).newReadHandle(*this, ol, ll);
}

DataHandle* URI::newReadHandle() const {
    ASSERT(!path_.empty());
    ASSERT(!scheme_.empty());
    return URIManager::lookUp(scheme_).newReadHandle(*this);
}

const std::string URI::authority() const {
    std::string authority;
    if (!user_.empty())
        authority = user_ + "@";
    if (!host_.empty())
        authority += host_;
    if (port_ != -1)
        authority += ":" + std::to_string(port_);

    return authority;
}

const std::string URI::query() const {
    std::string query;
    for (auto &attributeValue : queryValues_) {
        if (!query.empty())
            query += "&";

        query += attributeValue.first + "=" + attributeValue.second;
    }
    return query;
}

std::string URI::asString() const {
    ASSERT(!path_.empty());
    ASSERT(!scheme_.empty());
    return URIManager::lookUp(scheme_).asString(*this);
}

std::string URI::asRawString() const {
    std::string auth = authority();
    if (!auth.empty())
        auth = "//"+auth;

    std::string qry = query();
    if (!qry.empty())
        qry = "?"+qry;

    return scheme_ + ":" + auth + path_ + qry + (fragment_.empty() ? "" : "#"+fragment_);
}

void URI::print(std::ostream& s) const {
    s << "URI[scheme=" << scheme_ << ",user=" << user_ << ",host=" << host_ << ",port=" << port_ << ",path=" << path_ << ",query=" << query() << ",fragment=" << fragment_ << "]";
}

void URI::encode(Stream &s) const {
    s << scheme_;
    s << user_;
    s << host_;
    s << port_;
    s << path_;
    s << query();
    s << fragment_;
}

//----------------------------------------------------------------------------------------------------------------------

}  // namespace eckit
