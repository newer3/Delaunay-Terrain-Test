#ifndef QEM_H
#define QEM_H

#include<iostream>
#include<vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class MeshS {
public:
	glm::vec4 getPlane(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC);
	std::pair<float, glm::vec3> getErrorandVertex(glm::mat4 Q, glm::vec3 v1, glm::vec3 v2);

private:

};
#endif