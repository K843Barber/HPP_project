#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../graphics/graphics.h"
#include "vector.h"
#include "particle.h"
//================================================================================
const float circleRadius = 0.0025; 
const float circleColor = 0;
//================================================================================
void print_particle_info(particle_t p) {
    printf("Mass: %4.10f\n", p.mass);
    printf("Brightness: %4.1f\n", p.brightness);
    printf("Position: ");
    print_vector_info(p.position);
    printf("Velocity: ");
    print_vector_info(p.velocity);
}
//================================================================================
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
//================================================================================
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
//================================================================================
void draw_particle(particle_t p) {
    DrawCircle(p.position.x, p.position.y, 1, 1, circleRadius*sqrt(p.mass), circleColor);
}