//
//  Library.h
//  2dz3
//
//  Created by Иван Трифонов on 26.05.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#ifndef Library_h
#define Library_h

#include <list>
#include <mutex>
#include <future>
#include <thread>
#include <map>

#include "Book.h"

class Library {
    std::string _name = "";    //  имя библиотеки
    std::list<std::list<std::list<Book>>> _storage;    //  хранилище книг в библиотеке
    size_t _wardrobes = 0;    //  число шкафов в библиотеке (расчетное)
    size_t _shelves = 0;    //  число полок на шкафу (const)
    size_t _books = 0;    //  максимальное число книг на полке
    size_t _all_books = 0;    //  индивидуальный номер крайней книги
    std::vector<size_t> _deleted = {};    //  хранилище свободных индивидуальных номеров
    std::string _position = "";    //  текущее местоположение книги
public:
    
//  базовые конструкторы
    
    Library() {}    //  разработан конструктор с пустой инициализацией
    Library(std::string name) : _name(name) {}    //  разработан конструктор с инициализацией имени библиотеки
    Library(const Library&) = delete;    //  в рамках данной работы не реализуются
    Library(Library&&) = delete;    //  в рамках данной работы не реализуются
    Library& operator = (const Library&) = delete;    //  в рамках данной работы не реализуются
    Library& operator = (Library&&) = delete;    //  в рамках данной работы не реализуются
    ~Library() {}    //  разработан деструктор
    
//  запуск библиотеки
    
    void one_argument();
    void make_library();    //  создание пустой библиотеки
    void two_arguments(std::string position);
    void read_library(std::string& name);    //  открыть сохраненную библиотеку
    void get_library();    //  получить местоположение библиотеки (дополняет read_library())
    void download_library(std::ifstream& position);    //  скачать открытую библиотеку в программу
    void read_wardrobe(std::ifstream& file, size_t wardrobe);    //  чтение файла с данными шкафа
    void read_shelve(std::ifstream& file, std::list<std::list<Book>>::iterator jter);    //  чтение данных полки из файла
    void read_book(std::ifstream& file, std::list<Book>::iterator kter);    //  чтение конкретно взятой книги из файла
    
//  фции терминала
    
    void terminal();    //  терминал - обработчик команд пользователя
    
    void add_book();    //  добавить книгу
    void safe_library();    //  сохранить библиотеку
    void search_for_books();    //  найти книгу
    void sort_books();    //  сортировка книг библиотеки по запросу пользователя
    void read_books();    //  считывание нескольких книг из файла и занесение их в библиотеку
    void summ_books();    //  суммирование 2х выбранных книг и добавление результата в библиотеку
    void print_book(size_t id);    //  напечатать книгу
    void print_all_books();    //  напечатать все книги
    
    void terminal_handler(size_t id);    //  обработчик фций терминала
    void book_edit(int num_book);    //  редактировать книгу
    void delete_book(size_t id);    // удалить книгу
    void commands();    // чтение всех функциональных возможностей библиотеки
    void exit_mode(std::string& answer1);    //  выход из библиотеки
    
//  вспомогательные фции
    
    void read_user(std::string& answer);    //  чтение команды пользователя
    
    void storage_finder(std::string _title, std::string _author, size_t _year, size_t _pages_number, std::string _genre);    //  поиск места для новой книги и сохранение
    void add_to_deleted(std::string _title, std::string _author, size_t _year, size_t _pages_number, std::string _genre);    //  добавление новой книги по адресу удаленной старой
    void count_wardrobes();    //  актуализировать число шкафов
    void add_new_wardrobe();    //  добавить шкаф
    void add_new_shelve();    //  добавить в последний шкаф полку
    
    size_t count_wardrobes(size_t num_book);    //  определить номер шкафа книги
    size_t count_shelve(size_t num_book);    //  определить номер полки книги
    size_t count_book_pos(size_t num_book);    //  определить позицию книги на полке
    
    bool check_corrected(std::vector<int>& num_books);    //  проверить корректность введения индивидуального номера книги (для void terminal_handler(size_t id))
    
    void string_sorting_books(std::string param_name);    //  в дополнение к sort_books();
    void size_t_sorting_books(std::string param_name);    //  в дополнение к sort_books();
    
//  сохранение библиотеки
    
    void get_position();    //  определение местоположения библиотеки для сохранения
    void choose_position();    //  выбрать позицию для сохранения библиотеки
    void new_position();    //  изменение текущего местоположения библиотеки
    bool incorrect_write_to_file();    //  проверка возможности открытия файда для записи
    void write_to_wardrobe(std::ofstream& os, size_t i);    //  записать в файл сведения о шкафе
    void write_shelve(std::ofstream& os, std::list<std::list<Book>>::iterator iter);    //  записать в файл полку
    void last_write_to_wardrobe(std::ofstream& os);    //  записать последний шкаф в файл
    void last_shelve_last_write_to_wardrobe(std::ofstream& os);    //  записать в последний шкаф последнюю полку
    
    Book& get_book_by_id(size_t id);    //  получение ссылки на книгу по ее id
    
    void delete_library();    //  удалить библиотеку из оперативной памяти

};

std::string setup_read_user();    //  прочитать команду пользователя на этапе запуска библиотеки

#endif /* Library_h */
