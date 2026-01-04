#pragma once

#include "../terrain/outline.hpp"
#include "base.hpp"

namespace render {

struct TerrainChain : base {

	std::vector<std::vector<terrain::IslandPos>> outline;

	using base::base;
	void init() override {
		outline = terrain::MapOutline(d->scene.map);
	};
	void render() override {
		for (auto &o : outline) {
			SDL_SetRenderDrawColor(d->r, 255, 50, 50, 255);
			std::vector<SDL_FPoint> pts;
			for (auto &q : o) {
				pts.push_back(SDL_FPoint{
					100.0f + static_cast<float>(q.x * 30),
					d->window.h - 100.0f - static_cast<float>(q.y * 30),
				});
			}
			SDL_RenderLines(d->r, pts.data(), static_cast<int>(pts.size()));

			SDL_SetRenderDrawColor(d->r, 50, 255, 50, 255);

			auto &start = pts[0];
			auto &end = pts[pts.size() - 1];

			SDL_RenderLine(d->r, start.x, start.y, end.x, end.y);
		}
	};
};

}; // namespace render
