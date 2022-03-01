typedef struct quad_node {
    double mass;
    vector_t centre_of_mass;
    box_bounds_t box;
    struct quad_node *north_west;
    struct quad_node *north_east;
    struct quad_node *south_east;
    struct quad_node *south_west;
} quadtree_t;

void quadtree_build_tree(quadtree_t** root, particle_t* particles, int N);

void quadtree_print(quadtree_t* tree);

void quadtree_add_particle(quadtree_t** tree, particle_t particle, box_bounds_t box);

void quadtree_draw(quadtree_t* tree);

// int theta_bounds(quadtree_t* tree, particle_t particle, double theta_max);