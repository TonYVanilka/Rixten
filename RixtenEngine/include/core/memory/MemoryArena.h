#pragma once
#include <cstddef>
#include <cstdint>

struct SlotHeader {
    size_t sizeToNextHeader;
    size_t size;
    bool isUse;
};

struct MemoryArena {
    
    //constexpr size_t MAXmemoryMb = MAX_MEMORY_MB;
    //constexpr size_t MINmemoryMb = MIN_MEMORY_MB;
    
    const size_t MinMemoryAllocation = 32;
    const size_t MaxMemoryAllocation = 1024;
    
    size_t MemorySize;
    
    char* memory;
    size_t current;
    
    static MemoryArena& GetInstance();
    MemoryArena();
    ~MemoryArena();

    size_t allocate(size_t size, uint8_t aling);
    void deallocate(uint32_t index);
    void deallocateByOffset(size_t offset);

    void free();
    
    char* getPtr();
    
    void resize(size_t newSize);
    void defragmentation();

    void* operator[](uint32_t index) { return getHeaderByIndex(index) + sizeof(SlotHeader); }

    bool IsFreeSlots;
private:

    MemoryArena(const MemoryArena&) = delete;
    MemoryArena& operator=(const MemoryArena&) = delete;

    char* getHeaderByIndex(uint32_t index);
    size_t alignUP(size_t value, uint8_t align) { return (value + align - 1) & ~(align - 1); };

    uint16_t freeSlotsCount;
};