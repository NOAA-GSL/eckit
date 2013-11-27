/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/maths/Value.h"

namespace eckit {
namespace maths {

//--------------------------------------------------------------------------------------------

Value::Value()
{
}

Value::Value(const args_t &args) : Expression(args)
{
}

Value::~Value()
{
}

ExpPtr Value::evaluate( Scope &ctx ) const
{
    return self();
}


//--------------------------------------------------------------------------------------------

} // namespace maths
} // namespace eckit
