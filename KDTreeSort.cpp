#include "KDTreeSort.h"
#include <algorithm>
#include <queue>

bool compareByX(const glm::vec3& a, const glm::vec3& b);
bool compareByY(const glm::vec3& a, const glm::vec3& b);

void KDTree::buildKdTree(std::vector<glm::vec3> points) {
	if (points.empty()) {
		root_ = nullptr;
		return;
	}
	rawData = points;
	root_ = setNode(rawData,0);
}
void KDTree::buildKdTree2(std::vector<glm::vec3> points) {
	if (points.empty()) {
		root_ = nullptr;
		return;
	}
	rawData = points;
	root_ = setNode(rawData);
}
KDNode* KDTree::setNode(std::vector<glm::vec3> points, int depth) {
	if (points.empty()) return nullptr;
	if (depth % 2 == 0) sort(points.begin(), points.end(), compareByX);
	else sort(points.begin(), points.end(), compareByY);
	KDNode* node = new KDNode();
	int mid = points.size() / 2;
	std::vector<glm::vec3> leftpoints(points.begin(), points.begin() + mid);
	std::vector<glm::vec3> rightpoints(points.begin()+mid+1, points.end());
	node->point = points[mid];
	node->left = setNode(leftpoints, depth + 1);
	node->right = setNode(rightpoints, depth + 1);
	return node;
}
KDNode* KDTree::setNode(std::vector<glm::vec3> points) {
	if (points.empty()) return nullptr;
	sort(points.begin(), points.end(), compareByX);
	std::vector<glm::vec3> pointsSortByX = points;
	sort(points.begin(), points.end(), compareByY);
	KDNode* node = new KDNode();
	int mid = points.size() / 2;
	int dx =pointsSortByX.back().x - pointsSortByX.front().x;
	int dy = points.back().y - points.front().y;
	if (dx > dy) points = pointsSortByX;
	std::vector<glm::vec3> leftpoints(points.begin(), points.begin() + mid);
	std::vector<glm::vec3> rightpoints(points.begin() + mid + 1, points.end());
	node->point = points[mid];
	node->left = setNode(leftpoints);
	node->right = setNode(rightpoints);
	return node;
}

void KDTree::getSortPoints() {
	if (root_ == nullptr) return;
	std::queue<KDNode*> width;
	width.push(root_);
	while (!width.empty()) {
		KDNode* current = width.front();
		sortData.push_back(current->point);
		width.pop();
		if (current->left != nullptr) {
			width.push(current->left);
		}
		if (current->right != nullptr) {
			width.push(current->right);
		}
	}
}

bool compareByX(const glm::vec3& a, const glm::vec3& b) {
	return a.x < b.x;
}
bool compareByY(const glm::vec3& a, const glm::vec3& b) {
	return a.y < b.y;
}