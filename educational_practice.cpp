#include <iostream>
#include <string>
#include <iostream>
#include <Windows.h> 
#include <vector> 
#include <fstream>

using namespace std;

// прототипы
vector<string> breakdown_into_words(string name_file, string& original_string);
vector<int> counting_words(vector<string> words);
void writing_to_file_result(string name_file, vector<string> words);
int sorting_and_sorttime(vector<string>& words);
void writing_to_file_analysis(string name_file, string original_string, int time, int size_words, vector<int> amount_words);
void sort(vector <string>& words);
bool check_double(string str);

int main()
{
    setlocale(0, ""); // поддержка кириллицы в консоли (вывод)
    SetConsoleCP(1251); // поддержка кириллицы в консоли (ввод)
    SetConsoleOutputCP(1251); // поддержка кириллицы в консоли (ввод)

    cout << "Введите номер текста: ";

    string name_file, source_string; //переменные имени файла и исходной строки
    cin >> name_file;

    //разбивка исходного текста на массив слов
    vector <string> words = breakdown_into_words(name_file, source_string);

    //подсчет количества слов на каждую букву
    vector<int> amount_words = counting_words(words);

    //сортировка слов по алфавиту и вычисление времени сортировки
    int time = sorting_and_sorttime(words);

    //запись в файл analysis
    writing_to_file_analysis(name_file, source_string, time, words.size(), amount_words);

    //запись в файл result
    writing_to_file_result(name_file, words);

    return 0;
}

vector<string> breakdown_into_words(string name_file, string& source_string)
{
    fstream file_original; //создаем переменную файла

    string source_str = "original_" + name_file + ".txt"; //создаем строку имени файла
    file_original.open(source_str, ios::in); // открываем файл на запись из файла

    vector<string> words;
    string s = "";

    char ch;

    if (file_original.is_open()) //если файл открылся 
    {
        while (file_original.get(ch)) //пока можем считать символ из файла
        {
            source_string += ch; //прибавляем символ к строке 
        }
        file_original.close(); // Закрытие файла
    }
    else
    {
        cout << "Исходный файл не открылся";
        exit(0);
    }

    source_string = '"' + source_string + '"';

    char symbol, symbol_next;

    for (int i = 0; i < source_string.length() - 1; i++)
    {
        symbol = source_string[i] - 256; //потому что считывается из таблицы [0,255], а программа работает с таблицей [-128, 127]
        symbol_next = source_string[i + 1] - 256;

        if (symbol >= 'А' && symbol <= 'Я' || symbol >= 'а' && symbol <= 'я' || symbol == 'Ё' || symbol == 'ё') //если символ строки буква из латиницы

        {
            s += symbol; //собираем слово 
            if ((symbol_next < 'а' || symbol_next > 'я') && symbol_next != 'ё' && symbol_next != '-') //разделитель - все кроме маленьких букв
            {
                words.push_back(s); //добавляем в массив строку
                s = ""; //обнуляем строку
            }
        }
        else
        {
            if (symbol >= '0' && symbol <= '9' || symbol == ',' && symbol_next >= '0' && symbol_next <= '9') //если символ строки цифра или запятая 
            {
                s += symbol; //собираем слово 
                if ((symbol_next < '0' || symbol_next > '9') && symbol_next != ',') //
                {
                    words.push_back(s); //добавляем в массив строку
                    s = ""; //обнуляем строку
                }
            }
            else
            {
                if (symbol == '-' && symbol_next >= '0' && symbol_next <= '9') //если символ строки минус и следующий символ цифра
                {
                    s += symbol; //собираем слово 
                }
                else if (symbol == '-' && (symbol_next >= 'А' && symbol_next <= 'Я' || symbol_next >= 'а' && symbol_next <= 'я' || symbol_next == 'Ё' || symbol_next == 'ё') && s != "")
                {
                    s += symbol;
                }
            }
        }
    }
    return words;
}

vector<int> counting_words(vector<string> words)
{
    vector<int> amount_words(33); //вектор длиной 33, заполнен нулями. Каждый элемент - это количество слов на данную букву

    string cyrillic_hi = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    string cyrillic_lo = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

    for (int i = 0; i < words.size(); i++)
    {
        for (int j = 0; j < cyrillic_hi.size(); j++) //проходим по всему массиву количества
        {
            if (cyrillic_hi[j] == words[i][0]) //если первая буква равна какой-то букве из верхнего регистра, то увеличиваем
                amount_words[j]++;
            if (cyrillic_lo[j] == words[i][0]) //если первая буква равна какой-то букве из нижнего регистра, то увеличиваем
                amount_words[j]++;
        }
    }
    return amount_words;
}

void writing_to_file_result(string name_file, vector <string> words)
{
    fstream file_result;

    string result_str = "result_" + name_file + ".txt";
    file_result.open(result_str, ios::out); // открываем файл на запись в него, если файла нет, то он создастся

    //вывод слов
    for (int i = 0; i < words.size(); i++)
    {
        file_result << words[i] << endl;
    }
    file_result.close();
}

int sorting_and_sorttime(vector<string>& words)
{
    string tmp;

    int start_time = clock();
    sort(words);
    int end_time = clock();

    return end_time - start_time;
}

void writing_to_file_analysis(string name_file, string source_string, int time, int size_words, vector<int> amount_words)
{
    fstream file_analysis;

    string analysis_str = "analysis_" + name_file + ".txt";
    file_analysis.open(analysis_str, ios::out); // открываем файл на запись в него, если файла нет, то он создастся

    file_analysis
        << "Исходный текст: " << endl
        << "<<" << source_string << ">>" << endl
        << "Параметры выбранного варианта (3): кириллица, по алфавиту, по возрастанию, учитывать числа, сортировка вставками" << endl
        << "Количество слов: " << size_words << endl
        << "Время сортировки: " << static_cast<double>(time) / 1000 << " с" << endl
        << "Статистика: " << endl;

    cout
        << "Исходный текст: " << endl
        << "<<" << source_string << ">>" << endl
        << "Параметры выбранного варианта (3): кириллица, по алфавиту, по возрастанию, учитывать числа, сортировка вставками" << endl
        << "Количество слов: " << size_words << endl
        << "Время сортировки: " << static_cast<double>(time) / 1000 << " с" << endl
        << "Статистика: " << endl;

    string cyrillic_lo = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";

    //вывод количества слов на каждую букву
    for (int i = 0; i < amount_words.size(); i++)
    {
        file_analysis << cyrillic_lo[i] << ": " << amount_words[i] << endl; //выводим букву и количество слов на эту букву
        cout << cyrillic_lo[i] << ": " << amount_words[i] << endl; //выводим букву и количество слов на эту букву
    }
    file_analysis.close(); //закрываем файл
}

void sort(vector <string>& words)
{
    string buff;
    int i, j, number_buff, number_j;
    string cyrillic_hi = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    string cyrillic_lo = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    string numbers_string = "0123456789";

    vector <string> numbers_array; //массив чисел
    vector <string> words_array; //массив слов 
    for (i = 0; i < words.size(); i++)
    {
        if (words[i][0] == '-' || numbers_string.find(words[i][0]) != -1) //если первый символ - или число
            numbers_array.push_back(words[i]); //то добавляем в массив чисел
        else
            words_array.push_back(words[i]); // иначе добавляем в массив слов
    }

    //сортируем массив слов 
    for (i = 1; i < words_array.size(); i++)
    {



        if (cyrillic_hi.find(words_array[i][0]) != -1) //если первый символ слова из верхнего регистра
            number_buff = cyrillic_hi.find(words_array[i][0]); // то номер равен номеру символа в верхнем регистре
        else
            number_buff = cyrillic_lo.find(words_array[i][0]); // иначе в нижнем регистре

        buff = words_array[i]; // запомним обрабатываемый элемент
        // и начнем перемещение элементов слева от него
        // пока запомненный не окажется меньше чем перемещаемый

        for (j = i - 1; j >= 0; j--)
        {
            if (cyrillic_hi.find(words_array[j][0]) != -1) //если первый символ слова из верхнего регистра
                number_j = cyrillic_hi.find(words_array[j][0]); // то номер равен номеру символа в верхнем регистре
            else
                number_j = cyrillic_lo.find(words_array[j][0]);

            if (number_j <= number_buff) //сравниваем номера первых букв в алфавите
                break;

            words_array[j + 1] = words_array[j];

        }

        words_array[j + 1] = buff; // и поставим запомненный на его новое место	
    }
    double double_buff, double_j; //переменные для обработки чисел 

    for (i = 1; i < numbers_array.size(); i++)
    {
        double_buff = stod(numbers_array[i]);

        buff = numbers_array[i]; // запомним обрабатываемый элемент
        // и начнем перемещение элементов слева от него
        // пока запомненный не окажется меньше чем перемещаемый

        for (j = i - 1; j >= 0; j--)
        {
            double_j = stod(numbers_array[j]);
            if (double_j <= double_buff) //сравниваем числа
                break;

            numbers_array[j + 1] = numbers_array[j];
        }

        numbers_array[j + 1] = buff; // и поставим запомненный на его новое место	
    }

    words = {};

    //соединяем массив обратно
    for (i = 0; i < words_array.size(); i++)
    {
        words.push_back(words_array[i]);
    }

    for (i = 0; i < numbers_array.size(); i++)
    {
        if (check_double(numbers_array[i])) //если число существует 
            words.push_back(numbers_array[i]);
    }
}

bool check_double(string str) //ф-я check double с логичесим типом
{
    bool k = 1;
    //проверка на возможность существования такого числа
    if ((str.find('-') < 1 || str.find('-') == -1) && ((str.find(',') < str.size() - 1 && str.find(',') > 0) || str.find(',') == -1))
    {
        if (str.length() > 1) //если длина больше 1
        {
            //проверка на возможность существования такого числа
            //после нуля всегда точка или ничего, если ноль первый
            //после "-" если ноль, то должна быть точка или ничего 
            int count_minus = 0, count_point = 0;
            for (int i = 0; i < str.length(); i++)
            {
                if (str[i] == '-')
                {
                    count_minus++;
                }
                if (str[i] == ',')
                {
                    count_point++;
                } //если у нас всего один символ то он не может быть точкой или минусом
            }
            if ((str[0] == '0' && str[1] != ',') || count_point > 1 ||
                (str[0] == '-' && str[1] == ',') ||
                (str[0] == '-' && str[1] == '0' && str[2] != ',') || count_minus > 1)
            {
                k *= 0;
            } //если 1й символ "0" и 2й не "." или число больше 1 или 1й симв "-" и второй "." или 1й симв "-" и 2й "0" и 3й не "." или число больше 1,
              //то программа выводит ошибку "Вы ввели не число" и просит снова Ввести число
        }
        else
            if (str == "-")
            {
                k *= 0;
            } //если строка состоит только из минуса, выводится ошибка
        if (k == 1)
        {

            for (int i = 0; i < str.length(); i++)
            {
                if ((str[i] < '0' || str[i] > '9') && str[i] != ',' && str[i] != '-')
                    //если в строке символ не от 0 до 9 и нет точки или минуса
                {
                    k *= 0;
                    //то выводит ошибку (не пропускает буквы и прочие символы)
                    break;
                }
            }
        }
    }
    else
    {
        k *= 0;
    } //если в итоге число не существует, выводится ошибка

    return k;
}
