#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

namespace util {

template <typename T> constexpr char default_to_char(const T &v) {
	if constexpr (std::is_same_v<T, bool>) {
		return v ? '#' : ' ';
	} else if constexpr (std::is_arithmetic_v<T>) {
		return v > T{} ? '#' : ' ';
	} else if constexpr (std::is_pointer_v<T>) {
		return v != nullptr ? '#' : ' ';
	}
};

template <typename T> class Matrix {
public:
	Matrix() = default;

	Matrix(std::size_t w, std::size_t h) : w_(w), h_(h), data_(w * h) {};

	Matrix(std::size_t w, std::size_t h, const T &value)
		: w_(w), h_(h), data_(h * w, value) {};

	[[nodiscard]] T &operator()(std::size_t x, std::size_t y) noexcept {
		assert(y < h_ && x < w_);
		return data_[x * h_ + y];
	};

	[[nodiscard]] const T &operator()(
		std::size_t x, std::size_t y) const noexcept {
		assert(y < h_ && x < w_);
		return data_[x * h_ + y];
	};

	[[nodiscard]] T *operator[](std::size_t x) noexcept {
		assert(x < w_);
		return data_.data() + x * h_;
	};

	[[nodiscard]] const T *operator[](std::size_t x) const noexcept {
		assert(x < w_);
		return data_.data() + x * h_;
	};

	[[nodiscard]] std::size_t w() const noexcept {
		return w_;
	};

	[[nodiscard]] std::size_t h() const noexcept {
		return h_;
	};

	[[nodiscard]] std::size_t size() const noexcept {
		return data_.size();
	};

	[[nodiscard]] T *data() noexcept {
		return data_.data();
	};

	[[nodiscard]] const T *data() const noexcept {
		return data_.data();
	};

	void fill(const T &value) {
		std::fill(data_.begin(), data_.end(), value);
	};

	void dumpASCII() const {
		for (std::size_t y = h_; y-- > 0;) {
			for (std::size_t x = 0; x < w_; x++) {
				std::cout << default_to_char((*this)(x, y));
			}
			std::cout << '\n';
		}
	};

	// 判断写法： m.dumpASCII([](auto v) { return v >= 0 ? '@' : ' '; });
	template <typename F> void dumpASCII(F &&to_char) const {
		for (std::size_t y = h_; y-- > 0;) {
			for (std::size_t x = 0; x < w_; x++) {
				std::cout << to_char((*this)(x, y));
			}
			std::cout << '\n';
		}
	};

private:
	std::size_t w_ = 0;
	std::size_t h_ = 0;
	std::vector<T> data_;
};
}; // namespace util
