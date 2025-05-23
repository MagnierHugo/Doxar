#pragma once

#include "Component.hpp"


class Script : public Component {
public:
	Script() = default;
	Script(const Script&) = default;
	Script(Script&&) = default;
	~Script() = default;

	virtual nlohmann::json Serialize() {
		return nlohmann::json();
	}

	virtual void Deserialize(nlohmann::json json) { }

	virtual void OnInspect() { }

};