#include <CherrySoda/Util/Calc.h>

#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Calc;
using cherrysoda::Random;
using cherrysoda::STL;

namespace type = cherrysoda::type;

type::UInt32 Random::TrueRandomNext()
{
	static STL::TrueRandom random;
	return STL::RandomNext(random);
}

STL::Stack<Random> Calc::ms_randomStack({ Random() });
