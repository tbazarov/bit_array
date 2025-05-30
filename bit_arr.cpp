#include "bit_arr.hpp"
#include <cstring>
#include <algorithm>
#include <sstream>

namespace
{
    const int BITS_PER_BYTE =  8;
    const int CAPACITY_MULTIPLIER =  2;
}

namespace 
{
	inline int give_memory(int size_bits) 
    {
		return (size_bits + 7) / BITS_PER_BYTE;
	}

    inline int new_bytes_needed(int new_size)
    {
        return (new_size + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
    }

    inline int current_bytes(int size_bits)
    {
        return (size_bits + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
    }

}

BitArray::BitArray() : value(nullptr), size_bits(0), capacity(0) {}

BitArray::~BitArray() {
    delete[] value;
}

BitArray::BitArray(int size_bits, unsigned long value) {
    if (size_bits < 0) throw std::invalid_argument("Кол-во битов не может быть отрицательным");
    allocate_memory(size_bits);
    for (int i = 0; i < size_bits && i < static_cast<int>(sizeof(unsigned long) * BITS_PER_BYTE); ++i) {
        set(i, (value >> i) & 1);
    }
}

BitArray::BitArray(const BitArray& b) : size_bits(b.size_bits), capacity(b.capacity) {
    delete[] value;  
    if (capacity > 0) {
        value = new char[capacity];
        memcpy(value, b.value, capacity);
    } else {
        value = nullptr;
    }
}

void BitArray::swap(BitArray& b) {
    std::swap(value, b.value);
    std::swap(size_bits, b.size_bits);
    std::swap(capacity, b.capacity);
}

BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        BitArray temp(b);
        swap(temp);
    }
    return *this;
}



void BitArray::resize(int new_size, bool val) {
    if (new_size < 0) {
        throw std::invalid_argument("Размер не может быть отрицательным");
    }
    
    
    if (new_size <= size_bits) {
        size_bits = new_size;
        return;
    }
    
    
    if (new_bytes_needed(new_size) > capacity) {
        char* new_value = new char[new_bytes_needed(new_size)](); 
        
        if (value) {
            memcpy(new_value, value, current_bytes(size_bits));
            delete[] value;
        }
        
        value = new_value;
        capacity = new_bytes_needed(new_size);
    }
    
    for (int i = size_bits; i < new_size; ++i) 
    {
        set(i, val);
    }

    size_bits = new_size;
        
}

void BitArray::clear() {
    size_bits = 0;
}

void BitArray::push_back(bool bit) {
    resize(size_bits + 1, bit);
}

BitArray& BitArray::operator&=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < (size_bits + 7) / BITS_PER_BYTE; ++i) {
        value[i] &= b.value[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < (size_bits + 7) / BITS_PER_BYTE; ++i) {
        value[i] |= b.value[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < (size_bits + 7) / BITS_PER_BYTE; ++i) {
        value[i] ^= b.value[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n < 0) throw std::invalid_argument("Количество сдвигов не может быть отрицательным");
    if (n >= size_bits) {
        reset();
        return *this;
    }

    for (int i = 0; i < size_bits - n; ++i) {
        set(i, operator[](i + n));
    }
    for (int i = std::max(size_bits - n, 0); i < size_bits; ++i) {
        reset(i);
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0) throw std::invalid_argument("Количество сдвигов не может быть отрицательным");
    if (n >= size_bits) {
        reset();
        return *this;
    }

    for (int i = size_bits - 1; i >= n; --i) {
        set(i, operator[](i - n));
    }
    for (int i = 0; i < n && i < size_bits; ++i) {
        reset(i);
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= size_bits) throw std::out_of_range("Выход за границу");
    int char_index = n / BITS_PER_BYTE;
    int bit_index = n % BITS_PER_BYTE;
    if (val) {
        value[char_index] |= (1 << bit_index);
    } else {
        value[char_index] &= ~(1 << bit_index);
    }
    return *this;
}

BitArray& BitArray::set() {
    memset(value, 0xFF, (size_bits + 7) / BITS_PER_BYTE);
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    memset(value, 0, (size_bits + 7) / BITS_PER_BYTE);
    return *this;
}

bool BitArray::any() const {
    for (int i = 0; i < (size_bits + 7) / BITS_PER_BYTE; ++i) {
        if (value[i]) return true;
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (int i = 0; i < (size_bits + 7) / BITS_PER_BYTE; ++i) {
        result.value[i] = ~value[i];
    }
    return result;
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < size_bits; ++i) {
        count += operator[](i);
    }
    return count;
}

bool BitArray::operator[](int i) const {
    if (i < 0 || i >= size_bits) throw std::out_of_range("Выход за границу");
    int char_index = i / BITS_PER_BYTE;
    int bit_index = i % BITS_PER_BYTE;
    return (value[char_index] >> bit_index) & 1;
}

int BitArray::size() const {
    return size_bits;
}

bool BitArray::empty() const {
    return size_bits == 0;
}

std::string BitArray::to_string() const {
    std::string result(size_bits, '0');
    for (int i = 0; i < size_bits; ++i) {
        result[i] = '0' + operator[](i);
    }
    return result;
}

void BitArray::allocate_memory(int size_bits) {
    this->size_bits = size_bits;
    capacity = ((size_bits + 7) / BITS_PER_BYTE) * CAPACITY_MULTIPLIER;
    value = new char[capacity];
    memset(value, 0, capacity);
}

void BitArray::check_size_compatibility(const BitArray& b) const {
    if (size_bits != b.size_bits) {
        throw std::invalid_argument("Массивы должны иметь одинаковый размер");
    }
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result ^= b2;
    return result;
}
