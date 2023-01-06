#pragma once

#include <core/core.hpp>

#include <platform/render_device.hpp>

namespace prosper
{
	struct VertexBuffer : DynamicBuffer
	{
		VertexBuffer(Allocator* allocator) : DynamicBuffer(allocator) {}

		constexpr operator std::span<float>() const
		{
			return std::span{(float*) data, this->size};
		}
	};

	struct IndexBuffer : DynamicBuffer
	{
		IndexBuffer(Allocator* allocator) : DynamicBuffer(allocator) {}

		constexpr operator std::span<unsigned int>() const
		{
			return std::span{(unsigned int*) data, this->size};
		}
	};

	struct RenderData
	{
		VertexBuffer vertex_data;

		IndexBuffer index_data;

		size_t num_vertices{},
			num_indices{},
			num_indices_pairs{};

		RenderData(Allocator* allocator) : vertex_data(allocator), index_data(allocator) {}

		template <typename ...Floats>
		inline void push_vertex(const Floats& ...values)
		{
			num_vertices++;

			(..., vertex_data.push(values));
		}

		template <typename ...UInts>
		inline void push_indices(const UInts& ...values)
		{
			num_indices_pairs++;

			num_indices += sizeof...(values);

			(..., index_data.push(values));
		}

		void clear()
		{
			num_vertices = {};
			num_indices = {};
			num_indices_pairs = {};

			vertex_data.clear();
			index_data.clear();
		}
	};

	struct RendererBase
	{
		RenderDevice* dev;

		RenderData data;

		RendererBase(Platform* platform) : dev(platform->get_render_device()), data(platform->get_allocator()) {}

		// Helper methods

		Vector<float, 2> screen_to_normalized_device_coordinates(int x, int y)
		{
			const auto& viewport = dev->get_viewport();

			static const auto model = TranslationMatrix(0.f, 0.f, 0.f);
			const auto projection = OrthographicMatrix(0, (float) viewport.w, 0.f, (float) viewport.h, -1.f, 1.f);
			const auto normalized_device_coordinates = (projection * model) * Matrix<float, 1, 4>{static_cast<float>(x), static_cast<float>(y), 0.f, 1.f};
			return {normalized_device_coordinates(0, 0), normalized_device_coordinates(1, 0) * -1.f};
		}
	};
}