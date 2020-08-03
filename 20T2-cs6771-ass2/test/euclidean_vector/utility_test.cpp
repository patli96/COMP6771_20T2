#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <vector>

/********************************************************************
 * UTILITY FUNCTION TEST CASE                                       *
 * This test case is for testing utility functions.                 *
 * Each utility function has its own section.                       *
 * Besides basic norm calculation, the norm test case also test     *
 * the implementation of cache as well.                             *
 ********************************************************************/
TEST_CASE("utility function test case") {
	auto const a = comp6771::euclidean_vector(4, 2.0);
	auto b = comp6771::euclidean_vector(0, 0);
	auto c = comp6771::euclidean_vector(4, 0);
	auto d = comp6771::euclidean_vector(4, 2.0);

	SECTION("norm") {
		auto norm_a = euclidean_norm(a);
		CHECK(norm_a == 4);
		CHECK_THROWS_WITH(euclidean_norm(b),
		                  "euclidean_vector with no dimensions does not have a norm");
		CHECK(euclidean_norm(d * 2) == 8);
		auto norm_d = euclidean_norm(d);
		CHECK(norm_d == 4);
	}

	SECTION("unit") {
		CHECK(fmt::format("{}", comp6771::unit(a)) == "[0.5 0.5 0.5 0.5]");
		CHECK_THROWS_WITH(comp6771::unit(b),
		                  "euclidean_vector with no dimensions does not have a unit vector");
		CHECK_THROWS_WITH(comp6771::unit(c),
		                  "euclidean_vector with zero euclidean normal does not have a unit vector");
	}

	SECTION("dot") {
		auto const v1 = comp6771::euclidean_vector{1, 2};
		auto const v2 = comp6771::euclidean_vector{3, 4};
		CHECK(comp6771::dot(v1, v2) == 11);
		CHECK_THROWS_WITH(
		   comp6771::dot(a, v1),
		   fmt::format("Dimensions of LHS{} and RHS{} do not match", a.dimensions(), v1.dimensions()));
	}
}