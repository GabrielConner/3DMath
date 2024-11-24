#pragma once
#include "OpenGL.h"
#include "TypeDefs.h"
#include <iostream>

struct Vec2 {
	float x = 0;
	float y = 0;

	Vec2() : x(0), y(0) {}
	Vec2(float val) : x(val), y(val) {}
	Vec2(float m, float n) : x(m), y(n) {}

	glm::vec2 toGLM() {
		return glm::vec2(x, y);
	}

	float magnitude() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Vec2& operator -=(const float val) {
		this->x -= val;
		this->y -= val;
		return *this;
	}
	Vec2& operator -=(const Vec2& val) {
		this->x -= val.x;
		this->y -= val.y;
		return *this;
	}
	Vec2& operator +=(const float val) {
		this->x += val;
		this->y += val;
		return *this;
	}
	Vec2& operator +=(const Vec2& val) {
		this->x += val.x;
		this->y += val.y;
		return *this;
	}
	Vec2& operator *=(const Vec2& val) {
		this->x *= val.x;
		this->y *= val.y;
		return *this;
	}
	Vec2& operator =(const Vec2& val) {
		this->x = val.x;
		this->y = val.y;
		return *this;
	}
	Vec2 operator *(const Vec2& val) {
		Vec2 temp = *this;
		temp.x *= val.x;
		temp.y *= val.y;
		return temp;
	}
	Vec2 operator /(const Vec2& val) {
		Vec2 temp = *this;
		temp.x /= val.x;
		temp.y /= val.y;
		return temp;
	}
	Vec2 operator -(const Vec2& val) {
		Vec2 temp = *this;
		temp.x -= val.x;
		temp.y -= val.y;
		return temp;
	}
	Vec2 operator +(const Vec2& val) {
		Vec2 temp = *this;
		temp.x += val.x;
		temp.y += val.y;
		return temp;
	}

    friend std::ostream& operator <<(std::ostream& stream, const Vec2& val);
};

struct iVec2 {
	int x = 0;
	int y = 0;

	iVec2() : x(0), y(0) {}
	iVec2(int val) : x(val), y(val) {}
	iVec2(int m, int n) : x(m), y(n) {}
	iVec2(double m, int n) : x(m), y(n) {}
	iVec2(int m, double n) : x(m), y(n) {}
	iVec2(double m, double n) : x(m), y(n) {}
};

struct uiVec2 {
	uint x = 0;
	uint y = 0;

	uiVec2() : x(0), y(0) {}
	uiVec2(uint val) : x(val), y(val) {}
	uiVec2(uint m, uint n) : x(m), y(n) {}
};

struct iVec3 {
    int x = 0;
    int y = 0;
    int z = 0;

    iVec3() : x(0), y(0), z(0) {}
    iVec3(int val) : x(val), y(val), z(val) {}
    iVec3(int m, int n, int o) : x(m), y(n), z(o) {}
};

struct Vec3 {
    float x = 0;
    float y = 0;
    float z = 0;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float val) : x(val), y(val), z(val) {}
    Vec3(float m, float n, float o) : x(m), y(n), z(o) {}
    Vec3(Vec2 vec) : x(vec.x), y(vec.y), z(0) {}
    Vec3(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
    Vec3(glm::vec4 vec) : x(vec.x), y(vec.y), z(vec.z) {}

    glm::vec3 toGLM() {
        return glm::vec3(x, y, z);
    }

    Vec2 toVec2() {
        return Vec2(x, y);
    }

    float magnitude() {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    Vec3& operator -=(const float val) {
        this->x -= val;
        this->y -= val;
        this->z -= val;
        return *this;
    }
    Vec3& operator -=(const Vec3& val) {
        this->x -= val.x;
        this->y -= val.y;
        this->z -= val.z;
        return *this;
    }
    Vec3& operator +=(const float val) {
        this->x += val;
        this->y += val;
        this->z += val;
        return *this;
    }
    Vec3& operator +=(const Vec3& val) {
        this->x += val.x;
        this->y += val.y;
        this->z += val.z;
        return *this;
    }
    Vec3& operator *=(const Vec3& val) {
        this->x *= val.x;
        this->y *= val.y;
        this->z *= val.z;
        return *this;
    }
    Vec3& operator =(const Vec3& val) {
        this->x = val.x;
        this->y = val.y;
        this->z = val.z;
        return *this;
    }
    Vec3 operator *(const Vec3& val) {
        Vec3 temp = *this;
        temp.x *= val.x;
        temp.y *= val.y;
        temp.z *= val.z;
        return temp;
    }
    Vec3 operator /(const Vec3& val) {
        Vec3 temp = *this;
        temp.x /= val.x;
        temp.y /= val.y;
        temp.z /= val.z;
        return temp;
    }
    Vec3 operator -(const Vec3& val) {
        Vec3 temp = *this;
        temp.x -= val.x;
        temp.y -= val.y;
        temp.z -= val.z;
        return temp;
    }
    Vec3 operator +(const Vec3& val) {
        Vec3 temp = *this;
        temp.x += val.x;
        temp.y += val.y;
        temp.z += val.z;
        return temp;
    }

    friend std::ostream& operator <<(std::ostream& stream, const Vec3& val);
};

struct Vec4 {
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;

    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(float val) : x(val), y(val), z(val), w(val) {}
    Vec4(float ant, float boat, float cat, float dog) : x(ant), y(boat), z(cat), w(dog) {}

    glm::vec4 toGLM() {
        return glm::vec4(x, y, z, w);
    }

    Vec4& operator =(const Vec4& val) {
        this->x = val.x;
        this->y = val.y;
        this->z = val.z;
        this->w = val.w;
        return *this;
    }
};