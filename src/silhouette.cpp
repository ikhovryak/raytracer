#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"

#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"
using namespace glm;
using namespace agl;

float hit_sphere(const ray& ray, const vec3& center, float radius)
{
    vec3 oc = ray.origin() - center;
    float a = dot(ray.direction(), ray.direction());
    float b = 2.0f * dot(oc, ray.direction());
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return -1;
    return (-b - sqrt(discriminant)) / 2 * a;
}

color ray_color(const ray& ray)
{
    if (hit_sphere(ray, point3(0.0f, 0.0f, -1.0f), 0.5) > 0) {
        return color(1, 0, 0);
    }

    vec3 dir = normalize(ray.direction());
    float t = 0.5f * (dir.y + 1.0f);
    color white = color(1.0f, 1.0f, 1.0f);
    color lightBlue = color(0.5f, 0.7f, 1.0f);
    return white * (1 - t) + lightBlue * t;
}

void ray_trace(ppm_image& image)
{
    int height = image.height();
    int width = image.width();
    float aspect = width / (float)height;
    float world_height = 2.0f;
    float world_width = world_height * aspect;
    float focal_length = 1.0f;
    vec3 camera_pos = vec3(0);
    vec3 lower_left = camera_pos
        - 0.5f * vec3(world_width, world_height, 0)
        - vec3(0, 0, focal_length);
    for (int i = 0; i < height; i++) // rows
    {
        for (int j = 0; j < width; j++) // cols
        {
            float u = j / ((float)width - 1);
            float v = (height - i - 1) / ((float)height - 1);
            vec3 world_pos = lower_left +
                vec3(u * world_width, v * world_height, 0);
            color c = ray_color(ray(camera_pos, world_pos - camera_pos));
            image.set_vec3(i, j, c);
        }
    }
   image.save("silhouette.png");
}
