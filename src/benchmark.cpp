#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <queue>
#include <chrono>
#include <random>

#include "geometry.h"
#include "object.h"
#include "const.h"
#include "simulation.h"
#include "forces/direct_force_calculator.h"
#include "forces/barnes_hut_calculator.h"

std::vector<Object> generate_bodies(int n, unsigned seed = 42) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> pos_dist(-1e5, 1e5);
    std::uniform_real_distribution<double> vel_dist(-10.0, 10.0);
    std::uniform_real_distribution<double> mass_dist(1e20, 1e25);

    std::vector<Object> objects;
    for (int i = 0; i < n; i++) {
        Object obj({pos_dist(rng), pos_dist(rng)}, {vel_dist(rng), vel_dist(rng)}, mass_dist(rng));
        objects.push_back(obj);
    }
    return objects;
}

void benchmark_force_error(int n, double theta) {
    auto objects = generate_bodies(n);

    auto direct_objects = objects;
    auto bh_objects = objects;

    DirectForceCalculator direct;
    BarnesHutCalculator barnes_hut;

    direct.update_accelerations(direct_objects);
    barnes_hut.update_accelerations(bh_objects);

    double total_relative_error = 0.0;
    double total_squared_error = 0.0;
    double max_relative_error = 0.0;

    int valid_objects = 0;

    for (int i = 0; i < n; i++) {
        auto diff = bh_objects[i].acceleration - direct_objects[i].acceleration;

        double error = diff.magnitude();
        double reference = direct_objects[i].acceleration.magnitude();

        // Avoid dividing by ~zero
        if (reference > 1e-12) {
            double relative_error = error / reference;

            total_relative_error += relative_error;
            total_squared_error += relative_error * relative_error;
            max_relative_error = std::max(max_relative_error, relative_error);

            valid_objects++;
        }
    }

    double mean_error = total_relative_error / valid_objects;
    double rms_error = std::sqrt(
        total_squared_error / valid_objects
    );

    std::cout << "Theta: " << theta << "\n";
    std::cout << "Bodies: " << n << "\n";
    std::cout << "Mean relative error: " << mean_error << "\n";
    std::cout << "RMS relative error: " << rms_error << "\n";
    std::cout << "Max relative error: " << max_relative_error << "\n";
}

int main(int argc, char* argv[])
{
    double dt = 0.00005;
    // int n = 100;
    // int num_steps = 100;
    // if (argc == 3) {
    //     n = std::atoi(argv[1]);
    //     num_steps = std::atoi(argv[2]);
    // }

    std::vector<int> n_arr =         {8,    16,   32,   64,   128,  256,  512,  1024, 2048, 4096, 8192, 16384};
    std::vector<int> num_steps_arr = {10000, 7000, 4000, 3000, 1000, 600, 300, 100,  75,  50,  40, 35};
    for (int i = 0; i < n_arr.size(); i++) {
        benchmark_force_error(n_arr[i], 0.25);
        benchmark_force_error(n_arr[i], 0.5);
        benchmark_force_error(n_arr[i], 0.75);
        benchmark_force_error(n_arr[i], 1.0);
        benchmark_force_error(n_arr[i], 1.5);
    }

    for (int i = 0; i < n_arr.size(); i++) {
        int n = n_arr[i];
        int num_steps = num_steps_arr[i];

        std::vector<double> trial_rates;
        const int num_trials = 5;

        for (int t = 0; t < num_trials; t++) {
            Simulation simulation(std::make_unique<DirectForceCalculator>(), generate_bodies(n));
            // Simulation simulation(std::make_unique<BarnesHutCalculator>(), generate_bodies(n));

            auto start = std::chrono::steady_clock::now();
            for (int step = 0; step < num_steps; step++) {
                simulation.step(dt);
            }
            auto end = std::chrono::steady_clock::now();

            double seconds = std::chrono::duration<double>(end - start).count();
            trial_rates.push_back(num_steps / seconds);
        }

        double mean = std::accumulate(trial_rates.begin(), trial_rates.end(), 0.0) / num_trials;
        double sq_sum = 0.0;
        for (double r : trial_rates) sq_sum += (r - mean) * (r - mean);
        double stddev = std::sqrt(sq_sum / (num_trials - 1));
        double stderr_of_mean = stddev / std::sqrt(num_trials);

        std::cout << "Bodies: " << n << "\n";
        std::cout << "Steps/sec: " << mean << " +/- " << stderr_of_mean << "\n";
    }

    return 0;
}



