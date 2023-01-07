#pragma once

#include <core/core.hpp>

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
		size_t x{}, y{}, w{}, h{};

		DynamicList<UIElement> children{};

		enum class Type
		{
			Frame
		} type{};

		template <typename T>
		T& add_child(const T& child);

		// Wrappers

		struct FrameData
		{
			Color color{};
		};

		union
		{
			FrameData frame{};
		}data{};
	};

	struct UIFrame : UIElement
	{

	};

	template<typename T>
	T& UIElement::add_child(const T& child)
	{
		children.push_back(child);
		return &children[children.size()];
	}

	namespace tests
	{
		namespace ui
		{
			static inline const auto f = ([]()
			{
				auto e = UIFrame{};
				//e.add_child<UIFrame>({});
				return 1;
			})();
		}
	}
}