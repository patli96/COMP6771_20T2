#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <algorithm>
#include <concepts/concepts.hpp>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <range/v3/iterator.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility.hpp>
#include <range/v3/view.hpp>
#include <utility>
#include <vector>

namespace gdwg {
	template<concepts::regular N, concepts::regular E>
	requires concepts::totally_ordered<N> //
	   and concepts::totally_ordered<E> //
	   class graph {
	public:
		class iterator;

		struct value_type {
			N from;
			N to;
			E weight;
		};

		// Your member functions go here

		/****************************** CONSTRUCTOR ******************************/
		graph() = default;
		graph(std::initializer_list<N> il)
		: graph(il.begin(), il.end()) {}

		template<ranges::forward_iterator I, ranges::sentinel_for<I> S>
		requires ranges::indirectly_copyable<I, N*> graph(I first, S last) {
			for (auto it = first; it != last; ++it) {
				this->insert_node(*it);
			}
		}

		template<ranges::forward_iterator I, ranges::sentinel_for<I> S>
		requires ranges::indirectly_copyable<I, value_type*> graph(I first, S last) {
			for (auto it = first; it != last; ++it) {
				this->insert_node((*it).from);
				this->insert_node((*it).to);
				this->insert_edge((*it).from, (*it).to, (*it).weight);
			}
		}

		graph(graph&& other) noexcept {
			nodes_ = std::move(other.nodes_);
			edges_ = std::move(other.edges_);
		}

		auto operator=(graph&& other) noexcept -> graph& {
			nodes_ = std::move(other.nodes_);
			edges_ = std::move(other.edges_);
			return *this;
		}

		graph(graph const& other) {
			for (auto iter = other.nodes_.begin(); iter != other.nodes_.end(); ++iter) {
				nodes_.insert(*iter);
			}
			for (auto map_iter = other.edges_.begin(); map_iter != other.edges_.end(); ++map_iter) {
				auto src_ptr = other.nodes_.find(*(map_iter->first));
				edges_.insert({*src_ptr, std::set<dst_weight_pair, compare_edges>{}});
				for (auto set_iter = map_iter->second.begin(); set_iter != map_iter->second.end();
				     ++set_iter) {
					edges_[*src_ptr].insert({set_iter->dst, set_iter->weight});
				}
			}
		}

		auto operator=(graph const& other) -> graph& {
			if (this == &other) {
				return *this;
			}
			for (auto iter = other.nodes_.begin(); iter != other.nodes_.end(); ++iter) {
				nodes_.insert(*iter);
			}
			for (auto map_iter = other.edges_.begin(); map_iter != other.edges_.end(); ++map_iter) {
				auto src_ptr = other.nodes_.find(*(map_iter->first));
				edges_.insert({*src_ptr, std::set<dst_weight_pair, compare_edges>{}});
				for (auto set_iter = map_iter->second.begin(); set_iter != map_iter->second.end();
				     ++set_iter) {
					edges_[*src_ptr].insert({set_iter->dst, set_iter->weight});
				}
			}
			return *this;
		}

		/****************************** MODIFIERS ******************************/
		auto insert_node(N const& value) -> bool {
			auto returned_pair_1 = nodes_.insert(std::make_shared<N>(value));
			if (returned_pair_1.second != true) {
				return false;
			}
			auto node_as_key = nodes_.find(value);
			auto returned_pair_2 =
			   edges_.insert({*node_as_key, std::set<dst_weight_pair, compare_edges>{}});
			return returned_pair_1.second and returned_pair_2.second;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!this->is_node(src) or !this->is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or "
				                         "dst "
				                         "node does not exist");
			}
			auto src_ptr = nodes_.find(src);
			auto dst_ptr = nodes_.find(dst);
			auto returned_pair = edges_[*src_ptr].insert({*dst_ptr, std::make_shared<E>(weight)});
			return returned_pair.second;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!this->is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}
			if (this->is_node(new_data)) {
				return false;
			}
			this->insert_node(new_data);
			auto new_data_ptr = nodes_.find(new_data);
			auto old_data_ptr = nodes_.find(old_data);
			auto old_connections = std::set<dst_weight_pair, compare_edges>{};
			auto map_iter = edges_.begin();
			while (map_iter != edges_.end()) {
				if (*(map_iter->first) == old_data) {
					for (auto set_iter = map_iter->second.begin(); set_iter != map_iter->second.end();
					     ++set_iter) {
						old_connections.insert(*set_iter);
					}
				}
				else {
					auto weight_vector = std::vector<E>{};
					for (auto set_iter = map_iter->second.begin(); set_iter != map_iter->second.end();
					     ++set_iter) {
						if (*(set_iter->dst) == old_data) {
							edges_[map_iter->first].insert({*new_data_ptr, set_iter->weight});
							weight_vector.push_back(*(set_iter->weight));
						}
					}
					for (auto edge_weight : weight_vector) {
						this->erase_edge(*(map_iter->first), old_data, edge_weight);
					}
				}
				++map_iter;
			}
			edges_.erase(*old_data_ptr);
			nodes_.erase(*old_data_ptr);
			for (auto dst_pair : old_connections) {
				edges_[*new_data_ptr].insert(dst_pair);
			}
			return true;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!this->is_node(old_data) or !this->is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new "
				                         "data if they don't exist in the graph");
			}
			// store all the weights of edges between the old and new node
			auto self_connection_weight_vec = this->weights(old_data, new_data);
			auto self_connection_weight_vec_2 = this->weights(new_data, old_data);
			self_connection_weight_vec.insert(self_connection_weight_vec.end(),
			                                  self_connection_weight_vec_2.begin(),
			                                  self_connection_weight_vec_2.end());
			// sotre all the outgoing and incoming edges of the old node
			auto old_outgoing_edges = std::vector<std::pair<N, E>>{};
			auto old_incoming_edges = std::vector<std::pair<N, E>>{};
			for (auto map_iter = edges_.begin(); map_iter != edges_.end(); ++map_iter) {
				if (*(map_iter->first) != old_data and *(map_iter->first) != new_data) {
					auto weight_vector = std::vector<E>{};
					for (auto set_iter = map_iter->second.begin(); set_iter != map_iter->second.end();
					     ++set_iter) {
						if (*(set_iter->dst) == old_data) {
							weight_vector.push_back(*(set_iter->weight));
						}
					}
					for (auto edge_weight : weight_vector) {
						old_incoming_edges.push_back({*(map_iter->first), edge_weight});
					}
				}
				else if (*(map_iter->first) == old_data) {
					for (auto set_iter = map_iter->second.begin(); set_iter != map_iter->second.end();
					     ++set_iter) {
						if (*(set_iter->dst) != new_data) {
							old_outgoing_edges.push_back({*(set_iter->dst), *(set_iter->weight)});
						}
					}
				}
			}
			// erase the old node and replace the new node with the old node
			this->erase_node(old_data);
			// insert back all the outgoing and incoming edges of the old node
			for (auto src_weight_pair : old_incoming_edges) {
				this->insert_edge(std::get<0>(src_weight_pair), new_data, std::get<1>(src_weight_pair));
			}
			for (auto dst_pair : old_outgoing_edges) {
				this->insert_edge(new_data, std::get<0>(dst_pair), std::get<1>(dst_pair));
			}
			// add edges between the old and new node as self connecting edges
			if (!self_connection_weight_vec.empty()) {
				sort(self_connection_weight_vec.begin(), self_connection_weight_vec.end());
				self_connection_weight_vec.erase(
				   unique(self_connection_weight_vec.begin(), self_connection_weight_vec.end()),
				   self_connection_weight_vec.end());
				for (auto self_weight : self_connection_weight_vec) {
					this->insert_edge(new_data, new_data, self_weight);
				}
			}
		}

		auto erase_node(N const& value) -> bool {
			if (!this->is_node(value)) {
				return false;
			}
			auto node_ptr = nodes_.find(value);
			auto edge_iter_vector = std::vector<std::pair<iterator, iterator>>{};
			for (auto map_iter = edges_.begin(); map_iter != edges_.end(); ++map_iter) {
				if (*(map_iter->first) != value) {
					auto weight_vector = std::vector<E>{};
					for (auto set_iter = map_iter->second.begin(); set_iter != map_iter->second.end();
					     ++set_iter) {
						if (*(set_iter->dst) == value) {
							weight_vector.push_back(*(set_iter->weight));
						}
					}
					for (auto edge_weight : weight_vector) {
						this->erase_edge(*(map_iter->first), value, edge_weight);
					}
				}
			}
			edges_.erase(*node_ptr);
			nodes_.erase(*node_ptr);
			return true;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!this->is_node(src) or !this->is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they "
				                         "don't exist in the graph");
			}
			// if (this->find(src, dst, weight) == this->end()) {
			// 	return false;
			// }
			auto src_ptr = nodes_.find(src);
			for (auto set_iter = edges_.at(*src_ptr).begin(); set_iter != edges_.at(*src_ptr).end();
			     ++set_iter) {
				if (*(set_iter->dst) == dst and *(set_iter->weight) == weight) {
					edges_.at(*src_ptr).erase(set_iter);
					return true;
				}
			}
			return false;
		}

		auto erase_edge(iterator i) -> iterator;

		auto erase_edge(iterator i, iterator s) -> iterator;

		auto clear() noexcept -> void {
			nodes_.clear();
			edges_.clear();
		}

		/****************************** ACCESSORS ******************************/
		[[nodiscard]] auto is_node(N const& value) const -> bool {
			return nodes_.find(value) != nodes_.end();
		}

		[[nodiscard]] auto empty() const -> bool {
			return nodes_.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (!this->is_node(src) or !this->is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node "
				                         "don't exist in the graph");
			}
			auto set_iter = edges_.at(*(nodes_.find(src))).begin();
			while (set_iter != edges_.at(*(nodes_.find(src))).end()) {
				if (*(set_iter->dst) == dst) {
					return true;
				}
				set_iter++;
			}
			return false;
		}

		[[nodiscard]] auto nodes() const -> std::vector<N> {
			std::vector<N> nodes_vector;
			std::transform(nodes_.begin(),
			               nodes_.end(),
			               std::back_inserter(nodes_vector),
			               [](std::shared_ptr<N> node_ptr) { return *node_ptr; });
			return nodes_vector;
		}

		[[nodiscard]] auto weights(N const& from, N const& to) const -> std::vector<E> {
			if (!this->is_node(from) or !this->is_node(to)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't "
				                         "exist in the graph");
			}
			if (this->is_connected(from, to)) {
				auto weights_vector = std::vector<E>{};
				auto from_ptr = nodes_.find(from);
				for (auto set_iter = edges_.at(*from_ptr).begin(); set_iter != edges_.at(*from_ptr).end();
				     ++set_iter)
				{
					if (*(set_iter->dst) == to) {
						weights_vector.push_back(*(set_iter->weight));
					}
				}
				return weights_vector;
			}
			return std::vector<E>{};
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			auto iter = this->begin();
			while (iter != this->end()) {
				if (std::get<0>(*(iter)) == src and std::get<1>(*(iter)) == dst
				    and std::get<2>(*(iter)) == weight) {
					return iter;
				}
				++iter;
			}
			return iter;
		}

		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			if (!this->is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist "
				                         "in "
				                         "the graph");
			}
			std::vector<N> nodes_vector;
			auto src_ptr = nodes_.find(src);
			std::transform(edges_.at(*src_ptr).begin(),
			               edges_.at(*src_ptr).end(),
			               std::back_inserter(nodes_vector),
			               [](dst_weight_pair dst_pair) { return *(dst_pair.dst); });
			sort(nodes_vector.begin(), nodes_vector.end());
			nodes_vector.erase(unique(nodes_vector.begin(), nodes_vector.end()), nodes_vector.end());
			return nodes_vector;
		}

		// RANGE ACCESS
		[[nodiscard]] auto begin() const -> iterator {
			if (edges_.empty()) {
				return iterator(edges_.begin(),
				                edges_.end(),
				                edges_.begin(),
				                typename std::set<dst_weight_pair, compare_edges>::iterator{});
			}
			auto outer = edges_.begin();
			while (outer != edges_.end()) {
				if (outer->second.empty()) {
					++outer;
				}
				else {
					break;
				}
			}
			if (outer == edges_.end()) {
				return iterator(edges_.begin(),
				                edges_.end(),
				                outer,
				                typename std::set<dst_weight_pair, compare_edges>::iterator{});
			}
			return iterator(edges_.begin(), edges_.end(), outer, outer->second.begin());
		}

		[[nodiscard]] auto end() const -> iterator {
			return iterator(edges_.begin(), edges_.end(), edges_.end(), {});
		}

		/****************************** COMPARISONS ******************************/
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			if (edges_.size() != other.edges_.size()) {
				return false;
			}
			auto iter_1 = this->begin();
			auto iter_2 = other.begin();
			while (iter_1 != this->end() and iter_2 != other.end()) {
				if (*(iter_1) != *(iter_2)) {
					return false;
				}
				++iter_1;
				++iter_2;
			}
			return static_cast<bool>(iter_1 == this->end() and iter_2 == other.end());
		}

		/****************************** EXTRACTOR ******************************/
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			if (g.empty()) {
				return os;
			}
			for (auto map_it = g.edges_.begin(); map_it != g.edges_.end(); ++map_it) {
				os << *(map_it->first) << " (\n";
				for (auto set_it = map_it->second.begin(); set_it != map_it->second.end(); ++set_it) {
					os << "    " << *(set_it->dst) << " | " << *(set_it->weight) << "\n";
				}
				os << ")\n";
			}
			return os;
		}

	private:
		struct dst_weight_pair {
			std::shared_ptr<N> dst;
			std::shared_ptr<E> weight;

			auto operator<=>(const dst_weight_pair& other) const noexcept -> std::strong_ordering {
				return dst <=> other.dst;
			}
			auto operator<=>(N other_dst) const noexcept -> std::strong_ordering {
				return dst <=> other_dst;
			}
		};

		struct compare_edges {
			using is_transparent = void;

			auto operator()(dst_weight_pair const& edge_1, dst_weight_pair const& edge_2) const noexcept
			   -> bool {
				if (*(edge_1.dst) == *(edge_2.dst)) {
					return *(edge_1.weight) < *(edge_2.weight);
				}
				return *(edge_1.dst) < *(edge_2.dst);
			}

			auto operator()(dst_weight_pair const& edge, N other_dst) const noexcept -> bool {
				if (*(edge.dst) == other_dst) {
					return *(edge.weight) < other_dst;
				}
				return *(edge.dst) < other_dst;
			}

			auto operator()(N other_dst, dst_weight_pair const& edge) const noexcept -> bool {
				if (other_dst == *(edge.dst)) {
					return other_dst < *(edge.weight);
				}
				return other_dst < *(edge.dst);
			}
		};

		struct compare_nodes {
			using is_transparent = void;

			auto operator()(std::shared_ptr<N> const& node_1,
			                std::shared_ptr<N> const& node_2) const noexcept -> bool {
				return *node_1 < *node_2;
			}
			auto operator()(std::shared_ptr<N> const& node_1, N node_2) const noexcept -> bool {
				return *node_1 < node_2;
			}
			auto operator()(N node_1, std::shared_ptr<N> const& node_2) const noexcept -> bool {
				return node_1 < *node_2;
			}
		};

		std::set<std::shared_ptr<N>, compare_nodes> nodes_;
		std::map<std::shared_ptr<N>, std::set<dst_weight_pair, compare_edges>, compare_nodes> edges_;
	};

	template<concepts::regular N, concepts::regular E>
	requires concepts::totally_ordered<N>and concepts::totally_ordered<E> class graph<N, E>::iterator {
		using graph_map =
		   typename std::map<std::shared_ptr<N>, std::set<dst_weight_pair, compare_edges>, compare_nodes>;
		using inner_iterator = typename std::set<dst_weight_pair, compare_edges>::const_iterator;
		using outer_iterator =
		   typename std::map<std::shared_ptr<N>, std::set<dst_weight_pair, compare_edges>, compare_nodes>::const_iterator;

	public:
		using value_type = ranges::common_tuple<N, N, E>;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		// Iterator constructor
		iterator() = default;

		iterator(outer_iterator graph_begin,
		         outer_iterator graph_end,
		         outer_iterator outer,
		         inner_iterator inner) noexcept
		: graph_begin_(graph_begin)
		, graph_end_(graph_end)
		, outer_(outer)
		, inner_(inner) {}

		// Iterator source
		auto operator*() -> ranges::common_tuple<N const&, N const&, E const&> {
			return ranges::common_tuple<N const&, N const&, E const&>{*(outer_->first),
			                                                          *(inner_->dst),
			                                                          *(inner_->weight)};
		}

		// Iterator traversal
		auto operator++() -> iterator& {
			if (inner_ != outer_->second.end()) {
				++inner_;
				if (inner_ != outer_->second.end()) {
					return *this;
				}
			}
			++outer_;
			if (outer_ == graph_end_) {
				inner_ = inner_iterator();
				return *this;
			}
			while (outer_->second.empty()) {
				if (outer_ == graph_end_) {
					inner_ = inner_iterator();
					return *this;
				}
				++outer_;
			}
			inner_ = outer_->second.begin();
			return *this;
		}

		auto operator++(int) -> iterator {
			auto temp = *this;
			++*this;
			return temp;
		}

		auto operator--() -> iterator& {
			if (inner_ == inner_iterator()) {
				--outer_;
				while (outer_->second.empty()) {
					if (outer_ == graph_begin_) {
						inner_ = inner_iterator();
						return *this;
					}
					--outer_;
				}
				if (outer_ == graph_begin_) {
					inner_ = inner_iterator();
					return *this;
				}
				inner_ = outer_->second.end();
				--inner_;
			}
			else {
				if (inner_ == outer_->second.begin()) {
					--outer_;
					inner_ = outer_->second.end();
				}
				--inner_;
			}
			return *this;
		}

		auto operator--(int) -> iterator {
			auto temp = *this;
			--*this;
			return temp;
		}

		// Iterator comparison
		auto operator==(iterator const& other) const -> bool = default;

	private:
		outer_iterator graph_begin_;
		outer_iterator graph_end_;
		outer_iterator outer_;
		inner_iterator inner_;
	};

	template<concepts::regular N, concepts::regular E>
	requires concepts::totally_ordered<N>and concepts::totally_ordered<E> auto
	graph<N, E>::erase_edge(iterator i) -> iterator {
		auto next_iter = this->begin();
		while (*next_iter != *i) {
			++next_iter;
		}
		++next_iter;
		this->erase_edge(std::get<0>(*i), std::get<1>(*i), std::get<2>(*i));
		return next_iter;
	};

	template<concepts::regular N, concepts::regular E>
	requires concepts::totally_ordered<N>and concepts::totally_ordered<E> auto
	graph<N, E>::erase_edge(iterator i, iterator s) -> iterator {
		auto iter = this->begin();
		while (iter != s) {
			++iter;
		}
		auto edges_to_erase = std::vector<ranges::common_tuple<N, N, E>>{};
		while (i != s) {
			edges_to_erase.push_back(*i);
			++i;
		}
		for (auto const& [src, dst, weight] : edges_to_erase) {
			this->erase_edge(src, dst, weight);
		}
		return iter;
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP