#include "core/memory/MemoryArena.h"

MemoryArena& MemoryArena::GetInstance() {
    static MemoryArena memoryArena;
    return memoryArena;
}

MemoryArena::MemoryArena() : MemorySize(MinMemoryAllocation), 
    memory(new char[MemorySize]), current(0),
    IsFreeSlots(false), freeSlotsCount(0){
}

MemoryArena::~MemoryArena() {
    free();
}

size_t MemoryArena::allocate(size_t size, uint8_t aling) {
    
    size_t totalSize = (current + sizeof(SlotHeader) + size);
    
    if (totalSize > MemorySize) {
        if(IsFreeSlots) {
            size_t currOffset = 0;
            for(int i = 0; i < freeSlotsCount; i++) {
                SlotHeader* usedHeader = reinterpret_cast<SlotHeader*>(getHeaderByIndex(i));
                if (!usedHeader->isUse) {
                    if(freeSlotsCount-- <= 0) IsFreeSlots = false;
                    
                    usedHeader->isUse = true;
                    usedHeader->size = currOffset;
                    currOffset += sizeof(SlotHeader);
                    
                    currOffset = alignUP(currOffset, aling);
                    
                    usedHeader->size = currOffset - usedHeader->size;
                    
                    return currOffset;
                }  
                currOffset += usedHeader->sizeToNextHeader;
            }
        } else {
            size_t newSize = MemorySize * 2;
            while (totalSize > newSize) {
                newSize *= 2;
            }
            resize(newSize);
        }
    }

    // Place data in memory

    SlotHeader* header = reinterpret_cast<SlotHeader*>(memory + current);
    header->sizeToNextHeader = current;
    header->isUse = true;
    current += sizeof(SlotHeader);

    current = alignUP(current, aling);
    size_t offset = current;
    current += size;

    header->sizeToNextHeader = current - header->sizeToNextHeader;
    header->size = header->sizeToNextHeader;

    freeSlotsCount++;

    return offset;
}

void MemoryArena::deallocate(uint32_t index) {
    SlotHeader* header = reinterpret_cast<SlotHeader*>(getHeaderByIndex(index));
    header->isUse = false;
    IsFreeSlots = true;
}

void MemoryArena::deallocateByOffset(size_t offset) {
    SlotHeader* header = reinterpret_cast<SlotHeader*>(memory + offset - sizeof(SlotHeader));
    header->isUse = false;
    IsFreeSlots = true;
}

char* MemoryArena::getPtr() {
    return memory;
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
    if (newSize > MaxMemoryAllocation) {
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

void MemoryArena::defragmentation() {
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