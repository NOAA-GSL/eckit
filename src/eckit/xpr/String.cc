/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/parser/JSON.h"

#include "eckit/xpr/String.h"
#include "eckit/xpr/Scope.h"

namespace eckit {
namespace xpr {

//--------------------------------------------------------------------------------------------

String::String( const std::string& v ) : v_(v)
{
}

String::String(ExpPtr e) : v_(0)
{
   v_ = String::extract(e->eval(false) );
}

String::String(Stream &s) : Value(s), v_(0)
{
    s >> v_;
}

void String::encode(Stream &s) const
{
    Value::encode(s);
    s << v_;
}

bool String::is(const ExpPtr &e)
{
    return dynamic_cast<String*>(e.get()) != 0;
}

void String::print(std::ostream&o) const
{
    o << nodeName() << "(\"" << v_ << "\")";
}

void String::asCode(std::ostream&o) const
{
    o << factoryName() << "(\"" << v_ << "\")";
}

void String::asJSON(JSON& s) const
{
    s << v_;
}

ExpPtr String::cloneWith(args_t& a) const {
    NOTIMP; // Should not be called
}

//--------------------------------------------------------------------------------------------

ClassSpec String::classSpec_ = {
    &Value::classSpec(),
    String::nodeName(),
};

Reanimator< String > String::reanimator_;

//--------------------------------------------------------------------------------------------

ExpPtr string(const std::string &s)
{
    return ExpPtr( new String(s) );
}

//--------------------------------------------------------------------------------------------

} // namespace xpr
} // namespace eckit