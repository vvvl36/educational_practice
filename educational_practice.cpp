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

int main()
{
    setlocale(0, ""); // поддержка кириллицы в консоли (вывод)
    SetConsoleCP(1251); // поддержка кириллицы в консоли (ввод)
    SetConsoleOutputCP(1251); // поддержка кириллицы в консоли (ввод)

    cout
        << "Файлы с текстами: " << endl
        << "1. Текст " << endl
        << "2. Tекст" << endl
        << "2. Tекст" << endl
        << "2. Tекст" << endl
        << "2. Tекст" << endl
        << "2. Tекст" << endl
        << "2. Tекст" << endl
        << "2. Tекст" << endl
        << "2. Tекст" << endl
        << "2. Текст" << endl
        << "Введите номер текста: ";

    string name_file, source_string; //переменные имени файла и исходной строки
    cin >> name_file;

    //разбивка исходного текста на массив слов
    vector <string> words = breakdown_into_words(name_file, source_string);

    //подсчет количества слов на каждую букву
    vector<int> amount_words = counting_words(words);

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
        symbol = source_string[i] - 256;
        symbol_next = source_string[i + 1] - 256;

        if (symbol >= 'А' && symbol <= 'Я' || symbol >= 'а' && symbol <= 'я' || symbol == 'Ё' || symbol == 'ё') //если символ строки буква из латиницы

        {
            s += symbol; //собираем слово 
            if ((symbol_next < 'а' || symbol_next > 'я') && symbol_next != 'ё') //разделитель - все кроме маленьких букв
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
            }
        }
    }
    return words;
}

vector<int> counting_words(vector<string> words)
{
    vector<int> amount_words(33); //вектор длиной 33, заполнен нулями. Каждый элемент - это количество слов на данную букву

    string kirillica_hi = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; //алфавит русский
    string kirillica_lo = "абвгдеёжзийклмнопрстуфхцчшщъэьэюя"; //алфавит русский
    for (int i = 0; i < words.size(); i++)
    {
        for (int j = 0; j < kirillica_hi.size(); j++) //проходим по всему массиву количества
        {
            if (kirillica_hi[j] == words[i][0]) //если первая буква равна какой-то букве из верхнего регистра, то увеличиваем
                amount_words[j]++;
            if (kirillica_lo[j] == words[i][0]) //если первая буква равна какой-то букве из нижнего регистра, то увеличиваем
                amount_words[j]++;
        }
    }
    return amount_words;
}

void writing_to_file_result(string name_file, vector <string> words)
{
    fstream file_result;

    string analysis_str = "result_" + name_file + ".txt";
    file_result.open(analysis_str, ios::out); // открываем файл на запись в него, если файла нет, то он создастся

        //вывод слов
    for (int i = 0; i < words.size(); i++)
    {
        file_result << words[i] << endl;
    }
    file_result.close();
}