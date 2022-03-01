#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "vector.h"
#include "particle.h" 
#include "box_bounds.h"
#include "quad.h"
#include "forces.h"
//======================================================================================================
typedef struct quad_force_args {
    quadtree_t* tree;
    particle_t* particles;
    double theta_max;   
    vector_t* forces;
    int start;
    int stop;
} quad_force_args_t;
//======================================================================================================
const double epsilon = 1e-3;
//======================================================================================================
#if VERSION < 3
// version 2
int vector_in_box(vector_t v, box_bounds_t box) {
    if(v.x < box.xupper && v.x > box.xlower && v.y < box.yupper && v.y > box.ylower) {
        return 1;
    } else {
        return 0;
    }
}
#else
// version 3 -
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
// Version1 
#if VERSION == 1
int theta_bounds(quadtree_t* tree, particle_t particle, double theta_max) {
    double xwidth = tree->box.xupper - tree->box.xlower;

    if(xwidth / norm(r_vector(tree->centre_of_mass, particle.position)) < theta_max) {
        return 1;
    }
    else {
        return 0;
    }
}
#elif VERSION == 2
// Version2 
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
#if VERSION == 1
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
//====================================================================================
void* quad_force_wrapper(void* argument) {
    quad_force_args_t* args  = argument;
    
    for(int i = args->start; i < args->stop; i++) {
        args->forces[i] = quad_force(args->tree, args->particles[i], args->theta_max);
    }

    return NULL;
}
//====================================================================================
void compute_quad_forces(vector_t* forces, quadtree_t* tree, particle_t* particles, int N, double theta_max, int nthreads) {

    pthread_t* thread_ptr = malloc(nthreads*sizeof(pthread_t));
    quad_force_args_t* args = malloc(nthreads*sizeof(quad_force_args_t));

    for(int i = 0; i < nthreads; i++) {
        args[i].tree = tree;
        args[i].particles = particles;
        args[i].theta_max = theta_max;
        args[i].forces = forces;
        args[i].start = (i * N) / nthreads;
        args[i].stop = ((i + 1)*N) / nthreads;
        pthread_create(&thread_ptr[i], NULL, quad_force_wrapper, &args[i]);
    }

    for(int i = 0; i < nthreads; i++) {
        pthread_join(thread_ptr[i], NULL);
    }
}
//====================================================================================
