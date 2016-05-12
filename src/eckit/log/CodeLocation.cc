/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <sstream>
#include <cstring>

#include "eckit/log/CodeLocation.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

std::string CodeLocation::asString() const {
    std::ostringstream oss;
    print(oss);
    return oss.str();
}

CodeLocation::operator bool() const {
    return file_ != 0;
}

void CodeLocation::print(std::ostream& os) const {
    if ( file_ ) {
        os << " @(" << file_ << " +" << line_;
        if ( strlen(func_) )
            os << " " << func_;
        os << ")";
    }
}

//-----------------------------------------------------------------------------

} // namespace eckit

