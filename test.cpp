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

struct VecCont {
	const std::vector<int> a;
	size_t size() const { return a.size(); }
	VecCont(const std::vector<int>& a) : a(a) {}
	VecCont(std::vector<int>&& a) : a(std::move(a)) {}
	const int& operator[](const int idx) const { return a[idx]; }
};

using cta = const_ton<Dummy>;
using cta = const_ton<Dummy>;
using vcta = std::vector<const_ton<Dummy>>;
using vi = std::vector<int>;
using ctvi = const_ton<VecCont>;

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

/*TEST_CASE("Const_ton containing vectors") {
	vi v1;
	v1.emplace_back(0);
	v1.emplace_back(2);
	VecCont vc{v1};
	ctvi ctv1{vc};

	THEN ("The perfect forwarding constructor does not modify its lvalue arguments.") {
		REQUIRE(v1.size() == 2);
		REQUIRE(v1[0] == 0);
		REQUIRE(v1[1] == 2);
		REQUIRE(vc.size() == 2);
		REQUIRE(vc[0] == 0);
		REQUIRE(vc[1] == 2);
	}
	THEN ("The vector inside const_ton now contains the right elements.") {
		REQUIRE(ctv1->size() == 2);
		REQUIRE((*ctv1)[0] == 0);
		REQUIRE((*ctv1)[1] == 2);
	}

	v1[0] = 10;
	v1[1] = 12;
	VecCont vc2{std::move(v1)};
	ctvi ctv2{std::move(vc)};
	THEN ("The perfect forwarding constructor modifies its rvalue arguments.") {
		REQUIRE(v1.size() == 0);
		REQUIRE(vc.size() == 0);
	}
	THEN ("The vector inside const_ton now contains the right elements.") {
		REQUIRE(ctv2->size() == 2);
		REQUIRE((*ctv2)[0] == 10);
		REQUIRE((*ctv2)[1] == 12);
	}

	SECTION("Copy assign const_ton<VecCont> to const_ton<VecCont>") {
		ctv1 = ctv2;
		THEN ("The source vector being assigned from remains the same.") {
			REQUIRE(ctv2->size() == 2);
			REQUIRE((*ctv2)[0] == 10);
			REQUIRE((*ctv2)[1] == 12);
		}
		THEN ("The target vector being assigned to now is updated.") {
			REQUIRE(ctv1->size() == 2);
			REQUIRE((*ctv1)[0] == 10);
			REQUIRE((*ctv1)[1] == 12);
		}
	}

	SECTION("Move assign const_ton<VecCont> to const_ton<VecCont>") {
		ctv1 = ctv2;
		THEN ("The source vector being assigned from now is empty.") {
			REQUIRE(ctv2->size() == 0);
		}
		THEN ("The vector inside const_ton now contains the right elements.") {
			REQUIRE(ctv1->size() == 2);
			REQUIRE((*ctv1)[0] == 10);
			REQUIRE((*ctv1)[1] == 12);
		}
	}
}*/
