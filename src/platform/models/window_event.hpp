namespace lib
{
	struct WindowEvent
	{
		enum class Type
		{
			Invalid,
			Resize,
			Input
		};

		Type type{};

		union
		{
			struct Input
			{
				enum class Type
				{
					Keydown,
					Keyup,
					CursorMove
				};

			} input;

			struct Resize
			{
				int left, right, top, bottom, width, height;

			} resize;
		};
	};
}