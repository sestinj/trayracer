#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.h"
#include "hittable.h"
#include "material.h"


class triangle : public hittable {
    public:
        triangle(const vec3& a, const vec3& b, const vec3& c, shared_ptr<material> m) {
            this->a = a;
            this->b = b;
            this->c = c;
            this->normal = unit_vector(cross(b - a, c - a));
            this->mat_ptr = m;
            this->rhs = dot(normal, a);
        }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        vec3 a;
        vec3 b;
        vec3 c;
        vec3 normal;
        shared_ptr<material> mat_ptr;
        double rhs; // RHS of the plane equation
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Find where the ray intersects the plane of the triangle
    if (dot(normal, r.direction()) < 1e-8) {
        return false;
    };

    double t = (rhs - dot(normal, r.origin())) / dot(normal, r.direction());
    if (t < t_min || t_max < t) {
        return false;
    }

    point3 p = r.at(t);

    if (!inside_triangle(p, a, b, c))
        return false;
    
    rec.t = t;
    rec.p = p;
    vec3 outward_normal = dot(normal, r.direction()) < 0 ? normal : -normal;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif