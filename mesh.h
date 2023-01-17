#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "hittable.h"
#include "vec3.h"
#include "happly.h"
#include <vector>

class mesh : public hittable {
    public:
        // mesh(shared_ptr<std::vector<vec3>> vertices, shared_ptr<material> m) {
        //     this->vertices = vertices;
        //     this->mat_ptr = m;
        //     this->build_triangles();
        // }
        mesh(const std::string& filename, shared_ptr<material> m, const vec3& offset = vec3(0, 0, 0), const vec3& scale = vec3(1, 1, 1)) {
            this->mat_ptr = m;
            this->vertices = make_shared<std::vector<vec3>>();
            happly::PLYData plyIn(filename);
            std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
            std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();
            for (const auto& face : fInd) {
                vec3 vecs[3];
                int i = 0;
                for (const auto& index : face) {
                    vec3 vertex = scale * vec3(vPos[index][0], vPos[index][1], vPos[index][2]) + offset;
                    this->vertices->push_back(vertex);
                    vecs[i] = vertex;
                    i++;
                }
                this->triangles.push_back(make_shared<triangle>(vecs[0], vecs[1], vecs[2], mat_ptr));
            }

            this->bounding_box = this->get_bounding_box();
        };

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual std::array<vec3, 2> get_bounding_box() const;

    private:
        virtual void build_triangles();

    public:
        shared_ptr<std::vector<vec3>> vertices;
        shared_ptr<material> mat_ptr;
        std::vector<shared_ptr<triangle>> triangles;
        std::array<vec3, 2> bounding_box;
};

void mesh::build_triangles() {
    for (int i = 0; i < vertices->size(); i += 3) {
        triangles.push_back(make_shared<triangle>(
            vertices->at(i), vertices->at(i + 1), vertices->at(i + 2), mat_ptr));
    }
}

std::array<vec3, 2> mesh::get_bounding_box() const {
    // Bounding box with orientation facing the camera
    double min_x, min_y, min_z = infinity;
    double max_x, max_y, max_z = -infinity;
    for (int i = 0; i < vertices->size(); ++i) {
        vec3 v = vertices->at(i);
        double x, y, z = v[0], v[1], v[2];
        if (x > max_x)
            max_x = x;
        if (x < min_x)
            min_x = x;
        if (y > max_y)
            max_y = y;
        if (y < min_y)
            min_y = y;
        if (z > max_z)
            max_z = z;
        if (z < min_z)
            min_z = z;     
    }

    return {
        vec3(min_x, min_y, min_z),
        vec3(max_x, max_y, max_z)
    };
}

bool mesh::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // Smarter way to do this would be to use a bounding box

    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& triangle : triangles) {
        if (triangle->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
};

#endif