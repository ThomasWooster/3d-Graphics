#pragma once

#include "OBJMesh.h"
#include <glm/mat4x4.hpp>

class OBJObject
{
public:
	OBJObject(glm::mat4 transform, aie::OBJMesh* object);
	virtual ~OBJObject();

	glm::mat4 getTransform() { return m_transform; }
	aie::OBJMesh* getOBJMesh() { return m_objectMesh; }

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_objectMesh;
};

