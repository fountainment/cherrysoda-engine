#include "Test.h"

#include <string>

namespace {

TEST(StringTestFormat, BasicAndTruncation)
{
	EXPECT_EQ(String("42-x"), StringUtil::Format("%d-%s", 42, "x"));
	EXPECT_EQ(String("3.14"), StringUtil::Format("%.2f", 3.14159f));
	EXPECT_EQ(String("hello"), StringUtil::Format("hello"));

	// The internal vsnprintf buffer holds 256 bytes including the terminator,
	// so longer output gets truncated to 255 characters.
	String longInput(300, 'a');
	String formatted = StringUtil::Format("%s", longInput.c_str());
	EXPECT_EQ(255u, formatted.length());
	EXPECT_EQ('a', formatted.back());
}

TEST(StringTestSplit, DelimitersAndEmptyTokens)
{
	auto parts = StringUtil::Split("a,b,c", ',');
	ASSERT_EQ(3u, STL::Count(parts));
	EXPECT_EQ(String("a"), parts[0]);
	EXPECT_EQ(String("b"), parts[1]);
	EXPECT_EQ(String("c"), parts[2]);

	// Consecutive delimiters keep the empty token between them
	parts = StringUtil::Split("a,,b", ',');
	ASSERT_EQ(3u, STL::Count(parts));
	EXPECT_EQ(String(""), parts[1]);

	EXPECT_TRUE(STL::IsEmpty(StringUtil::Split("", ',')));

	// Default delimiter is a space
	parts = StringUtil::Split("a b");
	ASSERT_EQ(2u, STL::Count(parts));
	EXPECT_EQ(String("b"), parts[1]);
}

TEST(StringTestTrim, BothEndsAndCustomChar)
{
	EXPECT_EQ(String("abc"), StringUtil::Trim("  abc  "));
	EXPECT_EQ(String("abc"), StringUtil::Trim("abc"));
	EXPECT_EQ(String(""), StringUtil::Trim("   "));
	EXPECT_EQ(String("abc"), StringUtil::Trim("__abc__", '_'));
}

TEST(StringTestCase, LowerAndUpper)
{
	EXPECT_EQ(String("abc xyz 123"), StringUtil::ToLower("AbC XyZ 123"));
	EXPECT_EQ(String("ABC XYZ 123"), StringUtil::ToUpper("AbC XyZ 123"));
}

TEST(StringTestIndexOf, FoundAndMissing)
{
	EXPECT_EQ(2, StringUtil::IndexOf("hello", 'l'));
	EXPECT_EQ(-1, StringUtil::IndexOf("hello", 'z'));
}

TEST(StringTestConvert, ToFloatToIntToString)
{
	EXPECT_FLOAT_EQ(3.5f, StringUtil::ToFloat("3.5"));
	EXPECT_FLOAT_EQ(-0.5f, StringUtil::ToFloat("-0.5"));
	EXPECT_EQ(-7, StringUtil::ToInt("-7"));
	EXPECT_EQ(0, StringUtil::ToInt("0"));
	EXPECT_EQ(String("123"), StringUtil::ToString(123));
	EXPECT_EQ(String("1.500000"), StringUtil::ToString(1.5f));
}

TEST(StringTestConvert, SafeToSuccessAndFailure)
{
	int intVal = 0;
	EXPECT_TRUE(StringUtil::SafeTo<int>("42", intVal));
	EXPECT_EQ(42, intVal);
	EXPECT_FALSE(StringUtil::SafeTo<int>("not a number", intVal));

	float floatVal = 0.f;
	EXPECT_TRUE(StringUtil::SafeTo<float>("2.5", floatVal));
	EXPECT_FLOAT_EQ(2.5f, floatVal);
	EXPECT_FALSE(StringUtil::SafeTo<float>("xyz", floatVal));
}

TEST(StringTestPath, DirectoryAndFileName)
{
	EXPECT_EQ(String("a/b/"), StringUtil::Path_GetDirectoryName("a/b/c.txt"));
	EXPECT_EQ(String("/"), StringUtil::Path_GetDirectoryName("/root"));
	EXPECT_EQ(String(""), StringUtil::Path_GetDirectoryName("file.txt"));

	EXPECT_EQ(String("c.txt"), StringUtil::Path_GetFileName("a/b/c.txt"));
	EXPECT_EQ(String("c.txt"), StringUtil::Path_GetFileName("c.txt"));
	EXPECT_EQ(String("c.txt"), StringUtil::Path_GetFileName("a\\b\\c.txt"));

	// Without a directory part the extension is stripped correctly
	EXPECT_EQ(String("c"), StringUtil::Path_GetFileNameWithoutExtension("c.txt"));
	EXPECT_EQ(String("c"), StringUtil::Path_GetFileNameWithoutExtension("c"));
}

TEST(StringTestHash, BKDR)
{
	EXPECT_EQ(0, StringUtil::GetHashBKDR(""));
	EXPECT_EQ(97, StringUtil::GetHashBKDR("a"));      // 'a' = 97
	EXPECT_EQ(12805, StringUtil::GetHashBKDR("ab"));  // 97 * 131 + 98
	EXPECT_NE(StringUtil::GetHashBKDR("abc"), StringUtil::GetHashBKDR("acb"));

	// Explicit length limits the hashed range
	EXPECT_EQ(StringUtil::GetHashBKDR("ab"), StringUtil::GetHashBKDR("abc", 2));

	// Hash is masked to stay non-negative
	EXPECT_GE(StringUtil::GetHashBKDR("some longer string that could hash anywhere"), 0);
}

TEST(StringTestHex, HexStrToUInt32)
{
	EXPECT_EQ(255u, StringUtil::HexStrToUInt32("ff"));
	EXPECT_EQ(26u, StringUtil::HexStrToUInt32("1A"));
	EXPECT_EQ(255u, StringUtil::HexStrToUInt32("Ff"));
	EXPECT_EQ(0u, StringUtil::HexStrToUInt32(""));
	EXPECT_EQ(0xFFFu, StringUtil::HexStrToUInt32("ffffff", 3));
}

TEST(StringTestUtf8, AsciiAndMultibyte)
{
	auto ascii = StringUtil::Utf8ToUnicode32("A");
	ASSERT_EQ(1u, STL::Count(ascii));
	EXPECT_EQ(65u, ascii[0]);

	// U+4E2D encodes to E4 B8 AD in UTF-8
	auto chinese = StringUtil::Utf8ToUnicode32("\xe4\xb8\xad");
	ASSERT_EQ(1u, STL::Count(chinese));
	EXPECT_EQ(0x4E2Du, chinese[0]);

	auto mixed = StringUtil::Utf8ToUnicode32("A\xe4\xb8\xad""B");
	ASSERT_EQ(3u, STL::Count(mixed));
	EXPECT_EQ(65u, mixed[0]);
	EXPECT_EQ(0x4E2Du, mixed[1]);
	EXPECT_EQ(66u, mixed[2]);
}

TEST(StringTestStringID, EqualityAndEmpty)
{
	StringID a("cherrysoda_test_a");
	StringID a2("cherrysoda_test_a");
	StringID b("cherrysoda_test_b");

	EXPECT_EQ(a, a2);
	EXPECT_NE(a, b);
	EXPECT_EQ(a.GetID(), a2.GetID());
	EXPECT_FALSE(a.IsEmpty());
	EXPECT_TRUE(StringID().IsEmpty());
	EXPECT_TRUE(StringID("").IsEmpty());

	// Implicit conversion to the underlying hash
	EXPECT_EQ(StringUtil::GetHashBKDR("cherrysoda_test_a"), static_cast<type::Int32>(a));
}

} // namespace
