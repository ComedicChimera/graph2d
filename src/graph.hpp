#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

#include "ast.hpp"

#define RESOLUTION 500

std::vector<float> buildGraphMesh(std::unique_ptr<ASTNode>& node, double xmin, double xmax, double ymin, double ymax);

#endif