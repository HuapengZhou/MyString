//
//  BufferManager.hpp
//  Assignment2
//
//

#ifndef BufferManager_h
#define BufferManager_h
#include <iostream>
#include <cstring>

namespace ECE141 {

    template <typename T>
    class BufferManager {
    public:
        BufferManager() {
            capacity = 0;
            bufferPointer = nullptr;
        }    //finish me - default constructor
        BufferManager(const BufferManager& aCopy) {
            this->capacity = aCopy.capacity;
            //this->capacity = aCopy.capacity;
            this->bufferPointer = new T[this->capacity + 1];
            std::strcpy(this->bufferPointer, aCopy.bufferPointer);
            // for(int i = 0; i < this->size; i++) {
            //     this->bufferPointer[i] = aCopy[i];
            // }
            // this->bufferPointer[this->size] = nullptr;
        } //finish me - copy constructor

        BufferManager(const char* aBuffer) {
            this->capacity = strlen(aBuffer) + 1;
            this->bufferPointer = new T[this->capacity];
            std::strcpy(this->bufferPointer, aBuffer);
        }

        BufferManager(size_t aSize) {
            this->capacity = aSize;
            this->bufferPointer = new T[this->capacity];
            //this->bufferPointer = nullptr;
        } //finish me - constructor with predefined size

        //---- Add other ocf methods ---
        /*BufferManager& operator=(const BufferManager& aCopy) {
            return *this = (aCopy.capacity, aCopy.bufferPointer);
        }*/
        //BufferManager& operator=(const T* copyBuffer) {
        //    capacity = ;
        //    memcpy(bufferPointer, copyBuffer, copyLength); // 3rd argument should be size_of(T) * cL or not ?
        //    return *this;
        //}
        ~BufferManager() {
            capacity = 0;
            // delete bufferPointer;
            bufferPointer = nullptr;
        }
        //BufferManager& BufferInsertion(const char* insertChars, size_t beginIndex, size_t insertLength) {
        //    size_t newSize = insertLength + this->capacity;
        //    this->willExpand(newSize);
        //    for (size_t i = newSize - 1; i >= beginIndex + insertLength; i--) {
        //        bufferPointer[i] = bufferPointer[i - insertLength];
        //    }
        //    for (size_t i = beginIndex; i < beginIndex + insertLength; i++) {
        //        bufferPointer[i] = insertChars[i - beginIndex];
        //    }
        //    return *this;
        //}


        //---- Basic methods you must implement. Add more if you need to...
        size_t getCapacity() const {
            return capacity;
        }
        T* getBuffer() const {
            return bufferPointer;
        }

        size_t willExpand(size_t aNewSize, size_t anOffset = 0) {
            if (aNewSize > capacity) {
                T* tempPointer = new T[aNewSize];
                if (capacity != 0) {
                    std::strcpy(tempPointer, this->bufferPointer); // strcpy(a, b) b->a 
                    delete bufferPointer;
                }
                this->capacity = aNewSize;
                bufferPointer = tempPointer;
                return aNewSize;
            }
            std::cout << "wrong arguments or something\n";
            return 0;
        }
        size_t willCompact(size_t aNewSize, size_t anOffset = 0) {
            T* tempPointer = new T[aNewSize];
            /*for (size_t i = 0; i < anOffset; i++) {
                tempPointer[i] = bufferPointer[i];
            }*/
            memcpy(tempPointer, bufferPointer, anOffset * sizeof(T));
            //size_t pos = anOffset + (aNewSize - this->capacity);
            for (size_t i = anOffset; i < aNewSize; i++) {
                tempPointer[i] = bufferPointer[i + (this->capacity - aNewSize)];
            }
            tempPointer[aNewSize - 1] = '\0';
            this->capacity = aNewSize;
            //std::strcpy(tempPointer, this->bufferPointer);
            delete bufferPointer;
            bufferPointer = tempPointer;
            return aNewSize;
        }


    protected:
        size_t capacity;
        T* bufferPointer;
    };

}

#endif /* BufferManager_h */