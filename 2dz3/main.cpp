//
//  main.cpp
//  2dz3
//
//  Created by Иван Трифонов on 18.05.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "Library.h"
#include "Book.h"

int main(int argc, const char * argv[]) {
    
    //  запуск программы
    
    std::cout << "\n\nДобро пожаловать в электронную книжную библиотеку!\nЧтобы выйти из библиотеки вводите \"выйти\"\nПриятного использования!\n\n" << std::endl;
    Library library;
    try {
        if (argc == 1) {
            library.one_argument();
        }
        if (argc == 2) {
            library.two_arguments(argv[1]);
        }
        if (argc > 2) {
            throw (char*)"Слишком много аргументов на входе";
        }
    } catch (char* str) {
        std::cout << str << std::endl;
        return -1;
    }
    
    //  работа программы
    
    std::cout << "\n\nБиблиотека успешно загружена и готова к работе!\n";
    library.commands();
    std::cout << "Для получения списка команд для выполнения наберите команду ls" << std::endl;
    try {
        while(1) {
            library.terminal();
        }
    } catch (char* str) {
        std::cout << str << std::endl;
        return 0;
    }
    //  демонстрация сложениея 2х книг:
    //  
    return 0;
}
