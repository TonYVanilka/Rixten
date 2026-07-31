#pragma once
#include <cstddef>
#include <new>

#include "core/memory/MemoryArena.h"
#include "utils/logger.h"

template<typename T>
struct DArr {
    
private:

    MemoryArena& arena;
    size_t maxElementsCount;
    size_t elementCount;
    size_t arenaOffset;

public: 

    DArr(size_t maxElementsCount_);
    ~DArr();

    void push_back(const T& element);
    void set(uint32_t index, const T& element);
    void resize(size_t newElementCount);
    void delete_back();

    void free();

    size_t size() { return elementCount; }
    size_t size() const { return elementCount; }
    size_t maxSize() { return maxElementsCount; }

    T& back();
    bool empty();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
};

template <typename T>
inline DArr<T>::DArr(size_t maxElementsCount_) : maxElementsCount(maxElementsCount_),
    elementCount(0), arena(MemoryArena::GetInstance()) {
    arenaOffset = arena.allocate(sizeof(T) * maxElementsCount, alignof(T));
}

template <typename T>
inline DArr<T>::~DArr() {
    free();
}

template <typename T>
inline void DArr<T>::push_back(const T& element) {
    
    if(elementCount + 1 > maxElementsCount) {
        resize(maxElementsCount * 2);
    }

    void* ptr = arena.getPtr(arenaOffset + (elementCount * sizeof(T)));
    elementCount++;

    new(ptr) T(element); 
}

template <typename T>
inline void DArr<T>::set(uint32_t index, const T& element) {

    if(index > maxElementsCount) {
        LOG_ERROR("DArr out of the range!");
        return;
    }

    if(elementCount > maxElementsCount) {
        resize(maxElementsCount * 2);
    }

    if (index >= elementCount) {

        void* ptr = arena.getPtr(arenaOffset + (sizeof(T) * index));
        elementCount++;

        new(ptr) T(element); 
    } else {
        (*this)[index] = element;
    }

}

template <typename T>
inline void DArr<T>::resize(size_t newElementCount) {
    if (newElementCount == 0) LOG_WARN("Darr has 0 newElementCount to resize");
    LOG_WARN("DArr resize slot called");
    arenaOffset = arena.resizeSlot(arenaOffset, newElementCount * sizeof(T), alignof(T));
    maxElementsCount = newElementCount;
}

template <typename T>
inline void DArr<T>::delete_back() {
    if(elementCount < 1) return;
    elementCount--;
    reinterpret_cast<T*>(arena.getPtr(arenaOffset + (elementCount * sizeof(T))))->~T();
}

template <typename T>
inline void DArr<T>::free() {
    while(elementCount > 0) {
        elementCount--;
        reinterpret_cast<T*>(arena.getPtr(arenaOffset + (elementCount * sizeof(T))))->~T();
    }

    arena.deallocateByOffset(arenaOffset);
}

template <typename T>
inline T& DArr<T>::back() {
    if(elementCount == 0) { LOG_FATAL("DArr back function has 0 element count");}
    return *reinterpret_cast<T*>(arena.getPtr(arenaOffset + ((elementCount - 1) * sizeof(T))));
}

template <typename T>
inline bool DArr<T>::empty() {
    return elementCount == 0;
}

template <typename T>
inline T& DArr<T>::operator[](size_t index) {
    if(index >= maxElementsCount) { LOG_FATAL("DArr index out of the range"); }
    return *reinterpret_cast<T*>(arena.getPtr(arenaOffset + (sizeof(T) * index)));
}

template <typename T>
inline const T& DArr<T>::operator[](size_t index) const {
    if(index >= maxElementsCount) { LOG_FATAL("DArr index out of the range"); }
    return *reinterpret_cast<T*>(arena.getPtr(arenaOffset + (sizeof(T) * index)));
}
