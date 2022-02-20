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

} // namespace
