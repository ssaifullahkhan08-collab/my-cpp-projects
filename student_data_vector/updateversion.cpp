#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

string FileName = "students1.csv";

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
    vector<Student> array;

public:
    int maxRoll()
    {
        int max = 100000;
        for (auto el : array)
        {
            max = el.Roll_no;
        }
        return max;
    }

    void AddData(int Roll_no, string Name, float GPA)
    {
        sortingMenu(1);
        Roll_no = maxRoll() + 1;
        array.push_back({Roll_no, Name, GPA});
    }

    void ShowArrayData()
    {
        if (array.empty())
        {
            cout << "No Data Yet:\n";
            return;
        }

        TableFormat();
        int approachind = 1;
        for (Student el : array)
        {
            cout << left << setw(5) << approachind++ << setw(10) << el.Roll_no << setw(15) << el.Name << right << setw(5) << setprecision(3) << el.GPA << endl;
        }
    }

    void DeleteDataMenu(int option)
    {
        if (array.empty())
        {
            cout << "No Data Yet:\n";
            return;
        }

        int approachind = 0;
        bool found = false;
        if (option == 1)
        {
            array.pop_back();
            found = true;
        }
        else if (option == 2)
        {
            int Roll_no = SafeInt("Enter Roll No to Delete: ");
            for (Student el : array)
            {
                if (el.Roll_no == Roll_no)
                {
                    array.erase(array.begin() + approachind);
                    found == true;
                    return;
                }
                approachind++;
            }
        }
        else if (option == 3)
        {
            string Name = SafeString("Enter Name To Delete Data: ");
            approachind = 0;
            for (Student el : array)
            {
                if (el.Name == Name)
                {
                    array.erase(array.begin() + approachind);
                    found = true;
                    return;
                }
                approachind++;
            }
        }

        if (!found)
        {
            cout << "Data Not Found:\n";
            return;
        }
        ShowArrayData();
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

    void TableFormat()
    {
        cout << left << setw(5) << "No#" << setw(10) << "Roll No#" << setw(15) << "Name" << right << setw(5) << "CGPA" << endl;
        cout << setfill('-') << setw(50) << "-" << endl;
        cout << setfill(' ');
    }

    void SearchData(int option)
    {
        if (array.empty())
        {
            cout << "No Data Yet:\n";
            return;
        }

        bool found = false;
        if (option == 1)
        {
            int Roll_no = SafeInt("Enter Roll No To Search: ");
            TableFormat();
            int approachind = 1;
            for (Student el : array)
            {
                if (el.Roll_no == Roll_no)
                {
                    found = true;
                    cout << left << setw(5) << approachind++ << setw(10) << el.Roll_no << setw(15) << el.Name << right << setw(5) << setprecision(3) << el.GPA << endl;
                }
            }
        }
        else if (option == 2)
        {
            string Name = SafeString("Enter Name To Search: ");
            TableFormat();
            int approachind = 1;
            for (Student el : array)
            {
                if (el.Name == Name)
                {
                    found = true;
                    cout << left << setw(5) << approachind++ << setw(10) << el.Roll_no << setw(15) << el.Name << right << setw(5) << setprecision(3) << el.GPA << endl;
                }
            }
        }
        else if (option == 3)
        {
            float GPA = SafeFloat("Enter CGPA To Search: ");
            TableFormat();
            int approachind = 1;
            for (Student el : array)
            {
                if (el.GPA == GPA)
                {
                    found = true;
                    cout << left << setw(5) << approachind++ << setw(10) << el.Roll_no << setw(15) << el.Name << right << setw(5) << setprecision(3) << el.GPA << endl;
                }
            }
        }

        if (!found)
        {
            cout << "Element Not Found:\n";
            return;
        }
    }

    void sortingMenu(int option)
    {
        if (option == 1)
            sort(array.begin(), array.end(), CompareByRollNoAscending);
        else if (option == 2)
            sort(array.begin(), array.end(), CompareByRollNodescending);
        else if (option == 3)
            sort(array.begin(), array.end(), CompareByGPAscending);
        else if (option == 4)
            sort(array.begin(), array.end(), CompareByGPAdescending);
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
        file.close();
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
            array.push_back({Roll_no, name, GPA});
        }
        file.close();
    }

    void UpdateData(int option)
    {
        bool found = false;
        string name;
        float GPA;
        int Roll_no = SafeInt("Enter Roll No: ");
        if (option == 1)
        {
            for (auto &el : array)
            {
                if (el.Roll_no == Roll_no)
                {
                    found = true;
                    name = SafeString("Enter New Name: ");
                    el.Name = name;
                    cout<<name<<","<<el.Name<<endl;
                    break;
                }
            }
        }
        else if (option == 2)
        {
            for (auto &el : array)
            {
                if (el.Roll_no == Roll_no)
                {
                    found = true;
                    GPA = SafeFloat("Enter New GPA: ");
                    el.GPA = GPA;
                    cout<<GPA<<","<<el.GPA<<endl;
                    break;
                }
            }
        }

        if (!found)
        {
            cout << "Roll No Not Found:\n";
        }
    }
};

void StartProgram()
{
    Array a1;
    a1.LoadData(FileName);
    string Name;
    int Roll_no = 0;
    float GPA;
    int option;
    int sub_option;
    do
    {
        cout << "---------------Program Start--------------------\n";
        cout << "1- Add New Student:\n2- All Students Data:\n3- Remove Student Data Section:\n4- Search Student Data Section:\n5- Arrange Student Data Section:\n6- Update Student Data Section\n0- Exit:\n";
        cout << "------------------------------------------------\n";
        option = a1.SafeInt("Choose Option: ");
        switch (option)
        {
        case 1:
            cout << "------------------Add Data---------------------\n";
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
            cout << "---------------Remove Data Menu----------------\n";
            cout << "1- Remove Last Data\n2- Remove by Roll No\n3- Remove by Name\n0- Back\n";
            cout << "-----------------------------------------------\n";
            sub_option = a1.SafeInt("Choose option to Delete Data: ");
            switch (sub_option)
            {
            case 1:
                a1.DeleteDataMenu(sub_option);
                break;
            case 2:
                a1.DeleteDataMenu(sub_option);
                break;
            case 3:
                a1.DeleteDataMenu(sub_option);
                break;
            case 0:
                break;
            default:
                cout << "Invalid Option:\n";
            }
            a1.SaveData(FileName);
            break;
        case 4:
            cout << "-----------------Search Data-----------------\n";
            cout << "1- Search by Roll No\n2- Search By Name\n3- Search by GPA\n0- Back\n";
            cout << "----------------------------------------------\n";
            sub_option = a1.SafeInt("Choose option to Search: ");
            switch (sub_option)
            {
            case 1:
                a1.SearchData(sub_option);
                break;
            case 2:
                a1.SearchData(sub_option);
                break;
            case 3:
                a1.SearchData(sub_option);
                break;
            case 0:
                break;
            default:
                cout << "Invalid Option:\n";
            }  
            break;
        case 5:
            cout << "-----------------Array Sorting Menu----------------\n";
            cout << "1- By Roll No (Ascending):\n2- By Roll No (Descending):\n3- By GPA (Ascending)\n4- By GPA (Descending)\n0- Back\n";
            cout << "----------------------------------------------------\n";
            sub_option = a1.SafeInt("Choose option to Sort Data: ");
            switch (sub_option)
            {
            case 1:
                a1.sortingMenu(sub_option);
                cout << "-------------Array in Ascending Order(Roll No)------------\n";
                a1.ShowArrayData();
                break;
            case 2:
                cout << "-------------Array in Descending Order(Roll No)------------\n";
                a1.sortingMenu(sub_option);
                a1.ShowArrayData();
                break;
            case 3:
                cout << "-------------Array in Ascending Order(GPA)------------\n";
                a1.sortingMenu(sub_option);
                a1.ShowArrayData();
                break;
            case 4:
                cout << "-------------Array in Descending Order(GPA)------------\n";
                a1.sortingMenu(sub_option);
                a1.ShowArrayData();
                break;
            case 0:
                break;
            default:
                cout << "Incorrect Option:\n";
            }
            break;
        case 6:
            cout << "-------------------Update Data Section----------------------\n";
            cout << "1- Update Name\n2- Update GPA\n0- Back\n";
            cout << "------------------------------------------------------------\n";
            sub_option = a1.SafeInt("Choose option: ");
            switch (sub_option)
            {
            case 1:
                a1.UpdateData(sub_option);
                break;
            case 2:
                a1.UpdateData(sub_option);
                break;
            case 0:
                break;
            default:
                cout << "Invalid Option:\n";
            }
            a1.SaveData(FileName);
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