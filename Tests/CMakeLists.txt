set(CHERRYSODA_EXECUTABLE_FOLDER Tests)
set(CHERRYSODA_EXTRA_LIBS gtest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h CalcTest.cpp)
add_cherrysoda_executable(CalcTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h CollideTest.cpp)
add_cherrysoda_executable(CollideTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h UtilTest.cpp)
add_cherrysoda_executable(UtilTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h EaseTest.cpp)
add_cherrysoda_executable(EaseTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h ColorTest.cpp)
add_cherrysoda_executable(ColorTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h StringTest.cpp)
add_cherrysoda_executable(StringTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h MathTest.cpp)
add_cherrysoda_executable(MathTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h ColliderTest.cpp)
add_cherrysoda_executable(ColliderTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h SceneTest.cpp)
add_cherrysoda_executable(SceneTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h StateMachineTest.cpp)
add_cherrysoda_executable(StateMachineTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h CoroutineTest.cpp)
add_cherrysoda_executable(CoroutineTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h VirtualInputTest.cpp)
add_cherrysoda_executable(VirtualInputTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h TrackerTest.cpp)
add_cherrysoda_executable(TrackerTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h TilerTest.cpp)
add_cherrysoda_executable(TilerTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h UtilExtTest.cpp)
add_cherrysoda_executable(UtilExtTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h AllTests.cpp)
add_cherrysoda_executable(AllTests)
