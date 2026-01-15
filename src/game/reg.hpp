#pragma once

#include <entt/entt.hpp>

namespace game {

class Reg {

private:
	entt::registry reg;

public:
	[[nodiscard]] entt::entity create() {
		return reg.create();
	};

	void destroy(const entt::entity &e) {
		reg.destroy(e);
	};

	// view

	template <typename... Components> [[nodiscard]] auto view() {
		return reg.view<Components...>();
	};

	template <typename... Components> [[nodiscard]] auto view() const {
		return reg.view<Components...>();
	};

	// emplace

	template <typename Type, typename... Args>
	decltype(auto) emplace(entt::entity e, Args &&...args) {
		return reg.emplace<Type>(e, std::forward<Args>(args)...);
	}

	// get

	template <typename... Components>
	[[nodiscard]] decltype(auto) get(const entt::entity &e) {
		return reg.get<Components...>(e);
	};

	template <typename... Components>
	[[nodiscard]] decltype(auto) get(const entt::entity &e) const {
		return reg.get<Components...>(e);
	};

	// try_get

	template <typename... Components>
	[[nodiscard]] decltype(auto) tryGet(const entt::entity &e) {
		return reg.try_get<Components...>(e);
	};

	template <typename... Components>
	[[nodiscard]] decltype(auto) tryGet(const entt::entity &e) const {
		return reg.try_get<Components...>(e);
	};

	// remove
	template <typename... Components> void remove(const entt::entity &e) {
		reg.remove<Components...>(e);
	};
};

}; // namespace game
