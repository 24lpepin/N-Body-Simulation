#include <cmath>

// inline constexpr double G = 0.0000000000667; //std::pow(6.67,-11); m^3 kg^{-1} s^{-2}
inline constexpr double G = 39.478; // au^3 M_\odot^{-1} yr^{-2}
inline constexpr double EPSILON = 0.00005;

inline constexpr int MAX_PATH_LEN = 20000;

inline constexpr int DISTANCE_TO_PIXEL_CONVERSION = 100; // 100 pixels / 1 au