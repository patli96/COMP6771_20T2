#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

#include <compare>
#include <fmt/format.h>
#include <functional>
#include <gsl/gsl-lite.hpp>
#include <list>
#include <memory>
#include <ostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/view.hpp>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what) noexcept
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		/*********************************************************
		 * All key parts are implemented as the specs required,  *
		 * except the utility function - norm is put in friends  *
		 * to implement the "cache" as required.                 *
		 *********************************************************/
		// CONSTRUCTORS
		euclidean_vector();
		explicit euclidean_vector(int);
		euclidean_vector(int, double);
		euclidean_vector(std::vector<double>::const_iterator, std::vector<double>::const_iterator);
		euclidean_vector(std::initializer_list<double>);
		euclidean_vector(euclidean_vector const&);
		euclidean_vector(euclidean_vector&&) noexcept;

		// DESTRUCTOR
		~euclidean_vector() noexcept = default;

		// OPERATIONS
		auto operator=(euclidean_vector const&) -> euclidean_vector&;
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;
		auto operator[](int) -> double&;
		auto operator[](int) const -> double;
		auto operator+() const -> euclidean_vector;
		auto operator-() const -> euclidean_vector;
		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(double) -> euclidean_vector&;
		auto operator/=(double) -> euclidean_vector&;
		explicit operator std::vector<double>() const;
		explicit operator std::list<double>() const;

		// MEMBER FUNCTIONS
		[[nodiscard]] auto at(int) const -> double;
		[[nodiscard]] auto at(int) -> double&;
		[[nodiscard]] auto dimensions() const -> int;

		// FRIENDS
		// equal
		friend auto operator==(euclidean_vector const& lhs, euclidean_vector const& rhs) -> bool {
			if (lhs.dimensions_ != rhs.dimensions_) {
				return false;
			}
			auto span_lhs =
			   std::span<double>(lhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(lhs.dimensions_));
			auto span_rhs =
			   std::span<double>(rhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(rhs.dimensions_));
			return ranges::equal(span_lhs, span_rhs);
		}
		// not equal
		friend auto operator!=(euclidean_vector const& lhs, euclidean_vector const& rhs) -> bool {
			return !(lhs == rhs);
		}
		// addition
		friend auto operator+(euclidean_vector const& lhs, euclidean_vector const& rhs)
		   -> euclidean_vector {
			if (lhs.dimensions_ != rhs.dimensions_) {
				throw euclidean_vector_error(fmt::format("Dimensions of LHS{} and RHS{} do not match",
				                                         lhs.dimensions_,
				                                         rhs.dimensions_));
			}
			auto result = euclidean_vector(lhs.dimensions_);
			auto span_lhs =
			   std::span<double>(lhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(lhs.dimensions_));
			auto span_rhs =
			   std::span<double>(rhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(rhs.dimensions_));
			auto span_result = std::span<double>(result.magnitudes_.get(),
			                                     gsl::narrow_cast<std::size_t>(result.dimensions_));
			std::transform(span_lhs.begin(),
			               span_lhs.end(),
			               span_rhs.begin(),
			               span_result.begin(),
			               std::plus<>());
			return result;
		}
		// substraction
		friend auto operator-(euclidean_vector const& lhs, euclidean_vector const& rhs)
		   -> euclidean_vector {
			if (lhs.dimensions_ != rhs.dimensions_) {
				throw euclidean_vector_error(fmt::format("Dimensions of LHS{} and RHS{} do not match",
				                                         lhs.dimensions_,
				                                         rhs.dimensions_));
			}
			auto result = euclidean_vector(lhs.dimensions_);
			auto span_lhs =
			   std::span<double>(lhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(lhs.dimensions_));
			auto span_rhs =
			   std::span<double>(rhs.magnitudes_.get(), gsl::narrow_cast<std::size_t>(rhs.dimensions_));
			auto span_result = std::span<double>(result.magnitudes_.get(),
			                                     gsl::narrow_cast<std::size_t>(result.dimensions_));
			for (int i = 0; gsl::narrow_cast<std::size_t>(i) < span_lhs.size(); ++i) {
				auto size_type_i = gsl::narrow_cast<std::size_t>(i);
				span_result[size_type_i] = span_lhs[size_type_i] - span_rhs[size_type_i];
			}
			return result;
		}
		// multiply
		friend auto operator*(euclidean_vector const& ev, double num) -> euclidean_vector {
			auto result = euclidean_vector(ev.dimensions_);
			auto span =
			   std::span<double>(ev.magnitudes_.get(), gsl::narrow_cast<std::size_t>(ev.dimensions_));
			auto span_result = std::span<double>(result.magnitudes_.get(),
			                                     gsl::narrow_cast<std::size_t>(result.dimensions_));
			ranges::transform(span.begin(), span.end(), span_result.begin(), [&num](auto const& c) {
				return c * num;
			});
			return result;
		}
		friend auto operator*(double num, euclidean_vector const& ev) -> euclidean_vector {
			return ev * num;
		}
		// divide
		friend auto operator/(euclidean_vector const& ev, double num) -> euclidean_vector {
			if (num == 0) {
				throw euclidean_vector_error("Invalid vector division by 0");
			}
			auto result = euclidean_vector(ev.dimensions_);
			auto span =
			   std::span<double>(ev.magnitudes_.get(), gsl::narrow_cast<std::size_t>(ev.dimensions_));
			auto span_result = std::span<double>(result.magnitudes_.get(),
			                                     gsl::narrow_cast<std::size_t>(result.dimensions_));
			ranges::transform(span.begin(), span.end(), span_result.begin(), [&num](auto const& c) {
				return c / num;
			});
			return result;
		}
		// output stream
		friend auto operator<<(std::ostream& os, euclidean_vector const& ev) -> std::ostream& {
			auto span =
			   std::span<double>(ev.magnitudes_.get(), gsl::narrow_cast<std::size_t>(ev.dimensions_));
			os << "[" << span[0];
			for (int i = 1; gsl::narrow_cast<std::size_t>(i) < span.size(); ++i) {
				auto size_type_i = gsl::narrow_cast<std::size_t>(i);
				os << " " << span[size_type_i];
			}
			os << "]";
			return os;
		}

		// FRIEND UTILITY FUNCTION - norm
		friend auto euclidean_norm(euclidean_vector const& v) -> double {
			if (v.dimensions() == 0) {
				throw euclidean_vector_error("euclidean_vector with no dimensions does not have a "
				                             "norm");
			}
			if (v.cache_ != 0) {
				return v.cache_;
			}
			auto to_vector = static_cast<std::vector<double>>(v);
			auto norm = std::sqrt(
			   std::inner_product(to_vector.begin(), to_vector.end(), to_vector.begin(), 0.0));
			v.cache_ = norm;
			return v.cache_;
		}

	private:
		int dimensions_;
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
		mutable double cache_; // to store the norm calculated last time
		                       // it will be set to 0 if the vector got modified
	};

	// UTILITY FUNCTIONS
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
