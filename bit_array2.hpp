#include <iostream>
#include<vector>


class BitArray
{
public:
  BitArray();
  ~BitArray();

  //Конструирует массив, хранящий заданное количество бит.
  //Первые sizeof(long) бит можно инициализровать с помощью параметра value.
  explicit BitArray(int num_bits, unsigned long value = 0); //сделал
  
  BitArray(const BitArray& b); //сделал

  //Обменивает значения двух битовых массивов.
  void swap(BitArray& b); //сделал

  BitArray& operator=(const BitArray& b);//cделал


  //Изменяет размер массива. В случае расширения, новые элементы 
  //инициализируются значением value.
  void resize(int num_bits, bool value = false); //в процессе
  //Очищает массив.
  void clear();//сделал
  //Добавляет новый бит в конец массива. В случае необходимости 
  //происходит перераспределение памяти.
  void push_back(bool bit);//сделал

  //Битовые операции над массивами.
  //Работают только на массивах одинакового размера.
  //Обоснование реакции на параметр неверного размера входит в задачу.
  BitArray& operator&=(const BitArray& b);//сделал
  BitArray& operator|=(const BitArray& b);//cделал
  BitArray& operator^=(const BitArray& b);//сделал
 
  //Битовый сдвиг с заполнением нулями.
  BitArray& operator<<=(int n);
  BitArray& operator>>=(int n);
  BitArray operator<<(int n) const;
  BitArray operator>>(int n) const;


  //Устанавливает бит с индексом n в значение val.
  BitArray& set(int n, bool val = true);//сделал
  //Заполняет массив истиной.
  BitArray& set();

  //Устанавливает бит с индексом n в значение false.
  BitArray& reset(int n);
  //Заполняет массив ложью.
  BitArray& reset();

  //true, если массив содержит истинный бит.
  bool any();
  //true, если все биты массива ложны.
  bool none();
  //Битовая инверсия
  BitArray operator~();
  //Подсчитывает количество единичных бит.
  int count() const;


  //Возвращает значение бита по индексу i.
  bool operator[](int i);

  int size();
  bool empty();
  
  //Возвращает строковое представление массива.
  std::string to_string();

  private:
  int num_bits = 0;  
  unsigned long value = 0;
  std::vector<bool> array;
  unsigned long max_value = 0;
};

bool operator==(const BitArray & a, const BitArray & b);
bool operator!=(const BitArray & a, const BitArray & b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);