#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>

double interpolate(double x0, double x1, double y0, double y1, double x) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

double generateRandomNumber(std::vector<double> sample) {
   int n = sample.size();
   std::sort(sample.begin(), sample.end());

    std::vector<double> cumulativeProbs(n);
    for (int i = 0; i < n; i++) {
        cumulativeProbs[i] = static_cast<double>(i + 1) / n;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    double randomValue = dis(gen);

    int index = 0;
    while (index < n && cumulativeProbs[index] < randomValue) {
        index++;
    }

    if (index == 0) {
        // If the random value falls before the first sample value, interpolate between the first two sample values
        double generatedValue = interpolate(0, cumulativeProbs[0], sample[0], sample[1], randomValue);
          return generatedValue;
    } else if (index == n) {
        // If the random value falls after the last sample value, interpolate between the last two sample values
        double generatedValue = interpolate(cumulativeProbs[n - 2], cumulativeProbs[n - 1], sample[n - 2], sample[n - 1], randomValue);
          return generatedValue;
    } else {
        // If the random value falls between two sample values, interpolate between them
        double generatedValue = interpolate(cumulativeProbs[index - 1], cumulativeProbs[index], sample[index - 1], sample[index], randomValue);
        return generatedValue;
    }
}

void a(const std::vector<double>& ecdfSamples) {
    // Generate a large number of random numbers based on the ECDF
    const int numSamples = 100000;
    std::vector<double> generatedSamples(numSamples);
    for (int i = 0; i < numSamples; ++i) {
        generatedSamples[i] = generateRandomNumber(ecdfSamples);
    }

    // Compute statistics of the generated samples
    double mean = std::accumulate(generatedSamples.begin(), generatedSamples.end(), 0.0) / numSamples;
    double variance = 0.0;
    for (double value : generatedSamples) {
        variance += (value - mean) * (value - mean);
    }
    variance /= numSamples;
    double standardDeviation = std::sqrt(variance);

    // Print the statistics
    std::cout << "Generated samples statistics:" << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Variance: " << variance << std::endl;
    std::cout << "Standard Deviation: " << standardDeviation << std::endl;
}
int main() {
    // Define the ECDF samples
    std::vector<double> ecdfSamples = {1.2, 2.5, 3.7, 4.1, 5.9};

    // Generate 10 continuous random numbers based on the ECDF
    for (int i = 0; i < 10; ++i) {
        double randomNumber = generateRandomNumber(ecdfSamples);
        std::cout << "Random number " << i+1 << ": " << randomNumber << std::endl;
    }

    a(ecdfSamples);
    
     // Compute statistics of the generated samples
    double mean = std::accumulate(ecdfSamples.begin(), ecdfSamples.end(), 0.0) / ecdfSamples.size();
    double variance = 0.0;
    for (double value : ecdfSamples) {
        variance += (value - mean) * (value - mean);
    }
    variance /= ecdfSamples.size();
    double standardDeviation = std::sqrt(variance);

    // Print the statistics
    std::cout << "ecdfSamples samples statistics:" << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Variance: " << variance << std::endl;
    std::cout << "Standard Deviation: " << standardDeviation << std::endl;
    return 0;
}
