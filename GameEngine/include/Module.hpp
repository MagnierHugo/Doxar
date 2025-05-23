#pragma once

class Module {
public:
	Module() = default;
    virtual ~Module() = default;
    virtual inline void Init() { }
	virtual inline void Update() { }
	virtual inline void Render() { }
	virtual inline void Shutdown() { }
protected:
};