#include "IOLayer.hpp"

IOLayer::InputStream::InputStream(const char* device) : buf(device), std::istream(&buf) {}

IOLayer::InputStream& IOLayer::operator>>(IOLayer::InputStream& is, input_event& inp) {
    is.read((char*)(&inp), sizeof(input_event));
    return is;
}

