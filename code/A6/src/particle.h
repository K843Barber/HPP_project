typedef struct particle {
    vector_t position;
    double mass;
    vector_t velocity;
    double brightness;
} particle_t;

int read_particles(particle_t* particles, const char* input_file_name, int N);

int write_particles(particle_t* particles, const char* output_file_name, int N);

void print_particle_info(particle_t p);

void draw_particle(particle_t p);

