#ifndef CONST_TON_HPP
#define CONST_TON_HPP

#include <type_traits>

template<typename T>
struct const_ton {
private:
	// Detect, whether we have a const_ton.
	template<typename A>
	struct is_const_ton_hlpr {
		static constexpr bool value = false;
	};

	template<typename A>
	struct is_const_ton_hlpr<const_ton<A>> {
		static constexpr bool value = true;
	};

	template<typename A>
	struct is_const_ton {
		static constexpr bool value = is_const_ton_hlpr<typename std::decay<A>::type>::value;
	};

	template<typename T2, typename = typename std::enable_if<!is_const_ton<T2>::value, void>::type>
	static constexpr T2&& forward(typename std::remove_reference<T2>::type& t) noexcept
	{
		return static_cast<T2&&>(t);
	}

	/*template<typename T2, typename = typename std::enable_if<!is_const_ton<T2>::value, void>::type>
	static constexpr T2&& forward(typename std::remove_reference<T2>::type&& t) noexcept
	{
		static_assert(!std::is_lvalue_reference<T2>::value,
				"Can not forward an rvalue as an lvalue.");
		return static_cast<T2&&>(t);
	}*/

	template<typename T2, typename A = typename std::decay<T2>::type::type, typename = typename std::enable_if<is_const_ton<T2>::value, void>::type>
	static constexpr A&& forward(typename std::remove_reference<T2>::type& t) noexcept {
		  return static_cast<A&&>(std::forward<T2>(t).reference_to_element());
	}

	/*template<typename T2, typename A = typename std::decay<T2>::type::type, typename = typename std::enable_if<is_const_ton<T2>::value, void>::type>
	static constexpr A&& forward(typename std::remove_reference<T2>::type&& t) noexcept {
		  return std::forward<T2>(t).reference_to_element();
	}*/

	constexpr T& reference_to_element() & noexcept {
		return val;
	}

	constexpr T&& reference_to_element() && noexcept {
		return std::move(val);
	}

public:
	T val;
	using type = T;

	template<typename... A>
	const_ton(A&&... a) :
		val(const_ton::forward<A>(a)...)
	{}

	const_ton(const const_ton& other) {
		*this = other;
	}

	const_ton(const_ton&& other) {
		*this = std::move(other);
	}

	static_assert(is_const_ton<const_ton<int>>::value);
	static_assert(is_const_ton<const_ton<int>&>::value);
	static_assert(!is_const_ton<const_ton<int>*>::value);
	static_assert(!is_const_ton<int>::value);

	template<typename A, typename = typename std::enable_if<!is_const_ton<A>::value, void>::type>
	const_ton<T>& operator=(A&& a) {
		val.~T();
		new(&val) T(const_ton<T>::forward<A>(a));
		return *this;
	}

	const_ton& operator=(const const_ton& t) {
		val.~T();
		new(&val) T(t.val);
		return *this;
	}

	const_ton& operator=(const_ton&& t) {
		val.~T();
		new(&val) T(std::move(t.val));
		return *this;
	}

	/*template<typename A>
	const_ton<T>& operator=(const const_ton<A>& t) {
		val.~T();
		new(&val) T(t.val);
		return *this;
	}

	template<typename A>
	const_ton<T>& operator=(const_ton<A>&& t) {
		val.~T();
		new(&val) T(std::move(t.val));
		t.~T();
		return *this;
	}*/

	T* operator->() {
		return &val;
	}

	const T* operator->() const {
		return &val;
	}

	T& operator*() {
		return val;
	}

	const T& operator*() const {
		return val;
	}
};

#endif // CONST_TON_HPP
