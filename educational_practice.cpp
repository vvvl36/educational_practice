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
        << source_string << endl
        << "Параметры выбранного варианта (3): " << endl
        << "1. Кириллица " << endl
        << "2. По алфавиту " << endl
        << "3. По возрастанию " << endl
        << "4. Учитывать числа " << endl
        << "5. Сортировка вставками " << endl
        << "Количество слов: " << size_words << endl
        << "Время сортировки: " << static_cast<double>(time) / 1000 << " с" << endl //static_cast это приведение типа к другому типу (в данном случае инт к дабл)
        << "Количество символов: " << source_string.length() << endl
        << "Количество слов на каждую букву алфавита: " << endl;

    string kirillica_hi = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; //алфавит русский

    //вывод количества слов на каждую букву
    for (int i = 0; i < amount_words.size(); i++)
    {
        file_analysis << kirillica_hi[i] << ": " << amount_words[i] << endl; //выводим букву и количество слов на эту букву
    }
    file_analysis.close(); //закрываем файл
}

void sort(vector <string>& words)
{
    string buff;
    int i, j;
    for (i = 1; i < words.size(); i++)
    {
        buff = words[i]; // запомним обрабатываемый элемент
        // и начнем перемещение элементов слева от него
        // пока запомненный не окажется меньше чем перемещаемый
        for (j = i - 1; j >= 0 && words[j] >= buff; j--)
            words[j + 1] = words[j];

        words[j + 1] = buff; // и поставим запомненный на его новое место	
    }
}