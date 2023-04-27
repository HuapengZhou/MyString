//
//  String.hpp
//  Assignment3
//
//  Created by rick gessner on 1/29/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef String_hpp
#define String_hpp

#include <stdio.h>
#include <iostream>
#include "BufferManager.hpp"
//#include "Searchable.hpp"
//#include "Sortable.hpp"

//It's up to you to decide the how the string and buffermanager
//will work together -- IS_A vs HAS-A.

namespace ECE141 {


    template<typename T = char, size_t aPresize = 32>
    class String : public BufferManager<T> {
    public:

        //add rest of the OCF methods...
        String(const char* aBuffer = "") :BufferManager<T>(aBuffer) {
            this->length = std::strlen(aBuffer) + 1;

            //this->bufferPointer = new char[this->length];
            //this->capacity = this->length;
            //std::strcpy(this->bufferPointer, aBuffer);

        } //default ctor
        String(const String& aString) :BufferManager<T>(aString) {
            this->length = aString.size() + 1;

            //this->capacity = this->length;
            //this->bufferPointer = new char[this->length];
            //std::strcpy(this->bufferPointer, aString.bufferPointer);

        }

        String& operator=(const String& aCopy) {
            if (this == &aCopy) {
                return *this;
            }
            this->bufferPointer = new char[aCopy.size()];
            this->length = aCopy.size();
            this->capacity = this->length;
            std::strcpy(this->bufferPointer, aCopy.bufferPointer);
            return *this;
        }
        //add version to assign from const char*
        ~String() {
            length = 0;
        }
        size_t size() const { return length - 1; }

        //mutation methods...
        T& operator[](size_t pos) {
            return this->bufferPointer[pos];
        }

        String& BufferInsertion(const char* insertChars, size_t beginIndex, size_t insertLength) {
            size_t newSize = insertLength + this->capacity;
            this->willExpand(newSize);
            for (size_t i = newSize - 1; i >= beginIndex + insertLength; i--) {
                this->bufferPointer[i] = this->bufferPointer[i - insertLength];
            }
            for (size_t i = beginIndex; i < beginIndex + insertLength; i++) {
                this->bufferPointer[i] = insertChars[i - beginIndex];
            }
            return *this;
        }

        String  operator+(const String& aString) {
            //char* temp = aString.bufferPointer;
            //size_t insertionLength = aString.capacity;
            //BufferManager bufferItem = this->BufferInsertion(temp, this->capacity, insertionLength);
            //String newString = String(bufferItem.getBuffer());
            size_t temp = this->capacity - 1;
            String newString = String(insert(temp, aString, 0, aString.length - 1));
            erase(temp, aString.length - 1);
            return newString;
        }
        //add method to add const char*
        String  operator+(const char* aChar) {
            //BufferManager bufferItem = this->BufferInsertion(aChar, this->capacity - 1, strlen(aChar));
            size_t temp = this->capacity - 1;
            String newString = String(insert(temp, aChar, 0, strlen(aChar)));
            erase(temp, strlen(aChar));
            return newString;
        }
        //add method to support "hello"+theStringObject

        String& operator+=(const String& aString) {
            return insert(this->capacity - 1, aString, 0, aString.length - 1);
            /*char* temp = aString.bufferPointer;
            size_t insertionLength = aString.capacity;
            this->BufferInsertion(temp, this->capacity - 1, insertionLength);
            this->length += insertionLength;
            return *this;*/
        }
        //add method to append const char*

        String& operator+=(const char* item) {
            return insert(this->capacity - 1, item, 0, strlen(item));
            /*size_t insertionLength = strlen(item);
            this->BufferInsertion(item, this->capacity - 1, insertionLength);
            this->length += insertionLength;
            return *this;*/
        }

        String& insert(size_t anIndex, const String& aStr, size_t aStrIndex,
            size_t aStrCount) {
            char* temp = aStr.bufferPointer;
            size_t tempLength = strlen(temp);
            size_t insertLength = aStrCount < tempLength ? aStrCount : tempLength;
            char* insertionChars = new char[insertLength + 1];
            for (size_t i = 0; i < insertLength; i++) {
                insertionChars[i] = temp[i + aStrIndex];
            }
            insertionChars[insertLength] = '\0';
            this->length += strlen(insertionChars);
            this->BufferInsertion(insertionChars, anIndex, strlen(insertionChars));
            return *this;
        }
        //add method to insert const char*

        String& insert(size_t anIndex, const char* insertChars) {
            this->length += strlen(insertChars);
            this->BufferInsertion(insertChars, anIndex, strlen(insertChars));
            return *this;
        }

        String& insert(size_t anIndex, const char* aBuffer, size_t aStrIndex, size_t aStrCount) {
            char* insertionChars = new char[aStrCount + 1];
            for (size_t i = 0; i < aStrCount; i++) {
                insertionChars[i] = aBuffer[i + aStrIndex];
            }
            insertionChars[aStrCount] = '\0';
            this->length += strlen(insertionChars);
            this->BufferInsertion(insertionChars, anIndex, strlen(insertionChars));
            return *this;
        }

        String& insert(size_t anIndex, T aChar)
        {
            this->length += 1;
            char* temp = new char[1];
            temp[0] = aChar;
            this->BufferInsertion(temp, anIndex, 1);
            return *this;
        }



        String& replace(size_t anIndex, size_t aMaxCopyLen, const String& aString) {
            erase(anIndex, aMaxCopyLen);
            insert(anIndex, aString, 0, aString.length - 1);
            return *this;
        }
        //add method to replace const char*
        String& replace(size_t anIndex, size_t aMaxCopyLen, const char* aChar) {
            erase(anIndex, aMaxCopyLen);
            insert(anIndex, aChar, 0, strlen(aChar));
            return *this;
        }
        String& erase(size_t anIndex, size_t aCount) {
            size_t newSize = this->capacity - aCount;
            if (this->capacity <= anIndex + aCount) {
                newSize = anIndex + 1;
                //anIndex -= 1;
            }
            this->willCompact(newSize, anIndex);
            this->length = this->capacity;
            return *this;
        }

        //Comparision methods...
        // @return 0: equal -1: < 1: >
        int compare(const char* aChar) const {
            size_t len1 = this->length;
            size_t len2 = strlen(aChar) + 1;
            size_t i = 0;
            while (i < len1 && i < len2) {
                if (this->bufferPointer[i] == aChar[i]) {
                    i += 1;
                }
                else if (this->bufferPointer[i] < aChar[i]) {
                    return -1;
                }
                else {
                    return 1;
                }
            }
            if (len1 == len2) {
                return 0;
            }
            if (i == len2) {
                return 1;
            }
            else {
                return -1;
            }
        }
        int compare(const String& aString) const {
            char* temp = aString.bufferPointer;
            return this->compare(temp);
        }
        //add method to add compare const char*
        bool operator==(const char* aChar) const {
            return compare(aChar) == 0;
            //return true; 
        }
        bool operator!=(const char* aChar) const {
            return compare(aChar) != 0;
            //return !(*this == aString); 
        }
        bool operator<(const char* aChar) const {
            return compare(aChar) < 0;
            //return false; 
        }
        bool operator<=(const char* aChar) const {
            return compare(aChar) <= 0;
            //return false; 
        }
        bool operator>(const char* aChar) const {
            return compare(aChar) > 0;
            //return false; 
        }
        bool operator>=(const char* aChar) const {
            return compare(aChar) >= 0;
            //return false; 
        }
        bool operator==(const String& aString) const {
            return compare(aString) == 0;
            //return true; 
        }
        bool operator!=(const String& aString) const {
            return compare(aString) != 0;
            //return !(*this == aString); 
        }
        bool operator<(const String& aString) const {
            return compare(aString) < 0;
            //return false; 
        }
        bool operator<=(const String& aString) const {
            return compare(aString) <= 0;
            //return false; 
        }
        bool operator>(const String& aString) const {
            return compare(aString) > 0;
            //return false; 
        }
        bool operator>=(const String& aString) const {
            return compare(aString) >= 0;
            //return false; 
        }
        //Add version(s) to support const char*...

        //Search...
        int find(const String& aString, size_t anIndex = 0) {
            char* temp = aString.getBuffer();
            size_t left = anIndex;
            size_t right = this->capacity;
            size_t charsLength = strlen(temp);
            while (left + charsLength <= right)
            {
                size_t thisLeft = left;
                size_t findLeft = 0;
                while (this->bufferPointer[thisLeft] == temp[findLeft]) {
                    thisLeft += 1;
                    findLeft += 1;
                    if (findLeft == charsLength) {
                        return left;
                    }
                }
                left += 1;
            }
            return 0;
            //return 0;
        }

        friend std::ostream& operator << (std::ostream& anOut, const String& aStr) {
            char* temp = aStr.getBuffer();
            for (size_t i = 0; i < aStr.size(); i++) {
                anOut << temp[i];
            }
            return anOut;
        }
        friend std::istream& operator >> (std::istream& anOut, String& aString) {
            char* temp = aString.getBuffer();
            for (size_t i = 0; i < aString.size(); i++) {
                anOut >> temp[i];
            }
            return anOut;
        }



    protected:
        size_t length;
    };

} //end namespace

#endif /* String_hpp */