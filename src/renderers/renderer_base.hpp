#pragma once

#include <core/core.hpp>

#include <platform/render_device.hpp>

namespace prosper
{
	struct VertexBuffer : DynamicBuffer
	{
		size_t count{};

		VertexBuffer(Allocator& allocator) : DynamicBuffer(allocator) {}

		constexpr operator std::span<float>() const
		{
			return std::span{(float*) data, this->size};
		}
	};

	struct IndexBuffer : DynamicBuffer
	{
		size_t count{};

		IndexBuffer(Allocator& allocator) : DynamicBuffer(allocator) {}

		constexpr operator std::span<unsigned int>() const
		{
			return std::span{(unsigned int*) data, this->size};
		}
	};

	struct RenderData
	{
		VertexBuffer vertices;

		IndexBuffer indices;

		RenderData(Allocator& allocator) : vertices(allocator), indices(allocator) {}

		template <typename ...Floats>
		inline void push_vertex(const Floats& ...values)
		{
			vertices.count++;

			(..., vertices.push(values));
		}

		template <typename ...UInts>
		inline void push_indices(const UInts& ...values)
		{
			indices.count = sizeof...(values);

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
		RenderDevice* dev;

		RenderData data;

		RendererBase(RenderDevice* render_device, Allocator& allocator) : dev(render_device), data(allocator) {}

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