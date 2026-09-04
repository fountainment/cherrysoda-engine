#include "Test.h"

#include <CherrySoda/Util/CheatListener.h>
#include <CherrySoda/Util/Commands.h>
#include <CherrySoda/Util/ErrorLog.h>
#include <CherrySoda/Util/SimpleCurve.h>

#include <cstdio>
#include <fstream>
#include <sstream>

namespace {

TEST(SimpleCurveTest, EndpointsAndMidpoint)
{
	SimpleCurve curve(Math::Vec2(0.f, 0.f), Math::Vec2(10.f, 0.f), Math::Vec2(5.f, 10.f));

	EXPECT_EQ(Math::Vec2(0.f, 0.f), curve.GetPoint(0.f));
	EXPECT_EQ(Math::Vec2(10.f, 0.f), curve.GetPoint(1.f));
	// The quadratic midpoint sits halfway towards the control
	EXPECT_EQ(Math::Vec2(5.f, 5.f), curve.GetPoint(0.5f));
}

TEST(SimpleCurveTest, LengthOfStraightCurve)
{
	SimpleCurve line(Math::Vec2(0.f, 0.f), Math::Vec2(10.f, 0.f), Math::Vec2(5.f, 0.f));
	EXPECT_FLOAT_EQ(10.f, line.GetLengthParametric(16));
}

TEST(SimpleCurveTest, DoubleControlPushesControlOut)
{
	SimpleCurve curve(Math::Vec2(0.f, 0.f), Math::Vec2(10.f, 0.f), Math::Vec2(5.f, 5.f));
	curve.DoubleControl();
	EXPECT_EQ(Math::Vec2(5.f, 10.f), curve.m_control);
}

TEST(CheatListenerTest, FiresOnceOnCodeAndTrimsBuffer)
{
	CheatListener listener;

	// Each key has its own pressed state so exactly one input fires per
	// Update, like a real Pressed query
	const char ids[4] = {'U', 'D', 'L', 'R'};
	bool pressed[4] = {false, false, false, false};
	for (int i = 0; i < 4; ++i) {
		listener.AddInput(ids[i], [&pressed, i] { return pressed[i]; });
	}
	auto type = [&](int i) {
		pressed[i] = true;
		listener.Update();
		pressed[i] = false;
	};

	int fired = 0;
	listener.AddCheat("UUDD", [&fired] { ++fired; });

	// A wrong sequence does nothing; the buffer holds the last four inputs
	type(2); // L
	type(3); // R
	type(0); // U
	type(1); // D
	EXPECT_EQ(0, fired);
	EXPECT_EQ("LRUD", listener.CurrentInput());

	// Entering the code fires the cheat exactly once and clears the buffer
	type(0); // U
	type(0); // U
	type(1); // D
	type(1); // D
	EXPECT_EQ(1, fired);
	EXPECT_TRUE(listener.CurrentInput().empty());

	// The cheat is one-shot: entering it again does nothing
	type(0);
	type(0);
	type(1);
	type(1);
	EXPECT_EQ(1, fired);
	EXPECT_EQ("UUDD", listener.CurrentInput());
}

TEST(CommandsTest, FunctionKeyActionsRegisterAndFire)
{
	int fired = 0;
	Commands::SetFunctionKeyAction(0, [&fired] { ++fired; });
	Commands::SetFunctionKeyAction(11, [&fired] { fired += 10; });

	Commands::ExecuteFunctionKeyAction(0);
	EXPECT_EQ(1, fired);
	Commands::ExecuteFunctionKeyAction(11);
	EXPECT_EQ(11, fired);

	// Unbound and rebound keys
	Commands::SetFunctionKeyAction(0, nullptr);
	Commands::ExecuteFunctionKeyAction(0);
	EXPECT_EQ(11, fired);

	Commands::SetFunctionKeyAction(11, nullptr);
}

TEST(ErrorLogTest, WritesHeaderAndPreservesOldErrors)
{
	String filename = "test_error_log_tmp.txt";
	// The file may not exist on a first run, so a nonzero result is fine here
	(void)std::remove(filename.c_str());

	const String& previous = ErrorLog::Filename();
	ErrorLog::Filename(filename);

	auto readAll = [&filename] {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return String(buffer.str());
	};

	ErrorLog::Write("first error");
	String first = readAll();
	EXPECT_NE(String::npos, first.find(ErrorLog::Marker));
	EXPECT_NE(String::npos, first.find("first error"));
	EXPECT_NE(String::npos, first.find("CherrySoda Engine"));

	// The second write keeps the first error below the new one
	ErrorLog::Write("second error");
	String second = readAll();
	EXPECT_NE(String::npos, second.find("second error"));
	EXPECT_NE(String::npos, second.find("first error"));
	EXPECT_LT(second.find("second error"), second.find("first error"));

	(void)std::remove(filename.c_str());
	ErrorLog::Filename(previous);
}

} // namespace
