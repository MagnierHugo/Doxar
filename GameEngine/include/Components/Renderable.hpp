#pragma once

#include "Component.hpp"

class Renderable : public Component {

public:
	virtual inline void Render() const = 0;
	static void SetRenderer(class Renderer* renderer_) {
		renderer = renderer_;
	}
protected:
		static class Renderer* renderer;

private:

};