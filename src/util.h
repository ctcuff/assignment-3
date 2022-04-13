#include <random>

namespace Util {
    // Returns a random between min and max (both inclusive)
    int generateRandomNumber(int min, int max) {
        std::random_device seed;
        std::mt19937 rng(seed());
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

        return dist(rng);
    }
}
