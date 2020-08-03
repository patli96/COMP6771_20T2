#include <iostream>
#include <string>

#include "gdwg/graph.hpp"

auto main() -> int {
	// auto g = gdwg::graph<std::string, int>{};
	// g.insert_node("hello");
	// g.insert_node("how");
	// g.insert_node("are");
	// g.insert_node("you?");

	// g.insert_edge("hello", "how", 5);
	// g.insert_edge("hello", "are", 8);
	// g.insert_edge("hello", "are", 2);

	// g.insert_edge("how", "you?", 1);
	// g.insert_edge("how", "hello", 4);

	// g.insert_edge("are", "you?", 3);

	// std::cout << g << "\n";

	// auto g2 = gdwg::graph<std::string, int>(g);

	// std::cout << g2 << "\n";

	// This is a structured binding.
	// https://en.cppreference.com/w/cpp/language/structured_binding
	// It allows you to unpack your tuple.
	// for (auto const& [from, to, weight] : g) {
	// 	std::cout << from << " -> " << to << " (weight " << weight << ")\n";
	// }

	using graph = gdwg::graph<int, int>;
	auto const l = std::vector<graph::value_type>{{4, 1, -4},
	                                              {3, 2, 2},
	                                              {2, 4, 2},
	                                              {2, 1, 1},
	                                              {6, 2, 5},
	                                              {6, 3, 10},
	                                              {1, 5, -1},
	                                              {3, 6, -8},
	                                              {4, 5, 3},
	                                              {5, 2, 7},
	                                              {5, 10, 1}};
	auto g = graph(l.begin(), l.end());
	// auto it1 = g.begin();
	// std::cout << std::get<0>(*it1) << " -> " << std::get<1>(*it1) << " (weight " <<
	// std::get<2>(*it1)
	//           << " )\n";
	// ++it1;
	// std::cout << std::get<0>(*it1) << " -> " << std::get<1>(*it1) << " (weight " <<
	// std::get<2>(*it1)
	//           << " )\n";
	for (auto it = g.begin(); it != g.end(); ++it) {
		std::cout << std::get<0>(*it) << " -> " << std::get<1>(*it) << " (weight " << std::get<2>(*it)
		          << " )\n";
	}
}
