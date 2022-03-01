#include <stdio.h>
#include <math.h>
#include <omp.h>
#include "vector.h"
#include "particle.h" 
#include "box_bounds.h"
#include "quad.h"
#include "forces.h"
//======================================================================================================
const double epsilon = 1e-3;
//======================================================================================================
#if VERSION > 1
int vector_in_box(vector_t v, box_bounds_t box) {
    return v.x < box.xupper && v.x > box.xlower && v.y < box.yupper && v.y > box.ylower;  
}
#endif
//======================================================================================================
vector_t r_vector(vector_t v1, vector_t v2) {
    vector_t r;

    r.x = v2.x - v1.x;
    r.y = v2.y - v1.y;

    return r;
}

double norm(vector_t v) {
    return sqrt(v.x*v.x + v.y*v.y);   
}

double norm3(vector_t v) {
    double n3;

    n3 = sqrt(v.x*v.x + v.y*v.y) + epsilon; 
    n3 = n3 * n3 * n3;
    return n3;
}
//======================================================================================================
#if VERSION < 3
int theta_bounds(quadtree_t* tree, particle_t particle, double theta_max) {
    double xwidth = tree->box.xupper - tree->box.xlower;

    if(xwidth / norm(r_vector(tree->centre_of_mass, particle.position)) < theta_max) {
        return 1;
    }
    else {
        return 0;
    }
}
#elif VERSION == 3
// Version 2 
int theta_bounds(quadtree_t* tree, particle_t particle, double theta_max, vector_t* r, double* n) {
    double xwidth = tree->box.xupper - tree->box.xlower;
    *r = r_vector(tree->centre_of_mass, particle.position);
    *n = norm(*r);

    if(xwidth / *n < theta_max) {
        return 1;
    }
    else {
        return 0;
    }
}

#else
// version 4
int theta_bounds(quadtree_t* tree, particle_t particle, double theta_max, vector_t* r, double* n) {
    *r = r_vector(tree->centre_of_mass, particle.position);
    *n = norm(*r);

    return tree->box.xupper - tree->box.xlower < theta_max * (*n);
}
#endif
//======================================================================================================
#if VERSION < 3
// Version 1 
vector_t quad_force(quadtree_t* tree, particle_t particle, double theta_max) {
    vector_t f;
    vector_t r;
    double a;

    if(tree == NULL) {
        f.x = 0.0;
        f.y = 0.0;
    } else if(!vector_in_box(particle.position, tree->box) && (theta_bounds(tree, particle, theta_max) 
              || (tree->north_west == NULL && tree->north_east == NULL && tree->south_west == NULL && tree->south_east == NULL))) {
        r = r_vector(tree->centre_of_mass, particle.position);
        a = -(G * particle.mass * tree->mass) / norm3(r);
        f.x = a * r.x;
        f.y = a * r.y;
    } else {
        vector_t f1 = quad_force(tree->north_west, particle, theta_max);
        vector_t f2 = quad_force(tree->north_east, particle, theta_max);
        vector_t f3 = quad_force(tree->south_west, particle, theta_max);
        vector_t f4 = quad_force(tree->south_east, particle, theta_max);
        f.x = f1.x + f2.x + f3.x + f4.x;
        f.y = f1.y + f2.y + f3.y + f4.y;
    }
    return f;
}
#else
// Version 2 - 
vector_t quad_force(quadtree_t* tree, particle_t particle, double theta_max) {
    vector_t f;
    vector_t r;
    double n;
    double a;

    if(tree == NULL) {
        f.x = 0.0;
        f.y = 0.0;
    } else if(!vector_in_box(particle.position, tree->box) && (theta_bounds(tree, particle, theta_max, &r, &n) 
              || (tree->north_west == NULL && tree->north_east == NULL && tree->south_west == NULL && tree->south_east == NULL))) {
        n += epsilon;
        a = -(G * particle.mass * tree->mass) / (n*n*n);
        f.x = a * r.x;
        f.y = a * r.y;
    } else {
        vector_t f1 = quad_force(tree->north_west, particle, theta_max);
        vector_t f2 = quad_force(tree->north_east, particle, theta_max);
        vector_t f3 = quad_force(tree->south_west, particle, theta_max);
        vector_t f4 = quad_force(tree->south_east, particle, theta_max);
        f.x = f1.x + f2.x + f3.x + f4.x;
        f.y = f1.y + f2.y + f3.y + f4.y;
    }
    return f;
}
#endif
// //====================================================================================
void compute_quad_forces(vector_t* forces, quadtree_t* tree, particle_t* particles, int N, double theta_max) {
    #pragma omp parallel for
    for(int i = 0; i < N; i++) {
            forces[i] = quad_force(tree, particles[i], theta_max);
    }
}
//=======================================================================================