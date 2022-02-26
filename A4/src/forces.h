double G;

vector_t quad_force(quadtree_t* tree, particle_t particle, double theta_max);

vector_t force(particle_t pi, particle_t pj);

vector_t total_force(particle_t* p, int i, int N);

void compute_forces(vector_t* forces, particle_t* particles, int N);

void compute_quad_forces(vector_t* forces, quadtree_t* tree, particle_t* particles, int N, double theta_max);