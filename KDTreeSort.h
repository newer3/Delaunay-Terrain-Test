#pragma once
#ifndef KDTREESORT_H
#define KDTREESORT_H
#include<iostream>
#include<vector>

#include <glm.hpp>

struct KDNode {
    glm::vec3 point;
    KDNode* left;
    KDNode* right;
};
class KDTree {
public:
    void buildKdTree(std::vector<glm::vec3> points);
    void buildKdTree2(std::vector<glm::vec3> points);
    void getSortPoints();
    std::vector<glm::vec3> rawData;
    std::vector<glm::vec3> sortData;
private:
    KDNode* root_ =nullptr;
    KDNode* setNode(std::vector<glm::vec3> points, int depth);
    KDNode* setNode(std::vector<glm::vec3> points);
};


#endif