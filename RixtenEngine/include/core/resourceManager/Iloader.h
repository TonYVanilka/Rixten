#pragma once

template <typename T>
struct Iloader {
public:
    virtual ~Iloader() = default;
    virtual T load(const char* path) = 0;
};
