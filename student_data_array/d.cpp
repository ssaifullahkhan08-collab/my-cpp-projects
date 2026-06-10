#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;
#define size 20

string FileName = "students.csv";

struct Student
{
    int Roll_no;
    string Name;
    float GPA;
};

// sorting area
bool CompareByRollNoAscending(Student a, Student b)
{
    return a.Roll_no < b.Roll_no;
}

bool CompareByRollNodescending(Student a, Student b)
{
    return a.Roll_no > b.Roll_no;
}

bool CompareByGPAscending(Student a, Student b)
{
    return a.GPA < b.GPA;
}

bool CompareByGPAdescending(Student a, Student b)
{
    return a.GPA > b.GPA;
}

class Array
{
    Student array[size] = {};
    int index = -1;

public:
    void AddData(int Roll_no, string Name, float GPA)
    {
        index++;
        if (index >= size)
        {
            bool IsArrayFull = false;
            for (int i = 0; i < size; i++)
            {
                if (array[i].Roll_no == 0)
                {
                    IsArrayFull = true;
                    array[i].Roll_no = Roll_no;
                    array[i].Name = Name;
                    array[i].GPA = GPA;
                    cout << "Value Added to empty index Successfully:\n";
                    break;
                }
            }

            if (!IsArrayFull)
            {
                cout << "Array Full:\n";
            }
        }
        else if (index < size)
        {
            array[index].Roll_no = Roll_no;
            array[index].Name = Name;
            array[index].GPA = GPA;
            cout << "Value added successfully:\n";
        }
    }

    void ShowArrayData()
    {
        if (index == -1)
        {
            cout << "No Data Yet:\n";
            return;
        }
        cout << "-------------------------------------------------------------------\n";
        cout << "No#\tRoll NO\t\tName\t\t\t\tGPA\n";
        cout << "-------------------------------------------------------------------\n";
        int approachind = 1;
        for (int i = 0; i < size; i++)
        {
            if (array[i].Roll_no != 0)
                cout << approachind++ << "\t" << array[i].Roll_no << "\t\t" << array[i].Name << "\t\t\t\t" << array[i].GPA << endl;
        }
        cout << "-------------------------------------------------------------------\n";
    }

    void DeleteData()
    {
        if (index == -1)
        {
            cout << "No Data Yet:\n";
            return;
        }

        int Roll_no = SafeInt("Enter Roll No to Delete: ");
        bool found = false;
        for (int i = 0; i < size; i++)
        {
            if (array[i].Roll_no == Roll_no)
            {
                found = true;
                array[i].Roll_no = 0;
                array[i].Name = "";
                array[i].GPA = 0.0;
                cout << "Data Deleted Successfully:\n";
            }
        }
        if (!found)
        {
            cout << "Value not Found:\n";
        }
    }

    int SafeInt(string message)
    {
        int value;
        while (true)
        {
            cout << message;
            cin >> value;
            if (!cin.fail())
                return value;
            cout << "Invalid Input! Please enter Numeric Data:\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        return value;
    }

    float SafeFloat(string message)
    {
        float value;
        while (true)
        {
            cout << message;
            cin >> value;
            if (!cin.fail())
                return value;
            cout << "Invalid input! Please Enter numeric Value:\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        return value;
    }

    void SearchData(int Roll_no)
    {
        bool found = false;
        for (int i = 0; i < size; i++)
        {
            if (array[i].Roll_no == Roll_no)
            {
                found = true;
                cout << "-------------------------------------------------------------------\n";
                cout << "No#\tRoll NO\t\tName\t\t\t\tGPA\n";
                cout << "-------------------------------------------------------------------\n";
                cout << i << "\t" << array[i].Roll_no << "\t\t" << array[i].Name << "\t\t\t\t" << array[i].GPA << endl;
                cout << "-------------------------------------------------------------------\n";
                break;
            }
        }

        if (!found)
        {
            cout << "Element Not Found:\n";
            return;
        }
    }

    // sorted by roll no
    void sortingMenu(int option)
    {
        if (option == 1)
            sort(array, array + size, CompareByRollNoAscending);
        else if (option == 2)
            sort(array, array + size, CompareByRollNodescending);
        else if (option == 3)
            sort(array, array + size, CompareByGPAscending);
        else if (option == 4)
            sort(array, array + size, CompareByGPAdescending);
        // update index value
        index = size;
    }

    string SafeString(string message)
    {
        string value;
        cout << message;
        getline(cin >> ws, value);
        return value;
    }

    void SaveData(string filename)
    {
        ofstream file(filename);
        file << "Roll No#,Name,GPA\n";
        for (Student el : array)
        {
            if (el.Roll_no != 0)
                file << el.Roll_no << "," << el.Name << "," << el.GPA << endl;
        }
    }

    void LoadData(string FileName)
    {
        ifstream file(FileName);
        if (!file.is_open())
        {
            cout << "File not Found:\n";
            return;
        }
        string line;
        getline(file, line);
        while (getline(file, line))
        {
            if (line.empty())
                continue;
            int Roll_no;
            string name, temp;
            float GPA;
            stringstream ss(line);
            getline(ss, temp, ',');
            Roll_no = stoi(temp);
            getline(ss, name, ',');
            getline(ss, temp, ',');
            GPA = stof(temp);
            AddData(Roll_no, name, GPA);
        }
    }
};

void StartProgram()
{
    Array a1;
    a1.LoadData(FileName);
    int Roll_no;
    string Name;
    float GPA;
    int option;
    int sorted_option;
    do
    {
        cout << "---------------Program Start--------------------\n";
        cout << "1- Add New Student:\n2- All Students:\n3- Remove Student:\n4- Search Student Details:\n5- Arrange Student:\n0- Exit:\n";
        cout << "------------------------------------------------\n";
        option = a1.SafeInt("Choose Option: ");
        switch (option)
        {
        case 1:
            cout << "------------------Add Data---------------------\n";
            Roll_no = a1.SafeInt("Enter Roll No: ");
            Name = a1.SafeString("Enter Name: ");
            GPA = a1.SafeFloat("Enter GPA: ");
            a1.AddData(Roll_no, Name, GPA);
            a1.SaveData(FileName);
            break;
        case 2:
            cout << "-----------------Array Data------------------\n";
            a1.ShowArrayData();
            break;
        case 3:
            a1.DeleteData();
            break;
        case 4:
            cout << "-----------------Search Data-----------------\n";
            Roll_no = a1.SafeInt("Enter Data to Search: ");
            a1.SearchData(Roll_no);
            break;
        case 5:
            cout << "-----------------Array Sorting Menu----------------\n";
            cout << "1-By Roll No (Ascending):\n2-By Roll No (Descending):\n3- By GPA (Ascending)\n4-By GPA (Descending)\n0- Exit\n";
            cout << "----------------------------------------------------\n";
            sorted_option = a1.SafeInt("Choose option to Sort Data: ");
            switch (sorted_option)
            {
            case 1:
                a1.sortingMenu(sorted_option);
                cout << "-------------Array in Ascending Order(Roll No)------------\n";
                a1.ShowArrayData();
                break;
            case 2:
                cout << "-------------Array in Descending Order(Roll No)------------\n";
                a1.sortingMenu(sorted_option);
                a1.ShowArrayData();
                break;
            case 3:
                cout << "-------------Array in Ascending Order(GPA)------------\n";
                a1.sortingMenu(sorted_option);
                a1.ShowArrayData();
                break;
            case 4:
                cout << "-------------Array in Descending Order(GPA)------------\n";
                a1.sortingMenu(sorted_option);
                a1.ShowArrayData();
                break;
            default:
                cout << "Incorrect Option:\n";
            }
            break;
        case 0:
            cout << "-----------------Program End-----------------\n";
            break;
        default:
            cout << "Invalid Option:\n";
        }

    } while (option != 0);
}

int main()
{
    StartProgram();
}