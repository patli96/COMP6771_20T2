#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iterator>
#include <vector>

/********************************************************************
 * ACCESSORS TEST CASE                                              *
 * This test case is for testing accessors.                         *
 * Each kind of accessor has its own section.                       *
 ********************************************************************/
TEST_CASE("ACCESSORS TEST CASES") {
	auto g = gdwg::graph<int, std::string>{};

	SECTION("is_node") {
		auto n = 5;
		g.insert_node(n);
		g.insert_node(7);
		CHECK(g.is_node(n));
		CHECK(!g.is_node(8));
	}

	SECTION("empty") {
		CHECK(g.empty());
	}

	SECTION("is_connected") {
		g.insert_node(8);
		g.insert_node(9);
		g.insert_edge(8, 9, "abc");
		CHECK(g.is_connected(8, 9));
		CHECK(!g.is_connected(9, 8));
	}

	SECTION("nodes") {
		g.insert_node(8);
		g.insert_node(9);
		CHECK(g.nodes() == std::vector<int>{8, 9});
	}

	SECTION("weights") {
		using graph = gdwg::graph<int, int>;
		auto const l = std::vector<graph::value_type>{
		   {4, 1, -4},
		   {3, 2, 2},
		   {2, 4, 2},
		   {2, 1, 1},
		   {2, 1, 10},
		   {2, 1, 9},
		   {6, 2, 5},
		   {6, 3, 10},
		   {1, 5, -1},
		   {3, 6, -8},
		   {4, 5, 3},
		   {5, 2, 7},
		};
		auto g1 = graph(l.begin(), l.end());
		CHECK(g1.weights(2, 1) == std::vector<int>{1, 9, 10});
		CHECK(g1.weights(3, 3).empty());
	}

	SECTION("find") {
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
		auto g1 = graph(l.begin(), l.end());
		CHECK(g1.find(6, 3, 10) != g1.end());
		auto it = g1.find(3, 6, -8);
		CHECK(*it == ranges::common_tuple<int, int, int>{3, 6, -8});
		CHECK(g1.find(1, 1, 1) == g1.end());
	}

	SECTION("connections") {
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
		auto g1 = graph(l.begin(), l.end());
		auto v = g1.connections(2);
		CHECK(v == std::vector<int>{1, 4});
	}
}