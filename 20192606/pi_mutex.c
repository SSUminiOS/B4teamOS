#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define a global variable to store the number of points inside the circle
int points_inside_circle = 0;

// Define a mutex lock
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to generate random double precision number between 0 and 1
double random_double() {
    return (double)rand() / RAND_MAX;
}

// Function to check if a point is inside the circle
int is_inside_circle(double x, double y) {
    return x * x + y * y <= 1;
}

// Function to generate random points and update points_inside_circle
void *generate_points(void *arg) {
    int *num_points_ptr = (int *)arg;
    int num_points = *num_points_ptr;
    int local_points_inside_circle = 0;

    for (int i = 0; i < num_points; i++) {
        double x = random_double();
        double y = random_double();

        if (is_inside_circle(x, y)) {
            local_points_inside_circle++;
        }
    }

    // Acquire lock before updating the global variable
    pthread_mutex_lock(&mutex);
    points_inside_circle += local_points_inside_circle;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int pi_mutex() {
    int num_points = 1000000; // Number of random points
    int num_threads = 4; // Number of threads
    pthread_t threads[num_threads];

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, generate_points, &num_points);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate pi
    double pi_estimate = (4.0 * points_inside_circle) / (num_points * num_threads);
    printf("Estimated value of π using Monte Carlo method: %f\n", pi_estimate);

    return 0;
}
