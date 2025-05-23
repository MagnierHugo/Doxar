#pragma once

#include <Core/Maths/vec3.hpp>
#include <Core/Maths/vec2.hpp>

#include "Color.hpp"


struct Vertex {
	/**
	 * \brief Union field allowing to access the same data using different names
	 */
	union {
		struct {
			struct { float x, y, z; } m_position; 
			struct { float u, v; } m_textureCoord; 
			struct { float nx, ny, nz; } m_normal;
		};
		float m_data[8]{ .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f };
	};

	/**
	 * \brief Default Ctor
	 */
	Vertex() = default;
	/**
	 * \brief Default Dtor
	 */
	~Vertex() = default;

	/**
	 * \brief Ctor with 2 glm::vec3 and glm::vec2
	 * \param pos -> Vertex Position as glm::vec3
	 * \param texCoord -> Texture pixel position
	 * \param normal -> Vertex Normal
	 */
	Vertex(const glm::vec3 position, const glm::vec2 textureCoord, const glm::vec3 normal);

	/**
	 * \brief Ctor with glm::vec3 and glm::vec2
	 * \param pos -> Vertex Position as glm::vec3
	 * \param texCoord -> Texture pixel position
	 */
	Vertex(const glm::vec3 position, const glm::vec2 textureCoord);

	/**
	 * \brief Ctor with 2 glm::vec3
	 * \param pos -> Vertex Position as glm::vec3
	 * \param normal -> Vertex Normal
	 */
	Vertex(const glm::vec3 position, const glm::vec3 normal);

	/**
	 * \brief Ctor with glm::vec3 and glm::vec2
	 * \param texCoord -> Texture pixel position
	 * \param normal -> Vertex Normal
	 */
	Vertex(const glm::vec2 textureCoord, const glm::vec3 normal);


	void SetPosition(const float x, const float y, const float z);
	void SetPosition(const glm::vec3 position);

	void SetNormal(const float nx, const float ny, const float nz);
	void SetNormal(const glm::vec3 normal);

	void SetTextureCoord(const float u, const float v);
	void SetTextureCoord(const glm::vec2 textureCoord);
};