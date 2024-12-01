#pragma once
#include "OpenGL.h"
#include "Vector.h"
#include "MathConstants.h"

struct Camera {
    Vec3 position;
    Vec3 rotation;
    float FOV;
    float nearClip;
    float farClip;

    glm::mat4 perspectiveView;
    glm::mat4 orthographicView;
    glm::mat4 viewMatrix;

    Camera(Vec3 pos, Vec3 rot, float fov, float N, float F) : position(pos), rotation(rot), FOV(fov), nearClip(N), farClip(F), perspectiveView(0), orthographicView(0), viewMatrix(1) {}

    void setPerspectiveView(float fov, float ratio, float N, float F) {
        FOV = fov;
        nearClip = N;
        farClip = F;
        perspectiveView = glm::perspective(FOV, ratio, nearClip, farClip);
    }

    void setOrthographicView(float width, float height, float depth) {
        orthographicView = glm::ortho(-width, width, -height, height, -depth, depth);
    }

    void setViewMatrix() {
        viewMatrix = glm::mat4(1);
        viewMatrix = glm::rotate(viewMatrix, rotation.x * (float)_degToRad, glm::vec3(1, 0, 0));
        viewMatrix = glm::rotate(viewMatrix, rotation.y * (float)_degToRad, glm::vec3(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, rotation.z * (float)_degToRad, glm::vec3(0, 0, 1));
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-position.x, position.y, position.z));
    }

    Vec3 directionFacing() {
        Vec3 ret;
        
        ret.x = cos(_degToRad * (rotation.y - 90)) * cos(_degToRad * rotation.x);
        ret.y = sin(_degToRad * rotation.x);
        ret.z = cos(_degToRad * rotation.x) * sin(_degToRad * (rotation.y + 90));

        return ret;
    }

    void setRotation(Vec3 rot) {
        rotation.x = glm::mod(rot.x ,360.f);
        rotation.y = glm::mod(rot.y ,360.f);
        rotation.z = glm::mod(rot.z ,360.f);
    }

    void addRotation(Vec3 rot) {
        rotation.x = glm::mod(rotation.x + rot.x, 360.f);
        rotation.y = glm::mod(rotation.y + rot.y, 360.f);
        rotation.z = glm::mod(rotation.z + rot.z, 360.f);
    }

    void setRotation(float x, float y ,float z) {
        rotation.x = glm::mod(x, 360.f);
        rotation.y = glm::mod(y, 360.f);
        rotation.z = glm::mod(z, 360.f);
    }

    void addRotation(float x, float y, float z) {
        rotation.x = glm::mod(rotation.x + x, 360.f);
        rotation.y = glm::mod(rotation.y + y, 360.f);
        rotation.z = glm::mod(rotation.z + z, 360.f);
    }
};