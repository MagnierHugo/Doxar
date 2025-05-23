#include "Render/Vertex.hpp"


Vertex::Vertex(const glm::vec3 position, const glm::vec2 textureCoord, const glm::vec3 normal) {
	m_data[0] = position.x;
	m_data[1] = position.y;
	m_data[2] = position.z;
	m_data[3] = textureCoord.x;
	m_data[4] = textureCoord.y;
	m_data[5] = normal.x;
	m_data[6] = normal.y;
	m_data[7] = normal.z;
}

Vertex::Vertex(const glm::vec3 position, const glm::vec2 textureCoord) {
	m_data[0] = position.x;
	m_data[1] = position.y;
	m_data[2] = position.z;
	m_data[3] = textureCoord.x;
	m_data[4] = textureCoord.y;
}

Vertex::Vertex(const glm::vec3 position, const glm::vec3 normal) {
	m_data[0] = position.x;
	m_data[1] = position.y;
	m_data[2] = position.z;
	m_data[5] = normal.x;
	m_data[6] = normal.y;
	m_data[7] = normal.z;
}

Vertex::Vertex(const glm::vec2 textureCoord, const glm::vec3 normal) {
	m_data[3] = textureCoord.x;
	m_data[4] = textureCoord.y;
	m_data[5] = normal.x;
	m_data[6] = normal.y;
	m_data[7] = normal.z;
}


void Vertex::SetPosition(const float x, const float y, const float z) {
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
}

void Vertex::SetPosition(const glm::vec3 position) {
	m_data[0] = position.x;
	m_data[1] = position.y;
	m_data[2] = position.z;
}


void Vertex::SetNormal(const float nx, const float ny, const float nz) {
	m_data[5] = nx;
	m_data[6] = ny;
	m_data[7] = nz;
}

void Vertex::SetNormal(const glm::vec3 normal) {
	m_data[5] = normal.x;
	m_data[6] = normal.y;
	m_data[7] = normal.z;
}


void Vertex::SetTextureCoord(const float u, const float v) {
	m_data[3] = u;
	m_data[4] = v;
}

void Vertex::SetTextureCoord(const glm::vec2 textureCoord) {
	m_data[3] = textureCoord.x;
	m_data[4] = textureCoord.y;
}