#include "CppUnitTest.h"

#include <game/models/gamestate.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace lib;

struct Tile
{
	enum Flag
	{

	};

	size_t flags{};
};


namespace tests
{
	TEST_CLASS(tests)
	{
	public:
		GameState gs;
		TEST_METHOD(entity_has_component)
		{
			//auto e = gs.add_entity();
			//gs.add_component<PositionComponent>(e);
			//
			//Assert::IsTrue(gs.has_component<PositionComponent>(e));
		}

	};
}
