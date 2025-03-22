#include <string>
#include <stdexcept>

class BitArray {
public:
    BitArray();
    ~BitArray();

    BitArray(int size, unsigned long value = 0);
    BitArray(const BitArray& b);

    void swap(BitArray& b);
    BitArray& operator=(const BitArray& b);

    void resize(int size, bool value = false);
    void clear();
    void push_back(bool bit);

    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);

    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    BitArray& set(int n, bool val = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();

    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;

    //bool operator[](int i);
    bool operator[](int i) const;

    // int size();
    int size() const;

    bool empty() const;

    std::string to_string();

private:
    char* value;                         // Указатель на массив данных  
    int size_bits;                       // Общее количество битов
    int capacity;                      // размер зарезервированной памяти

    void allocate_memory(int size);
    void copy_from(const BitArray& b);
    void check_size_compatibility(const BitArray& b) const;
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

