#pragma once

namespace comp {
	/**
	 * @brief Store the scene graph as a sort of linked list (can be ordered to be continuous in memory)
	 * @link https://skypjack.github.io/2019-06-25-ecs-baf-part-4/
	 * @link https://skypjack.github.io/2019-08-20-ecs-baf-part-4-insights/
	 *
	 * @exemple
	 *
	 *	auto curr = comp.first;
	 *	for (unsigned int i; i < comp.children; ++i) {
	 *		curr = registry.get<relationship>(curr).next;
	 *	}
	 *
	 */
	struct Relationship {
		unsigned int childCount = 0;
		entt::entity first = { entt::null };
		entt::entity next = { entt::null };
		entt::entity parent = { entt::null };
	};
}
