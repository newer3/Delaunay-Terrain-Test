#include"DeTriangulate.h"

using namespace std;

void DT::genMesh(vector<glm::vec3> points)
{
	
	//big triangle
	points.push_back({-10,-10,0});
	points.push_back({ 10,-10,0 });
	points.push_back({ 0,10,0 });
	vertices = points;
	int Vnum = points.size();


	triangle.push_back({ Vnum - 3,Vnum - 2,Vnum - 1 });
	adj.push_back({ -1,-1,-1 });

	int Tnum = 1;

	//insert loop
	for (int p = 0; p < Vnum - 3; ++p) {
		glm:: vec3 P = points[p];
		//定位点在哪个三角形内
		int t = Tnum-1;
		while (t>=0&&(isPointInTriangle(P, points[triangle[t][0]], points[triangle[t][1]], points[triangle[t][2]]) == false)) t--;
		if (t < 0) {
			cout << "locate error" <<p<< endl;
			continue;
		}
		
		Tnum += 2;
		triangle.push_back({ p,triangle[t][1],triangle[t][2] });
		triangle.push_back({ p,triangle[t][2],triangle[t][0] });

		int adj1 = adj[t][0]; int adj2 = adj[t][1]; int adj3 = adj[t][2];
		if (adj1 >= 0) {
			for (int m = 0; m < 3; m++) {
				if (adj[adj1][m] == t) {
					adj[adj1][m] = t;
					break;
				}
			}
		}
		if (adj2 >= 0) {
			for (int m = 0; m < 3; m++) {
				if (adj[adj2][m] == t) {
					adj[adj2][m] = Tnum-2;
					break;
				}
			}
		}
		if (adj3 >= 0) {
			for (int m = 0; m < 3; m++) {
				if (adj[adj3][m] == t) {
					adj[adj3][m] = Tnum-1;
					break;
				}
			}
		}
		adj.push_back({ t,adj[t][1],Tnum - 1 });
		adj.push_back({ Tnum - 2,adj[t][2],t });

		triangle[t][2] = triangle[t][1];
		triangle[t][1] = triangle[t][0];
		triangle[t][0] = p;

		adj[t][1] = adj[t][0];
		adj[t][2] = Tnum - 2;
		adj[t][0] = Tnum - 1;

		if (adj[t][1] >= 0) rationalize(t);
		if (adj[Tnum - 2][1] >= 0) rationalize(Tnum - 2);
		if (adj[Tnum - 1][1] >= 0) rationalize(Tnum - 1);
	}
	for (int t = 0; t < triangle.size(); t++) {
		if ((triangle[t][0] < Vnum - 4) && (triangle[t][1] < Vnum - 4) && (triangle[t][2] < Vnum - 4))
			result.push_back(triangle[t]);
	}
}


void DT::rationalize(int t) {
	rationcount++;
	glm::vec3 v1 = vertices[triangle[t][2]];
	glm::vec3 v2 = vertices[triangle[t][1]];
	int o = -1;
	int oid = -1;
	for (int k = 0; k < 3; k++) {
		if ((triangle[adj[t][1]][k] != triangle[t][1]) && (triangle[adj[t][1]][k] != triangle[t][2])) {
			o = triangle[adj[t][1]][k];
			oid = k;
			break;
		}
	}


		glm::vec3 v3 = vertices[o];
		glm::vec3 p = vertices[triangle[t][0]];
		if (isPointInCircle(p, v1, v2, v3)) {
			int L = t;
			int R = adj[L][1];
			int C = adj[L][2];
			int A = adj[R][(oid + 2) % 3];

			if (A >= 0) {
				for (int m = 0; m < 3; m++) {
					if (adj[A][m] == R) {
						adj[A][m] = L;
						break;
					}
				}
			}

			if (C >= 0) {
				for (int m = 0; m < 3; m++) {
					if (adj[C][m] == L) {
						adj[C][m] = R;
						break;
					}
				}
			}
			for (int m = 0; m < 3; m++) {
				if (triangle[R][m] == o) {
					triangle[R][(m + 2) % 3] = triangle[t][0];
					break;
				}
			}
			for (int m = 0; m < 3; m++) {
				if (adj[R][m] == L) {
					adj[R][m] = C;
					break;
				}
			}
			for (int m = 0; m < 3; m++) {
				if (adj[R][m] == A) {
					adj[R][m] = L;
					break;
				}
			}
			for (int m = 0; m < 3; m++) {
				if (triangle[R][0] != triangle[t][0]) {
					int temp1 = triangle[R][0]; int temp2 = adj[R][0];
					triangle[R][0] = triangle[R][1];
					adj[R][0] = adj[R][1];
					triangle[R][1] = triangle[R][2];
					adj[R][1] = adj[R][2];
					triangle[R][2] = temp1;
					adj[R][2] = temp2;
				}
			}
			triangle[L][2] = o;
			for (int m = 0; m < 3; m++) {
				if (adj[L][m] == C) {
					adj[L][m] = R;
					break;
				}
			}
			for (int m = 0; m < 3; m++) {
				if (adj[L][m] == R) {
					adj[L][m] = A;
					break;
				}
			}
			if (adj[L][1] >= 0) rationalize(L); 
			if (adj[R][1] >= 0) rationalize(R);
		}
	
}

bool DT::isPointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	localcount++;
	glm::vec3 ab = b - a;
	glm::vec3 bc = c - b;
	glm::vec3 ca = a - c;
	glm::vec3 ap = p - a;
	glm::vec3 bp = p - b;
	glm::vec3 cp = p - c;

	float tolerance = 0.000001f;
	float negtolerance = -0.000001f;

	return (glm::cross(ab, ap).z >= negtolerance && glm::cross(bc, bp).z >= negtolerance && glm::cross(ca, cp).z >= negtolerance)
		|| (glm::cross(ab, ap).z <= tolerance && glm::cross(bc, bp).z <= tolerance && glm::cross(ca, cp).z <= tolerance);
}

bool DT::isPointInCircle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 o)
{
	float cosa = (a.x - o.x) * (b.x - o.x) + (a.y - o.y) * (b.y - o.y);
	float cosb = (b.x - p.x) * (a.x - p.x) + (b.y - p.y) * (a.y - p.y);
	float sina = (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
	float sinb = (b.x - p.x) * (a.y - p.y) - (b.y - p.y) * (a.x - p.x);
	return(((cosa < 0) && (cosb < 0)) || (-cosa * sinb > cosb * sina));
}
