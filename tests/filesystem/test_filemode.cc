/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>

#include "eckit/filesystem/LocalPathName.h"
#include "eckit/filesystem/PathExpander.h"
#include "eckit/filesystem/PathName.h"
#include "eckit/filesystem/FileMode.h"
#include "eckit/io/DataHandle.h"
#include "eckit/os/AutoUmask.h"
#include "eckit/testing/Test.h"

using namespace std;
using namespace eckit;
using namespace eckit::testing;

namespace eckit {
namespace test {

//----------------------------------------------------------------------------------------------------------------------


CASE("Create a file controling its mode") {
    SECTION("---,---,---") {
        FileMode m(("---,---,---"));
        FileMode m2;
        EXPECT_NO_THROW(m2 = ("---,---,---"));
        EXPECT(m == m2);
        std::cout << m << " = " << m.mode() << std::endl;
        EXPECT(m.str() == std::string(("---,---,---")));
    }
    SECTION("rwx,rwx,rwx") {
        FileMode m("rwx,rwx,rwx");
        FileMode m2;
        EXPECT_NO_THROW(m2 = "rwx,rwx,rwx");
        EXPECT(m == m2);
        std::cout << m << " = " << m.mode() << std::endl;
        EXPECT(m.str() == std::string("rwx,rwx,rwx"));
    }
    SECTION("rwx,---,---") {
        FileMode m("rwx,---,---");
        FileMode m2;
        EXPECT_NO_THROW(m2 = "rwx,---,---");
        EXPECT(m == m2);
        std::cout << m << " = " << m.mode() << std::endl;
        EXPECT(m.str() == std::string("rwx,---,---"));
    }
    SECTION("---,rwx,---") {
        FileMode m("---,rwx,---");
        FileMode m2;
        EXPECT_NO_THROW(m2 = "---,rwx,---");
        EXPECT(m == m2);
        std::cout << m << " = " << m.mode() << std::endl;
        EXPECT(m.str() == std::string("---,rwx,---"));
    }
    SECTION("---,---,rwx") {
        FileMode m("---,---,rwx");
        FileMode m2;
        EXPECT_NO_THROW(m2 = "---,---,rwx");
        EXPECT(m == m2);
        std::cout << m << " = " << m.mode() << std::endl;
        EXPECT(m.str() == std::string("---,---,rwx"));
    }
    SECTION("rw-,r--,r--") {
        FileMode m("rw-,r--,r--");
        FileMode m2;
        EXPECT_NO_THROW(m2 = "rw-,r--,r--");
        EXPECT(m == m2);
        std::cout << m << " = " << m.mode() << std::endl;
        EXPECT(m.str() == std::string("rw-,r--,r--"));
    }
    SECTION("rwx,r-x,r-x") {
        FileMode m("rwx,r-x,r-x");
        FileMode m2;
        EXPECT_NO_THROW(m2 = "rwx,r-x,r-x");
        EXPECT(m == m2);
        std::cout << m << " = " << m.mode() << std::endl;
        EXPECT(m.str() == std::string("rwx,r-x,r-x"));
    }
}

CASE("Handle bad strings") {

    FileMode m;

    SECTION("Bad chars")
    {
        EXPECT_THROWS_AS(m = "Owx,rwx,rwx", BadValue);
        EXPECT_THROWS_AS(m = "rOx,rwx,rwx", BadValue);
        EXPECT_THROWS_AS(m = "rwO,rwx,rwx", BadValue);

        EXPECT_THROWS_AS(m = "rwx,Owx,rwx", BadValue);
        EXPECT_THROWS_AS(m = "rwx,rOx,rwx", BadValue);
        EXPECT_THROWS_AS(m = "rwx,rwO,rwx", BadValue);

        EXPECT_THROWS_AS(m = "rwx,rwx,Owx", BadValue);
        EXPECT_THROWS_AS(m = "rwx,rwx,rOx", BadValue);
        EXPECT_THROWS_AS(m = "rwx,rwx,rwO", BadValue);

        EXPECT_THROWS_AS(m = "w--,---,---", BadValue);
        EXPECT_THROWS_AS(m = "x--,---,---", BadValue);
        EXPECT_THROWS_AS(m = "-r-,---,---", BadValue);
        EXPECT_THROWS_AS(m = "-x-,---,---", BadValue);
        EXPECT_THROWS_AS(m = "--r,---,---", BadValue);
        EXPECT_THROWS_AS(m = "--w,---,---", BadValue);

        EXPECT_THROWS_AS(m = "---,w--,---", BadValue);
        EXPECT_THROWS_AS(m = "---,x--,---", BadValue);
        EXPECT_THROWS_AS(m = "---,-r-,---", BadValue);
        EXPECT_THROWS_AS(m = "---,-x-,---", BadValue);
        EXPECT_THROWS_AS(m = "---,--r,---", BadValue);
        EXPECT_THROWS_AS(m = "---,--w,---", BadValue);

        EXPECT_THROWS_AS(m = "---,---,w--", BadValue);
        EXPECT_THROWS_AS(m = "---,---,x--", BadValue);
        EXPECT_THROWS_AS(m = "---,---,-r-", BadValue);
        EXPECT_THROWS_AS(m = "---,---,-x-", BadValue);
        EXPECT_THROWS_AS(m = "---,---,--r", BadValue);
        EXPECT_THROWS_AS(m = "---,---,--w", BadValue);
    }
}

CASE("Create a file and set its permissions")
{
    AutoUmask mask(0);
    PathName p ("foo.txt");
    std::unique_ptr<DataHandle> dh(p.fileHandle());
    dh->openForAppend(0);
    FileMode m("rw-,r--,r--");
    p.chmod(m);
    dh->close();

    FileMode n(FileMode::fromPath(p.asString()));
}

//----------------------------------------------------------------------------------------------------------------------

}  // namespace test
}  // namespace eckit

int main(int argc, char** argv) {
    return run_tests(argc, argv);
}
