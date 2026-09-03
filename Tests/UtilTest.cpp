#include "Test.h"

namespace {

TEST(UtilTestVirtualMap, Normal)
{
	VirtualMap<int> v(1000, 1000);
	EXPECT_EQ(v.Get(0, 0), 0);

	v.Set(0, 0, -1);
	EXPECT_EQ(v.Get(0, 0), -1);

	// Empty value
	VirtualMap<int> v1(1000, 1000, -2);
	EXPECT_EQ(v1.Get(0, 0), -2);

	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j) {
			if (i % 2 == 0 && j % 3 == 0) {
				v1.Set(i, j, 0);
			}
		}
	}
	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j) {
			if (i % 2 == 0 && j % 3 == 0) {
				EXPECT_EQ(v1.Get(i, j), 0);
			}
			else {
				EXPECT_EQ(v1.Get(i, j), -2);
			}
		}
	}

	// Default Empty Value
	VirtualMap<bool> v2(40, 23);
	v2.Set(1, 1, true);
	for (int i = 0; i < 40; ++i) {
		for (int j = 0; j < 23; ++j) {
			EXPECT_EQ(i == 1 && j == 1, v2.Get(i, j));
		}
	}

	// Bug 2022.02.23
	VirtualMap<bool> v3(80, 46);
	v3.Set(1, 1, true);
	for (int i = 0; i < 80; ++i) {
		for (int j = 0; j < 46; ++j) {
			EXPECT_EQ(i == 1 && j == 1, v3.Get(i, j));
		}
	}
	VirtualMap<bool> v4(46, 80);
	v4.Set(1, 1, true);
	for (int i = 0; i < 46; ++i) {
		for (int j = 0; j < 80; ++j) {
			EXPECT_EQ(i == 1 && j == 1, v4.Get(i, j));
		}
	}
}

TEST(UtilTestVirtualMap, Extrem)
{
	VirtualMap<bool> v(10000, 10000);
	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 10000; ++j) {
			EXPECT_FALSE(v.Get(i, j));
		}
	}

	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 10000; ++j) {
			v.Set(i, j, true);
		}
	}

	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 10000; ++j) {
			EXPECT_TRUE(v.Get(i, j));
		}
	}

	v.Set(999, 889, false);
	EXPECT_FALSE(v.Get(999, 889));
}

TEST(UtilTestChooser, WeightedChoose)
{
	Chooser<int> chooser;
	chooser.Add(10, 0.7f);
	chooser.Add(20, 0.3f);

	EXPECT_FALSE(chooser.IsEmpty());
	EXPECT_TRUE(chooser.CanChoose());

	bool sawTen = false;
	bool sawTwenty = false;
	for (int i = 0; i < 100; ++i) {
		Calc::PushRandom(1000 + i);
		int chosen = chooser.Choose();
		Calc::PopRandom();
		sawTen = sawTen || chosen == 10;
		sawTwenty = sawTwenty || chosen == 20;
	}
	EXPECT_TRUE(sawTen);
	EXPECT_TRUE(sawTwenty);
}

TEST(UtilTestChooser, ZeroWeightNeverChosen)
{
	Chooser<int> chooser;
	chooser.Add(10, 0.f);
	chooser.Add(20, 1.f);

	for (int i = 0; i < 100; ++i) {
		Calc::PushRandom(2000 + i);
		EXPECT_EQ(20, chooser.Choose());
		Calc::PopRandom();
	}
}

TEST(UtilTestChooser, FallbackBehaviors)
{
	// Empty chooser reports no choices
	Chooser<int> empty;
	EXPECT_TRUE(empty.IsEmpty());
	EXPECT_FALSE(empty.CanChoose());

	// A single choice is returned without touching the random stack
	Chooser<int> single(5, 1.f);
	EXPECT_EQ(5, single.Choose());

	// Zero total weight cannot choose; the fallback value of the internal
	// default Choice is uninitialized for trivial T, so only the flag is asserted.
	Chooser<int> allZero;
	allZero.Add(9, 0.f);
	EXPECT_FALSE(allZero.CanChoose());

	// Negative weights clamp to zero
	Chooser<int> clamped;
	clamped.Add(1, -5.f);
	clamped.Add(2, 1.f);
	for (int i = 0; i < 50; ++i) {
		Calc::PushRandom(3000 + i);
		EXPECT_EQ(2, clamped.Choose());
		Calc::PopRandom();
	}
}

TEST(UtilTestChooser, FromStringParsing)
{
	// Bare key gets weight 1
	Chooser<String> single = Chooser<String>::FromString("only");
	EXPECT_FALSE(single.IsEmpty());
	EXPECT_TRUE(single.CanChoose());

	// Key:weight pairs, comma separated (whitespace around keys is trimmed)
	Chooser<String> weighted = Chooser<String>::FromString("a:2,b:3");
	EXPECT_TRUE(weighted.CanChoose());

	// A zero-weight entry never wins
	Chooser<String> biased = Chooser<String>::FromString("loser:0,winner:1");
	for (int i = 0; i < 50; ++i) {
		Calc::PushRandom(4000 + i);
		EXPECT_EQ(String("winner"), biased.Choose());
		Calc::PopRandom();
	}

	// Unweighted entries all stay reachable
	Chooser<String> plain = Chooser<String>::FromString("x,y");
	bool sawX = false;
	bool sawY = false;
	for (int i = 0; i < 100; ++i) {
		Calc::PushRandom(5000 + i);
		String chosen = plain.Choose();
		Calc::PopRandom();
		sawX = sawX || chosen == "x";
		sawY = sawY || chosen == "y";
	}
	EXPECT_TRUE(sawX);
	EXPECT_TRUE(sawY);
}

TEST(UtilTestSTL, VectorHelpers)
{
	STL::Vector<int> v;
	EXPECT_TRUE(STL::IsEmpty(v));

	STL::Add(v, 1);
	STL::Add(v, 2);
	STL::Add(v, 3);
	EXPECT_EQ(3u, STL::Count(v));
	EXPECT_EQ(1, STL::Front(v));
	EXPECT_EQ(3, STL::Back(v));
	EXPECT_TRUE(STL::Contains(v, 2));
	EXPECT_FALSE(STL::Contains(v, 9));
	EXPECT_EQ(12u, STL::ByteSize(v));

	EXPECT_TRUE(STL::Remove(v, 2));
	EXPECT_FALSE(STL::Remove(v, 2));
	EXPECT_EQ(2u, STL::Count(v));

	STL::Vector<int> w;
	STL::Add(w, 7);
	STL::AddRange(v, w);
	EXPECT_EQ(3u, STL::Count(v));
	EXPECT_TRUE(STL::Contains(v, 7));

	STL::Sort(v);
	EXPECT_EQ(1, v[0]);
	EXPECT_EQ(3, v[1]);
	EXPECT_EQ(7, v[2]);
	STL::Sort(v, [](int a, int b) { return a > b; });
	EXPECT_EQ(7, v[0]);

	int first = -1;
	EXPECT_TRUE(STL::TryGetFirst(v, first));
	EXPECT_EQ(7, first);
	STL::Vector<int> emptyVector;
	EXPECT_FALSE(STL::TryGetFirst(emptyVector, first));
}

TEST(UtilTestSTL, MapHelpers)
{
	STL::Map<int, String> m;
	STL::Add(m, STL::MakePair(1, String("one")));
	STL::Add(m, STL::MakePair(2, String("two")));

	EXPECT_TRUE(STL::ContainsKey(m, 1));
	EXPECT_FALSE(STL::ContainsKey(m, 3));

	String value;
	EXPECT_TRUE(STL::TryGetValue(m, 2, value));
	EXPECT_EQ(String("two"), value);
	EXPECT_FALSE(STL::TryGetValue(m, 3, value));

	const String* address = STL::GetValueAddress(m, 1);
	EXPECT_NE(nullptr, address);
	EXPECT_EQ(String("one"), *address);
	EXPECT_EQ(nullptr, STL::GetValueAddress(m, 9));

	EXPECT_TRUE(STL::RemoveKey(m, 1));
	EXPECT_FALSE(STL::RemoveKey(m, 1));
	EXPECT_EQ(1u, STL::Count(m));

	STL::HashSet<int> set;
	STL::Add(set, 5);
	EXPECT_TRUE(STL::Contains(set, 5));
}

TEST(UtilTestSTL, StackAndQueueHelpers)
{
	STL::Stack<int> stack;
	STL::Push(stack, 1);
	STL::Push(stack, 2);
	EXPECT_EQ(2, STL::Top(stack));
	EXPECT_EQ(2, STL::Pop(stack));
	EXPECT_EQ(1, STL::Pop(stack));

	STL::Queue<int> queue;
	STL::Push(queue, 1);
	STL::Push(queue, 2);
	EXPECT_EQ(1, STL::Pop(queue));
	EXPECT_EQ(2, STL::Pop(queue));

	STL::PriorityQueueMinTop<int> minHeap;
	STL::Push(minHeap, 5);
	STL::Push(minHeap, 1);
	STL::Push(minHeap, 3);
	EXPECT_EQ(1, STL::Pop(minHeap));
	EXPECT_EQ(3, STL::Pop(minHeap));
	EXPECT_EQ(5, STL::Pop(minHeap));

	int a = 1;
	int b = 2;
	STL::Swap(a, b);
	EXPECT_EQ(2, a);
	EXPECT_EQ(1, b);
}

// Registered once per test binary; only a handful of tags may exist in a process.
static const BitTag s_utilTestTagA("csd_util_test_tag_a");
static const BitTag s_utilTestTagB("csd_util_test_tag_b");

TEST(UtilTestBitTag, IdsAndLookup)
{
	EXPECT_NE(s_utilTestTagA.ID(), s_utilTestTagB.ID());

	// The bit value is derived from the ID
	EXPECT_EQ(1u << s_utilTestTagA.ID(), static_cast<BitTagValueType>(s_utilTestTagA));
	EXPECT_EQ(1u << s_utilTestTagB.ID(), static_cast<BitTagValueType>(s_utilTestTagB));

	// Lookup by name returns the registered tag
	EXPECT_EQ(s_utilTestTagA.ID(), BitTag::Get("csd_util_test_tag_a").ID());

	// A default BitTag has no bits set
	EXPECT_EQ(0u, static_cast<BitTagValueType>(BitTag()));

	EXPECT_LE(2, BitTag::TotalTags());
}

} // namespace
