#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO;
    Hit intersection = {0,0,0};
    vec3 ec = ray.endpoint-center;
    double a = dot(ray.direction, ray.direction); 
    double b = 2*dot(ray.direction, ec);
    double c = dot(ec, ec) - radius*radius; 
    double dis = b*b - 4*a*c; 

    if (dis >= 0) {
        double t1 = ( -1*b - sqrt(dis) )/ (2*a);
        
        intersection.dist = t1; 
        intersection.object = this;
        return intersection;
    }
    return intersection; 

}
    

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    //TODO; // compute the normal direction
    normal = (point - center)/radius;
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
