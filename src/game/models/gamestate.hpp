#pragma once

#include <platform/core/defs.hpp>

#include <platform/models/vector.hpp>

namespace lib
{
	enum class ComponentFlags
	{
		Health = (1 << 0),
		Position = (1 << 1),
		Velocity = (1 << 2),
		Movespeed = (1 << 2),
	};

	#define MAKE_COMPONENT(NAME, TYPE)							\
	struct NAME##Component : TYPE								\
	{															\
		static constexpr auto Flag = ComponentFlags::##NAME;	\
	};

	MAKE_COMPONENT(Health, Integer)
	MAKE_COMPONENT(Movespeed, Integer)
	MAKE_COMPONENT(Position, Vector)
	MAKE_COMPONENT(Velocity, Vector)

	class GameState
	{
	public:
		static constexpr auto MaxEntities = 32;

		size_t simulation_count{};
		float simulation_interval{};
		float simulation_time{};

		template <typename Component>
		constexpr Component& get_component(Entity entity);

		template <typename Component>
		constexpr const Component& get_component(Entity entity) const;

		template <typename Component>
		constexpr const bool has_component(Entity entity) const
		{
			return (_entity_component_flags[entity] & static_cast<size_t>(Component::Flag));
		}

		#define DECLARE_COMPONENT_LIST(COMPONENT)												\
		public:																					\
		template <>																				\
		constexpr COMPONENT& get_component<COMPONENT>(Entity entity)							\
		{																						\
			return _##COMPONENT##s[entity];														\
		}																						\
																								\
		template <>																				\
		constexpr const COMPONENT& get_component<COMPONENT>(Entity entity) const				\
		{																						\
			return _##COMPONENT##s[entity];														\
		}																						\
																								\
		private:																				\
		COMPONENT _##COMPONENT##s[MaxEntities]{};												\

		DECLARE_COMPONENT_LIST(HealthComponent);
		DECLARE_COMPONENT_LIST(PositionComponent);
		DECLARE_COMPONENT_LIST(VelocityComponent);
		DECLARE_COMPONENT_LIST(MovespeedComponent);

		#undef DECLARE_COMPONENT
	private:
		int _entity_component_flags[MaxEntities]{};
	};
}