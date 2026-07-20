#pragma once
#include <cstddef>
#include <cstdint>

struct SlotHeader {
    size_t sizeToNextHeader;
    size_t size;
    bool isUse;
};

// Global arena of memory, can resize
struct MemoryArena {
    
    //constexpr size_t MAXmemoryMb = MAX_MEMORY_MB;
    //constexpr size_t MINmemoryMb = MIN_MEMORY_MB;

    // 1 kb = 1024 bytes

    const size_t MinMemoryAllocationKB = 64 * 1024;

    // defragmentation doesn't work at all
    const size_t MaxMemoryAllocationKB = 64 * 1024;

    size_t MemorySize;
    
    char* memory;
    size_t current;
    
    static MemoryArena& GetInstance();
    MemoryArena();
    ~MemoryArena();

    size_t allocate(size_t size, uint8_t align);
    void deallocate(uint32_t index);
    void deallocateByOffset(size_t offset);
    size_t resizeSlot(size_t offset, size_t newSize, uint8_t align);

    void free();
    
    char* getPtr();
    
    void resize(size_t newSize);
    void defragmentation(); // not work

    void* operator[](uint32_t index) { return getHeaderByIndex(index) + sizeof(SlotHeader); }

    bool IsFreeSlots;
private:

    MemoryArena(const MemoryArena&) = delete;
    MemoryArena& operator=(const MemoryArena&) = delete;

    char* getHeaderByIndex(uint32_t index);
    size_t alignUP(size_t value, uint8_t align) { return (value + align - 1) & ~(align - 1); };

    uint16_t freeSlotsCount;
};