#include "core/memory/MemoryArena.h"

#include "utils/logger.h"

MemoryArena& MemoryArena::GetInstance() {
    static MemoryArena memoryArena;
    return memoryArena;
}

MemoryArena::MemoryArena() : MemorySize(MinMemoryAllocationKB), 
    memory(new char[MemorySize]), current(0),
    IsFreeSlots(false), freeSlotsCount(0){
}

MemoryArena::~MemoryArena() {
    free();
}

size_t MemoryArena::allocate(size_t size, uint8_t align) {
    if (size == 0) {
        LOG_FATAL("GLOBAL ARENA ALLOCATE SIZE 0");
        return 0;
    }

    size_t totalSize = (current + sizeof(SlotHeader) + size);
    
    if (totalSize > MemorySize) {
        if (totalSize > MemorySize) {
            size_t currOffset = 0;
            int currentSlot = 0;
            while (IsFreeSlots && currentSlot < freeSlotsCount) {
                SlotHeader* usedHeader = reinterpret_cast<SlotHeader*>(getHeaderByIndex(currentSlot));
                if (!usedHeader->isUse && totalSize >= usedHeader->sizeToNextHeader) {

                    usedHeader->isUse = true;
                    usedHeader->size = currOffset;
                    currOffset += sizeof(SlotHeader);
                    currOffset = alignUP(currOffset, align);
                    usedHeader->size = currOffset - usedHeader->size;
                    
                    freeSlotsCount--;
                    if (freeSlotsCount == 0) IsFreeSlots = false;
                    
                    return currOffset;
                }
                currOffset += usedHeader->sizeToNextHeader;
                currentSlot++;
            }
            size_t newSize = MemorySize * 2;
            while (totalSize > newSize) {
                newSize *= 2;
            }
            resize(newSize);
        }
    }

    // Place data in memory

    SlotHeader* header = reinterpret_cast<SlotHeader*>(memory + current);
    //header->sizeToNextHeader = current;
    size_t HeaderStart = current;
    header->isUse = true;
    current += sizeof(SlotHeader);

    current = alignUP(current, align);
    size_t offset = current;
    current += size;

    header->sizeToNextHeader = current - HeaderStart;
    header->size = header->sizeToNextHeader;
    return offset;
}

void MemoryArena::deallocate(uint32_t index) {
    SlotHeader* header = reinterpret_cast<SlotHeader*>(getHeaderByIndex(index));
    header->isUse = false;
    IsFreeSlots = true;
    freeSlotsCount++;
}

void MemoryArena::deallocateByOffset(size_t offset) {
    SlotHeader* header = reinterpret_cast<SlotHeader*>(memory + offset - sizeof(SlotHeader));
    header->isUse = false;
    IsFreeSlots = true;
    freeSlotsCount++;
}

// no tested before !!!
size_t MemoryArena::resizeSlot(size_t offset, size_t newSize, uint8_t align) {
    LOG_WARN("Slot in global arena was resized!");
    SlotHeader* usedHeader = reinterpret_cast<SlotHeader*>(memory + offset - sizeof(SlotHeader));

    if (newSize <= usedHeader->size) {
        usedHeader->size = newSize;
        return offset;
    }

    size_t oldSize = usedHeader->size;
    size_t newOffset = allocate(newSize, align);
    
    char* newPtr = memory + newOffset;
    char* oldPtr = memory + offset;

    for (size_t i = 0; i < oldSize; i++) {
        newPtr[i] = oldPtr[i];
    }

    SlotHeader* header = reinterpret_cast<SlotHeader*>(memory + offset - sizeof(SlotHeader));   
    header->isUse = false;
    IsFreeSlots = true;
    freeSlotsCount++;
    return newOffset;
}

char* MemoryArena::getPtr(size_t arenaOffset) {
    return memory + arenaOffset;
}

void MemoryArena::free() {
    delete[] memory;
}

char* MemoryArena::getHeaderByIndex(uint32_t index) {
    size_t offset = 0;

    for (int i = 0; i < index; i++) {
        SlotHeader* header;
        header = reinterpret_cast<SlotHeader*>(memory + offset);
        offset += header->sizeToNextHeader;
    }
    return memory + offset;
}

void MemoryArena::resize(size_t newSize) {
    LOG_DEBUG("Global arena was resized!");
    if (newSize > MaxMemoryAllocationKB) {
        defragmentation();
        return;
    };

    char* newMemory = new char[newSize];
    for (size_t i = 0; i < MemorySize; i++) {
        newMemory[i] = memory[i];
    }

    MemorySize = newSize;
    delete[] memory;
    memory = newMemory;
}

// doesn't work at all
void MemoryArena::defragmentation() {
    LOG_DEBUG("defragmentatation of global arena called!!! it fatal");
    char* dfMemory = new char[MemorySize];
    current = 0;
    size_t dfOffset = 0;

    while (true) {
        if (current >= MemorySize) break;

        SlotHeader* header = reinterpret_cast<SlotHeader*>(memory + current);

        current = header->size;

        for (; dfOffset < current; dfOffset++) {
            dfMemory[dfOffset] = memory[dfOffset];
        }
        current = header->sizeToNextHeader;
    }
}