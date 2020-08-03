#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

/********************************************************************
 * ITERATOR TEST CASE                                               *
 ********************************************************************/
TEST_CASE("ITERATOR TEST CASES") {
	using graph = gdwg::graph<int, int>;
	auto const l = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};
	auto g = graph(l.begin(), l.end());

	SECTION("constructor") {
		auto iter = gdwg::graph<int, int>::iterator();
	}

	SECTION("source") {
		auto iter = g.begin();
		CHECK(*iter == ranges::common_tuple<int, int, int>(1, 5, -1));
	}

	SECTION("traversal") {
		for (auto const& [from, to, weight] : g) {
			std::cout << from << " -> " << to << " (weight " << weight << ")\n";
		}
		std::cout << "\n";
	}

	SECTION("comparison") {
		auto g2 = gdwg::graph<int, int>();
		CHECK(g2.begin() == g2.end());
	}
}