//
//  library_setup.cpp
//  2dz3
//
//  Created by Иван Трифонов on 26.05.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include <stdio.h>

#include "Library.h"

std::mutex mutex;

std::string setup_read_user() {
    std::string answer;
    std::getline(std::cin, answer);
    if (answer == "выйти") {
        throw (char*)"Вы решили принудительно прекратить работать с библиотекой. Библиотека выключается. Успехов!";
    }
    return answer;
}

void Library::one_argument() {
    std::cout << "Создать новую библиотеку или прочитать имеющуюся? (прочитать/создать)" << std::endl;
    std::string answer = setup_read_user();
    if (answer == "прочитать") {
        get_library();
        return;
    }
    if (answer == "создать") {
        make_library();
        return;
    }
    std::cout << "incorrect input, try again" << std::endl;
    one_argument();
}

void Library::make_library() {
    std::cout << "Введите начальные сведения о библиотеке:" << std::endl;
    std::cout << "наименование библиотеки:\n";
    _name = setup_read_user();
    std::cout << "полок на шкафу в библиотеке:\n";
    std::string result = setup_read_user();
    _shelves = std::atoi(result.c_str());
    result.clear();
    std::cout << "максимальное количество книг на полке:\n";
    result = setup_read_user();
    _books = std::atoi(result.c_str());
    if ((_shelves == 0) || (_books == 0)) {
        std::cout << "Введенные сведения некорректны. Пожалуйста, исправьте.\n";
        make_library();
        return;
    }
}

void Library::two_arguments(std::string position) {
    read_library(position);
}

void Library::read_library(std::string& position) {
    std::ifstream read(position);
    if (read.fail()) {
        std::cout << "библиотека не найдена, попробуйте еще раз" << std::endl;
        get_library();
        return;
    }
    _position = position;
    download_library(read);
}

void Library::get_library() {
    std::cout << "Введите имя (местоположение) библиотеки:" << std::endl;
    std::string answer = setup_read_user();
    read_library(answer);
}

void Library::download_library(std::ifstream& position) {
    std::getline(position, _name, '\n');    //  тут и ниже - чтение начальных сведений о библиотеке
    std::string answer;
    std::getline(position, answer, '\n');
    _wardrobes = static_cast<size_t>(std::atoi(answer.c_str()));
    answer.clear();
    std::getline(position, answer, '\n');
    _shelves = static_cast<size_t>(std::atoi(answer.c_str()));
    answer.clear();
    std::getline(position, answer, '\n');
    _books = static_cast<size_t>(std::atoi(answer.c_str()));
    answer.clear();
    std::getline(position, answer, '\n');
    _all_books = static_cast<size_t>(std::atoi(answer.c_str()));
    answer.clear();
    std::getline(position, answer, '\n');
    _deleted.clear();
    size_t _deleted_size = static_cast<size_t>(std::atoi(answer.c_str()));
    _deleted.resize(_deleted_size);
    answer.clear();
    for (size_t i = 0; i < _deleted_size; ++i) {
        std::getline(position, answer, '\n');
        _deleted[i] = static_cast<size_t>(std::atoi(answer.c_str()));
        answer.clear();
    }
    position.close();
    _storage.resize(_wardrobes);    //  выделена память под скачивание шкафов
    std::vector<std::ifstream> files(_wardrobes);
    for (size_t i = 0; i < _wardrobes; ++i) {
        files[i].open(_position + "_wardrobe_" + std::to_string(i + 1));
        if (files[i].fail()) {
            std::cout << "Ошибка открытия файла - содержимого шкафа №" << i + 1 << ".\nПопробуйте еще раз.\n";
            get_library();
            return;
        }
    }
    std::vector<std::future<void>> vec_fut;
    vec_fut.reserve(_wardrobes);
    for (size_t i = 0; i < _wardrobes; ++i) {
        vec_fut.push_back(std::async(&Library::read_wardrobe, this, std::ref(files[i]), i));
    }
    for (size_t i = 0; i < _wardrobes; ++i) {
        vec_fut[i].wait();
        files[i].close();
    }
}

void Library::read_wardrobe(std::ifstream& file, size_t wardrobe) {
    std::list<std::list<std::list<Book>>>::iterator iter = _storage.begin();
    for (size_t i = 0; i < wardrobe; ++i) {    //  получить итератор на нужный шкаф
        ++iter;
    }
    std::string answer;
    std::getline(file, answer);
    size_t shelves = static_cast<size_t>(std::atoi(answer.c_str()));
    answer.clear();
    mutex.lock();
    (*iter).resize(shelves);    //  изменение _storage
    mutex.unlock();
    std::list<std::list<Book>>::iterator jter = (*iter).begin();
    for (; jter != (*iter).end(); ++jter) {
        read_shelve(file, jter);
    }
}

void Library::read_shelve(std::ifstream& file, std::list<std::list<Book>>::iterator jter) {
    std::string answer;
    std::getline(file, answer);
    size_t books = static_cast<size_t>(std::atoi(answer.c_str()));
    answer.clear();
    (*jter).resize(books);
    std::list<Book>::iterator kter = (*jter).begin();
    for (; kter != (*jter).end(); ++kter) {
        read_book(file, kter);
    }
}

void Library::read_book(std::ifstream& file, std::list<Book>::iterator kter) {
    std::string _title = "";
    std::string _author = "";
    size_t _year = 0;
    size_t _pages_number = 0;
    std::string _genre = "";
    size_t _individual_number = 0;
    std::getline(file, _title);
    std::getline(file, _author);
    std::string result;
    std::getline(file, result);
    _year = static_cast<size_t>(std::atoi(result.c_str()));
    result.clear();
    std::getline(file, result);
    _pages_number = static_cast<size_t>(std::atoi(result.c_str()));
    result.clear();
    std::getline(file, _genre);
    std::getline(file, result);
    _individual_number = static_cast<size_t>(std::atoi(result.c_str()));
    result.clear();
    (*kter).push_all_brackets(_title, _author, _year, _pages_number, _genre);
    (*kter).push_ind_num(_individual_number);
}
