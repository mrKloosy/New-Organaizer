#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <algorithm>

using namespace std;

enum Color { Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(Color text, Color background)
{
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Esc = 27 };

void SetCursor(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hStdOut, coord);
}

class Note
{
protected:
    int timeH = 0;
    int timeM = 0;
    string description;
    int priority = 0;
    vector<Note> notes;

public:
    void SetTimeH(int timeH)
    {
        if (timeH >= 0 and timeH <= 23)
        {
            this->timeH = timeH;
        }
    }

    void SetTimeM(int timeM)
    {
        if (timeM >= 0 and timeM <= 59)
        {
            this->timeM = timeM;
        }
    }

    void SetDescription(const string& description)
    {
        if (!description.empty())
        {
            this->description = description;
        }
    }

    void SetPriority(int priority)
    {
        if (priority >= 0 and priority <= 10)
        {
            this->priority = priority;
        }
    }

    int GetTimeH() const
    {
        return timeH;
    }

    int GetTimeM() const
    {
        return timeM;
    }

    string GetDescription() const
    {
        return description;
    }

    int GetPriority() const
    {
        return priority;
    }

    void SaveToFile()
    {
        ofstream file("notes.txt");
        if (file.is_open())
        {
            for (const auto& note : notes)
            {
                file << note.GetTimeH() << "," << note.GetTimeM() << ";" << note.GetDescription() << "," << note.GetPriority() << "\n";
            }
            file.close();
            cout << "Заметки сохранены в файле.\n";
        }
        else
        {
            cerr << "Ошибка открытия файла для сохранения.\n";
        }
    }

    void LoadFromFile()
    {
        notes.clear();
        ifstream file("notes.txt");
        if (file.is_open())
        {
            while (!file.eof())
            {
                Note note;
                file >> note.timeH;
                file.ignore();
                file >> note.timeM;
                file.ignore();
                getline(file, note.description, ',');
                file >> note.priority;
                cin.ignore(INT_MAX, '\n');
                if (!note.description.empty())
                {
                    notes.push_back(note);
                }
            }
            file.close();
            cout << "Заметки загружены из файла.\n";
        }
        else
        {
            cerr << "Ошибка открытия файла для загрузки.\n";
        }
    }

    void DisplayNotes()
    {
        system("cls");
        cout << "Список заметок:\n";
        for (const auto& note : notes)
        {
            cout << "Время: " << note.GetTimeH() << ":" << note.GetTimeM() << " Описание: " << note.GetDescription() << " Важность: " << note.GetPriority() << "\n";
        }
        system("pause");
    }

    void SearchByKeyword()
    {
        system("cls");
        string keyword;
        cout << "Введите ключевое слово для поиска: ";
        cin >> keyword;
        vector<Note> temp;
        for (const auto& note : notes)
        {
            if (note.GetDescription().find(keyword) != string::npos)
            {
                temp.push_back(note);
            }
        }
        if (temp.empty())
        {
            cout << "Заметки с ключевым словом не найдены.\n";
        }
        else
        {
            cout << "Совпадающие заметки:\n";
            for (const auto& note : temp)
            {
                cout << "Время: " << note.GetTimeH() << ":" << note.GetTimeM() << "\nОписание: " << note.GetDescription() << "\nВажность: " << note.GetPriority() << "\n\n";
            }
        }
        system("pause");
    }

    void SortByTime()
    {
        system("cls");
        sort(notes.begin(), notes.end(), [](const Note& a, const Note& b)
            {
                if (a.GetTimeH() < b.GetTimeH())
                {
                    return true;
                }
                else if (a.GetTimeH() > b.GetTimeH())
                {
                    return false;
                }
                else
                {
                    return a.GetTimeM() < b.GetTimeM();
                }
            });
        SaveToFile();
        cout << "Заметки отсортированы по времени.\n";
        system("pause");
    }

    void EditNote()
    {
        system("cls");

        if (notes.empty())
        {
            cout << "Нет доступных заметок для редактирования.\n";
            system("pause");
            return;
        }
        cout << "Список заметок:\n";
        for (size_t i = 0; i < notes.size(); ++i)
        {
            cout << i + 1 << ". " << "Время: " << notes[i].GetTimeH() << ":" << notes[i].GetTimeM()
                << " Описание: " << notes[i].GetDescription() << " Важность: " << notes[i].GetPriority() << "\n";
        }
        cout << "Введите номер заметки для редактирования: ";
        int noteNumber;
        cin >> noteNumber;
        if (noteNumber >= 1 and noteNumber <= notes.size())
        {
            Note& note = notes[noteNumber - 1];
            cout << "Введите новое время (часы): ";
            cin >> note.timeH;
            cout << "Введите новое время (минуты): ";
            cin >> note.timeM;

            if (note.GetTimeH() < 0 or note.GetTimeH() > 23)
            {
                cout << "Ошибка, введите время в часах снова: ";
                cin >> note.timeH;
            }

            if (note.GetTimeM() < 0 or note.GetTimeM() > 59)
            {
                cout << "Ошибка, введите время в минутах снова: ";
                cin >> note.timeM;
            }
            cin.ignore(INT_MAX, '\n');
            cout << "Введите новое описание: ";
            getline(cin, note.description);
            cout << "Введите новую важность (от 1 до 10): ";
            cin >> note.priority;
            cout << "Заметка отредактирована.\n";
            cout << "Обновленный список заметок:\n";
            for (const auto& note : notes)
            {
                cout << "Время: " << note.GetTimeH() << ":" << note.GetTimeM()
                    << " Описание: " << note.GetDescription() << " Важность: " << note.GetPriority() << "\n";
            }
            cout << "Хотите сохранить изменения?: ";
            string choice;
            cin >> choice;
            transform(choice.begin(), choice.end(), choice.begin(), ::toupper);
            if (choice == "ДА")
            {
                SaveToFile();
                cout << "Изменения сохранены.\n";
            }
            else
            {
                cout << "Изменения не сохранены.\n";
            }
        }
        else
        {
            cout << "Неверный номер заметки.\n";
        }
        cin.ignore(INT_MAX, '\n');
        system("pause");
    }

    void DeleteNote()
    {
        system("cls");
        cout << "Введите номер заметки для удаления: ";
        int noteNumber;
        cin >> noteNumber;
        if (noteNumber >= 1 and noteNumber <= notes.size())
        {
            notes.erase(notes.begin() + noteNumber - 1);

            cout << "Заметка удалена.\n";
        }
        else {
            cout << "Неверный номер заметки.\n";
        }
        SaveToFile();
        cin.ignore(INT_MAX, '\n');
        system("pause");
    }

    void CreateAndSaveNote()
    {
        Note note;
        bool tH = false, tM = false;
        cout << "Введите время в часах: ";
        do {
            cin >> note.timeH;
            if (note.GetTimeH() < 0 and note.GetTimeH() > 23)
            {
                cout << "Ошибка, введите время в часах снова: ";
            }
            else
            {
                tH = true;
            }

        } while (!tH);
        cout << "Введите время в минутах: ";
        do {
            cin >> note.timeM;
            if (note.GetTimeM() < 0 and note.GetTimeM() > 59)
            {
                cout << "Ошибка, введите время в минутах снова: ";
            }
            else
            {
                tM = true;
            }
        } while (!tM);
        cin.ignore(INT_MAX, '\n');
        cout << "Введите описание: ";
        getline(cin, note.description);
        cout << "Введите важность (от 1 до 10): ";
        cin >> note.priority;
        cin.ignore(INT_MAX, '\n');
        notes.push_back(note);
        SaveToFile();
        cout << "Заметка добавлена и сохранена в файле.\n";
        system("pause");
    }

    void SortByPriority()
    {
        system("cls");
        sort(notes.begin(), notes.end(), [](const Note& a, const Note& b)
            {
                return a.GetPriority() > b.GetPriority();
            });
        cout << "Заметки отсортированы по важности.\n";
        SaveToFile();
        cin.ignore(INT_MAX, '\n');

        system("pause");
    }

    void Run()
    {
        char choice;
        int selectedAction = 0;

        do
        {
            DisplayMenu(selectedAction);

            choice = _getch();
            switch (choice)
            {
            case Up:
                selectedAction = (selectedAction - 1 + 8) % 8;
                break;
            case Down:
                selectedAction = (selectedAction + 1) % 8;
                break;
            case Enter:
                PerformAction(selectedAction);
                break;
            case Esc:
                cout << "Выход из программы.\n";
                break;
            }

            if (choice != Enter) 
            {
                //system("pause");
                system("cls");
            }
        } while (choice != Esc);
    }
    void DisplayMenu(int currentAction)
    {
        system("cls");

        cout << "Выберите действие:\n";
        for (size_t i = 0; i < 8; i++)
        {
            if (i == static_cast<size_t>(currentAction))
            {
                SetColor(Magenta, Black);
                cout << "-> ";
            }
            else
            {
                SetColor(White, Black);
                cout << "   ";
            }
            switch (i)
            {
            case 0:
                cout << "Внести новую заметку";
                break;
            case 1:
                cout << "Редактировать заметку";
                break;
            case 2:
                cout << "Удалить заметку";
                break;
            case 3:
                cout << "Показать все заметки";
                break;
            case 4:
                cout << "Поиск по ключевым словам";
                break;
            case 5:
                cout << "Сортировать по важности";
                break;
            case 6:
                cout << "Сортировать по времени";
                break;
            case 7:
                cout << "Выйти";
                break;
            }
            cout << "\n";
        }

        SetColor(White, Black);
    }

    void PerformAction(int selectedAction)
    {
        switch (selectedAction)
        {
        case 0:
            CreateAndSaveNote();
            break;
        case 1:
            EditNote();
            break;
        case 2:
            DeleteNote();
            break;
        case 3:
            DisplayNotes();
            break;
        case 4:
            SearchByKeyword();
            break;
        case 5:
            SortByPriority();
            break;
        case 6:
            SortByTime();
            break;
        case 7:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Некорректный ввод. Пункт не найден.\n";
            break;
        }
    }
    Note()
    {
        //LoadFromFile();
        //priority = 0;
        //timeH = 0;
        //timeM = 0;
        
    }
    Note(int value)
    {
        if (value == 1)
        {
            LoadFromFile();
            Run();
        }
    }
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "RUS");
    int value = 1;
    Note t(value);

    return 0;
}