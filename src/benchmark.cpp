#include "object.h"
#include "object_loader.h"
#include "simulation.h"
#include "forces/direct_force_calculator.h"
#include "forces/barnes_hut_calculator.h"

#include <cmath>
#include <numeric>
#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

struct BenchmarkConfig {
    struct Run { int n; int num_steps; };
    std::vector<Run> sizes = {
        {8, 10000}, {16, 7000}, {32, 4000}, {64, 3000}, {128, 1000},
        {256, 600}, {512, 300}, {1024, 100}, {2048, 75},
        {4096, 50}, {8192, 40}, {16384, 35},
    };
    std::vector<double> thetas = {0.25, 0.5, 0.75, 1.0, 1.5};
    int num_trials = 5;
    double dt = 0.00005;
};

void run_performance() {
    BenchmarkConfig config;
    ObjectLoader object_loader; // Seed defaults to 42 if unspecified
    double dt = config.dt;
    for (int i = 0; i < config.sizes.size(); i++) {
        int n = config.sizes[i].n;
        int num_steps = config.sizes[i].num_steps;

        std::vector<double> trial_rates;
        const int num_trials = 5;

        for (int t = 0; t < num_trials; t++) {
            Simulation simulation(std::make_unique<DirectForceCalculator>(), object_loader.create_objects(n));
            // Simulation simulation(std::make_unique<BarnesHutCalculator>(config.thetas[i]), object_loader.create_objects(n));

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
}

void run_force_error() { //TODO
    // auto objects = generate_bodies(n);

    // auto direct_objects = objects;
    // auto bh_objects = objects;

    // DirectForceCalculator direct;
    // BarnesHutCalculator barnes_hut;

    // direct.update_accelerations(direct_objects);
    // barnes_hut.update_accelerations(bh_objects);

    // double total_relative_error = 0.0;
    // double total_squared_error = 0.0;
    // double max_relative_error = 0.0;

    // int valid_objects = 0;

    // for (int i = 0; i < n; i++) {
    //     auto diff = bh_objects[i].acceleration - direct_objects[i].acceleration;

    //     double error = diff.magnitude();
    //     double reference = direct_objects[i].acceleration.magnitude();

    //     // Avoid dividing by ~zero
    //     if (reference > 1e-12) {
    //         double relative_error = error / reference;

    //         total_relative_error += relative_error;
    //         total_squared_error += relative_error * relative_error;
    //         max_relative_error = std::max(max_relative_error, relative_error);

    //         valid_objects++;
    //     }
    // }

    // double mean_error = total_relative_error / valid_objects;
    // double rms_error = std::sqrt(
    //     total_squared_error / valid_objects
    // );

    // std::cout << "Theta: " << theta << "\n";
    // std::cout << "Bodies: " << n << "\n";
    // std::cout << "Mean relative error: " << mean_error << "\n";
    // std::cout << "RMS relative error: " << rms_error << "\n";
    // std::cout << "Max relative error: " << max_relative_error << "\n";
}

void run_energy_drift() { //TODO
}


int main(int argc, char* argv[])
{
    run_performance();

    return 0;
}



