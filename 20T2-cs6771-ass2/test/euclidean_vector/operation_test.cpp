#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <vector>

/************************************************************************
 * OPERATION TEST CASE                                                  *
 * This test case is for testing operations.                            *
 * All compound operations are tested in "compound operations" section. *
 * The rest operations are tested in their own sections.                *
 ************************************************************************/
TEST_CASE("operation test case") {
	SECTION("copy assignment") {
		auto a = comp6771::euclidean_vector(5, 8.8);
		auto b = comp6771::euclidean_vector{1.2, 3.4};
		b = a;
		CHECK(fmt::format("{}", a) == "[8.8 8.8 8.8 8.8 8.8]");
		CHECK(fmt::format("{}", b) == "[8.8 8.8 8.8 8.8 8.8]");
	}

	SECTION("move assignment") {
		auto b = comp6771::euclidean_vector{1.2, 3.4};
		auto a = std::move(b);
		CHECK(fmt::format("{}", a) == "[1.2 3.4]");
	}

	SECTION("subscript") {
		std::vector<double> v = {4.5, 6.23, 6.123, 56.2};
		auto b = comp6771::euclidean_vector(v.begin(), v.end());
		double a{b[1]};
		b[2] = 99.99;
		CHECK(fmt::format("{}", a) == "6.23");
		CHECK(fmt::format("{}", b) == "[4.5 6.23 99.99 56.2]");
	}

	SECTION("unary plus") {
		auto a = comp6771::euclidean_vector(4, 1.5);
		CHECK(fmt::format("{}", +a) == "[1.5 1.5 1.5 1.5]");
	}

	SECTION("negation") {
		auto const a = comp6771::euclidean_vector{-6, 1};
		auto const b = comp6771::euclidean_vector{6, -1};
		CHECK(fmt::format("{}", -a) == "[6 -1]");
		CHECK(fmt::format("{}", -b) == "[-6 1]");
	}

	SECTION("compound operations") {
		auto a = comp6771::euclidean_vector{1, 2, 3, 4};
		auto b = comp6771::euclidean_vector{4, 3, 2, 1};
		auto c = comp6771::euclidean_vector{1, 1, 1};

		SECTION("compound addtion") {
			a += b;
			CHECK_THROWS_WITH(
			   a += c,
			   fmt::format("Dimensions of LHS{} and RHS{} do not match", a.dimensions(), c.dimensions()));
			CHECK(fmt::format("{}", a) == "[5 5 5 5]");
			CHECK(fmt::format("{}", b) == "[4 3 2 1]");
		}

		SECTION("compound subtraction") {
			a -= b;
			CHECK_THROWS_WITH(
			   a -= c,
			   fmt::format("Dimensions of LHS{} and RHS{} do not match", a.dimensions(), c.dimensions()));
			CHECK(fmt::format("{}", a) == "[-3 -1 1 3]");
			CHECK(fmt::format("{}", b) == "[4 3 2 1]");
		}

		SECTION("compound multiplication") {
			double num = 1.2;
			a *= num;
			CHECK(fmt::format("{}", a) == "[1.2 2.4 3.6 4.8]");
			CHECK(fmt::format("{}", a *= double{-0.1}) == "[-0.12 -0.24 -0.36 -0.48]");
		}

		SECTION("compound division") {
			a /= 2;
			CHECK(fmt::format("{}", a) == "[0.5 1 1.5 2]");
			CHECK_THROWS_WITH(a /= 0, "Invalid vector division by 0");
		}
	}

	SECTION("vector type conversion") {
		auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};
		auto const vf = static_cast<std::vector<double>>(a);
		CHECK(vf == std::vector<double>{0.0, 1.0, 2.0});
	}

	SECTION("list type conversion") {
		auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};
		auto const lf = static_cast<std::list<double>>(a);
		CHECK(lf == std::list<double>{0.0, 1.0, 2.0});
	}
}