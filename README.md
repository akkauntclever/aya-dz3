# Библиотека

## руководство по эксплуатации

### ! Важно

    При использовании библиотеки категорически запрещается изменять файлы библиотеки. Изменение файлов библиотеки влечет за собой необратимые последствия.

### добавление книг в библиотеку

    Чтобы добавить книгу в библиотеку, необходимо воспользоваться консолью. В случае, если книг для добавления много, то к использованию предлагается запись книг в библиотеку через файл. Рекомендуемая последовательность действий:
1) создайте пустой файл без расширения (наличие расширение не принципиально)
2) запишите в файл число книг, которые планируется добавить в библиотеку
3) в файл в следующей последовательности запишите (каждый пункт с новой строки): 
  ●  наименование книги (title)
  ●  автора книги
  ●  год выпуска книги
  ●  количество страниц в книге
  ●  жанр книги
4) повторите пункт 2) для необходимого количества книг.
Каждая новая строчка начинается с новой строки.
! не надо записывать в файл ничего постороннего
5) из терминала библиотеки воспользуйтесь функцией read

### печать книг в консоль

    В библиотеке предоставлена возможность печатать книги как по индивидуальному номеру, так и все. Чтобы напечатать все книги, вместо указания индивидуальных номеров, после вызова `print`, необходимо написать `all`.

```
Трифонов Иван. Все права защищены.
```
