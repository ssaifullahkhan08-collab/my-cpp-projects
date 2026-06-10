#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
using namespace std;

class Node
{
public:
    string roll_no;
    string name;
    string program;
    string semester;
    float gpa;
    Node *next;
    Node *prev;

    Node(string roll_no, string name, string program, string semester, float gpa)
    {
        this->roll_no = roll_no;
        this->name = name;
        this->program = program;
        this->semester = semester;
        this->gpa = gpa;
        next = prev = NULL;
    }
};

class Data
{
    static const int size = 20;
    Node *table[size];
    Node *head;
    Node *tail;

    int hashFunction(string roll_no)
    {
        int hash = 0;
        for (char ch : roll_no)
        {
            hash += ch;
        }
        return hash % size;
    }

public:
    Data()
    {
        for (int i = 0; i < size; i++)
        {
            table[i] = NULL;
        }
        head = tail = NULL;
    }

    void Add_data(string roll_no, string name, string program, string semester, float gpa)
    {
        Node *newnode = new Node(roll_no, name, program, semester, gpa);
        if (head == NULL)
        {
            head = tail = newnode;
            return;
        }
        newnode->prev = tail;
        tail->next = newnode;
        tail = newnode;
    }

    void HashTable(string roll_no, string name, string program, string semester, float gpa)
    {
        int index = hashFunction(roll_no);
        Node *newnode = new Node{roll_no, name, program, semester, gpa};
        newnode->next = table[index];
        table[index] = newnode;
    }

    void search(string roll_no)
    {
        int index = hashFunction(roll_no);
        Node *ptr = table[index];
        int count = 1;
        cout << "______________________________________________________________________________________________________________________________\n";
        cout << "No#\tRoll No\t\tName\t\t\t\tProgram\t\t\t\t\tSemester\t\tGPA\n";
        cout << "______________________________________________________________________________________________________________________________\n";
        if (ptr == NULL)
        {
            cout << "NULL index:\n";
            return;
        }
        bool found = false;
        while (ptr != NULL)
        {
            if (ptr->roll_no == roll_no)
            {
                found = true;
                cout << count++ << "\t" << ptr->roll_no << "\t\t" << ptr->name << "\t\t\t" << ptr->program << "\t\t\t" << ptr->semester << "\t\t\t" << ptr->gpa << "\n";
            }
            ptr = ptr->next;
        }
        if (!found)
        {
            cout << "Data of respected Roll no not Found:\n";
        }
        cout << "______________________________________________________________________________________________________________________________\n";
    }

    void Delete_data()
    {
        if (head == NULL)
        {
            cout << "No Data Yet:\n";
            return;
        }

        if (head == tail)
        {
            head = tail = NULL;
            return;
        }

        Node *ptr = tail;
        tail = ptr->prev;
        tail->next = NULL;
        delete ptr;
    }

    void display()
    {
        if (head == NULL)
        {
            cout << "No Data Yet:\n";
            return;
        }
        int count = 1;
        cout << "______________________________________________________________________________________________________________________________\n";
        cout << "No#\tRoll No\t\tName\t\t\t\tProgram\t\t\t\t\tSemester\t\tGPA\n";
        cout << "______________________________________________________________________________________________________________________________\n";
        Node *ptr = head;
        while (ptr != NULL)
        {
            cout << count++ << "\t" << ptr->roll_no << "\t\t" << ptr->name << "\t\t\t" << ptr->program << "\t\t\t" << ptr->semester << "\t\t\t" << ptr->gpa << "\n";
            ptr = ptr->next;
        }
        cout << "______________________________________________________________________________________________________________________________\n";
    }

    void Diplsy_hashTable()
    {
        for (int i = 0; i < size; i++)
        {
            cout << "Index " << i << ": --> ";
            Node *ptr = table[i];

            while (ptr != NULL)
            {
                cout << " [" << ptr->roll_no << "," << ptr->name << "," << ptr->program << "," << ptr->semester << "," << ptr->gpa << "] --> ";
                ptr = ptr->next;
            }
            
            if(ptr == NULL){
                cout<<"NULL";
            }
            cout<<endl;
        }
    }

    void Save_data(string filename)
    {
        ofstream file(filename);
        Node *ptr = head;
        while (ptr != NULL)
        {
            file << ptr->roll_no << "," << ptr->name << "," << ptr->program << "," << ptr->semester << "," << ptr->gpa << endl;
            ptr = ptr->next;
        }
    }

    void Load_Data(string filename)
    {
        ifstream file(filename);
        string line;
        if (!file.is_open())
        {
            cout << "File Not Found:\n";
            return;
        }

        while (head != NULL)
        {
            Delete_data();
        }

        while (getline(file, line))
        {
            if (line.empty())
                continue;
            string roll_no, name, program, semester;
            float gpa;
            string temp;
            stringstream ss(line);
            getline(ss, roll_no, ',');
            getline(ss, name, ',');
            getline(ss, program, ',');
            getline(ss, semester, ',');
            getline(ss, temp, ',');
            gpa = stof(temp);

            Add_data(roll_no, name, program, semester, gpa);
            HashTable(roll_no, name, program, semester, gpa);
        }
    }
};

int Safe_int(string message)
{
    int value;
    while (true)
    {
        cout << message;
        cin >> value;

        if (!cin.fail())
        {
            return value;
        }
        cout << "Invalid! Please Enter Number Type: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

string Safe_string(string message)
{
    string value;
    cout << message;
    getline(cin >> ws, value);
    return value;
}

float Safe_float(string message)
{
    float value;
    while (true)
    {
        cout << message;
        cin >> value;

        if (!cin.fail())
        {
            return value;
        }
        cout << "Invalid! Please Enter float Type: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

void strat_program()
{
    Data d;
    int option;
    string roll_no, name, program, semester;
    float gpa;
    string filename = "database/data1.csv";
    d.Load_Data(filename);
    do
    {
        cout << "------------------------------------\n";
        cout << "1- Add Data\n2- Dislay Data\n3- Delete data\n4- Search Roll No\n5- Display HashTable\n0- Exit\n";
        cout << "------------------------------------\n";
        option = Safe_int("Choose Option: ");
        switch (option)
        {
        case 1:
            roll_no = Safe_string("Enter Roll No: ");
            name = Safe_string("Enter Name: ");
            program = Safe_string("Enter program: ");
            semester = Safe_string("Enter semester: ");
            gpa = Safe_float("Enter GPA: ");
            d.Add_data(roll_no, name, program, semester, gpa);
            d.HashTable(roll_no, name, program, semester, gpa);
            d.Save_data(filename);
            break;
        case 2:
            d.display();
            break;
        case 3:
            d.Delete_data();
            d.Save_data(filename);
            break;
        case 4:
            roll_no = Safe_string("Enter Roll No To Search: ");
            d.search(roll_no);
            break;
        case 5:
            cout<<"-------------------------------------------------------------------\n";
            cout<<"                      -HASH TABLE-                                 \n";
            cout<<"-------------------------------------------------------------------\n";
            d.Diplsy_hashTable();
            break;
        case 0:
            cout << "----------------------------------\n";
            cout << "          -Program End-           \n";
            cout << "----------------------------------\n";
            break;
        default:
            cout << "Invalid Option:\n";
        }
    } while (option != 0);
}

int main()
{
    strat_program();
}