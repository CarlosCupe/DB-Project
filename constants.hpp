#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

struct location {
    long address;
    int pos;
};

namespace constants {
    constexpr int BTREE_ORDER = 6;
    constexpr char FILE_NAME[] = "base.bin";
    constexpr char METADATA_NAME[] = "metadata.bin";
    constexpr int BUFFER_SIZE = 1200;
    constexpr int BLOCKING_FACTOR = 6;
}

#endif