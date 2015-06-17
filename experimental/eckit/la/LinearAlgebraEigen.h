/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

/// @file   LinearAlgebraEigen.h
/// @author Florian Rathgeber
/// @date   June 2015

#ifndef eckit_la_LinearAlgebraEigen_h
#define eckit_la_LinearAlgebraEigen_h

#include "eckit/eckit_config.h"

#ifdef ECKIT_HAVE_EIGEN

#include "eckit/la/LinearAlgebraBase.h"

namespace eckit {
namespace la {

//-----------------------------------------------------------------------------

class LinearAlgebraEigen : public LinearAlgebraBase {

public:
    LinearAlgebraEigen();

public:  // virtual methods

    virtual Scalar dot(const Vector&, const Vector&) const;
    virtual void gemv(const Matrix&, const Vector&, Vector&) const;
    virtual void gemm(const Matrix&, const Matrix&, Matrix&) const;
    virtual void spmv(const SparseMatrix&, const Vector&, Vector&) const;
    virtual void spmm(const SparseMatrix&, const Matrix&, Matrix&) const;
};

//-----------------------------------------------------------------------------

}  // namespace la
}  // namespace eckit

#endif  // ECKIT_HAVE_EIGEN

#endif
