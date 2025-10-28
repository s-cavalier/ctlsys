#include "IOLayer.hpp"

IOLayer::InputStream::InputStream(const char* device) : buf(device), std::istream(&buf) {}

std::istream& IOLayer::operator>>(std::istream& is, input_event& inp) {
    is.read((char*)(&inp), sizeof(input_event));
    return is;
}

