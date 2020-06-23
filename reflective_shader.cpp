#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

#include <iostream>
using namespace std; 
vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color, s_color;
    vec3 r_color;

    double max_t = std::numeric_limits<int>::max();
    double s = 0; 
    vec3 rp, rd;
    Hit h; 

    // find what objects are intersecting
    color = shader->Shade_Surface(ray, intersection_point,normal,recursion_depth);
    rd = (ray.direction - (2*dot(ray.direction,normal)*normal)); //(-1.0*ray.direction) + 2.0*normal * dot(normal, ray.direction)).normalized(); 
    rp = ray.endpoint-intersection_point; 
    s = sqrt(pow(rp[0], 2.0) + pow(rp[1], 2.0) + pow(rp[2], 2.0)); 
    Ray r(intersection_point, rd);

    // recursively calculate reflectivity
    if (recursion_depth >= world.recursion_depth_limit) {
        return (1-reflectivity) * color; 
    }

    // calculate color
    r_color = world.Cast_Ray(r,++recursion_depth); 
    color = (1-reflectivity) * color + reflectivity*r_color;  
 
    return color;
}
