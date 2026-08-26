#pragma once
#include <typeinfo>

#include "core/memory/DArr.h"
#include "utils/logger.h"

template<typename key, typename T>
struct sparse_set {
    
    DArr<T> dence;
    DArr<key> sparse;
    DArr<key> reverse;

public:

    sparse_set(uint32_t elementCount, uint32_t sparseCount = elementCount);
    ~sparse_set();

    void set(key key, T element);
    void remove(key key);

    bool has(key key);

    T& get(key key);
    uint32_t size() { return dence.size(); };

    DArr<T>& data();
};

template <typename key, typename T>
inline sparse_set<key, T>::sparse_set(uint32_t elementCount, uint32_t sparseCount) : 
    dence(elementCount), sparse(sparseCount), reverse(elementCount) {
}

template <typename key, typename T>
inline sparse_set<key, T>::~sparse_set() {
}

template <typename key, typename T>
inline void sparse_set<key, T>::set(key key, T element) {
    if (!has(key)) return;
    dence.push_back(element);
    sparse.set(key, dence.size() - 1);
    reverse.set(dence.size() - 1, key);
}

template <typename key, typename T>
inline void sparse_set<key, T>::remove(key key) {
    if (!has(key)) return;

    key lastKey = reverse[dence.size() - 1];
    key removedComponent = sparse[key];
    
    dence[removedComponent] = dence.back();
    sparse[lastKey] = removedComponent;
    reverse[removedComponent] = lastKey;

    dence.delete_back();
}

template <typename key, typename T>
inline bool sparse_set<key, T>::has(key key) {

    bool value = key < sparse.size()
        && sparse[key] < dence.size()
        && reverse[sparse[key]] == key;
    if (!value) LOG_ERROR(typeid(T).name(), " sprase set nonvalide key: ", key);
    return value;
}

template <typename key, typename T>
inline T& sparse_set<key, T>::get(key key) {
    if (has(key))
    return dence[sparse[key]];
}

template <typename key, typename T>
inline DArr<T>& sparse_set<key, T>::data() {
    return dence;
}