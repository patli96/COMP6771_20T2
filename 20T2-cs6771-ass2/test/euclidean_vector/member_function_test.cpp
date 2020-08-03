#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <vector>

/********************************************************************
 * MEMBER FUNCTION TEST CASE                                        *
 * This test case is for testing member functions.                  *
 * Each kind of member function has its own section.                *
 ********************************************************************/
TEST_CASE("member function test case") {
	auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};

	SECTION(".at()") {
		int const num = 1;
		auto b = a.at(num);
		CHECK(b == 1.0);
		CHECK(a.at(0) == 0.0);
		CHECK_THROWS_WITH(
		   a.at(3),
		   fmt::format("Index {} is not valid for this euclidean_vector object", a.dimensions()));
	}

	SECTION(".dimensions()") {
		auto b = a.dimensions();
		CHECK(b == 3);
	}
}