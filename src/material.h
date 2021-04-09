#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>
#include "AGLM.h"
#include "ray.h"
#include "hittable.h"

using namespace glm;
//using namespace agl;
using namespace std;

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec,
        glm::color& attenuation, ray& scattered) const = 0;
    virtual ~material() {}
};

class lambertian : public material {
public:
    lambertian(const glm::color& a) : albedo(a) 
    {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
        glm::color& attenuation, ray& scattered) const override
    {
        // todo
        vec3 scatter_direction = rec.normal + random_unit_vector();
        if (near_zero(scatter_direction))
        {
            scatter_direction = rec.normal;
        }
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    glm::color albedo;
};

class phong : public material {
public:
    phong(const glm::vec3& view) :
        diffuseColor(0, 0, 1),
        specColor(1, 1, 1),
        ambientColor(.01f, .01f, .01f),
        lightPos(5, 5, 0),
        viewPos(view),
        kd(0.45), ks(0.45), ka(0.1), shininess(10.0)
    {}

    phong(const glm::color& idiffuseColor,
        const glm::color& ispecColor,
        const glm::color& iambientColor,
        const glm::point3& ilightPos,
        const glm::point3& iviewPos,
        float ikd, float iks, float ika, float ishininess) :
        diffuseColor(idiffuseColor),
        specColor(ispecColor),
        ambientColor(iambientColor),
        lightPos(ilightPos),
        viewPos(iviewPos), kd(ikd), ks(iks), ka(ika), shininess(ishininess)
    {}

    virtual bool scatter(const ray& r_in, const hit_record& hit,
        glm::color& attenuation, ray& scattered) const override
    {
        // todo
        attenuation = glm::color(0);
        return false;
    }

public:
    glm::color diffuseColor;
    glm::color specColor;
    glm::color ambientColor;
    glm::point3 lightPos;
    glm::point3 viewPos;
    float kd;
    float ks;
    float ka;
    float shininess;
};

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

class metal : public material {
public:
    metal(const glm::color& a, float f) : albedo(a), fuzz(glm::clamp(f, 0.0f, 1.0f)) {}
    

    virtual bool scatter(const ray& r_in, const hit_record& rec,
        glm::color& attenuation, ray& scattered) const override
    {
        // todo
        

        vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    glm::color albedo;
    float fuzz;
};

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    
    vec3 n_times_cos_theta = vec3(n.x * cos_theta, n.y * cos_theta, n.z * cos_theta);
    vec3 n_plus_uv = uv + n_times_cos_theta;
    vec3 r_out_perp = vec3(n_plus_uv.x * etai_over_etat, n_plus_uv.y * etai_over_etat, n_plus_uv.z * etai_over_etat);
    float new_const = -sqrt(fabs(1.0 - dot(r_out_perp, r_out_perp)));
    vec3 r_out_parallel = vec3(n.x* new_const, n.y* new_const, n.z* new_const);
    return r_out_perp + r_out_parallel;
}

class dielectric : public material {
public:
    dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
        glm::color& attenuation, ray& scattered) const override
    {
        // todo
        /*attenuation = glm::color(0);
        return false;*/

        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = normalize(r_in.direction());
        vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, refracted);
        return true;
    }

public:
    float ir; // Index of Refraction
};


#endif
