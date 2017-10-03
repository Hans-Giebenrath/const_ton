#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "const_ton.hpp"
#include <vector>

struct A {
	const int a = 0;
	A() {}
	A(int a) : a(a) {}
	A(const A& a) : a(a.a) {}
	A& operator=(const A&) = delete;
	A& operator=(A&&) = delete;
};

using cta = const_ton<A>;
using cta = const_ton<A>;
using vcta = std::vector<const_ton<A>>;
using vi = std::vector<int>;

TEST_CASE("Simple assignment") {
	int constexpr assigned_value = 4;
	const_ton<A> t1;
	const_ton<A> t2(assigned_value);
	const int& ref = t1->a;

	t1 = t2;

	REQUIRE(t1->a == assigned_value);
	REQUIRE(ref == assigned_value);
}

TEST_CASE("Vector containing const ton") {
	vcta v;
	v.push_back(cta{});
	v.push_back(cta{2});
	v[0] = cta{4};

	REQUIRE(v[0]->a == 4);
	REQUIRE(v[1]->a == 2);
}
