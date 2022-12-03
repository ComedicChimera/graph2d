# Graph 2D

A 2D graphing calculator powered by OpenGL and the marching squares algorithm.

**Note:** I wrote this app in a day for a fun and to play around with marching
squares.  It is by no means a very good graphing calculator.  In particular, it
struggles quite a bit with discontinuities.  Use at your own risk.

## Building

You can compile `graph2d` using CMake.  Note that you will need to modify the
`CMakeLists.txt` to point to wherever your local versions of GLFW and GLAD are
installed.

## Usage

Call the `graph2d` executable passing an equation of the form:

    f(x, y) = g(x, y)

A window will open containing the graph.

For example, a call of the form:

    graph2d "y = x^2"

will graph a parabola, and a call of the form:

    graph2d "x^2 + y^2 = 25"

will graph a circle.

