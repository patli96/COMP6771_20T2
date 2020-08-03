#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iterator>
#include <string>
#include <vector>

/********************************************************************
 * COMPARISONS TEST CASE                                            *
 * This test case is for testing comparisons.                       *
 ********************************************************************/
TEST_CASE("COMPARISONS TEST CASES") {
	auto g1 = gdwg::graph<int, std::string>{};
	g1.insert_node(1);
	g1.insert_node(2);
	g1.insert_node(3);
	g1.insert_edge(1, 2, "hello");
	g1.insert_edge(1, 3, "hi");
	using graph = gdwg::graph<int, std::string>;
	auto const l = std::vector<graph::value_type>{{1, 2, "hello"}, {1, 3, "hi"}};
	auto g2 = graph(l.begin(), l.end());
	CHECK(g1 == g2);
}