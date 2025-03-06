#include <iostream>
#include <cstring>
#include <cmath>
#include "bit_array2.hpp"
#include <vector>


/*
Во всех заданиях намерено не поставлены спецификаторы static и const. Их необходимо добавить в нужных местах.
Нельзя менять названия public методов класса, а также типы и количество аргументов в них
*/
BitArray::BitArray(){
    int num_bits = 0;  
    unsigned long value = 0;
    std::vector<bool> array;
    unsigned long max_value = 0;
}


BitArray::BitArray(int num_bits, unsigned long value) : array(num_bits, false) , num_bits(num_bits), value(value) {
    /*Конструирует массив, хранящий заданное количество бит. Первые sizeof(long) бит можно инициализровать с помощью параметра value.*/
    int max_value = pow(2, num_bits);
    this->max_value = max_value;
    if (value <= max_value) {
        for (int i = 0; i < sizeof(long); i++) {
            array[i] = (value >> i) & 1;
        }
    }
}

BitArray::BitArray(const BitArray& b) : num_bits(b.num_bits), value(b.value), max_value(b.max_value), array(b.array){};

//Обменивает значения двух битовых массивов.
void BitArray::swap(BitArray& b){
    std::swap(array , b.array);
}

//оператор
BitArray& BitArray::operator=(const BitArray& b){
    if (this == &b){
        return *this;
    }
    num_bits = b.num_bits;
    value = b.value;
    max_value = b.max_value;
    if (num_bits > 0){
        for (int i = 0; i < num_bits; i++) {
            array[i] = b.array[i];
        }
    }
    return *this;
}

void BitArray::resize(int num_bits, bool value){
    /*идет рашсирение массива*/
    array.resize(num_bits, value);
    this->num_bits = num_bits;
}

void BitArray::clear(){
    array.clear();
}

 //Добавляет новый бит в конец массива. В случае необходимости 
  //происходит перераспределение памяти.
void BitArray::push_back(bool bit){
    array.push_back(bit);
    num_bits++;
}

//Битовые операции над массивами.
//Работают только на массивах одинакового размера.
//Обоснование реакции на параметр неверного размера входит в задачу.
BitArray& BitArray::operator&=(const BitArray& b){
    if (num_bits != b.num_bits){
        throw std::invalid_argument("Размер массивов разный");
    }
    for (int i = 0; i < num_bits; i++){
        array[i] = array[i] & b.array[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b){
    if (num_bits != b.num_bits){
        throw std::invalid_argument("Размер массивов разный");
    }
    for (int i = 0; i < num_bits; i++){
        array[i] = array[i] | b.array[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b){
    if (num_bits != b.num_bits){
        throw std::invalid_argument("Размер массивов разный");
    }
    for (int i = 0; i < num_bits; i++){
        array[i] = array[i] ^ b.array[i];
    }
    return *this;
}

/*
BitArray& operator<<=(int n);
BitArray& operator>>=(int n);
BitArray operator<<(int n) const;
BitArray operator>>(int n) const;
*/ 

BitArray& BitArray::operator<<=(int n){
    if (n <= 0){
        return *this;
    }
    if (n >= num_bits) {
        for (int i = 0; i < num_bits; ++i) {
            array[i] = false;
        }
    }
        
    for(int i = 0; i < num_bits - n; i++){
        array[i] = array[i + n];    
    }
    for (int i = num_bits - n; i < num_bits; ++i){
        array[i] = false;
    }
    
    return *this;
}

BitArray& BitArray::set(int n, bool val){
    /*Устанавливает бит с индексом n в значение val.*/
    //устанавливаем n-ый бит в value
    if (n < 0){
        throw std::invalid_argument("Индекс не может быть отрицательным");
    }
    array[n] = val;
    return *this;
}

BitArray& BitArray::set(){
    //делаем true все элементы
    for (int i = 0; i < num_bits; i++){
        array[i] = true;
    }
    return *this;
};

BitArray& BitArray::reset(int n){
    if(n < 0){
        throw std::invalid_argument("Индекс не может быть отрицательным");
    }
    array[n] = false;
    return *this;
}

BitArray& BitArray::reset(){//не true
    for (int i = 0; i < num_bits; i++){
        array[i] = false;
    }
    return *this;
}

//true, если массив содержит истинный бит.
bool BitArray::any(){
    for (int i = 0; i < num_bits; i++){
        if (array[i] == true){
            return true;
        }
    }
    return false;
}

bool BitArray::none(){
    //true если все биты ложные
    for (int i = 0; i < num_bits; i++){
        if (array[i] == true){
            return false;
        }
    }
    return true;
}

 //Битовая инверсия
 BitArray BitArray::operator~(){
    for(int i = 0; i < num_bits; i++){
        for(int j = num_bits; j >= 0; j--){
            array[i] = array[j];
        }
    }
    return *this;
}

//Подсчитывает количество единичных бит.
int BitArray::count() const{
    int cnt = 0;
    for (int i = 0; i < num_bits; i++){
        if (array[i] == true){
            cnt++;
        }
    }
    return cnt;
}

//Возвращает значение бита по индексу i.
bool BitArray::operator[](int i){
    if (i < 0){
        throw std::invalid_argument("Индекс не может быть отрицательным");
    }
    return array[i];
}

int BitArray::size(){
    return num_bits;
}

bool BitArray::empty(){
    //полагаю проверка на пустоту
    if (num_bits == 0){
        return true;
    }
    return false;
}

std::string BitArray::to_string(){
    std::string str = "";
    for (int i = 0; i < num_bits; i++){
        if(array[i] == true){
            str += "1";
        }
        else{
            str += "0";
        }
    }
    return str;
}

int main(){
    return 0;
}