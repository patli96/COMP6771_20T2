#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <vector>

/********************************************************************
 * CONSTRUCTOR TEST CASE                                            *
 * This test case is for testing constructors.                      *
 * Each kind of constructor has its own section.                    *
 ********************************************************************/
TEST_CASE("constructor test case") {
	SECTION("default constructor") {
		auto a = comp6771::euclidean_vector();
		CHECK(fmt::format("{}", a) == "[0]");
		CHECK(a.dimensions() == 1);
	}

	SECTION("int-double constructor") {
		auto a = comp6771::euclidean_vector(2, 4.0);
		auto x = int{3};
		auto y = double{3.24};
		auto b = comp6771::euclidean_vector(x, y);
		CHECK(fmt::format("{}", a) == "[4 4]");
		CHECK(fmt::format("{}", b) == "[3.24 3.24 3.24]");
	}

	SECTION("single-argument constructor") {
		auto const a = comp6771::euclidean_vector(1);
		int i{3};
		auto const b = comp6771::euclidean_vector(i);
		CHECK(fmt::format("{}", a) == "[0]");
		CHECK(fmt::format("{}", b) == "[0 0 0]");
	}

	SECTION("iterator constructor") {
		std::vector<double> v = {4.5, 6.23, 6.123, 56.2};
		auto b = comp6771::euclidean_vector(v.begin(), v.end());
		CHECK(fmt::format("{}", b) == "[4.5 6.23 6.123 56.2]");
	}

	SECTION("initialiser list constructor") {
		auto b = comp6771::euclidean_vector{1.0, 2.0, 3.0};
		CHECK(fmt::format("{}", b) == "[1 2 3]");
	}

	SECTION("copy constructor") {
		auto a = comp6771::euclidean_vector{1.0, 2.0, 3.0};
		auto b = comp6771::euclidean_vector(a);
		CHECK(fmt::format("{}", a) == "[1 2 3]");
		CHECK(fmt::format("{}", b) == "[1 2 3]");
	}

	SECTION("move constructor") {
		auto a = comp6771::euclidean_vector{1.0, 2.0, 3.0};
		auto b = comp6771::euclidean_vector(std::move(a));
		CHECK(fmt::format("{}", b) == "[1 2 3]");
	}
}