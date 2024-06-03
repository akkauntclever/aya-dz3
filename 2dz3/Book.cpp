//
//  Book.cpp
//  2dz3
//
//  Created by Иван Трифонов on 27.05.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include <stdio.h>
#include "Book.h"

//  базовые конструкторы

Book::Book(const Book& book) {
    _title = book._title;
    _author = book._author;
    _year = book._year;
    _pages_number = book._pages_number;
    _genre = book._genre;
}

Book::Book(Book&& book) {
    _title = book._title;
    _author = book._author;
    _year = book._year;
    _pages_number = book._pages_number;
    _genre = book._genre;
    _individual_number = book._individual_number;
    book._title = "";
    book._author = "";
    book._year = 0;
    book._pages_number = 0;
    book._genre = "";
    _individual_number = 0;
}

Book& Book::operator = (const Book& book) {
    _title = book._title;
    _author = book._author;
    _year = book._year;
    _pages_number = book._pages_number;
    _genre = book._genre;
    return *this;
}

Book& Book::operator = (Book&& book) {
    _title = book._title;
    _author = book._author;
    _year = book._year;
    _pages_number = book._pages_number;
    _genre = book._genre;
    _individual_number = book._individual_number;
    book._title = "";
    book._author = "";
    book._year = 0;
    book._pages_number = 0;
    book._genre = "";
    _individual_number = 0;
    return *this;
}

//  фции работы с книгой

void Book::push_all_brackets(std::string title, std::string author, size_t year, size_t pages_number, std::string genre) {
    _title = title;
    _author = author;
    _year = year;
    _pages_number = pages_number;
    _genre = genre;
}

void Book::change_bracket() {
    std::string flag;
    while (1) {
        std::cout << "Введите команду: (закончить/title/author/year/pages_number/genre)" << std::endl;
        std::getline(std::cin, flag);
        if (flag == "закончить") {
            break;
        }
        if (flag == "title") {
            std::cout << "Введите новое значение поля title: (старое: " << _title << ")" << std::endl;
            _title.clear();
            std::getline(std::cin, _title);
        }
        if (flag == "author") {
            std::cout << "Введите новое значение поля author: (старое: " << _author << ")" << std::endl;
            _author.clear();
            std::getline(std::cin, _author);
        }
        if (flag == "year") {
            std::cout << "Введите новое значение поля year: (старое: " << _year << ")" << std::endl;
            std::string result;
            std::getline(std::cin, result);
            _year = std::atoi(result.c_str());
        }
        if (flag == "pages_number") {
            std::cout << "Введите новое значение поля pages_number: (старое: " << _pages_number << ")" << std::endl;
            std::string result;
            std::getline(std::cin, result);
            _pages_number = std::atoi(result.c_str());
        }
        if (flag == "genre") {
            std::cout << "Введите новое значение поля genre: (старое: " << _genre << ")" << std::endl;
            _genre.clear();
            std::getline(std::cin, _genre);
        }
        flag.clear();
    }
}

void Book::print_book() {
    std::cout << "title: " << _title << std::endl;
    std::cout << "author: " << _author << std::endl;
    std::cout << "year: " << _year << std::endl;
    std::cout << "pages_number: " << _pages_number << std::endl;
    std::cout << "genre: " << _genre << std::endl;
    std::cout << "individual_number: " << _individual_number << std::endl;
}

void Book::get_print_information() {
    std::cout << "Индивидуальный номер книги: " << _individual_number << std::endl;
    std::cout << "Введите поля для вывода: (title author year pages_number genre individual_number)" << std::endl;
    std::string answer;
    std::vector<bool> print_vector {
        0, 0, 0, 0, 0, 0
    };
    std::getline(std::cin, answer);
    std::stringstream result;
    for (size_t i = 0; i < answer.size(); ++i) {
        result << answer[i];
    }
    while (result >> answer) {
        if (answer == "title") {
            print_vector[0] = 1;
        }
        if (answer == "author") {
            print_vector[1] = 1;
        }
        if (answer == "year") {
            print_vector[2] = 1;
        }
        if (answer == "pages_number") {
            print_vector[3] = 1;
        }
        if (answer == "genre") {
            print_vector[4] = 1;
        }
        if (answer == "individual_number") {
            print_vector[5] = 1;
        }
        answer.clear();
    }
    print_book(print_vector);
}

void Book::print_book(std::vector<bool> print_vector) {
    if (print_vector[0] == 1) {
        std::cout << "title: " << _title << std::endl;
    }
    if (print_vector[1] == 1) {
        std::cout << "author: " << _author << std::endl;
    }
    if (print_vector[2] == 1) {
        std::cout << "year: " << _year << std::endl;
    }
    if (print_vector[3] == 1) {
        std::cout << "pages_number: " << _pages_number << std::endl;
    }
    if (print_vector[4] == 1) {
        std::cout << "genre: " << _genre << std::endl;
    }
    if (print_vector[5] == 1) {
        std::cout << "individual_number: " << _individual_number << std::endl;
    }
}

std::ostream& operator << (std::ostream& os, Book& book) {
    os << book._title << std::endl << book._author << std::endl << book._year << std::endl << book._pages_number << std::endl << book._genre << std::endl << book._individual_number << std::endl;
    return os;
}

std::string Book::get_bracket(std::string param_name) {
    if (param_name == "title") {
        return _title;
    }
    if (param_name == "author") {
        return _author;
    }
    if (param_name == "genre") {
        return _genre;
    }
    return "";
}

size_t Book::get_size_t_bracket(std::string param_name) {
    if (param_name == "year") {
        return _year;
    }
    if (param_name == "pages_number") {
        return _pages_number;
    }
    return 0;
}

void Book::delete_Book_content() {
    _title = "book deleted";
    _author = "book deleted";
    _year = 0;
    _pages_number = 0;
    _genre = "book deleted";
}

bool Book::book_checker(std::vector<bool>& parameters, std::vector<std::string>& values) {
    if (parameters[0]) {
        if (!check_text(values[0], _title)) {
            return false;
        }
    }
    if (parameters[1]) {
        if (!check_text(values[1], _author)) {
            return false;
        }
    }
    if (parameters[2]) {
        if (static_cast<size_t>(std::stoi(values[2])) != _year) {
            return false;
        }
    }
    if (parameters[3]) {
        if (static_cast<size_t>(std::stoi(values[3])) != _pages_number) {
            return false;
        }
    }
    if (parameters[4]) {
        if (!check_text(values[4], _genre)) {
            return false;
        }
    }
    return true;
}

bool Book::check_text(const std::string& str1, const std::string& str2) {
    if (str2.size() < str1.size()) {
        return false;
    }
    for(size_t i = 0; i < str2.size() - str1.size() + 1; ++i) {
        std::string temp_str = "";
        for (size_t j = 0; j < str1.size(); ++j) {
            temp_str += str2[i + j];
        }
        if (str1 == temp_str) {
            return true;
        }
    }
    return false;
}

Book Book::operator+ (const Book& book) {
    Book book1;
    book1._title = _title + ", " + book._title;
    book1._author = _author + ", " + book._author;
    book1._year = 0;
    book1._pages_number = _pages_number + book._pages_number;
    book1._genre = _genre + ", " + book._genre;
    book1._individual_number = 0;
    return book1;
}

//  фции работы с индивидуальным номером книги

void Book::push_ind_num(size_t num) {
    _individual_number = num;
}

void Book::print_ind_num() {
    std::cout << _individual_number << std::endl;
}

int Book::get_ind_num() {
    return static_cast<int>(_individual_number);
}
