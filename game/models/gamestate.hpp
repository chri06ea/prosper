#pragma once

#include <platform/core/defs.hpp>

#include <platform/models/vector.hpp>

namespace lib
{

	struct Entity
	{
		constexpr Entity(int id) : id(id)
		{

		}
		bool operator== (const Entity& rhs) { return id == rhs.id; };
		//bool operator!= (const Entity& rhs) { return id != rhs.id; };

		size_t id;
	};

	struct EntityList
	{
		std::vector<int> m_data{};

		int add_entity()
		{
			return 0;
		}

		auto begin() { return m_data.begin(); }
		auto end() { return m_data.end(); } // 200 is out of bounds
	};

	class GameState
	{
	public:
		size_t simulation_count{};
		float simulation_interval{};
		float simulation_time{};

		EntityList entity_list[100];
	private:
	};

	//class GameState
	//{
	//public:

	//	size_t simulation_count{};
	//	float simulation_interval{};
	//	float simulation_time{};

	//	int entity_count{};

	//	constexpr Entity add_entity()
	//	{
	//		return entity_count++;
	//	}

	//	#define DECLARE_ATTRIBUTE(TYPE, NAME)													\
	//	TYPE _##NAME{};																			\
	//	inline TYPE& health(Entity entity)																\
	//	{																						\
	//		return gs._##NAME[entity_index];													\
	//	}																						\
	//																							\
	//	inline bool has_##NAME(Entity entity) const													\
	//	{																						\
	//		return gs._flags[entity_index] & static_cast<size_t>(ComponentFlags::##NAME);		\
	//	}																						\
	//																							\
	//	inline void has_##NAME(Entity entity, bool yes)											\
	//	{																						\
	//		if(yes)																				\
	//			gs._flags[entity_index] |= static_cast<size_t>(ComponentFlags::##NAME);			\
	//		else																				\
	//			gs._flags[entity_index] &= ~static_cast<size_t>(ComponentFlags::##NAME);		\
	//	}

	//	DECLARE_ATTRIBUTE(int, health);
	//	DECLARE_ATTRIBUTE(Vector3, position);

	//private:

	//	enum ComponentFlags
	//	{
	//		health = (1 << 0),
	//		position = (1 << 1),
	//		velocity = (1 << 2),
	//		movespeed = (1 << 2),
	//	};

	//	GameState& gs;
	//	const size_t& entity_index;
	//};

	//class GameState
	//{
	//public:
	//	static constexpr auto MaxEntities = 32;

	//	int _flags[MaxEntities]{};
	//	int _health[MaxEntities]{};

	//	size_t simulation_count{};
	//	float simulation_interval{};
	//	float simulation_time{};

	//	int entity_count{};

	//	constexpr Entity add_entity()
	//	{
	//		return entity_count++;
	//	}

	//	template <typename Component>
	//	constexpr const bool has_component(Entity entity) const
	//	{
	//		return (_entity_component_flags[entity] & static_cast<size_t>(Component::Flag));
	//	}

	//private:
	//	int _entity_component_flags[MaxEntities]{};
	//};
}