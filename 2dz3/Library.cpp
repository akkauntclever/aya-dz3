//
//  Library.cpp
//  2dz3
//
//  Created by Иван Трифонов on 26.05.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include <stdio.h>

#include "Library.h"

std::mutex mutex1;

//  фции терминала

void Library::terminal() {
    std::cout << _name << " $ ";
    std::string result;
    read_user(result);
    if (result == "new") {
        add_book();
        return;
    }
    if (result == "search") {
        search_for_books();
        return;
    }
    if (result == "edit") {
        terminal_handler(1);
        return;
    }
    if (result == "delete") {
        terminal_handler(2);
        return;
    }
    if (result == "save") {
        safe_library();
        std::cout << "Библиотека сохранена." << std::endl;
        return;
    }
    if (result == "read") {
        read_books();
        return;
    }
    if (result == "sort") {
        sort_books();
        return;
    }
    if (result == "print") {
        terminal_handler(3);
        return;
    }
    if (result == "ls") {
        commands();
        return;
    }
    if (result == "summ") {
        summ_books();
        return;
    }
    std::cout << "command not found, try again" << std::endl;
}

void Library::add_book() {
    std::cout << "Введите сведения о новой книге:" << std::endl;
    std::string _title = "";
    std::string _author = "";
    size_t _year = 0;
    size_t _pages_number = 0;
    std::string _genre = "";
    std::cout << "название книги:" << std::endl;
    std::getline(std::cin, _title);
    std::cout << "автор книги:" << std::endl;
    std::getline(std::cin, _author);
    std::cout << "год издания книги:" << std::endl;
    std::string result;
    std::getline(std::cin, result);
    _year = static_cast<size_t>(std::atoi(result.c_str()));
    result.clear();
    std::cout << "страниц в книге:" << std::endl;
    std::getline(std::cin, result);
    _pages_number = static_cast<size_t>(std::atoi(result.c_str()));
    std::cout << "жанр книги:" << std::endl;
    std::getline(std::cin, _genre);
    storage_finder(_title, _author, _year, _pages_number, _genre);
}

void Library::safe_library() {
    get_position();    //  получить местонахождение библиотеки
    if (incorrect_write_to_file()) {    //  повторить в случае ошибки
        safe_library();
        return;
    }
    std::ofstream file(_position);    //  локально считать файл
    file << _name << '\n' << _wardrobes << '\n' << _shelves << '\n' << _books << '\n' << _all_books << '\n' << _deleted.size() << '\n';    //  запись текущих сведений о библиотеке
    for (auto pos : _deleted) {    //  запись сведений о наличии свободных ячеек
        file << std::to_string(pos) << '\n';
    }
    file.close();    //  конец работы с конфигурационным файлом
    std::vector<std::ofstream> files(_wardrobes);    //  создание базы для открытия нескольких новых файлов для записи в них шкафов
    std::vector<std::future<void>> vec_fut;
    vec_fut.reserve(_wardrobes);
    for (size_t i = 0; i < _wardrobes; ++i) {    // запись шкафов в отдельные файлы в многопоточном режиме
        files[i].open(_position + "_wardrobe_" + std::to_string(i + 1));
        vec_fut.push_back(std::async(&Library::write_to_wardrobe, this, std::ref(files[i]), i));
    }
    for (size_t i = 0; i < _wardrobes; ++i) {    //  закрыть все файлы со шкафами
        vec_fut[i].wait();
        files[i].close();
    }
}

void Library::search_for_books() {
    std::cout << "Введите параметры для поиска (через пробел): (title/author/year/pages_number/genre)\n";
    std::string result;
    std::vector<bool> parameters {
        0, 0, 0, 0, 0, 1
    };
    std::getline(std::cin, result);
    std::stringstream answer;
    for (size_t i = 0; i < result.size(); ++i) {
        answer << result[i];
    }
    result.clear();
    while (answer >> result) {
        if (result == "title") {
            parameters[0] = 1;
        }
        if (result == "author") {
            parameters[1] = 1;
        }
        if (result == "year") {
            parameters[2] = 1;
        }
        if (result == "pages_number") {
            parameters[3] = 1;
        }
        if (result == "genre") {
            parameters[4] = 1;
        }
        result.clear();
    }
    if ((parameters[0] == 0) && (parameters[1] == 0) && (parameters[2] == 0) && (parameters[3] == 0) && (parameters[4] == 0)) {
        std::cout << "Не введено ни одного параметра поиска. Попробуйте ещё раз." << std::endl;
        return;
    }
    std::vector<std::string> values(5);
    if (parameters[0] == 1) {
        std::cout << "Введите title для поиска:\n";
        std::getline(std::cin, values[0]);
    }
    if (parameters[1] == 1) {
        std::cout << "Введите author для поиска:\n";
        std::getline(std::cin, values[1]);
    }
    if (parameters[2] == 1) {
        std::cout << "Введите year для поиска:\n";
        std::getline(std::cin, values[2]);
    }
    if (parameters[3] == 1) {
        std::cout << "Введите pages_number для поиска:\n";
        std::getline(std::cin, values[3]);
    }
    if (parameters[4] == 1) {
        std::cout << "Введите genre для поиска:\n";
        std::getline(std::cin, values[4]);
    }
    bool found = false;
    for (auto iter = _storage.begin(); iter != _storage.end(); ++iter) {
        for (auto jter = (*iter).begin(); jter != (*iter).end(); ++jter) {
            for (auto kter = (*jter).begin(); kter != (*jter).end(); ++kter) {
                if ((*kter).book_checker(parameters, values)) {
                    (*kter).print_book(parameters);
                    found = true;
                }
            }
        }
    }
    if (!found) {
        std::cout << "Ничего не найдено, попробуйте ещё раз." << std::endl;
    }
}

void Library::sort_books() {
    std::cout << "Введите параметр сортировки: (title/author/year/pages_number/genre)\n";
    std::string result;
    std::getline(std::cin, result);
    if (result == "title") {
        string_sorting_books("title");
        return;
    }
    if (result == "author") {
        string_sorting_books("author");
        return;
    }
    if (result == "year") {
        size_t_sorting_books("year");
        return;
    }
    if (result == "pages_number") {
        size_t_sorting_books("pages_number");
        return;
    }
    if (result == "genre") {
        string_sorting_books("genre");
        return;
    }
    std::cout << "Некорректный ввод параметра сортировки, попробуйте ещё раз." << std::endl;
}

void Library::read_books() {
    std::cout << "Введите имя (местоположение) файла с книгами:" << std::endl;
    std::string result;
    std::getline(std::cin, result);
    std::ifstream file(result);
    if (file.fail()) {
        std::cout << result << std::endl;
        std::cout << "Ошибка открытия файла для чтения. Попробуйте ещё раз." << std::endl;
        return;
    }
    result.clear();
    while (file.peek() != EOF) {
        std::string _title = "";
        std::string _author = "";
        size_t _year = 0;
        size_t _pages_number = 0;
        std::string _genre = "";
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
        storage_finder(_title, _author, _year, _pages_number, _genre);
    }
}

void Library::summ_books() {
    std::cout << "Введите индивидуальный номер первой копируемой книги: ";
    std::string result;
    std::getline(std::cin, result);
    size_t id1 = static_cast<size_t>(std::atoi(result.c_str()));
    result.clear();
    if (id1 < 1 || id1 > _all_books) {
        std::cout << "Некорректный индивидуальный номер книги." << std::endl;
        return;
    }
    for (size_t i = 0; i < _deleted.size(); ++i) {
        if (id1 == _deleted[i]) {
            std::cout << "Книга с индивидуальным номером " << id1 << " была удалена." << std::endl;
            return;
        }
    }
    std::cout << "Введите индивидуальный номер второй копируемой книги: ";
    std::getline(std::cin, result);
    size_t id2 = static_cast<size_t>(std::atoi(result.c_str()));
    result.clear();
    if (id2 < 1 || id2 > _all_books) {
        std::cout << "Некорректный индивидуальный номер книги." << std::endl;
        return;
    }
    for (size_t i = 0; i < _deleted.size(); ++i) {
        if (id2 == _deleted[i]) {
            std::cout << "Книга с индивидуальным номером " << id2 << " была удалена." << std::endl;
            return;
        }
    }
    //  в рамках данного раздела новая книга не будет занимать вакантное место удаленной
    ++_all_books;  //  добавляется в библиотеке ещё одна книга
    size_t temp = _wardrobes;
    count_wardrobes();  //  установим правильное необходимое число шкафов
    if (temp < _wardrobes) {  //  если потребовался новый шкаф
        add_new_wardrobe();
        add_new_shelve();
    }
    if (_storage.back().back().size() == _books) {  //  если в шкафу последняя полка заполена
        add_new_shelve();
    }
    Book book = get_book_by_id(id1) + get_book_by_id(id2);  //  задана книга
    _storage.back().back().push_back(book);  // вставка копии заданной книги
    _storage.back().back().back().push_ind_num(_all_books);  //  присвоение новой книге индивидуального номера
    std::cout << "Книге присвоен индивидуальный номер: ";
    _storage.back().back().back().print_ind_num();  //  вывод в консоль везультата
}

void Library::print_book(size_t id) {
    get_book_by_id(id).print_book();
}

void Library::print_all_books() {
    for (auto iter = _storage.begin(); iter != _storage.end(); ++iter) {
        for (auto jter = (*iter).begin(); jter != (*iter).end(); ++jter) {
            for (auto kter = (*jter).begin(); kter != (*jter).end(); ++kter) {
                bool not_deleted = true;
                for (size_t i = 0; i < _deleted.size(); ++i) {
                    if ((*kter).get_ind_num() == _deleted[i]) {    //  проверка, удалена ли книга
                        not_deleted = false;
                    }
                }
                if (not_deleted) {
                    (*kter).print_book();
                }
            }
        }
    }
}

void Library::terminal_handler(size_t id) {
    //  получение индивидуального номера книги
    std::cout << "Введите индивидуальные номера книг (через пробелы): ";
    std::string result;
    std::vector<int> num_books;
    std::getline(std::cin, result);
    if (result == "all") {    //  реализуется для печати всех книг библиотеки
        print_all_books();
        return;
    }
    std::stringstream answer;
    for (size_t i = 0; i < result.size(); ++i) {
        answer << result[i];
    }
    while (answer >> result) {
        num_books.push_back(std::atoi(result.c_str()));
        result.clear();
    }
    //  режимы работы с индивидуальными номерами книг
    if (!check_corrected(num_books)) {    //  проверка правильности ввода индивидуальных номеров книг
        return;
    }
    if (id == 2) {    //  удаление книг
        std::vector<std::future<void>> vec_fut;
        vec_fut.reserve(num_books.size());
        for (int pos : num_books) {
            vec_fut.push_back(std::async(&Library::delete_book, this, static_cast<size_t>(pos)));
//            delete_book(static_cast<size_t>(pos));
        }
        for (size_t i = 0; i < num_books.size(); ++i) {    //  необходимо дождаться результатов работы потоков
            vec_fut[i].wait();
        }
        return;
    }
    for (int pos : num_books) {
        if (id == 1) {    //  редактирование книги
            book_edit(pos);
        }
        if (id == 3) {    //  печать книги
            print_book(static_cast<size_t>(pos));
        }
    }
}

void Library::book_edit(int num_book) {
    get_book_by_id(num_book).change_bracket();
}

void Library::delete_book(size_t id) {
    mutex1.lock();
    _deleted.push_back(id);
    mutex1.unlock();
    get_book_by_id(id).delete_Book_content();
}

void Library::commands() {
    std::cout << "Доступные к исполнению команды:" << std::endl;
    std::cout << "  new - Ввод нового объекта и добавление его в контейнер;" << std::endl;
    std::cout << "  search - Поиск объекта в контейнере по значениям полей с печатью данных о найденных объектах;" << std::endl;
    std::cout << "  edit - Редактирование объекта;" << std::endl;
    std::cout << "  delete - Удаление объекта из контейнера;" << std::endl;
    std::cout << "  save - Сохранение данных всех объектов в файле;" << std::endl;
    std::cout << "  read - Чтение данных объектов из файла;" << std::endl;
    std::cout << "  sort - Сортировка объектов контейнера по выбранному полю для list;" << std::endl;
    std::cout << "  print - Печать списка объектов;" << std::endl;
    std::cout << "  summ - Сумма 2х взятых из библиотеки книг с добавлением результата в библиотеку." << std::endl;
}

void Library::exit_mode(std::string& answer1) {
    std::cout << "Вы решили завершить работу с библиотекой. Сохранить сведения? (сохранить/удалить/вернуться)" << std::endl;
    std::string answer;
    std::getline(std::cin, answer);
    if (answer == "сохранить") {
        safe_library();
        throw (char*)"Информация сохранена.\nБиблиотека выключена.\nСпасибо за выбор нашей библиотеки!";
    }
    if (answer == "удалить") {
        delete_library();
        throw (char*) "Библиотека удалена.\nПрограмма завершила работу.\nСпасибо за выбор нашей библиотеки!";
    }
    if (answer == "вернуться") {
        read_user(answer1);
        return;
    }
    std::cout << "Некорректный ввод ответа. Попробуйте еще раз.\n";
    exit_mode(answer1);
}

//  вспомогательные фции

void Library::read_user(std::string& answer) {
    answer.clear();
    std::getline(std::cin, answer);
    if (answer == "выйти") {
        exit_mode(answer);
    }
}

void Library::storage_finder(std::string _title, std::string _author, size_t _year, size_t _pages_number, std::string _genre) {
    if (!_deleted.empty()) {  //  если есть вакантные позиции для вставки
        add_to_deleted(_title, _author, _year, _pages_number, _genre);
        return;
    }
    ++_all_books;  //  добавляется в библиотеке ещё одна книга
    size_t temp = _wardrobes;
    count_wardrobes();  //  установим правильное необходимое число шкафов
    if (temp < _wardrobes) {  //  если потребовался новый шкаф
        add_new_wardrobe();
        add_new_shelve();
    }
    if (_storage.back().back().size() == _books) {  //  если в шкафу последняя полка заполена
        add_new_shelve();
    }
    Book book(_title, _author, _year, _pages_number, _genre);  //  задана книга
    _storage.back().back().push_back(book);  // вставка копии заданной книги
    _storage.back().back().back().push_ind_num(_all_books);  //  присвоение новой книге индивидуального номера
    std::cout << "Книге присвоен индивидуальный номер: ";
    _storage.back().back().back().print_ind_num();  //  вывод в консоль везультата
}

void Library::add_to_deleted(std::string _title, std::string _author, size_t _year, size_t _pages_number, std::string _genre) {
    size_t id = _deleted.back();
    _deleted.pop_back();
    get_book_by_id(id).push_all_brackets(_title, _author, _year, _pages_number, _genre);
    std::cout << "Книге присвоен индивидуальный номер: " << id << std::endl;
}

void Library::count_wardrobes() {
    _wardrobes = (_all_books + (_shelves * _books) - 1) / (_shelves * _books);
}

void Library::add_new_wardrobe() {
    std::list<std::list<Book>> wardrobe;
    _storage.push_back(wardrobe);
}

void Library::add_new_shelve() {
    std::list<Book> shelve;
    _storage.back().push_back(shelve);
}

size_t Library::count_wardrobes(size_t num_book) {
    return static_cast<size_t>((num_book + (_shelves * _books) - 1) / (_shelves * _books));
}

size_t Library::count_shelve(size_t num_book) {
    num_book %= _shelves * _books;
    if (num_book == 0) {
        num_book = _shelves * _books;
    }
    return static_cast<size_t>((num_book + _shelves - 1) / _shelves);
}

size_t Library::count_book_pos(size_t num_book) {
    size_t result = static_cast<size_t>(num_book % _shelves);
    if (result == 0) {
        result = _shelves;
    }
    return result;
}

Book& Library::get_book_by_id(size_t id) {
    size_t wardrobe = count_wardrobes(id);
    size_t shelve = count_shelve(id);
    size_t book = count_book_pos(id);
    auto iter = _storage.begin();
    for (size_t i = 1; i < wardrobe; ++i) {
        ++iter;
    }
    auto jter = (*iter).begin();
    for (size_t j = 1; j < shelve; ++j) {
        ++jter;
    }
    auto kter = (*jter).begin();
    for (size_t k = 1; k < book ; ++k) {
        ++kter;
    }
    return (*kter);
    
}

bool Library::check_corrected(std::vector<int>& num_books) {
    for (int pos : num_books) {
        if (pos < 1 || pos > _all_books) {
            std::cout << "Некорректный индивидуальный номер книги." << std::endl;
            return false;
        }
        for (size_t i = 0; i < _deleted.size(); ++i) {
            if (pos == _deleted[i]) {
                std::cout << "Книга с индивидуальным номером " << pos << " была удалена." << std::endl;
                return false;
            }
        }
    }
    return true;
}

void Library::string_sorting_books(std::string param_name) {
    std::multimap<std::string, Book> map;
    for (auto iter = _storage.begin(); iter != _storage.end(); ++iter) {
        for (auto jter = (*iter).begin(); jter != (*iter).end(); ++jter) {
            for (auto kter = (*jter).begin(); kter != (*jter).end(); ++kter) {
                bool not_deleted = true;
                for (size_t i = 0; i < _deleted.size(); ++i) {
                    if ((*kter).get_ind_num() == _deleted[i]) {    //  проверка, удалена ли книга
                        not_deleted = false;
                    }
                }
                if (not_deleted) {
                    map.emplace((*kter).get_bracket(param_name), (*kter));
                }
            }
        }
    }
    auto map_iter = map.begin();
    for (auto iter = _storage.begin(); iter != _storage.end(); ++iter) {
        for (auto jter = (*iter).begin(); jter != (*iter).end(); ++jter) {
            for (auto kter = (*jter).begin(); kter != (*jter).end(); ++kter) {
                bool not_deleted = true;
                for (size_t i = 0; i < _deleted.size(); ++i) {
                    if ((*kter).get_ind_num() == _deleted[i]) {    //  проверка, удалена ли книга
                        not_deleted = false;
                    }
                }
                if (not_deleted) {
                    (*kter) = (*map_iter).second;
                    ++map_iter;
                }
            }
        }
    }
}

void Library::size_t_sorting_books(std::string param_name) {
    std::multimap<size_t, Book> map;
    for (auto iter = _storage.begin(); iter != _storage.end(); ++iter) {
        for (auto jter = (*iter).begin(); jter != (*iter).end(); ++jter) {
            for (auto kter = (*jter).begin(); kter != (*jter).end(); ++kter) {
                bool not_deleted = true;
                for (size_t i = 0; i < _deleted.size(); ++i) {
                    if ((*kter).get_ind_num() == _deleted[i]) {    //  проверка, удалена ли книга
                        not_deleted = false;
                    }
                }
                if (not_deleted) {
                    map.emplace((*kter).get_size_t_bracket(param_name), (*kter));
                }
            }
        }
    }
    auto map_iter = map.begin();
    for (auto iter = _storage.begin(); iter != _storage.end(); ++iter) {
        for (auto jter = (*iter).begin(); jter != (*iter).end(); ++jter) {
            for (auto kter = (*jter).begin(); kter != (*jter).end(); ++kter) {
                bool not_deleted = true;
                for (size_t i = 0; i < _deleted.size(); ++i) {
                    if ((*kter).get_ind_num() == _deleted[i]) {    //  проверка, удалена ли книга
                        not_deleted = false;
                    }
                }
                if (not_deleted) {
                    (*kter) = (*map_iter).second;
                    ++map_iter;
                }
            }
        }
    }
}

//  фции сохранения библиотеки

void Library::get_position() {
    if (_position != "") {
        choose_position();
        return;
    }
    if (_position == "") {
        new_position();
    }
}

void Library::choose_position() {
    std::cout << "Выберите позицию для сохранения: (текущее/новое)\n";
    std::string answer;
    std::getline(std::cin, answer);
    if (answer == "новое") {
        new_position();
        return;
    }
    if (answer != "текущее") {
        std::cout << "некорректный ввод ответа" << std::endl;
        choose_position();
    }
}

void Library::new_position() {
    std::cout << "Введите новое имя файла (местоположение) для сохранения книги:\n";
    std::getline(std::cin, _position);
}

bool Library::incorrect_write_to_file() {
    std::ofstream file(_position);
    if (file.fail()) {
        std::cout << "Не получилось открыть файл для сохранения, попробуйте ещё раз" << std::endl;
        return true;
    }
    file.close();
    return false;
}

void Library::write_to_wardrobe(std::ofstream& os, size_t i) {
    if (i == _wardrobes - 1) {
        last_write_to_wardrobe(os);
        return;
    }
    os << _shelves << '\n';
    std::list<std::list<std::list<Book>>>::iterator iter = _storage.begin();
    for (size_t j = 0; j < i; ++j) {
        ++iter;
    }
    std::list<std::list<Book>>::iterator jter = (*iter).begin();
    write_shelve(os, jter);
    for (size_t k = 1; k < _shelves; ++k) {    //  первая книга напечатана
        ++jter;
        write_shelve(os, jter);
    }
    
    
}

void Library::write_shelve(std::ofstream& os, std::list<std::list<Book>>::iterator jter) {
    os << std::to_string(_books) << '\n';
    std::list<Book>::iterator kter = (*jter).begin();
    for (size_t k = 0; k < _books; ++k) {
        os << *kter;
        ++kter;
    }
}

void Library::last_write_to_wardrobe(std::ofstream& os) {
    os << _storage.back().size() << '\n';
    std::list<std::list<Book>>::iterator jter = _storage.back().begin();
    for (size_t k = 0; k < _storage.back().size() - 1; ++k) {
        write_shelve(os, jter);
        ++jter;
    }
    last_shelve_last_write_to_wardrobe(os);
}

void Library::last_shelve_last_write_to_wardrobe(std::ofstream& os) {
    os << std::to_string(_storage.back().back().size()) << '\n';
    for (std::list<Book>::iterator kter = _storage.back().back().begin(); kter != _storage.back().back().end(); ++kter) {
        os << *kter;
    }
}

void Library::delete_library() {}
