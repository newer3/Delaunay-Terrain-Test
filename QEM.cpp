#include "QEM.h"

using namespace std;
std::pair<float, glm::vec3> MeshS::getErrorandVertex(glm::mat4 Q, glm::vec3 v1, glm::vec3 v2) {

	glm::mat4 A;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i < 3)
				A[i][j] = Q[i][j];
			else if (j < 3)
				A[i][j] = 0;
			else
				A[i][j] = 1;
		}
	}

	float determinant = glm::determinant(A);
	if (abs(determinant) > 0.000001) {//可逆，二次型有最小值
		glm::mat4 inverseA = glm::inverse(A);
		glm::vec4 b = { 0,0,0,1 };
		glm::vec4 v_bar = inverseA * b;
		glm::vec4 temp = Q * v_bar;
		float error = glm::dot(temp, v_bar);
		glm::vec3 ans;
		for (int i = 0; i < 3; i++) ans[i] = v_bar[i] / v_bar[3];
		return std::make_pair(error, ans);
	}
	else {//不可逆，取中点作为结果
		glm::vec3 ans;
		for (int i = 0; i < 3; i++) ans[i] = (v1[i] + v2[i]) / 2;
		glm::vec4 v_bar;
		for (int i = 0; i < 3; i++) v_bar[i] = ans[i];
		v_bar[3] = 1;
		glm::vec4 temp = Q * v_bar;
		float error = glm::dot(temp, v_bar);
		return std::make_pair(error, ans);
	}

}


glm::vec4 MeshS::getPlane(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC)
{
	glm::vec4 result = { 0,0,0,0 };
	glm::vec3 N = glm::cross((pointA - pointB), (pointC - pointB));

	result[0] = N[0];
	result[1] = N[1];
	result[2] = N[2];
	result[3] = -N[0] * pointA[0] - N[1] * pointA[1] - N[2] * pointA[2];
	float len = std::sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]);
	result[0] /= len;
	result[1] /= len;
	result[2] /= len;
	result[3] /= len;
	return result;

}