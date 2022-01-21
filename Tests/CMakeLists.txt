set(CHERRYSODA_EXECUTABLE_FOLDER Tests)
set(CHERRYSODA_EXTRA_LIBS gtest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h CalcTest.cpp)
add_cherrysoda_executable(CalcTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h CollideTest.cpp)
add_cherrysoda_executable(CollideTest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp Test.h AllTests.cpp)
add_cherrysoda_executable(AllTests)
