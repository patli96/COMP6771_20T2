#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <vector>

/********************************************************************
 * FRIEND TEST CASE                                                 *
 * This test case is for testing friend functions.                  *
 * Each friend function has its own section.                        *
 ********************************************************************/
TEST_CASE("friend test case") {
	auto const a = comp6771::euclidean_vector{1, 2};
	auto const b = comp6771::euclidean_vector{1, 2};
	auto const c = comp6771::euclidean_vector{1, 3};
	auto const d = comp6771::euclidean_vector{1};

	SECTION("equal") {
		CHECK((a == b) == true);
		CHECK((a == c) == false);
		CHECK((a == d) == false);
	}

	SECTION("not equal") {
		CHECK((a != b) == false);
		CHECK((a != c) == true);
		CHECK((a != d) == true);
	}

	SECTION("addition") {
		auto add_a_b = a + b;
		CHECK(fmt::format("{}", a) == "[1 2]");
		CHECK(fmt::format("{}", b) == "[1 2]");
		CHECK(fmt::format("{}", add_a_b) == "[2 4]");
		CHECK(fmt::format("{}", a + c) == "[2 5]");
		CHECK(fmt::format("{}", a) == "[1 2]");
		CHECK(fmt::format("{}", c) == "[1 3]");
		CHECK_THROWS_WITH(
		   a + d,
		   fmt::format("Dimensions of LHS{} and RHS{} do not match", a.dimensions(), d.dimensions()));
	}

	SECTION("subtraction") {
		auto substract_a_b = a - b;
		CHECK(fmt::format("{}", a) == "[1 2]");
		CHECK(fmt::format("{}", b) == "[1 2]");
		CHECK(fmt::format("{}", substract_a_b) == "[0 0]");
		CHECK(fmt::format("{}", a - c) == "[0 -1]");
		CHECK(fmt::format("{}", a) == "[1 2]");
		CHECK(fmt::format("{}", c) == "[1 3]");
		CHECK_THROWS_WITH(
		   a - d,
		   fmt::format("Dimensions of LHS{} and RHS{} do not match", a.dimensions(), d.dimensions()));
	}

	SECTION("multiply") {
		auto m1 = a * 2;
		auto m2 = 3 * a;
		CHECK(fmt::format("{}", a) == "[1 2]");
		CHECK(fmt::format("{}", m1) == "[2 4]");
		CHECK(fmt::format("{}", m2) == "[3 6]");
	}

	SECTION("divide") {
		auto d1 = a / 5;
		CHECK(fmt::format("{}", a) == "[1 2]");
		CHECK(fmt::format("{}", d1) == "[0.2 0.4]");
		CHECK_THROWS_WITH(a / 0.0, "Invalid vector division by 0");
	}
}