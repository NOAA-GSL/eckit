/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/mpi/Request.h"

#include "eckit/mpi/Comm.h"

namespace eckit {
namespace mpi {

//----------------------------------------------------------------------------------------------------------------------

Request::Request() {
    *this = eckit::mpi::comm().request();
}

Request::Request(RequestContent* p) :
    content_(p) {
    content_->attach();
}

Request::~Request() {
   content_->detach();
}

Request::Request(const Request& s) : content_(s.content_) {
    content_->attach();
}

Request& Request::operator=(const Request& s) {
    content_ = s.content_;
    content_->attach();
    return *this;
}

RequestContent::~RequestContent() {
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace mpi
} // namepsace eckit