#pragma once

#include <string>


class Resource {

public:
	virtual bool Load(const std::string& filePath) = 0;
	virtual void Cleanup() = 0;

private:

protected:

};