/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file   Setup.h
/// @author Simon Smart
/// @date   July 2017

#ifndef eckit_testing_Test_h
#define eckit_testing_Test_h

#include "eckit/eckit_config.h"
#include "eckit/log/Log.h"
#include "eckit/runtime/Main.h"

#ifdef ECKIT_HAVE_CXX11

#if __cplusplus <= 199711L
#error "eckit is configured with C++11 support, but the current compiler doesn't support C++11"
#endif

#define lest_FEATURE_AUTO_REGISTER 1
#include "eckit/testing/lest.h"

#else
#include "eckit/testing/lest_cpp03.h"
#endif

// This pragma disables warnings in qtcreator using Clang Code Model. These warnings are due to the
// expression decomposing functionality in lest, which intentionally does weird things by considering
// the operator precedence rules.
# pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace eckit {
namespace testing {


//----------------------------------------------------------------------------------------------------------------------


static lest::tests& specification() {
    static lest::tests spec;
    return spec;
}


int run_tests(int argc, char* argv[]) {

    eckit::Main::initialise(argc, argv);

    return lest::run(specification(), argc, argv, eckit::Log::info());
}


// The CASE macro will now work with both C++11 and C++03 versions of lest

#undef CASE
#define CASE(proposition) lest_CASE( specification(), proposition )


//----------------------------------------------------------------------------------------------------------------------

} // namespace testing
} // namespace eckit

#endif // eckit_testing_Test_h