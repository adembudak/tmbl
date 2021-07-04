#include "tmbl/utils/utils.h"
#include <random>

namespace tmbl::utils {

std::random_device randDevice;
std::mt19937 gen(randDevice());
auto distrib = std::uniform_int_distribution<>(0, 255);

byte randomByte() noexcept { return distrib(gen); }

}
