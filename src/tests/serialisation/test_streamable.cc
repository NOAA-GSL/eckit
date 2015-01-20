/*
 * (C) Copyright 1996-2014 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. In applying
 * this licence, ECMWF does not waive the privileges and immunities granted to it by virtue
 * of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
 */

/// @file test_streamable.cc
/// @date Jan 2015
/// @author Florian Rathgeber

#include <climits>
#include <cfloat>

#define BOOST_TEST_MODULE TestStreamable
#include "ecbuild/boost_test_framework.h"

#include "eckit/filesystem/PathName.h"
#include "eckit/serialisation/FileStream.h"
#include "eckit/serialisation/Streamable.h"

using namespace std;
using namespace eckit;

namespace eckit {
namespace test {

template <typename T>
class TestItem : public Streamable {

public:
	TestItem(const T& s) : payload_(s) {}

	TestItem(Stream& s) : Streamable(s), payload_() {
		s >> payload_;
	}

	// From Streamble
	virtual void encode(eckit::Stream& s) const {
		Streamable::encode(s);
		s << payload_;
	}
	virtual const eckit::ReanimatorBase& reanimator() const { return reanimator_; }

	// Class members
	static const eckit::ClassSpec& classSpec() { return classSpec_; }

	T payload_;

protected:

	virtual void print(std::ostream& s) const {
		s << "TestItem " << payload_;
	}

private:
	// -- Class members
	static eckit::ClassSpec            classSpec_;
	static eckit::Reanimator<TestItem> reanimator_;

	// -- Friends
	friend std::ostream& operator<<(std::ostream& s,const TestItem& p) {
		p.print(s); return s;
	}
};

typedef unsigned char uchar;
typedef long long llong;
typedef unsigned long long ullong;

template <typename T>
Reanimator< TestItem<T> > TestItem<T>::reanimator_;

/// Partially specialise ClassSpecs since they must have unique names
template <>
ClassSpec TestItem<char>::classSpec_ = {&Streamable::classSpec(),"TestItemChar",};
template <>
ClassSpec TestItem<uchar>::classSpec_ = {&Streamable::classSpec(),"TestItemUChar",};
template <>
ClassSpec TestItem<bool>::classSpec_ = {&Streamable::classSpec(),"TestItemBool",};
template <>
ClassSpec TestItem<int>::classSpec_ = {&Streamable::classSpec(),"TestItemInt",};
template <>
ClassSpec TestItem<uint>::classSpec_ = {&Streamable::classSpec(),"TestItemUInt",};
template <>
ClassSpec TestItem<short>::classSpec_ = {&Streamable::classSpec(),"TestItemShort",};
template <>
ClassSpec TestItem<ushort>::classSpec_ = {&Streamable::classSpec(),"TestItemUShort",};
template <>
ClassSpec TestItem<long>::classSpec_ = {&Streamable::classSpec(),"TestItemLong",};
template <>
ClassSpec TestItem<ulong>::classSpec_ = {&Streamable::classSpec(),"TestItemULong",};
template <>
ClassSpec TestItem<llong>::classSpec_ = {&Streamable::classSpec(),"TestItemLLong",};
template <>
ClassSpec TestItem<ullong>::classSpec_ = {&Streamable::classSpec(),"TestItemULLong",};
// NOTE: float is not implemented!
// template <>
// ClassSpec TestItem<float>::classSpec_ = {&Streamable::classSpec(),"TestItemFloat",};
template <>
ClassSpec TestItem<double>::classSpec_ = {&Streamable::classSpec(),"TestItemDouble",};
template <>
ClassSpec TestItem<string>::classSpec_ = {&Streamable::classSpec(),"TestItemString",};

#define test_decode(TYPE, INITIAL, SUFFIX) \
BOOST_AUTO_TEST_CASE( test_decode_##TYPE##_##SUFFIX ) \
{ \
	BOOST_TEST_MESSAGE("Manually (de)serialise Streamable with " #TYPE " member"); \
	PathName filename = PathName::unique( "data" ); \
	std::string filepath = filename.asString(); \
	TestItem<TYPE> t(INITIAL); \
	{ \
		FileStream sout( filepath.c_str(), "w" ); \
		t.encode(sout); \
	} \
	{ \
		FileStream sin( filepath.c_str(), "r" ); \
		TestItem<TYPE> t2(sin); \
		BOOST_TEST_MESSAGE("original: " << t.payload_); \
		BOOST_TEST_MESSAGE("streamed: " << t2.payload_); \
		BOOST_CHECK(t.payload_ == t2.payload_); \
	} \
	if (filename.exists()) filename.unlink(); \
}

#define test_reanimate(TYPE, INITIAL, SUFFIX) \
BOOST_AUTO_TEST_CASE( test_reanimate_##TYPE##_##SUFFIX ) \
{ \
	BOOST_TEST_MESSAGE("(de)serialise Streamable with " #TYPE " member via Reanimator"); \
	PathName filename = PathName::unique( "data" ); \
	std::string filepath = filename.asString(); \
	TestItem<TYPE> t(INITIAL); \
	{ \
		FileStream sout( filepath.c_str(), "w" ); \
		sout << t; \
	} \
	{ \
		FileStream sin( filepath.c_str(), "r" ); \
		TestItem<TYPE>* t2 = eckit::Reanimator< TestItem<TYPE> >::reanimate(sin); \
		BOOST_TEST_MESSAGE("orginal: " << t.payload_); \
		BOOST_TEST_MESSAGE("streamed: " << t2->payload_); \
		BOOST_CHECK(t.payload_ == t2->payload_); \
	} \
	if (filename.exists()) filename.unlink(); \
}

BOOST_AUTO_TEST_SUITE( TestStreamable )

test_decode(char, SCHAR_MAX, max)
test_decode(char, SCHAR_MIN, min)
test_reanimate(char, SCHAR_MAX, max)
test_reanimate(char, SCHAR_MIN, min)

test_decode(uchar, UCHAR_MAX, max)
test_reanimate(uchar, UCHAR_MAX, max)

test_decode(bool, true, true)
test_reanimate(bool, true, true)

test_decode(int, INT32_MAX, max)
test_decode(int, INT32_MIN, min)
test_reanimate(int, INT32_MAX, max)
test_reanimate(int, INT32_MIN, min)

test_decode(uint, UINT32_MAX, max)
test_reanimate(uint, UINT32_MAX, max)

test_decode(short, SHRT_MAX, max)
test_decode(short, SHRT_MIN, min)
test_reanimate(short, SHRT_MAX, max)
test_reanimate(short, SHRT_MIN, min)

test_decode(ushort, USHRT_MAX, max)
test_reanimate(ushort, USHRT_MAX, max)

// NOTE: long in eckit is always 32 bit!

test_decode(long, INT32_MAX, max)
test_decode(long, INT32_MIN, min)
test_reanimate(long, INT32_MAX, max)
test_reanimate(long, INT32_MIN, min)

test_decode(ulong, UINT32_MAX, max)
test_reanimate(ulong, UINT32_MAX, max)

test_decode(llong, LLONG_MAX, max)
test_decode(llong, LLONG_MIN, min)
test_reanimate(llong, LLONG_MAX, max)
test_reanimate(llong, LLONG_MIN, min)

test_decode(ullong, ULLONG_MAX, max)
test_reanimate(ullong, ULLONG_MAX, max)

// NOTE: float is not implemented!
// test_decode(float, FLT_MAX, max)
// test_decode(float, FLT_MIN, min)
// test_reanimate(float, FLT_MAX, max)
// test_reanimate(float, FLT_MIN, min)

test_decode(double, DBL_MAX, max)
test_decode(double, DBL_MIN, min)
test_reanimate(double, DBL_MAX, max)
test_reanimate(double, DBL_MIN, min)

test_decode(string, "Hello, World!", _)
test_reanimate(string, "Hello, World!", _)

BOOST_AUTO_TEST_SUITE_END()

} // namespace test
} // namespace hermes
