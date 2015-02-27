/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file Linear.h
/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @author Florian Rathgeber
/// @date November 2013

#ifndef eckit_xpr_Linear_h
#define eckit_xpr_Linear_h

#include "eckit/xpr/Function.h"

namespace eckit {
namespace xpr {

//--------------------------------------------------------------------------------------------

/// Generates a Linear combination of vectors
class Linear : public Function {

public: // methods

    static const char * nodeName() { return "Linear"; }

    Linear( ExpPtr e );

    Linear(Linear&&) = default;

    Linear(Stream& s);

    Linear& operator=(Linear&&) = default;

    struct Register { Register(); };

    virtual const ReanimatorBase& reanimator() const { return reanimator_; }
    static const ClassSpec& classSpec() { return classSpec_; }

private: // methods

    Linear(args_t& a);

    virtual std::string factoryName() const { return "xpr::linear"; }
    virtual const char * typeName() const { return nodeName(); }

    virtual void asCode( std::ostream& ) const;

    virtual ExpPtr cloneWith(args_t& a) const;

protected: // methods

    static ExpPtr compute( Scope& ctx , const args_t& p );

private: // static members

    static  ClassSpec classSpec_;
    static  Reanimator<Linear> reanimator_;
};

//--------------------------------------------------------------------------------------------

} // namespace xpr
} // namespace eckit

#endif