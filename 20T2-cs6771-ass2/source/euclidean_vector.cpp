// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fmt/format.h>
#include <functional>
#include <gsl/gsl-lite.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view.hpp>
#include <utility>

namespace comp6771 {

	/**************************** CONSTRUCTORS ****************************/
	// default constructor
	euclidean_vector::euclidean_vector() {
		cache_ = 0;
		dimensions_ = 1;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(gsl::narrow_cast<std::size_t>(1));
		magnitudes_[0] = 0.0;
	}

	// single-arg constructor
	euclidean_vector::euclidean_vector(int dimensions)
	: euclidean_vector(dimensions, 0.0) {}

	euclidean_vector::euclidean_vector(int dimensions, double magnitudes) {
		cache_ = 0;
		dimensions_ = dimensions;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(gsl::narrow_cast<std::size_t>(dimensions_));
		auto span = std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		ranges::fill(span, magnitudes);
	}

	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator start,
	                                   std::vector<double>::const_iterator end) {
		cache_ = 0;
		dimensions_ = static_cast<int>(std::distance(start, end));
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(gsl::narrow_cast<std::size_t>(dimensions_));
		ranges::copy(start, end, magnitudes_.get());
	}

	euclidean_vector::euclidean_vector(std::initializer_list<double> init_list) {
		cache_ = 0;
		dimensions_ = static_cast<int>(init_list.size());
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(init_list.size());
		ranges::copy(init_list.begin(), init_list.end(), magnitudes_.get());
	}

	// copy constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& ev) {
		cache_ = ev.cache_;
		dimensions_ = ev.dimensions_;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(gsl::narrow_cast<std::size_t>(dimensions_));
		for (int i = 0; i < dimensions_; ++i) {
			auto size_t_i = gsl::narrow_cast<std::size_t>(i);
			magnitudes_[size_t_i] = ev.magnitudes_[size_t_i];
		}
	}

	// move constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& ev) noexcept {
		cache_ = ev.cache_;
		dimensions_ = ev.dimensions_;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		magnitudes_ = std::make_unique<double[]>(gsl::narrow_cast<std::size_t>(dimensions_));
		for (int i = 0; i < dimensions_; ++i) {
			auto size_t_i = gsl::narrow_cast<std::size_t>(i);
			magnitudes_[size_t_i] = ev.magnitudes_[size_t_i];
		}
		std::exchange(ev.dimensions_, 0);
		std::exchange(ev.magnitudes_, nullptr);
	}

	/**************************** OPERATIONS ****************************/
	// copy assignment
	auto euclidean_vector::operator=(euclidean_vector const& rhs) -> euclidean_vector& {
		if (this == &rhs) {
			return *this;
		}
		cache_ = rhs.cache_;
		dimensions_ = rhs.dimensions_;
		magnitudes_ =
		   // NOLINTNEXTLINE(modernize-avoid-c-arrays)
		   std::make_unique<double[]>(gsl::narrow_cast<std::size_t>(dimensions_));
		for (int i = 0; i < dimensions_; ++i) {
			auto size_t_i = gsl::narrow_cast<std::size_t>(i);
			magnitudes_[size_t_i] = rhs.magnitudes_[size_t_i];
		}
		return *this;
	}

	// move assignment
	auto euclidean_vector::operator=(euclidean_vector&& rhs) noexcept -> euclidean_vector& {
		cache_ = rhs.cache_;
		dimensions_ = rhs.dimensions_;
		magnitudes_ =
		   // NOLINTNEXTLINE(modernize-avoid-c-arrays)
		   std::make_unique<double[]>(gsl::narrow_cast<std::size_t>(dimensions_));
		for (int i = 0; i < dimensions_; ++i) {
			auto size_t_i = gsl::narrow_cast<std::size_t>(i);
			magnitudes_[size_t_i] = rhs.magnitudes_[size_t_i];
		}
		rhs.cache_ = 0;
		rhs.dimensions_ = 0;
		rhs.magnitudes_ = nullptr;
		return *this;
	}

	// subscript
	auto euclidean_vector::operator[](int i) -> double& {
		assert((i >= 0) && (i < this->dimensions_));
		cache_ = 0; // reset cacahe_ to 0, since the norm would change.
		return magnitudes_[gsl::narrow_cast<std::size_t>(i)];
	}
	auto euclidean_vector::operator[](int i) const -> double {
		assert((i >= 0) && (i < this->dimensions_));
		return this->magnitudes_[gsl::narrow_cast<std::size_t>(i)];
	}

	// unary plus
	auto euclidean_vector::operator+() const -> euclidean_vector {
		return *this;
	}

	// negation
	auto euclidean_vector::operator-() const -> euclidean_vector {
		auto span = std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		std::transform(span.begin(), span.end(), span.begin(), std::negate<>());
		return *this;
	}

	// compound addition
	auto euclidean_vector::operator+=(euclidean_vector const& rhs) -> euclidean_vector& {
		if (dimensions_ != rhs.dimensions_) {
			throw euclidean_vector_error(
			   fmt::format("Dimensions of LHS{} and RHS{} do not match", dimensions_, rhs.dimensions_));
		}
		cache_ = 0; // reset cacahe_ to 0, since the norm would change.
		auto span_lhs =
		   std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		auto span_rhs =
		   std::span<double>(rhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(rhs.dimensions_));
		std::transform(span_lhs.begin(),
		               span_lhs.end(),
		               span_rhs.begin(),
		               span_lhs.begin(),
		               std::plus<>());
		return *this;
	}

	// compound substraction
	auto euclidean_vector::operator-=(euclidean_vector const& rhs) -> euclidean_vector& {
		if (dimensions_ != rhs.dimensions_) {
			throw euclidean_vector_error(
			   fmt::format("Dimensions of LHS{} and RHS{} do not match", dimensions_, rhs.dimensions_));
		}
		cache_ = 0; // reset cacahe_ to 0, since the norm would change.
		auto span_lhs =
		   std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		auto span_rhs =
		   std::span<double>(rhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(rhs.dimensions_));
		for (int i = 0; gsl::narrow_cast<std::size_t>(i) < span_lhs.size(); ++i) {
			auto size_type_i = gsl::narrow_cast<std::size_t>(i);
			span_lhs[size_type_i] = span_lhs[size_type_i] - span_rhs[size_type_i];
		}
		return *this;
	}

	// compound multiplication
	auto euclidean_vector::operator*=(double num) -> euclidean_vector& {
		cache_ = 0; // reset cacahe_ to 0, since the norm would change.
		auto span = std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		ranges::transform(span.begin(), span.end(), span.begin(), [&num](auto const& c) {
			return c * num;
		});
		return *this;
	}

	// compound division
	auto euclidean_vector::operator/=(double num) -> euclidean_vector& {
		if (num == 0) {
			throw euclidean_vector_error("Invalid vector division by 0");
		}
		cache_ = 0; // reset cacahe_ to 0, since the norm would change.
		auto span = std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		ranges::transform(span.begin(), span.end(), span.begin(), [&num](auto const& c) {
			return c / num;
		});
		return *this;
	}

	// vector type conversion
	euclidean_vector::operator std::vector<double>() const {
		auto span = std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		auto to_vector = std::vector<double>{span.begin(), span.end()};
		return to_vector;
	}

	// list type conversion
	euclidean_vector::operator std::list<double>() const {
		auto span = std::span<double>(magnitudes_.get(), gsl::narrow_cast<std::size_t>(dimensions_));
		auto to_list = std::list<double>(span.begin(), span.end());
		return to_list;
	}

	/**************************** MEMBER FUNCTIONS ****************************/
	// .at()
	[[nodiscard]] auto euclidean_vector::at(int dimension) const -> double {
		if ((dimension < 0) || (dimension >= dimensions_)) {
			throw euclidean_vector_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", dimension));
		}
		return magnitudes_[gsl::narrow_cast<std::size_t>(dimension)];
	}

	[[nodiscard]] auto euclidean_vector::at(int dimension) -> double& {
		if ((dimension < 0) || (dimension >= dimensions_)) {
			throw euclidean_vector_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", dimension));
		}
		cache_ = 0; // reset cacahe_ to 0, since the norm would change.
		return magnitudes_[gsl::narrow_cast<std::size_t>(dimension)];
	}

	// .dimensions()
	[[nodiscard]] auto euclidean_vector::dimensions() const -> int {
		return dimensions_;
	}

	/**************************** UTILITY FUNCTIONS ****************************/
	// unit
	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw euclidean_vector_error("euclidean_vector with no dimensions does not have a unit "
			                             "vector");
		}
		if (euclidean_norm(v) == 0) {
			throw euclidean_vector_error("euclidean_vector with zero euclidean normal does not have a "
			                             "unit vector");
		}
		auto norm = euclidean_norm(v);
		auto unit_v = v;
		for (int i = 0; i < v.dimensions(); ++i) {
			unit_v[i] = v[i] / norm;
		}
		return unit_v;
	}

	// dot
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			throw euclidean_vector_error(
			   fmt::format("Dimensions of LHS{} and RHS{} do not match", x.dimensions(), y.dimensions()));
		}
		auto x_to_vector = static_cast<std::vector<double>>(x);
		auto y_to_vector = static_cast<std::vector<double>>(y);
		return std::inner_product(x_to_vector.begin(), x_to_vector.end(), y_to_vector.begin(), 0.0);
	}

} // namespace comp6771
