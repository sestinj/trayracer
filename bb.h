#ifndef BB_H
#define BB_H

#include "vec3.h"
#include "ray.h"

class bb {
    public:
        bb(const vec3& min, const vec3&max) : min(min), max(max) {}

        virtual bool hit(const ray& r, double t_min, double t_max) const;

    public:
        vec3 min;
        vec3 max;
};

#endif