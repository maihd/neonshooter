#pragma once

#include <string.h>

#ifndef MOJO_DEFINED_ALLOCATOR
#define MOJO_DEFINED_ALLOCATOR
inline namespace Mojo
{
    struct Allocator
    {
        static Allocator* const Default;

        virtual void* Acquire(int size, int align);
        virtual void  Release(void* pointer);
    };
}
#endif

inline namespace Mojo
{
    template <typename TValue>
    struct HashTable
    {
    public:
        int        count;
        int        GetCapacity;
                   
        int*       hashs;
        int        hashCount;
                   
        int*       nexts;
        int*       keys;
        TValue*    values;

        Allocator* allocator;

        inline HashTable(Allocator* allocator = Allocator::Default)
            : count(0)
            , GetCapacity(0)

            , hashs(0)
            , hashCount(64)

            , nexts(0)
            , keys(0)
            , values(0)

            , allocator(allocator)
        {
        }

        inline HashTable(int hashCount, Allocator* allocator = Allocator::Default)
            : count(0)
            , GetCapacity(0)
            
            , hashs(0)
            , hashCount(hashCount)
            
            , nexts(0)
            , keys(0)
            , values(0)

            , allocator(allocator)
        {
        }

        inline ~HashTable()
        {
            allocator->Release(values);
            allocator->Release(keys);
            allocator->Release(nexts);
            allocator->Release(hashs);

            this->count     = 0;
            this->GetCapacity  = 0;

            this->hashs     = 0;
            this->hashCount = 0;

            this->nexts     = 0;
            this->keys      = 0;
            this->values    = 0;
        }

        inline HashTable(const HashTable& other) = delete;
        inline HashTable& operator=(const HashTable& other) = delete;

        inline HashTable(HashTable<TValue>&& other)
            : count(other.count)
            , GetCapacity(other.GetCapacity)

            , hashs(other.hashs)
            , hashCount(other.hashCount)

            , nexts(other.nexts)
            , keys(other.keys)
            , values(other.values)

            , allocator(other.allocator)
        {
            other.Detach();
        }
     
        inline HashTable<TValue>& operator=(HashTable<TValue>&& other)
        {
            this->~HashTable();
            
            this->count       = other.count;
            this->GetCapacity    = other.GetCapacity;

            this->hashs       = other.hashs;
            this->hashCount   = other.hashCount;

            this->nexts       = other.nexts;
            this->keys        = other.keys;
            this->values      = other.values;

            this->allocator  = other.allocator;
     
            other.Detach();
     
            return *this;
        }

        // Clear the this
        inline void Clear(void)
        {
            this->count = 0;
        }

        // Unref the this
        inline void Detach(void)
        {
            this->count     = 0;
            this->GetCapacity  = 0;

            this->hashs     = 0;
            this->hashCount = 0;

            this->nexts     = 0;
            this->keys      = 0;
            this->values    = 0;
        }

        // Clean memory usage
        inline void Cleanup(void)
        {
            this->~HashTable();
        }

        // Find index of entry with key
        int IndexOf(int key, int* outHash = 0, int* outPrev = 0) const
        {
            if (!this->hashs)
            {
                return -1;
            }

            int hash = (int)((unsigned)key % (unsigned)this->hashCount);
            int curr = this->hashs[hash];
            int prev = -1;

            while (curr > -1)
            {
                if (this->keys[curr] == key)
                {
                    break;
                }

                prev = curr;
                curr = this->nexts[curr];
            }

            if (outHash) *outHash = hash;
            if (outPrev) *outPrev = prev;
            return curr;
        }

        // Determine if hash table contains the entry with key
        inline bool ContainsKey(int key) const
        {
            return IndexOf(key) > -1;
        }

        // Get value of entry with key
        const TValue& GetValue(int key) const
        {
            int curr = IndexOf(key);
            return values[curr];
        }

        // Get value of entry with key
        const TValue& GetValue(int key, const TValue& defaultValue) const
        {
            int curr = IndexOf(key);
            return (curr > -1) ? values[curr] : defaultValue;
        }

        // Get value of entry with key. If entry exists return true, false otherwise.
        bool TryGetValue(int key, TValue* outValue) const
        {
            int curr = IndexOf(key);
            if (curr > -1)
            {
                *outValue = values[curr];
                return true;
            }
            else
            {
                return false;
            }
        }

        // Get value entry, if not exists create new. 
        // Return true if success, false otherwise.
        bool GetValueOrNewSlot(int key, TValue** value)
        {
            int hash, prev;
            int curr = this->IndexOf(key, &hash, &prev);

            if (curr < 0)
            {
                if (!this->hashs)
                {
                    this->hashs = (int*)allocator->Acquire(sizeof(int) * hashCount, alignof(int));
                    //ASSERT(this, "Out of memory");
                    
                    for (int i = 0; i < hashCount; i++)
                    {
                        this->hashs[i] = -1;
                    }

                    // Re-calculate hash
                    prev = -1;
                    hash = (int)((unsigned)key % (unsigned)this->hashCount);
                }

                if (this->count + 1 > this->GetCapacity)
                {
                    int   oldSize   = this->GetCapacity;
                    int   newSize   = oldSize | 32;

                    newSize -= 1;
                    newSize |= newSize >> 1;
                    newSize |= newSize >> 2;
                    newSize |= newSize >> 4;
                    newSize |= newSize >> 8;
                    newSize |= newSize >> 16;
                    newSize += 1;

                    void* newNexts  = allocator->Acquire(newSize * sizeof(int), alignof(int));
                    void* newKeys   = allocator->Acquire(newSize * sizeof(int), alignof(int));
                    void* newValues = allocator->Acquire(newSize * sizeof(TValue), alignof(TValue));

                    if (!newNexts || !newKeys || !newValues)
                    {
                        allocator->Release(newNexts);
                        allocator->Release(newKeys);
                        allocator->Release(newValues);
                        return false;
                    }
                    else
                    {
                        ::memcpy(newNexts, this->nexts, oldSize * sizeof(int));
                        ::memcpy(newKeys, this->keys, oldSize * sizeof(int));
                        ::memcpy(newValues, this->values, oldSize * sizeof(TValue));

                        allocator->Release(this->nexts);
                        allocator->Release(this->keys);
                        allocator->Release(this->values);
                    
                        this->GetCapacity = newSize;
                        this->nexts    = (int*)newNexts;
                        this->keys     = (int*)newKeys;
                        this->values   = (TValue*)newValues;
                    }
                }

                curr = this->count;
                if (prev > -1)
                {
                    this->nexts[prev] = curr;
                }
                else
                {
                    this->hashs[hash] = curr;
                }
                this->nexts[curr] = -1;
                this->keys[curr] = key;

                this->count = this->count + 1;
            }

            *value = &this->values[curr];
            return true;
        }

        // Get value entry, if not exists create new.
        // Return a reference to value entry if success, otherwise abort the process.
        TValue& GetValueOrNewSlot(int key)
        {
            TValue* innerValue;
            if (!this->GetValueOrNewSlot(key, &innerValue))
            {
                //ALWAYS_FALSE_ASSERT("Out of memory.");
            }
            return *innerValue;
        }

        // Set entry's value, if not exists create new
        bool SetValue(int key, const TValue& value)
        {
            TValue* innerValue;
            if (this->GetValueOrNewSlot(key, &innerValue))
            {
                *innerValue = value;
                return true;
            }
            else
            {
                return false;
            }
        }

        // Remove an entry that has given key
        bool Remove(int key)
        {
            int prev;
            int hash;
            int curr = this->IndexOf(key, &hash, &prev);
            return this->Erase(curr, hash, prev);
        }
        
        // Remove the entry at given index
        bool Erase(int index)
        {
            if (index > -1 && index < this->count)
            {
                return this->Remove(this->keys[index]);
            }
            else
            {
                return false;
            }
        }

        // Remove the entry at given index, hash entry, and previous entry
        bool Erase(int curr, int hash, int prev)
        {
            if (curr > -1)
            {
                if (prev > -1)
                {
                    this->nexts[prev] = -1;
                }
                else
                {
                    this->hashs[hash] = -1;
                }

                if (curr < this->count - 1)
                {
                    int last = this->count - 1;
                    this->nexts[curr]  = this->nexts[last];
                    this->keys[curr]   = this->keys[last];
                    this->values[curr] = this->values[last];

                    this->IndexOf(this->keys[curr], &hash, &prev);
                    if (prev > -1)
                    {
                        this->nexts[prev] = curr;
                    }
                    else
                    {
                        this->hashs[hash] = curr;
                    }
                }

                this->count = this->count - 1;
                return true;
            }
            else
            {
                return false;
            }
        }
    };
}