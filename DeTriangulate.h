#ifndef DETRIANGULATE_H
#define DETRIANGULATE_H
#include<iostream>
#include<vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class DT {
public:
	std::vector<glm::vec3> vertices;
	std::vector<std::vector<int>> triangle;
	std::vector<std::vector<int>> adj;
	std::vector<std::vector<int>> result;
	int rationcount = 0;
	int localcount = 0;

	void genMesh(std::vector<glm::vec3> points);
	void rationalize(int t);
	bool isPointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c );
	bool isPointInCircle(glm::vec3 o ,glm::vec3 a, glm::vec3 b, glm::vec3 p);
};
#endif // !DETRIANGULATE_H 
