//
//  Book.h
//  2dz3
//
//  Created by Иван Трифонов on 27.05.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#ifndef Book_h
#define Book_h
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

class Book {
    std::string _title = "";  //  название книги
    std::string _author = "";  //  автор книги
    size_t _year = 0;  //  год издания книги
    size_t _pages_number = 0;  //  страниц в книге
    std::string _genre = "";  //  жанр книги
    size_t _individual_number = 0;  //  индивидуальный номер книги
public:
    
//  базовые конструкторы
    
    Book() {}
    Book(std::string title, std::string author, size_t year, size_t pages_number, std::string genre) : _title(title), _author(author), _year(year), _pages_number(pages_number), _genre(genre) {}
    Book(const Book& book);
    Book(Book&& book);
    Book& operator = (const Book& book);
    Book& operator = (Book&& book);
    ~Book() {};
    
//  фции работы с книгой
    
    void push_all_brackets(std::string _title, std::string _author, size_t _year, size_t _pages_number, std::string _genre);    //  перезаполнить все поля книги
    void change_bracket();    //  перезаполнить выбранные поля книги
    void print_book();    //  вывести все поля книги
    void get_print_information();    // вывести выборочные поля книги
    void print_book(std::vector<bool> print_vector);    //  в дополнение к get_print_information()
    friend std::ostream& operator << (std::ostream& os, Book& book);    //  вывести все сведения о книге в консоль
    std::string get_bracket(std::string param_name);    //  вывести конкретный параметр из книги
    size_t get_size_t_bracket(std::string param_name);
    void delete_Book_content();    //  перезаполнение книжных полей на "удаленые"
    bool book_checker(std::vector<bool>& parameters, std::vector<std::string>& values);    //  проверка на соответствие книги параметрам поиска
    bool check_text(const std::string& str1, const std::string& str2);    //  проверка наличия str1 в str2
    Book operator+ (const Book& book);    //  сложение 2х книг
    
//  фции работы с индивидуальным номером книги
    
    void push_ind_num(size_t num);    //  ввести индивидуальный номер книги
    void print_ind_num();    //  вывести в консоль индивидуальный номер книги
    int get_ind_num();    //  вернуть индивидуальный номер книги
};

#endif /* Book_h */
