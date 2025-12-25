#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long total_points;
int total_threads;
long points_in_circle = 0;

pthread_mutex_t mutex;

void *count_points(void *arg) {
    long points_per_thread = total_points / total_threads;
    long local_count = 0;
    
    unsigned int seed = time(NULL) ^ (long)pthread_self();

    for (long i = 0; i < points_per_thread; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    points_in_circle += local_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_threads> <num_points>\n", argv[0]);
        return 1;
    }

    total_threads = atoi(argv[1]);
    total_points = atol(argv[2]);

    if (total_threads <= 0 || total_points <= 0) {
        fprintf(stderr, "Arguments must be positive integers.\n");
        return 1;
    }

    pthread_t *threads = malloc(total_threads * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < total_threads; i++) {
        pthread_create(&threads[i], NULL, count_points, NULL);
    }

    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double pi_estimate = 4.0 * (double)points_in_circle / (double)total_points;

    printf("Estimated Pi: %f\n", pi_estimate);
    printf("Total Points: %ld\n", total_points);
    printf("Threads used: %d\n", total_threads);

    pthread_mutex_destroy(&mutex);
    free(threads);

    return 0;
}