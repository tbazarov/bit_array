#include "bit_arr.hpp"
#include <cstring>
#include <algorithm>
#include <sstream>

// Базовый конструктор
BitArray::BitArray() : value(nullptr), num_bits(0), num_chars(0) {} // why we use список инициализации вместо инициализации в теле конструктора

// Деструктор
BitArray::~BitArray() {
    delete[] value;
}

// Конструктор с указанием количества бит и начального значения
BitArray::BitArray(int num_bits, unsigned long value) {
    if (num_bits < 0) throw std::invalid_argument("Числа не должны быть отрицательными");
    allocate_memory(num_bits);
    for (int i = 0; i < num_bits; ++i) {
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
    std::swap(num_bits, b.num_bits);
    std::swap(num_chars, b.num_chars);
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
void BitArray::resize(int num_bits, bool value) {
    if (num_bits < 0) throw std::invalid_argument("Число битов не может быть отрицательным");
    if (num_bits == this->num_bits) return;

    BitArray temp(*this);
    allocate_memory(num_bits);

    int min_bits = std::min(this->num_bits, num_bits);
    for (int i = 0; i < min_bits; ++i) {
        set(i, temp[i]);
    }

    for (int i = min_bits; i < num_bits; ++i) {
        set(i, value);
    }
}

// Очистка массива
void BitArray::clear() {
    delete[] value;
    value = nullptr;
    num_bits = 0;
    num_chars = 0;
}

// Добавление нового бита
void BitArray::push_back(bool bit) {
    resize(num_bits + 1, bit);
}

// Битовые операции
BitArray& BitArray::operator&=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < num_chars; ++i) {
        value[i] &= b.value[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < num_chars; ++i) {
        value[i] |= b.value[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    check_size_compatibility(b);
    for (int i = 0; i < num_chars; ++i) {
        value[i] ^= b.value[i];
    }
    return *this;
}


// Сдвиги
BitArray& BitArray::operator<<=(int n) {
    if (n < 0) throw std::invalid_argument("Неправильный сдвиг");
    if (n >= num_bits) {
        reset();
        return *this;
    }

    for (int i = num_bits - 1; i >= n; --i) {
        set(i, operator[](i - n));
    }
    for (int i = 0; i < n; ++i) {
        reset(i);
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0) throw std::invalid_argument("Неправильный сдвиг");
    if (n >= num_bits) {
        reset();
        return *this;
    }

    for (int i = 0; i < num_bits - n; ++i) {
        set(i, operator[](i + n));
    }
    for (int i = num_bits - n; i < num_bits; ++i) {
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
    if (n < 0 || n >= num_bits) throw std::out_of_range("Выход за пределы");
    int char_index = n / bits_in_char;
    int bit_index = n % bits_in_char;
    if (val) {
        value[char_index] |= (1 << bit_index);
    } else {
        value[char_index] &= ~(1 << bit_index);
    }
    return *this;
}

BitArray& BitArray::set() {
    memset(value, 0xFF, num_chars);
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    memset(value, 0, num_chars);
    return *this;
}

// Проверки
const bool BitArray::any() {
    for (int i = 0; i < num_chars; ++i) {
        if (value[i]) return true;
    }
    return false;
}

const bool BitArray::none() {
    return !any();
}

const BitArray BitArray::operator~() {
    BitArray result(*this);
    for (int i = 0; i < num_chars; ++i) {
        result.value[i] = ~value[i];
    }
    return result;
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < num_bits; ++i) {
        if (operator[](i)) ++count;
    }
    return count;
}

const bool BitArray::operator[](int i) const {
    if (i < 0 || i >= num_bits) throw std::out_of_range("Выход за пределы");
    int char_index = i / bits_in_char;
    int bit_index = i % bits_in_char;
    return (value[char_index] >> bit_index) & 1;
}

// Доступ к битам
// bool BitArray::operator[](int i) {
//     if (i < 0 || i >= num_bits) throw std::out_of_range("Выход за пределы");
//     int char_index = i / bits_in_char;
//     int bit_index = i % bits_in_char;
//     return (value[char_index] >> bit_index) & 1;
// }

// int BitArray::size() {
//     return num_bits;
// }

int BitArray::size() const{
    return num_bits;
}

const bool BitArray::empty() {
    return num_bits == 0;
}

std::string BitArray::to_string() {
    std::ostringstream oss;
    for (int i = 0; i < num_bits; ++i) {
        oss << (operator[](i) ? '1' : '0');
    }
    return oss.str();
}

// Приватные методы
void BitArray::allocate_memory(int num_bits) {
    this->num_bits = num_bits;
    num_chars = (num_bits + bits_in_char - 1) / bits_in_char;
    value = new char[num_chars];
    memset(value, 0, num_chars);
}

void BitArray::copy_from(const BitArray& b) {
    num_bits = b.num_bits;
    num_chars = b.num_chars;
    value = new char[num_chars];
    memcpy(value, b.value, num_chars); // realize in pointer
}

void BitArray::check_size_compatibility(const BitArray& b) const {
    if (num_bits != b.num_bits) {
        throw std::invalid_argument("BitArrays must have the same size for bitwise operations");
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