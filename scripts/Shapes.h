#pragma once
constexpr float triangleVertices[] = {
	-0.5, -0.5, 0, 0, 0,
	0.5, -0.5, 0, 1, 0,
	0, 0.5, 0, 1, 0
};
constexpr unsigned int triangleIndices[] = {
	0, 1, 2
};

constexpr float squareVertices[] = {
	-1, 1, 0, 0, 1,
	1, 1, 0, 1, 1,
	1, -1, 0, 1, 0,
	-1, -1, 0, 0, 0
};
constexpr unsigned int squareIndices[] = {
	0, 2, 1,
	0, 3, 2
};

constexpr float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f, 0, 0,
    -0.5f, 0.5f, -0.5f, 0, 0,
    0.5f, -0.5f, -0.5f, 0, 0,
    0.5f, 0.5f, -0.5f, 0, 0,

    -0.5f, -0.5f, 0.5f, 0, 0,
    -0.5f, 0.5f, 0.5f, 0, 0,
    0.5f, -0.5f, 0.5f, 0, 0,
    0.5f, 0.5f, 0.5f, 0, 0
};

constexpr unsigned int cubeIndices[] = {
    0, 1, 2,
    2, 1, 3,

    0, 4, 2,
    4, 2, 6,

    1, 7, 2,
    2, 7, 6,

    0, 5, 1,
    0, 4, 5,

    1, 5, 3,
    3, 5, 7,

    4, 6, 5,
    6, 5, 7
};