#include <iostream>
#include "bit_arr.hpp"

int main() 
{
        int size;
        std::cout << "Введите размер битового массива: ";
        std::cin >> size;

        if (size <= 0) {
            throw std::invalid_argument("Размер массива должен быть положительным числом");
        }

        BitArray bitArray(size);

        std::cout << "Введите значения битов (0 или 1):" << std::endl;
        for (int i = 0; i < size; ++i) {
            bool value;
            std::cin >> value;
            bitArray.set(i, value);
        }

        std::cout << "Битовый массив: " << bitArray.to_string() << std::endl;

        // Пример операций
        std::cout << "Пример операций:" << std::endl;
        BitArray inverted = ~bitArray;
        std::cout << "Инвертированный массив: " << inverted.to_string() << std::endl;

        BitArray shiftedLeft = bitArray << 1;
        std::cout << "Сдвиг влево на 1: " << shiftedLeft.to_string() << std::endl;

        BitArray shiftedRight = bitArray >> 1;
        std::cout << "Сдвиг вправо на 1: " << shiftedRight.to_string() << std::endl;

        // Сравнение массивов
        BitArray anotherArray(size);
        std::cout << "Введите значения для второго битового массива:" << std::endl;
        for (int i = 0; i < size; ++i) {
            bool value;
            std::cin >> value;
            anotherArray.set(i, value);
        }

        std::cout << "Второй битовый массив: " << anotherArray.to_string() << std::endl;

        if (bitArray == anotherArray) {
            std::cout << "Массивы равны." << std::endl;
        } else {
            std::cout << "Массивы не равны." << std::endl;
        }

        BitArray andResult = bitArray & anotherArray;
        std::cout << "Результат побитового И: " << andResult.to_string() << std::endl;

        BitArray orResult = bitArray | anotherArray;
        std::cout << "Результат побитового ИЛИ: " << orResult.to_string() << std::endl;

        BitArray xorResult = bitArray ^ anotherArray;
        std::cout << "Результат побитового XOR: " << xorResult.to_string() << std::endl;

        return 0;

    } 

    