#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include "vector.h"
#include "../graphics/graphics.h"
#include "particle.h"
#include "box_bounds.h"
#include "quad.h"
#include "forces.h"
//=================================================================================
const int windowWidth = 800;
const double L = 1, W = 1;
//=================================================================================
static double get_wall_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
    return seconds;
}
//=================================================================================
void update_particle(particle_t* p, vector_t force, double delta_t) {
    p->velocity.x += delta_t*force.x/p->mass;
    p->velocity.y += delta_t*force.y/p->mass;
    
    p->position.x += delta_t*p->velocity.x;
    p->position.y += delta_t*p->velocity.y;
}
 
void update_particles(particle_t* particles, vector_t* forces, int N, double delta_t) {
    for(int i = 0; i < N; i++) {
        update_particle(&particles[i], forces[i], delta_t);
    }
}
//=================================================================================
void update_screen(quadtree_t* tree, particle_t* particles, int N) {
    ClearScreen();
        
    for(int i = 0; i < N; i++) {
        draw_particle(particles[i]);
    }
    quadtree_draw(tree);
    Refresh();
    usleep(10000);
}
//=================================================================================
int main(int argc, char* argv[]) {

    if(argc != 8) {
        printf("Please give 6 arguments: N filename nsteps delta_t theta_max graphics \n");
        return -1;
    }

    int N = atoi(argv[1]);
    const char* input_file_name = argv[2];
    int nsteps = atoi(argv[3]);
    double delta_t = atof(argv[4]);
    double theta_max = atof(argv[5]);
    int graphics = atoi(argv[6]);
    int nthreads = atoi(argv[7]);

    particle_t* particles = malloc(N*sizeof(struct particle));
    vector_t* forces = malloc(N*sizeof(struct vector));
    quadtree_t* tree = NULL;

    G = 100.0/N;    

    if(read_particles(particles, input_file_name, N) != 0) {
        printf("Error reading particles\n");
        return -1;
    }
    
    if(graphics != 0) {
        InitializeGraphics(argv[0], windowWidth, windowWidth);
        SetCAxes(0, 1);
    }
    double time1 = get_wall_seconds();
    double timequad = 0.0;
    double timeFORCE = 0.0;

    for(int i = 0; i < nsteps; i++) {
        tree = NULL;
        double time2 = get_wall_seconds();
        quadtree_build_tree(&tree, particles, N/*, nthreads*/);
        double time3 = get_wall_seconds();
        
        compute_quad_forces(forces, tree, particles, N, theta_max, nthreads);
        double time4 = get_wall_seconds();
        
        timequad += time3 - time2;
        timeFORCE += time4 - time3;

        if(graphics != 0) {
            update_screen(tree, particles, N);
        }

        printf("."); fflush(stdout);

        update_particles(particles, forces, N, delta_t);
    }

    printf("\n");
    double time5 = get_wall_seconds();
    printf("quadtime: %f \nforcetime: %f \n", timequad, timeFORCE);
    printf("The total time required for the calculation is: %f.\n\n", time5 - time1);

    if(write_particles(particles, "result.gal", N) != 0) {
        printf("Error writing output!\n");
        return -1;
    }
    free(forces);
    free(particles);
    return 0;
}
//=================================================================================




