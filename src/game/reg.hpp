#pragma once

#include <entt/entt.hpp>

namespace game {

class Reg {

private:
	entt::registry reg;

public:
	[[nodiscard]] entt::entity create() {
		return reg.create();
	}

	void destroy(entt::entity e) {
		reg.destroy(e);
	}

	template <typename... Components> const auto view() const {
		return reg.view<Components...>();
	};

	template <typename Type, typename... Args>
	decltype(auto) emplace(entt::entity e, Args &&...args) {
		return reg.emplace<Type>(e, std::forward<Args>(args)...);
	}

	template <typename... Components>
	decltype(auto) get(const entt::entity &e) {
		return reg.get<Components...>(e);
	};

	template <typename... Components>
	decltype(auto) get(const entt::entity &e) const {
		return reg.get<Components...>(e);
	};
};

}; // namespace game
