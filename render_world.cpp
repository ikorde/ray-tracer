#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <iostream>
using namespace std;

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    //TODO;
    Hit h = {0,0,0};
    Hit h_curr = {0,0,0};
    float min_t =1000000; // std::numeric_limits<int>::max();
    //cout << objects.size() << endl; 
    for (int i=0; i<objects.size(); ++i) {
 	//if(objects[i]->number_parts <= 1) {
	//	h_curr = objects[i]->Intersection(ray,0);
	//}
	//else {
		for(size_t j=0; j<objects[i]->number_parts; ++j) {
			h_curr = objects[i]->Intersection(ray,j);
		
	//}
        	if (h_curr.dist <= min_t && h_curr.dist>=small_t) {
           		 min_t = h_curr.dist;
            		h = h_curr;
        	}
	}
        
    }
    if (debug_pixel) {
         cout << "DEBUG: Closest_Intersection obj,dist: " << h.object << "  ,  " << h.part << " ,   " << h.dist << endl <<endl;
     }
    return h;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    //TODO; // set up the initial view ray here
    Ray ray;
    ray.endpoint = camera.position;
    vec3 wp = camera.World_Position(pixel_index);
    ray.direction = (wp - ray.endpoint).normalized(); 
    
    vec3 color=Cast_Ray(ray,1);

    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    //TODO; // determine the color here
    Hit h = Closest_Intersection(ray);
    //if (debug_pixel) cout << "casting " <<h.dist << endl; 
    vec3 n; 
    if (h.object != 0 ) {
//	for(int i =0; i < h.object->number_parts; ++i) {
        	n = h.object->Normal(ray.Point(h.dist),h.part); 
//	}
        color = h.object->material_shader->Shade_Surface(ray, ray.Point(h.dist), n, recursion_depth); 
        //if(debug_pixel) cout << "debugger color " << color << endl; 
    }
    else {
        color = background_shader->Shade_Surface(ray, color, color, recursion_depth); 
    }

    // if (debug_pixel) {
    //     cout << "DEBUG: Cast_Ray ep,dir,color: " << ray.endpoint << "  ,  " << ray.direction << "  ,  " << color << endl <<endl;
    // }
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    //TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
