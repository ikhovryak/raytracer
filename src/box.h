#ifndef BOX_H_
#define BOX_H_

#include "hittable.h"
#include "AGLM.h"

float minim(vec3 a) {
    return glm::min(glm::min(a.x, a.y), a.z);
}

float maxim(vec3 a) {
    return glm::max(glm::max(a.x, a.y), a.z);
}

class box : public hittable {
public:
   box() : c(0), ax(0), ay(0), az(0), hx(0), hy(0), hz(0), mat_ptr(0) {}
   box(const glm::point3& center, 
       const glm::vec3& xdir, const glm::vec3& ydir, const glm::vec3& zdir,
       const glm::vec3& halfx, const glm::vec3& halfy, const glm::vec3& halfz,
       std::shared_ptr<material> m) : c(center), ax(xdir), ay(ydir), az(zdir), 
          hx(halfx), hy(halfy), hz(halfz), mat_ptr(m) {};


   virtual bool hit(const ray& r, hit_record& rec) const override
   {

       float tmin = -INFINITY;
       float tmax = INFINITY;
       vec3 as[3] = { ax, ay, az };
       vec3 hs[3] = { hx, hy, hz };
       
       vec3 p = c - r.orig;
       vec3 cur_norm;
       for (int i = 0; i < 3; i++) {
           cur_norm = as[i];


       for (int i = 0; i < 3; i++) {
           float e = dot(as[i], p);
           float f = dot(as[i], r.direction());
           if (abs(f) > std::numeric_limits<double>::epsilon()) {
               float t1 = (e + length(hs[i])) / f;
               float t2 = (e - length(hs[i])) / f;

               if (t1 > t2)
               {
                   float temp = t1;
                   t1 = t2;
                   t2 = t1;
               }

               if (t1 > tmin) {
                   tmin = t1;
                   cur_norm = as[i];
                   // normal is as[i]
               }
               if (t2 < tmax) {
                   tmax = t2;
                   cur_norm = as[i]; 

               }

               if (tmin > tmax) return false;
               if (tmax < 0) return false;
           }
           else if ((-e - length(hs[i]) > 0) || (-e + length(hs[i]) < 0)) {
               return false;
           }
       }
       }
       float t;
       if (tmin > 0) t = tmin;
       else t = tmax;
       
       rec.t = t; // save the time when we hit the object
       rec.p = r.at(t); // ray.origin + t * ray.direction
       rec.mat_ptr = mat_ptr;

       rec.set_face_normal(r, cur_norm);
       return true;
   }

public:
   glm::vec3 c;
   glm::vec3 ax;
   glm::vec3 ay;
   glm::vec3 az;
   glm::vec3 hx;
   glm::vec3 hy;
   glm::vec3 hz;
   std::shared_ptr<material> mat_ptr;
};

#endif
