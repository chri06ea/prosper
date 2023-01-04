#pragma once

#include "core.hpp"
#include "render_device.hpp"
#include "math.hpp"

namespace prosper
{

	struct VertexBuffer : Buffer
	{
		size_t count{};

		constexpr operator std::span<float>() const
		{
			return std::span{(float*) data, this->size()};
		}

		template <typename T>
		void push(const T& value)
		{
			Buffer::push(value);
		}
	};

	struct IndexBuffer : Buffer
	{
		size_t count{};

		constexpr operator std::span<unsigned int>() const
		{
			return std::span{(unsigned int*) data, this->size()};
		}

		template <typename T>
		void push(const T& value)
		{
			count++;

			Buffer::push(value);
		}
	};

	struct RenderData
	{
		VertexBuffer vertices{};

		IndexBuffer indices{};

		template <typename ...Floats>
		inline void push_vertex(const Floats& ...values)
		{
			vertices.count++;

			(..., vertices.push(values));
		}

		template <typename ...UInts>
		inline void push_indices(const UInts& ...values)
		{
			(..., indices.push(values));
		}

		void clear()
		{
			vertices.clear();

			indices.clear();
		}
	};

	struct RendererBase
	{
		// GPU interface
		RenderDevice* dev;

		// CPU side render data
		RenderData data{};

		RendererBase(RenderDevice* render_device) : dev(render_device) {}

		// Helper methods

		Vector<float, 2> screen_to_normalized_device_coordinates(int x, int y)
		{
			const auto& viewport = dev->get_viewport();

			static const auto model = TranslationMatrix(0.f, 0.f, 0.f);
			const auto projection = OrthographicMatrix(0, (float) viewport.w, 0.f, (float) viewport.h, -1.f, 1.f);
			const auto normalized_device_coordinates = (projection * model) * Vector<float, 4>{static_cast<float>(x), static_cast<float>(y), 0.f, 1.f};
			return {normalized_device_coordinates(0, 0), normalized_device_coordinates(1, 0)};
		}
	};
}