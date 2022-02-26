#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vector.h"
#include "../graphics/graphics.h"
#include "particle.h"
#include "box_bounds.h"
#include "quad.h"
#include "forces.h"
//=======================================================================================================
#if VERSION < 3
// Version 2
int find_box(double xcentre, double ycentre, vector_t p) {   

    if(p.x <= xcentre && p.y < ycentre) {
        return 0;
    } else if(p.x > xcentre && p.y <= ycentre) {
        return 1;
    } else if(p.x < xcentre && p.y >= ycentre) {
        return 2;
    } else {
        return 3;
    }
}
#else
// Version 3 - 
int find_box(double xcentre, double ycentre, vector_t p) {   

    int left = p.x < xcentre;
    int down = p.y < ycentre;

    if(left) {
        if(down) {
            return 0;
        } else {
            return 2;
        }
    } else {
        if(down) {
            return 1;
        } else {
            return 3;
        }
    }
}
#endif
//=======================================================================================================
#if VERSION < 2
//version 1
void quadtree_add_particle(quadtree_t** tree, particle_t particle, box_bounds_t box) {

    if(*tree == NULL) {
        *tree = (quadtree_t*)malloc(sizeof(quadtree_t));
        (*tree)->mass = particle.mass;
        (*tree)->centre_of_mass = particle.position;
        (*tree)->box = box;
        (*tree)->north_west = NULL;
        (*tree)->north_east = NULL;
        (*tree)->south_west = NULL;
        (*tree)->south_east = NULL;
    } else if((*tree)->north_east == NULL && (*tree)->north_west == NULL && (*tree)->south_west == NULL && (*tree)->south_east == NULL) {
        
        particle_t particle_holder = {(*tree)->centre_of_mass, (*tree)->mass, {0.0, 0.0}, 1};
        
        double xcentre = ((*tree)->box.xlower + (*tree)->box.xupper)/2;
        double ycentre = ((*tree)->box.ylower + (*tree)->box.yupper)/2;

        if(particle_holder.position.x <= xcentre && particle_holder.position.y < ycentre) {
            box_bounds_t new_box = {box.xlower, xcentre, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_west, particle_holder, new_box);
        } else if(particle_holder.position.x > xcentre && particle_holder.position.y <= ycentre) {
            box_bounds_t new_box = {xcentre, box.xupper, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_east, particle_holder, new_box);
        } else if(particle_holder.position.x < xcentre && particle_holder.position.y >= ycentre) {
            box_bounds_t new_box = {box.xlower, xcentre, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_west, particle_holder, new_box);
        } else if(particle_holder.position.x >= xcentre && particle_holder.position.y > ycentre) {
            box_bounds_t new_box = {xcentre, box.xupper, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_east, particle_holder, new_box);
        }

        if(particle.position.x <= xcentre && particle.position.y < ycentre) {
            box_bounds_t new_box = {box.xlower, xcentre, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_west, particle, new_box);
        } else if(particle.position.x > xcentre && particle.position.y <= ycentre) {
            box_bounds_t new_box = {xcentre, box.xupper, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_east, particle, new_box);
        } else if(particle.position.x < xcentre && particle.position.y >= ycentre) {
            box_bounds_t new_box = {box.xlower, xcentre, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_west, particle, new_box);
        } else if(particle.position.x >= xcentre && particle.position.y > ycentre) {
            box_bounds_t new_box = {xcentre, box.xupper, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_east, particle, new_box);
        }

        (*tree)->centre_of_mass.x = ((*tree)->centre_of_mass.x*(*tree)->mass + particle.position.x*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->centre_of_mass.y = ((*tree)->centre_of_mass.y*(*tree)->mass + particle.position.y*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->mass += particle.mass;

    } else {
        (*tree)->centre_of_mass.x = ((*tree)->centre_of_mass.x*(*tree)->mass + particle.position.x*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->centre_of_mass.y = ((*tree)->centre_of_mass.y*(*tree)->mass + particle.position.y*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->mass += particle.mass;

        double xcentre = ((*tree)->box.xlower + (*tree)->box.xupper)/2;
        double ycentre = ((*tree)->box.ylower + (*tree)->box.yupper)/2;

        if(particle.position.x <= xcentre && particle.position.y < ycentre) {
            box_bounds_t new_box = {box.xlower, xcentre, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_west, particle, new_box);
        } else if(particle.position.x > xcentre && particle.position.y <= ycentre) {
            box_bounds_t new_box = {xcentre, box.xupper, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_east, particle, new_box);
        } else if(particle.position.x < xcentre && particle.position.y >= ycentre) {
            box_bounds_t new_box = {box.xlower, xcentre, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_west, particle, new_box);
        } else if(particle.position.x >= xcentre && particle.position.y > ycentre) {
            box_bounds_t new_box = {xcentre, box.xupper, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_east, particle, new_box);
        }
    }
} 
#else
//=======================================================================================
//version2
void quadtree_add_particle(quadtree_t** tree, particle_t particle, box_bounds_t box) {

    if(*tree == NULL) {
        *tree = (quadtree_t*)malloc(sizeof(quadtree_t));
        (*tree)->mass = particle.mass;
        (*tree)->centre_of_mass = particle.position;
        (*tree)->box = box;
        (*tree)->north_west = NULL;
        (*tree)->north_east = NULL;
        (*tree)->south_west = NULL;
        (*tree)->south_east = NULL;
    } else if((*tree)->north_east == NULL && (*tree)->north_west == NULL && (*tree)->south_west == NULL && (*tree)->south_east == NULL) {
        
        particle_t particle_holder = {(*tree)->centre_of_mass, (*tree)->mass, {0.0, 0.0}, 1};
        
        double xcentre = (box.xlower + box.xupper)/2;
        double ycentre = (box.ylower + box.yupper)/2;  
    
        int i = find_box(xcentre, ycentre, particle_holder.position);

        if(i == 0) {
            box_bounds_t new_box = {box.xlower, xcentre, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_west, particle_holder, new_box);
        } else if(i == 1) {
            box_bounds_t new_box = {xcentre, box.xupper, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_east, particle_holder, new_box);
        } else if(i == 2) {
            box_bounds_t new_box = {box.xlower, xcentre, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_west, particle_holder, new_box);
        } else {
            box_bounds_t new_box = {xcentre, box.xupper, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_east, particle_holder, new_box);
        }

        int j = find_box(xcentre, ycentre, particle.position);

        if(j == 0) {
            box_bounds_t new_box = {box.xlower, xcentre, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_west, particle, new_box);
        } else if(j == 1) {
            box_bounds_t new_box = {xcentre, box.xupper, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_east, particle, new_box);
        } else if(j == 2) {
            box_bounds_t new_box = {box.xlower, xcentre, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_west, particle, new_box);
        } else {
            box_bounds_t new_box = {xcentre, box.xupper, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_east, particle, new_box);
        }

        (*tree)->centre_of_mass.x = ((*tree)->centre_of_mass.x*(*tree)->mass + particle.position.x*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->centre_of_mass.y = ((*tree)->centre_of_mass.y*(*tree)->mass + particle.position.y*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->mass += particle.mass;

    } else {
        (*tree)->centre_of_mass.x = ((*tree)->centre_of_mass.x*(*tree)->mass + particle.position.x*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->centre_of_mass.y = ((*tree)->centre_of_mass.y*(*tree)->mass + particle.position.y*particle.mass)/((*tree)->mass + particle.mass);
        (*tree)->mass += particle.mass;

        double xcentre = (box.xlower + box.xupper)/2;
        double ycentre = (box.ylower + box.yupper)/2;

        int k = find_box(xcentre, ycentre, particle.position);

        if(k == 0) {
            box_bounds_t new_box = {box.xlower, xcentre, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_west, particle, new_box);
        } else if(k == 1) {
            box_bounds_t new_box = {xcentre, box.xupper, box.ylower, ycentre};
            quadtree_add_particle(&(*tree)->south_east, particle, new_box);
        } else if(k == 2) {
            box_bounds_t new_box = {box.xlower, xcentre, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_west, particle, new_box);
        } else {
            box_bounds_t new_box = {xcentre, box.xupper, ycentre, box.yupper};
            quadtree_add_particle(&(*tree)->north_east, particle, new_box);
        }
    }
}
#endif
//==========================================================================================
void quadtree_build_tree(quadtree_t** tree, particle_t* particles, int N) {
    box_bounds_t box = {0, 1, 0, 1};
    for(int i = 0; i < N; i++) {
        quadtree_add_particle(tree, particles[i], box);
    }
}
//==========================================================================================
void quadtree_print(quadtree_t* tree) {
    if(tree == NULL) {
        printf("Tree is empty!\n"); 
        return;
    }
    printf("%f (%f, %f): \n", tree->mass, tree->centre_of_mass.x, tree->centre_of_mass.y);
   
    if (tree->south_west != NULL) {
        printf("SW: ");
        quadtree_print(tree->south_west);
    } 
    if (tree->south_east != NULL) {
        printf("SE: ");
        quadtree_print(tree->south_east);
    }
    if (tree->north_east != NULL) {
        printf("NE: ");
        quadtree_print(tree->north_east);
    }
    if (tree->north_west != NULL) {
        printf("NW: ");
        quadtree_print(tree->north_west);
    }
}

void quadtree_draw(quadtree_t* tree) {
    if(tree != NULL) {
        DrawRectangle(tree->box.xlower, tree->box.ylower, 1, 1, tree->box.xupper - tree->box.xlower, tree->box.yupper - tree->box.ylower, 0);
        quadtree_draw(tree->north_west);
        quadtree_draw(tree->north_east);
        quadtree_draw(tree->south_west);
        quadtree_draw(tree->south_east);
    }
}