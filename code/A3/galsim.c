// /*  Course: HPP 
//     Assignment No: 3
//     Author: Kieran and Panagiotis
// */

//====================PROGRAMS INCLUDED=======================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include <sys/time.h>

//====================GLOBAL VARIABLES/CONSTANTS==============
const float circleRadius = 0.0025; 
const float circleColor = 0;
const int windowWidth = 800;
const double L = 1, W = 1;
const double epsilon = 0.001;
double G;
//====================STRUCTS=================================
typedef struct vector {
    double x;
    double y;
} vector_t;

typedef struct particle {
    vector_t position;
    double mass;
    vector_t velocity;
    double brightness;
} particle_t;

//====================PRINT FUNCTIONS==========================
void print_vector_info(vector_t v) {
    printf("(%.20lf, %.20lf)\n", v.x, v.y);
}

void print_particle_info(particle_t p) {
    printf("Mass: %4.10f\n", p.mass);
    printf("Brightness: %4.1f\n", p.brightness);
    printf("Position: ");
    print_vector_info(p.position);
    printf("Velocity: ");
    print_vector_info(p.velocity);
}

void print_all_particle_info(particle_t* p, int n) {
    int i;

    for(i = 0; i < n; i++) {
        printf("Particle %d: \n", i+1);
        print_particle_info(p[i]);
        printf("\n");
    }
    printf("\n");
}

//====================TIMINGS=============================
static double get_wall_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
    return seconds;
}
//====================FORCE VECTORS========================== 
vector_t r_vector(vector_t v1, vector_t v2) {
    vector_t r;

    r.x = v2.x - v1.x;
    r.y = v2.y - v1.y;

    return r;
}

#if VERSION < 2
// Version 1
double norm3(vector_t v) {
    return pow((sqrt(pow((v.x), 2) + pow((v.y), 2)) + epsilon), 3);
}
#else
// Version 2
double norm3(vector_t v) {
    double n3;

    n3 = sqrt(v.x*v.x + v.y*v.y) + epsilon; 
    n3 = n3 * n3 * n3; 
    return n3;
}
#endif

#if VERSION < 3
// Version 1 - 2
vector_t force(particle_t pi, particle_t pj) {
    vector_t f;

    f.x = -(G * pi.mass * pj.mass * r_vector(pi.position, pj.position).x) 
            / norm3(r_vector(pi.position, pj.position));
    f.y = -(G * pi.mass * pj.mass * r_vector(pi.position, pj.position).y) 
            / norm3(r_vector(pi.position, pj.position));

    return f;
}

#else
//Version 3 - 
vector_t force(particle_t pi, particle_t pj) {
    vector_t f;
    vector_t r;
    double a;

    r = r_vector(pi.position, pj.position);
    a = -(G * pi.mass * pj.mass) / norm3(r);

    f.x = a * r.x;
    f.y = a * r.y;

    return f;
}
#endif

#if VERSION < 4
// Version 1-3
vector_t total_force(particle_t* p, int i, int N) {
    vector_t f = {0.0,0.0};
    
    for(int j = 0; j < N; j++) {
        if(i != j) {
            vector_t f_temp = force(p[j], p[i]);
            f.x += f_temp.x;
            f.y += f_temp.y;
        }
    }
    return f;
}
#else
// Version 4 -
void compute_forces(vector_t* forces, particle_t* particles, int N) {

    for(int i = 0; i < N; i++) {
        forces[i].x = 0.0;
        forces[i].y = 0.0;
    }
    
    for(int i = 0; i < N; i++) {
        
        for(int j = i + 1; j < N; j++) {
                vector_t f = force(particles[j], particles[i]);
                forces[i].x += f.x;
                forces[i].y += f.y;

                forces[j].x -= f.x;
                forces[j].y -= f.y;
        }
        
    }
}
#endif
//====================PARTICLE MOTION=========================

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

//====================GRAPHICS================================
void draw_particle(particle_t p) {
    DrawCircle(p.position.x, p.position.y, L, W, circleRadius*sqrt(p.mass), circleColor);
}

void update_screen(particle_t* particles, int N) {
    ClearScreen();
        
    for(int i = 0; i < N; i++) {
        draw_particle(particles[i]);
    }

    Refresh();
    usleep(10000);
}

int read_particles(particle_t* particles, const char* input_file_name, int N) {
    FILE* input_file = fopen(input_file_name, "rb");
    size_t nread = 0;
    
    if(!input_file) {
        printf("Error: failed to open input file '%s'.\n", input_file_name);
        return -1;
    }

    for(int i = 0; i < N; i++) {
        nread += fread(&particles[i].position.x, sizeof(double), 1, input_file);
        nread += fread(&particles[i].position.y, sizeof(double), 1, input_file);
        nread += fread(&particles[i].mass, sizeof(double), 1, input_file);
        nread += fread(&particles[i].velocity.x, sizeof(double), 1, input_file);
        nread += fread(&particles[i].velocity.y, sizeof(double), 1, input_file);
        nread += fread(&particles[i].brightness, sizeof(double), 1, input_file);
    }

    if(fclose(input_file) != 0) {
        printf("Error closing input file.\n");
        return -1;
    }
    return nread != 6*N;
}

int write_particles(particle_t* particles, const char* output_file_name, int N) {
    FILE* output_file = fopen(output_file_name, "wb");

    if(!output_file) {
        printf("Error opening output file '%s'.\n", output_file_name);
        return -1;
    }

    for(int i = 0; i < N; i++) {
        fwrite(&particles[i].position.x, sizeof(double), 1, output_file);
        fwrite(&particles[i].position.y, sizeof(double), 1, output_file);
        fwrite(&particles[i].mass, sizeof(double), 1, output_file);
        fwrite(&particles[i].velocity.x, sizeof(double), 1, output_file);
        fwrite(&particles[i].velocity.y, sizeof(double), 1, output_file);
        fwrite(&particles[i].brightness, sizeof(double), 1, output_file);
    }

    if(fclose(output_file) != 0) {
        printf("Error closing output file\n");
        return -1;
    }
    return 0;
}

//====================MAIN FUNCTION===========================
int main(int argc, char* argv[]) {
    if(argc != 6) {
        printf("Please give 5 arguments: N filename nsteps delta_t graphics\n");
        return -1;
    }

    int N = atoi(argv[1]);
    const char* input_file_name = argv[2];
    int nsteps = atoi(argv[3]);
    double delta_t = atof(argv[4]);
    int graphics = atoi(argv[5]);

    particle_t* particles = malloc(N*sizeof(struct particle));
    vector_t* forces = malloc(N*sizeof(struct vector));

    G = 100.0/N;

    if(read_particles(particles, input_file_name, N) != 0) {
        printf("Error reading particles\n");
        return -1;
    }
    
    double time1 = get_wall_seconds();

//--------------------Graphic Initialisation---------------------
    if(graphics != 0) {
        InitializeGraphics(argv[0], windowWidth, windowWidth);
        SetCAxes(0, 1);
    }
    
    for(int i = 0; i < nsteps; i++) {
        if(graphics != 0) {
            update_screen(particles, N);
        }
        #if VERSION < 4
        // Version 1 - 3
        for(int i = 0; i < N; i++) {
            forces[i] = total_force(particles, i, N);
        }
        #else 
        // Version 4
        compute_forces(forces, particles, N);
        #endif
        update_particles(particles, forces, N, delta_t);
    }

    double time2 = get_wall_seconds();
    printf("The time required for the calculation is: %f\n\n", time2 - time1);

    if(write_particles(particles, "result.gal", N) != 0) {
        printf("Error writing output!\n");
        return -1;
    }
    free(forces);
    free(particles);

    return 0;
}
//==================================================================================