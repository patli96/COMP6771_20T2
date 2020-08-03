#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iterator>
#include <vector>

/********************************************************************
 * CONSTRUCTORS TEST CASE                                           *
 * This test case is for testing constructors.                      *
 * Each kind of constructor has its own section.                    *
 ********************************************************************/
TEST_CASE("CONSTRUCTORS TEST CASES") {
	auto v = std::vector<int>{1, 2, 3, 4};

	SECTION("graph(std::initializer_list<N> il)") {
		auto g = gdwg::graph<int, int>({1, 2, 3, 4});
		CHECK(g.nodes() == v);
	}

	SECTION("N graph(I first, S last)") {
		auto g = gdwg::graph<int, int>(v.begin(), v.end());
		CHECK(g.nodes() == v);
	}

	SECTION("value_type graph(I first, S last)") {
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
		CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4, 5, 6});
		CHECK(g.connections(2) == std::vector<int>{1, 4});
	}

	SECTION("move constructor - graph(graph&& other) noexcept") {
		auto g = gdwg::graph<int, int>(v.begin(), v.end());
		g.insert_edge(1, 2, 111);
		g.insert_edge(1, 4, 222);
		auto g2 = gdwg::graph(std::move(g));
		CHECK(g2.nodes() == std::vector<int>{1, 2, 3, 4});
	}

	SECTION("move assignment - operator=(graph&& other) noexcept") {
		auto g = gdwg::graph<int, int>(v.begin(), v.end());
		g.insert_edge(1, 2, 111);
		g.insert_edge(1, 4, 222);
		auto g2 = std::move(g);
		CHECK(g2.nodes() == std::vector<int>{1, 2, 3, 4});
	}

	SECTION("copy constructor - graph(graph const& other)") {
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
		auto g2 = gdwg::graph(g);
		CHECK(g2 == g);
		g.insert_node(7);
		CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4, 5, 6, 7});
		CHECK(g2 != g);
	}

	SECTION("copy asssignment - operator=(graph const& other)") {
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
		auto g2 = g;
		CHECK(g2 == g);
		g.insert_node(7);
		CHECK(g.nodes() == std::vector<int>{1, 2, 3, 4, 5, 6, 7});
		CHECK(g2 != g);
	}
}