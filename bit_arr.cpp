#include "bit_arr.hpp"
#include <cstring>
#include <algorithm>
#include <sstream>

// Базовый конструктор
BitArray::BitArray() : value(nullptr), size_bits(0), capacity(0) {} 

// Деструктор
BitArray::~BitArray() {
    delete[] value;
}

// Конструктор с указанием количества бит и начального значения
BitArray::BitArray(int size_bits, unsigned long value) {
    if (size_bits < 0) throw std::invalid_argument("Числа не должны быть отрицательными");
    allocate_memory(size_bits);
    for (int i = 0; i < size_bits; ++i) {
        set(i, (value >> i) & 1);
    }
}

// Конструктор копирования
BitArray::BitArray(const BitArray& b) {
    copy_from(b);
}

// обмен двух массивов
void BitArray::swap(BitArray& b) {
    std::swap(value, b.value);
    std::swap(size_bits, b.size_bits);
    std::swap(capacity, b.capacity);
}

// Оператор присваивания
BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        delete[] value;
        copy_from(b);
    }
    return *this;
}

// Изменение размера массива
void BitArray::resize(int size_bits, bool value) {
    if (size_bits < 0) throw std::invalid_argument("Число битов не может быть отрицательным");
    if (size_bits == this->size_bits) return;

    BitArray temp(*this);
    allocate_memory(size_bits);

    int min_bits = std::min(this->size_bits, size_bits);
    for (int i = 0; i < min_bits; ++i) {
        set(i, temp[i]);
    }

    for (int i = min_bits; i < size_bits; ++i) {
        set(i, value);
    }
}

// Очистка массива
void BitArray::clear() {
    delete[] value;
    value = nullptr;
    size_bits = 0;
    capacity = 0;
}

// Добавление нового бита
void BitArray::push_back(bool bit) {
    resize(size_bits + 1, bit);
}

// Битовые операции
BitArray& BitArray::operator&=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < capacity; ++i) {
        value[i] &= b.value[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < capacity; ++i) {
        value[i] |= b.value[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < capacity; ++i) {
        value[i] ^= b.value[i];
    }
    return *this;
}


// Сдвиги
BitArray& BitArray::operator<<=(int n) {
    if (n < 0) throw std::invalid_argument("Неправильный сдвиг");
    if (n >= size_bits) {
        reset();
        return *this;
    }

    for (int i = size_bits - 1; i >= n; --i) {
        set(i, operator[](i - n));
    }
    for (int i = 0; i < n; ++i) {
        reset(i);
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0) throw std::invalid_argument("Неправильный сдвиг");
    if (n >= size_bits) {
        reset();
        return *this;
    }

    for (int i = 0; i < size_bits - n; ++i) {
        set(i, operator[](i + n));
    }
    for (int i = size_bits - n; i < size_bits; ++i) {
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

// Установка битов
BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= size_bits) throw std::out_of_range("Выход за пределы");
    int char_index = n / 8;
    int bit_index = n % 8;
    if (val) {
        value[char_index] |= (1 << bit_index);
    } else {
        value[char_index] &= ~(1 << bit_index);
    }
    return *this;
}

BitArray& BitArray::set() {
    memset(value, 0xFF, capacity);
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    memset(value, 0, capacity);
    return *this;
}

// Проверки
bool BitArray::any() const{
    for (int i = 0; i < capacity; ++i) {
        if (value[i]) return true;
    }
    return false;
}

bool BitArray::none() const{
    return !any();
}

BitArray BitArray::operator~() const{
    BitArray result(*this);
    for (int i = 0; i < capacity; ++i) {
        result.value[i] = ~value[i];
    }
    return result;
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < size_bits; ++i) {
        if (operator[](i)) ++count;
    }
    return count;
}

bool BitArray::operator[](int i) const {
    if (i < 0 || i >= size_bits) throw std::out_of_range("Выход за пределы");
    int char_index = i / 8;
    int bit_index = i % 8;
    return (value[char_index] >> bit_index) & 1;
}

// Доступ к битам
// bool BitArray::operator[](int i) {
//     if (i < 0 || i >= size_bits) throw std::out_of_range("Выход за пределы");
//     int char_index = i / 8;
//     int bit_index = i % 8;
//     return (value[char_index] >> bit_index) & 1;
// }

// int BitArray::size_bits() {
//     return size_bits;
// }

int BitArray::size() const{
    return size_bits;
}

bool BitArray::empty() const{
    return size_bits == 0;
}

std::string BitArray::to_string() {
    std::ostringstream oss;
    for (int i = 0; i < size_bits; ++i) {
        oss << (operator[](i) ? '1' : '0');
    }
    return oss.str();
}

// Приватные методы
void BitArray::allocate_memory(int size_bits) {
    this->size_bits = size_bits;
    capacity = (size_bits + 8 - 1) / 8;
    value = new char[capacity];
    memset(value, 0, capacity);
}

void BitArray::copy_from(const BitArray& b) {
    size_bits = b.size_bits;
    capacity = b.capacity;

    if (value) 
    {
        delete[] value; // очистка памяти, если она до этого была занята
    }

    value = new char[capacity];
    memcpy(value, b.value, capacity);
    
}

void BitArray::check_size_compatibility(const BitArray& b) const {
    if (size_bits != b.size_bits) {
        throw std::invalid_argument("Для побитовых операций массивы BitArrays должны иметь одинаковый размер.");
    }
}

// операторы сравнения
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