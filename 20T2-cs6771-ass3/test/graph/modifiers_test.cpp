#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <string>
#include <vector>

/********************************************************************
 * MODIFIERS TEST CASE                                              *
 * This test case is for testing modifiers.                         *
 * Each kind of modifier has its own section.                       *
 ********************************************************************/
TEST_CASE("MODIFIERS TEST CASES") {
	auto g = gdwg::graph<std::string, int>{};

	SECTION("inset_node") {
		CHECK(g.insert_node("how"));
		auto a = std::string("are");
		CHECK(g.insert_node(a));
		auto b = std::string("are");
		CHECK(g.insert_node(b) == false);
	}

	SECTION("insert_edge") {
		g.insert_node("Apple");
		g.insert_node("Banana");
		CHECK(g.insert_edge("Apple", "Banana", 3));
		CHECK(g.insert_edge("Apple", "Banana", 5));
		CHECK(g.insert_edge("Apple", "Banana", 3) == false);
		CHECK(g.find("Apple", "Banana", 5) != g.end());
		CHECK(g.find("Apple", "Banana", 3) != g.end());
		CHECK_THROWS_WITH(g.insert_edge("Apple", "Peach", 5),
		                  std::string("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
		                              "dst node does not exist"));
		CHECK_THROWS_WITH(g.insert_edge("Mango", "Peach", 5),
		                  std::string("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
		                              "dst node does not exist"));
	}

	SECTION("replace_node") {
		g.insert_node("Apple");
		g.insert_node("Banana");
		g.insert_edge("Apple", "Banana", 3);
		g.insert_edge("Banana", "Apple", 40);
		g.replace_node("Banana", "Spaceship");
		CHECK(g.nodes() == std::vector<std::string>{"Apple", "Spaceship"});
		CHECK(g.find("Apple", "Spaceship", 3) != g.end());
		CHECK(g.replace_node("Apple", "Spaceship") == false);
		CHECK_THROWS_WITH(g.replace_node("Hoem", "Moon"),
		                  std::string("Cannot call gdwg::graph<N, E>::replace_node on a node that "
		                              "doesn't exist"));
	}

	SECTION("merge_replace_node") {
		g.insert_node("A");
		g.insert_node("B");
		g.insert_node("C");
		g.insert_node("D");
		g.insert_edge("A", "B", 3);
		g.insert_edge("C", "B", 2);
		g.insert_edge("D", "B", 4);
		auto g2 = g;
		auto g3 = g;
		g.merge_replace_node("B", "A");
		using graph = gdwg::graph<std::string, int>;
		auto const l1 = std::vector<graph::value_type>{{"A", "A", 3}, {"C", "A", 2}, {"D", "A", 4}};
		auto g1_expected = graph(l1.begin(), l1.end());
		CHECK(g == g1_expected);
		g2.insert_edge("C", "A", 1);
		g2.insert_edge("C", "A", 2);
		g2.insert_edge("A", "C", 5);
		g2.merge_replace_node("B", "A");
		auto const l2 = std::vector<graph::value_type>{{"A", "A", 3},
		                                               {"A", "C", 5},
		                                               {"C", "A", 1},
		                                               {"C", "A", 2},
		                                               {"D", "A", 4}};
		auto g2_expected = graph(l2.begin(), l2.end());
		CHECK(g2 == g2_expected);
		CHECK_THROWS_WITH(g2.merge_replace_node("R", "S"),
		                  std::string("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
		                              "new "
		                              "data if they don't exist in the graph"));
	}

	SECTION("erase_node") {
		g.insert_node("Apple");
		g.insert_node("Banana");
		g.insert_edge("Apple", "Banana", 3);
		g.insert_edge("Banana", "Apple", 40);
		g.erase_node("Banana");
		auto g_expected = gdwg::graph<std::string, int>{};
		g_expected.insert_node("Apple");
		CHECK(g == g_expected);
		CHECK(g.erase_node("Banana") == false);
		g.erase_node("Apple");
		CHECK(g.empty());
	}

	SECTION("erase_edge(N, N, E)") {
		g.insert_node("Apple");
		g.insert_node("Banana");
		g.insert_edge("Apple", "Banana", 3);
		g.insert_edge("Banana", "Apple", 40);
		CHECK(g.erase_edge("Apple", "Banana", 3));
		CHECK(g.find("Banana", "Apple", 40) != g.end());
		CHECK(g.find("Apple", "Banana", 3) == g.end());
		CHECK(g.erase_edge("Apple", "Banana", 3) == false);
		CHECK_THROWS_WITH(g.erase_edge("Apple", "Peach", 3),
		                  std::string("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
		                              "they "
		                              "don't exist in the graph"));
	}

	SECTION("erase_edge(I)") {
		using graph = gdwg::graph<int, int>;
		auto v = std::vector<graph::value_type>{{1, 2, 3}, {1, 3, 4}, {1, 4, 20}, {2, 5, 50}};
		auto g2 = graph(v.begin(), v.end());
		auto it = g2.begin();
		auto it2 = g2.erase_edge(it);
		CHECK(*it2 == ranges::common_tuple<int, int, int>{1, 3, 4});
	}

	SECTION("erase_edge(I, S)") {
		using graph = gdwg::graph<int, int>;
		auto v = std::vector<graph::value_type>{{1, 2, 3}, {1, 3, 4}, {1, 4, 20}, {2, 5, 50}};
		auto g2 = graph(v.begin(), v.end());
		auto it1 = g2.begin();
		auto it2 = g2.begin();
		++it1;
		++it2;
		++it2;
		auto it3 = g2.erase_edge(it1, it2);
		CHECK(*it3 == ranges::common_tuple<int, int, int>{1, 4, 20});
	}

	SECTION("clear") {
		using graph = gdwg::graph<int, int>;
		auto v = std::vector<graph::value_type>{{1, 2, 3}, {1, 3, 4}, {1, 4, 20}, {2, 5, 50}};
		auto g2 = graph(v.begin(), v.end());
		g2.clear();
		CHECK(g2.nodes().empty());
	}
}