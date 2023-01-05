#pragma once

#include <vector>

namespace prosper
{
	struct UI
	{
		virtual void draw_box(int x, int y, int w, int h)
		{

		}
	};

	// Forward declarations
	struct UIFrame;

	struct UIElement
	{
		std::vector<UIElement> children{};

		enum class Type
		{
			Frame
		} type{};

		template <typename T>
		T* add_child(const T& child)
		{
			children.push_back(child);
		}

		size_t x{}, y{}, w{}, h{};

		struct FrameData
		{
			Color color{};
		};

		union
		{
			FrameData frame{};
		}data{};
	};
}