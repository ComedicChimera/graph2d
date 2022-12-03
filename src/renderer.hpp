#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "ast.hpp"

#include <stdexcept>

class ConfigError : public std::exception {
    std::string message;

public:
    ConfigError(const std::string& message) 
    : message(message)
    {}

    const char* what() const override { return message.c_str(); }
};

/* -------------------------------------------------------------------------- */

void initRenderer(std::unique_ptr<ASTNode>& expr, int width, int height);
void runRenderLoop();

#endif