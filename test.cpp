#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "const_ton.hpp"
#include <vector>

struct Dummy {
	const int a = 0;
	Dummy() {}
	Dummy(int a) : a(a) {}
	Dummy(const Dummy& a) : a(a.a) {}
	Dummy& operator=(const Dummy&) = delete;
	Dummy& operator=(Dummy&&) = delete;
};

using cta = const_ton<Dummy>;
using cta = const_ton<Dummy>;
using vcta = std::vector<const_ton<Dummy>>;
using vi = std::vector<int>;

TEST_CASE("Simple assignment") {
	int constexpr assigned_value = 4;
	cta t1;
	cta t2(assigned_value);
	cta t3(t2);
	const int& ref = t1->a;

	t1 = t2;

	REQUIRE(t1->a == assigned_value);
	REQUIRE(ref == assigned_value);
	REQUIRE(t3->a == assigned_value);
}

TEST_CASE("Vector containing const ton push back") {
	vcta v;
	v.push_back(cta{});
	v.push_back(cta{2});
	v[0] = cta{4};

	REQUIRE(v[0]->a == 4);
	REQUIRE(v[1]->a == 2);
}

TEST_CASE("Vector containing const ton emplace back") {
	vcta v;
	v.emplace_back(0);
	v.emplace_back(2);
	v[0] = 4;

	REQUIRE(v[0]->a == 4);
	REQUIRE(v[1]->a == 2);
}

TEST_CASE("Vectors containing const ton") {
	vcta v1;
	v1.emplace_back(0);
	v1.emplace_back(2);
	vcta v2;
	v2.emplace_back(10);
	v2.emplace_back(12);
	vcta v3;
	v3.emplace_back(100);
	v3.emplace_back(102);

	v1 = v2;
	REQUIRE(v1[0]->a == 10);
	REQUIRE(v1[1]->a == 12);

	v1 = std::move(v3);
	REQUIRE(v1[0]->a == 100);
	REQUIRE(v1[1]->a == 102);
}
