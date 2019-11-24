#include "OBJObject.h"



OBJObject::OBJObject(glm::mat4 transform, aie::OBJMesh* object)
{
	m_transform = transform;
	m_objectMesh = new aie::OBJMesh();
	m_objectMesh = object;

}


OBJObject::~OBJObject()
{
	delete(m_objectMesh);
}
