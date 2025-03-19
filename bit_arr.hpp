#include <string>
#include <stdexcept>

class BitArray {
public:
    BitArray();
    ~BitArray();

    explicit BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& b);

    void swap(BitArray& b);
    BitArray& operator=(const BitArray& b);

    void resize(int num_bits, bool value = false);
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

    const bool any();
    const bool none();
    const BitArray operator~();
    int count() const;

    //bool operator[](int i);
    const bool operator[](int i) const;

    // int size();
    int size() const;

    const bool empty();

    std::string to_string();

private:
    static const int bits_in_char = 8; // Количество бит в одном char
    char* value;                         // Указатель на массив данных
    int num_bits;                       // Общее количество бит
    int num_chars;                      // Количество char'ов для хранения битов

    void allocate_memory(int num_bits);
    void copy_from(const BitArray& b);
    void check_size_compatibility(const BitArray& b) const;
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

