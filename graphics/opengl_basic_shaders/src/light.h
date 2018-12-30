#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>
#include "entity.h"

struct Light
{
    int shaderPositionLoc;
    Entity transform;

    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 matDiffuse;
    glm::vec4 matSpecular;

    glm::vec4 getPos()
    {
    	return glm::vec4(transform.position, 0.0);
    }

    glm::vec4 getSpec()
    {
    	return specular * matSpecular;
    }

    glm::vec4 getDiffuse()
    {
    	return diffuse * matDiffuse;
    }
};

#endif
