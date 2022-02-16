#include "Test.h"

namespace {

TEST(UtilTestVirtualMap, Normal)
{
	VirtualMap<int> v(1000, 1000);
	EXPECT_EQ(v.GetValue(0, 0), 0);

	v.SetValue(0, 0, -1);
	EXPECT_EQ(v.GetValue(0, 0), -1);

	// Empty value
	VirtualMap<int> v1(1000, 1000, -2);
	EXPECT_EQ(v1.GetValue(0, 0), -2);

	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j) {
			if (i % 2 == 0 && j % 3 == 0) {
				v1.SetValue(i, j, 0);
			}
		}
	}
	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j) {
			if (i % 2 == 0 && j % 3 == 0) {
				EXPECT_EQ(v1.GetValue(i, j), 0);
			}
			else {
				EXPECT_EQ(v1.GetValue(i, j), -2);
			}
		}
	}
}

TEST(UtilTestVirtualMap, Extrem)
{
	VirtualMap<bool> v(10000, 10000);
	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 10000; ++j) {
			EXPECT_FALSE(v.GetValue(i, j));
		}
	}

	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 10000; ++j) {
			v.SetValue(i, j, true);
		}
	}

	for (int i = 0; i < 10000; ++i) {
		for (int j = 0; j < 10000; ++j) {
			EXPECT_TRUE(v.GetValue(i, j));
		}
	}

	v.SetValue(999, 889, false);
	EXPECT_FALSE(v.GetValue(999, 889));
}

} // namespace
