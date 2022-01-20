set(CHERRYSODA_EXECUTABLE_FOLDER Tests)
set(CHERRYSODA_EXTRA_LIBS gtest)

set(CHERRYSODA_EXECUTABLE_SRC TestMain.cpp CalcTest.cpp)
add_cherrysoda_executable(CalcTest)