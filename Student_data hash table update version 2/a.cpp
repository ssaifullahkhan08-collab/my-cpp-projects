#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

enum User
{
    None,
    Admin,
    Student,
    Teacher
};
enum Menu
{
    CreateAccount,
    LoginAccount,
    ForgotPassword,
    UpdateAccount,
    AccountDetail
};
enum Files
{
    StudentDataFile,
    TeacherDataFile
};

string StudentFile = "database/studentuserdata.csv";
string TeacherFile = "database/teacheruserdata.csv";

class Node
{
public:
    static int rollCounter;
    static int teacherIdCounter;

    string name;
    string gmail;
    string role;
    string password;
    string department;
    string program;
    int roll_no;
    string semester;
    float gpa;
    int teacherId;
    int sallary;
    Node *next;
    Node *prev;

    Node(string name, string gmail, string password, string role)
    {
        this->name = name;
        this->gmail = gmail;
        this->password = password;
        this->role = role;
        this->department = "";
        this->program = "";
        if (role == "Student")
        {
            this->roll_no = ++rollCounter;
            this->teacherId = 0;
        }
        else if (role == "Teacher")
        {
            this->roll_no = 0;
            this->teacherId = ++teacherIdCounter;
        }
        this->semester = "";
        this->gpa = 0.0;
        this->sallary = 0;
        next = prev = NULL;
    }
};

int Node::teacherIdCounter = 0;
int Node::rollCounter = 0;

class HashTable
{
    static const int size = 10;
    Node *table[size];

    int HashFunction(string gmail)
    {
        int hash = 0;
        for (char ch : gmail)
        {
            hash += ch;
        }
        return hash % size;
    }

public:
    HashTable()
    {
        for (int i = 0; i < size; i++)
        {
            table[i] = NULL;
        }
    }

    int safe_int(string message)
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
            cout << "Invalid!\nPlease Enter Number Type:\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        return value;
    }

    float safe_float(string message)
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
            cout << "Invalid!\nplease Enter Float Number:\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        return value;
    }

    string safeString(string message)
    {
        string value;
        cout << message;
        getline(cin >> ws, value);
        return value;
    }

    void createAccount(string name, string gmail, string password, string role)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        Node *newnode = new Node{name, gmail, password, role};
        newnode->next = table[index];
        table[index] = newnode;
    }

    void LoginAccount(string gmail, string password, string role)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail && ptr->password == password)
            {
                if (role == "Student")
                    user(ptr->name, gmail, password, role, StudentFile);
                else if (role == "Teacher")
                    user(ptr->name, gmail, password, role, TeacherFile);
                cout << "Login Successfully:\n";

                return;
            }
            else if (ptr->gmail == gmail && ptr->password != password)
            {
                cout << "Incorrect Password:\n";
            }
            ptr = ptr->next;
        }
    }

    void DisplayUsers(string role)
    {
        cout << "------------Role :" << role << "------------" << endl;
        for (int index = 0; index < size; index++)
        {
            cout << "Index :" << index << ": --> ";
            Node *ptr = table[index];
            if (ptr == NULL)
            {
                cout << "NULL";
            }
            else
            {
                while (ptr != NULL)
                {
                    if (ptr->role == role)
                        cout << "[" << ptr->gmail << "," << ptr->password << "," << ptr->role << "] --> ";
                    ptr = ptr->next;
                }
                cout << "NULL";
            }
            cout << endl;
        }
    }

    void SaveDataByHAshTable(string role, string filename)
    {
        ofstream file(filename);
        if (role == "Student")
            file << "Name,Gmail,Password,Role,Department,Program,Roll No,Semester,GPA\n";
        else if (role == "Teacher")
            file << "Name,Gmail,Password,Role,Department,Program,Teacher ID,Sallary\n";
        for (int i = 0; i < size; i++)
        {
            Node *ptr = table[i];
            while (ptr != NULL)
            {
                stringstream ss;
                ss << ptr->name << "," << ptr->gmail << "," << ptr->password << "," << ptr->role << "," << ptr->department << "," << ptr->program << ",";
                if (role == "Student")
                    ss << ptr->roll_no << "," << ptr->semester << "," << ptr->gpa;
                else if (role == "Teacher")
                    ss << ptr->teacherId << "," << ptr->sallary;
                file << ss.str() << endl;
                ptr = ptr->next;
            }
        }
        file.close();
    }

    void loadData(string role, string filename)
    {
        ifstream file(filename);
        string line;

        if (!file.is_open())
        {
            cout << "File Does Not Exists:\n";
            return;
        }

        getline(file, line);
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            vector<string> data;
            string temp;
            while (getline(ss, temp, ','))
            {
                data.push_back(temp);
            }

            if (role == "Student")
            {
                string name = data[0];
                string gmail = data[1];
                string password = data[2];
                string role = data[3];
                string department = data[4];
                string program = data[5];
                int roll_no = stoi(data[6]);
                if (roll_no > Node::rollCounter)
                    Node::rollCounter = roll_no;
                string Semester = data[7];
                float gpa = stof(data[8]);
                createAccount(name, gmail, password, role);
                updateProfile(name, gmail, password, department, program, role, roll_no, 0, Semester, gpa, 0);
            }
            else if (role == "Teacher")
            {
                string name = data[0];
                string gmail = data[1];
                string password = data[2];
                string role = data[3];
                string department = data[4];
                string program = data[5];
                int teacherid = stoi(data[6]);
                if (teacherid > Node::teacherIdCounter)
                    Node::teacherIdCounter = teacherid;
                int sallary = stoi(data[7]);
                createAccount(name, gmail, password, role);
                updateProfile(name, gmail, password, department, program, role, 0, teacherid, "", 0, sallary);
            }
        }
        file.close();
    }

    void AccountDetail(string gmail, string password, string role)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail && ptr->password == password)
            {
                if (ptr->role == "Teacher")
                {
                    cout << "-------------------Welcome " << ptr->name << "!----------------\n";
                    cout << "Name: " << ptr->name << "\nGmail: " << ptr->gmail << "\nPassword: " << ptr->password << endl;
                    if (!ptr->department.empty() && !ptr->program.empty() && !ptr->role.empty())
                        cout << "Department: " << ptr->department << "\nProgram: " << ptr->program << "\nSallary: " << ptr->sallary << endl;
                }
                else if (ptr->role == "Student")
                {
                    cout << "-------------------Welcome " << ptr->name << "!--------------------\n";
                    cout << "Roll No: " << ptr->roll_no << "\nName: " << ptr->name << "\nGmail: " << ptr->gmail << "\nPassword: " << ptr->password << endl;
                    if (!ptr->department.empty() && !ptr->program.empty() && !ptr->role.empty())
                        cout << "Department: " << ptr->department << "\nProgram: " << ptr->program << "\nSemester: " << ptr->semester << "\nGPA: " << ptr->gpa << endl;
                }
            }
            ptr = ptr->next;
        }
    }

    void updateProfile(string name, string gmail, string password, string department, string program, string role, int roll_no, int teacherid, string semester, float gpa, int sallary)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail && ptr->password == password)
            {
                ptr->name = name;
                ptr->department = department;
                ptr->program = program;
                ptr->role = role;
                if (ptr->role == "Teacher")
                {
                    ptr->sallary = sallary;
                }
                else if (ptr->role == "Student")
                {
                    ptr->semester = semester;
                    ptr->gpa = gpa;
                }
                return;
            }
            ptr = ptr->next;
        }
    }

    void ChangePassword(string gmail, string password)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail)
            {
                string newpassword = safeString("Enter New password: ");
                ptr->password = newpassword;
                return;
            }
            ptr = ptr->next;
        }
    }

    void DeleteAccount(string gmail, string password)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        Node *prev;
        if (ptr->gmail == gmail)
        {
            table[index] = ptr->next;
            delete ptr;
            return;
        }

        while (ptr != NULL && ptr->gmail != gmail)
        {
            prev = ptr;
            ptr = ptr->next;
        }
        prev->next = ptr->next;
        delete ptr;
    }

    void ChangeEmail(string gmail, string password, string role, string filename)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        string newname, newgmail, newpassword, newdept, newprogram, newrole, newsemester;
        int newteacherId = 0;
        int newroll_no = 0;
        int newsallary = 0;
        float newgpa = 0.0;
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail)
            {
                newname = ptr->name;
                newgmail = safeString("Enter new Gmail: ");
                newpassword = ptr->password;
                newdept = ptr->department;
                newprogram = ptr->program;
                newrole = ptr->role;
                if (newrole == "Student")
                {
                    newroll_no = ptr->roll_no;
                    newsemester = ptr->semester;
                    newgpa = ptr->gpa;
                }
                else if (newrole == "Teacher")
                {
                    newteacherId = ptr->teacherId;
                    newsallary = ptr->sallary;
                }
                DeleteAccount(gmail, password);
                createAccount(newname, newgmail, newpassword, newrole);
                updateProfile(newname, newgmail, newpassword, newdept, newprogram, newrole, newroll_no, newteacherId, newsemester, newgpa, newsallary);
                SaveDataByHAshTable(role, filename);
                return;
            }
            ptr = ptr->next;
        }
    }

    void ChangeName(string gmail, string password)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail)
            {
                string newname = safeString("Enter New Name: ");
                ptr->name = newname;
                return;
            }
            ptr = ptr->next;
        }
    }

    void forgotPassword(string name, string gmail)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        if (ptr == NULL)
        {
            cout << "Account Does not Exists on this Gmail:\n";
            return;
        }

        while (ptr != NULL)
        {
            if (ptr->name == name && ptr->gmail == gmail)
            {
                string newpassword = safeString("Create New password: ");
                ptr->password = newpassword;
                cout << "New PAssword Created Successfully:\n";
                cout << "Login to account with new password:\n";
                return;
            }
            else if (ptr->name != name && ptr->gmail == gmail)
            {
                cout << "You Enter incorrect Name:\n";
            }
            else if (ptr->name == name && ptr->gmail != gmail)
            {
                cout << "Account Does not Exists:\n";
            }
            ptr = ptr->next;
        }
    }

    bool isEmailExists(string gmail)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail)
            {
                return true;
            }
            ptr = ptr->next;
        }
        return false;
    }

    bool HaveSpaceInEmail(string gmail)
    {
        for (char ch : gmail)
        {
            if (ch == ' ')
            {
                return true;
            }
        }
        return false;
    }

    bool isValidEmail(string gmail)
    {
        for (char ch : gmail)
        {
            if (ch == '@' && !HaveSpaceInEmail(gmail))
            {
                int index = gmail.find('@');
                string a = gmail.substr(index);
                if (a == "@gmail.com")
                {
                    return true;
                }
                return false;
            }
        }
        return false;
    }

private:
    void user(string name, string gmail, string password, string role, string filename)
    {
        int option;
        string department, program, semester;
        int sallary;
        float gpa;
        do
        {
            cout << "--------------Welcome " << name << "!-------------------\n";
            cout << "1- My account details\n2- Update profile\n3- Change Password\n4- Change [Gmail/Email]:\n5- Change Your Name\n6- Delete My Account\n0- Logout\n";
            cout << "-------------------------------------------------\n";
            option = safe_int("Choose Option: ");
            switch (option)
            {
            case 1:
                cout << "----------------My Account Detail----------------\n";
                AccountDetail(gmail, password, role);
                break;
            case 2:
                cout << "-----------------Update Profile------------------\n";
                department = safeString("Enter Your Department Name: ");
                program = safeString("Enter Your Program Name: ");
                if (role == "Teacher")
                {
                    sallary = safe_int("Enter Sallary: ");
                }
                else if (role == "Student")
                {
                    semester = safeString("Enter Semester: ");
                    gpa = safe_float("Enter GPA: ");
                }
                else
                {
                    cout << "Enter role As [Teacher/Student]:\n";
                    break;
                }
                updateProfile(name, gmail, password, department, program, role, 0, 0, semester, gpa, sallary);
                SaveDataByHAshTable(role, filename);
                cout << "Acccount Updated Successfully:\n";
                break;
            case 3:
                cout << "------------------------------\n";
                cout << "        -Change Password-     \n";
                cout << "------------------------------\n";
                ChangePassword(gmail, password);
                SaveDataByHAshTable(role, filename);
                cout << "Your Account Password Change Successfully:\n";
                cout << "Login Again with new password:\n";
                return;
            case 4:
                cout << "-------------------------------\n";
                cout << "         -Change Gmail-        \n";
                cout << "-------------------------------\n";
                ChangeEmail(gmail, password, role, filename);
                cout << "Email Changed Successfully:\n";
                cout << "Login now with new Gmail:\n";
                return;
            case 5:
                cout << "-------------------------------\n";
                cout << "          -Change Name-        \n";
                cout << "-------------------------------\n";
                ChangeName(gmail, password);
                SaveDataByHAshTable(role, filename);
                cout << "Name Cahnge Successfully:\n";
                cout << "Login Again To Update Your name:\n";
                return;
            case 6:
                cout << "-------------------------------\n";
                cout << "        -Delete Account-       \n";
                cout << "-------------------------------\n";
                DeleteAccount(gmail, password);
                SaveDataByHAshTable(role, filename);
                cout << "Your Account Deleted Successfully:\n";
                return;
            case 0:
                break;
            default:
                cout << "Invalid option. Please try Again:\n";
            }
        } while (option != 0);
    }
};

void Userspecifier(string role, string filename)
{
    HashTable auth;
    auth.loadData(role, filename);
    int option;
    string name, gmail, password;
    do
    {
        cout << "--------------" << role << " Section-------------------\n";
        cout << "1- Create Account\n2- Login to Your Account\n3- Forgot Password\n4- Display " << role << " detail\n0- Back\n";
        cout << "---------------------------------------------------\n";
        option = auth.safe_int("Choose Option: ");
        switch (option)
        {
        case 1:
            cout << "-------------Create Account------------------\n";
            gmail = auth.safeString("Enter Gmail: ");
            if (auth.isValidEmail(gmail))
            {
                if (!auth.isEmailExists(gmail))
                {
                    name = auth.safeString("Enter Name: ");
                    password = auth.safeString("Enter Password: ");
                    auth.createAccount(name, gmail, password, role);
                    auth.SaveDataByHAshTable(role, filename);
                    cout << "Account Created Successfully:\n";
                }
                else
                {
                    cout << "Gmail Already Taken:\n";
                }
            }
            else if (!auth.isValidEmail(gmail))
            {
                cout << "Invalid Gmail:\n1- Must Have [@gmail.com]2- Having no space between\n";
            }
            break;
        case 2:
            cout << "----------------Login Account----------------\n";
            gmail = auth.safeString("Enter Gmail: ");
            if (auth.isValidEmail(gmail))
            {
                if (auth.isEmailExists(gmail))
                {
                    password = auth.safeString("Enter Password: ");
                    auth.LoginAccount(gmail, password, role);
                }
                else
                {
                    cout << "Account Does not Exists on Gmail: " << gmail << ":\n";
                }
            }
            else
            {
                cout << "Invalid Gmail:\n1- Must Have [@gmail.com].\n2- Have no space in between.\n";
            }
            break;
        case 3:
            cout << "-------------------Forgot password------------------\n";
            cout << "For creating new password please enter Name and Gmail:\n";
            gmail = auth.safeString("Enter Gmail: ");
            if (auth.isValidEmail(gmail))
            {
                if (auth.isEmailExists(gmail))
                {
                    name = auth.safeString("enter Name: ");
                    auth.forgotPassword(name, gmail);
                    auth.SaveDataByHAshTable(role, filename);
                }
                else
                {
                    cout << "Account on does not Exists on Gmail" << gmail << ":\n";
                }
            }
            else
            {
                cout << "Invalid Gmail:\n1- Must have [@gmail.com].\n2- Have no space between.\n";
            }
            break;
        case 4:
            cout << "-----------------All " << role << "----------------------\n";
            auth.DisplayUsers(role);
            break;
        case 0:
            return;
        default:
            cout << "Invalid Option:\n";
        }

    } while (option != 0);
}

// class linkedList
// {

// public:
//     Node *head;
//     Node *tail;
//     linkedList()
//     {
//         head = tail = NULL;
//     }

//     void AddData(string name, string gmail, string password, string role, string department, string program, int roll_no, string semester, float gpa, int sallary)
//     {
//         Node *newnode = new Node(name, gmail, password, role);
//         newnode->department = department;
//         newnode->program = program;
//         if (role == "Student")
//         {
//             newnode->roll_no = roll_no;
//             newnode->semester = semester;
//             newnode->gpa = gpa;
//         }
//         else if (role == "Teacher")
//         {
//             newnode->sallary = sallary;
//         }

//         if (head == NULL)
//         {
//             head = tail = newnode;
//             return;
//         }
//         newnode->prev = tail;
//         tail->next = newnode;
//         tail = newnode;
//     }

//     void DisplayData(string role)
//     {
//         Node *ptr = head;
//         if (head == NULL)
//         {
//             cout << "No Data Added Yet:\n";
//             return;
//         }

//         if (role == "Student")
//             cout << "------------------------------------------------------------------------------Student Data--------------------------------------------------\n";
//         else if (role == "Teacher")
//             cout << "------------------------------------------------------------------------------Teacher Data----------------------------------------------\n";
//         bool found = false;
//         int count = 1;
//         while (ptr != NULL)
//         {
//             if (ptr->role == role && role == "Student")
//             {
//                 found = true;
//                 cout << "------------------------------------------------------------------------[" << role << "- " << count++ << "]-------------------------------------------------------------\n";
//                 cout << "Roll No: " << ptr->roll_no << "\t\tName: " << ptr->name << "\t\tGmail: " << ptr->gmail << "\t\tPassword: " << ptr->password << "\tDepartment: " << ptr->department << endl;
//                 cout << "Program: " << ptr->program << "\tSemester: " << ptr->semester << "\tGPA: " << ptr->gpa << endl;
//                 cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
//             }
//             else if (ptr->role == role && role == "Teacher")
//             {
//                 found = true;
//                 cout << "------------------------------------------------------------------------[" << role << "- " << count++ << "]--------------------------------------------------------------\n";
//                 cout << "Name: " << ptr->name << "\t\tGmail: " << ptr->gmail << "\t\tPassword: " << ptr->password << "\tDepartment: " << ptr->department << endl;
//                 cout << "Program: " << ptr->program << "\tSallary: " << ptr->sallary << endl;
//                 cout << "----------------------------------------------------------------------------------------------------------------------------------------------\n";
//             }
//             ptr = ptr->next;
//         }

//         if (!found)
//         {
//             cout << "Student Data Not Found:\n";
//         }
//     }

//     void LoadData(string filename, string Role)
//     {
//         ifstream file(filename);
//         string line;
//         if (!file.is_open())
//         {
//             cout << "File Does Not Exists:\n";
//             return;
//         }

//         getline(file, line);
//         while (getline(file, line))
//         {
//             vector<string> data;
//             stringstream ss(line);
//             string temp;
//             while (getline(ss, temp, ','))
//             {
//                 data.push_back(temp);
//             }
//             string name = data[0];
//             string gmail = data[1];
//             string password = data[2];
//             string role = data[3];
//             string department = data[4];
//             string program = data[5];
//             int roll_no = 0;
//             string semester = "";
//             float gpa = 0.0;
//             int sallary = 0;
//             if (role == Role)
//             {
//                 roll_no = stoi(data[6]);
//                 semester = data[7];
//                 gpa = stof(data[8]);
//                 sallary = stoi(data[9]);
//                 AddData(name, gmail, password, role, department, program, roll_no, semester, gpa, sallary);
//             }
//             else if (role == Role)
//             {
//                 roll_no = stoi(data[6]);
//                 semester = data[7];
//                 gpa = stof(data[8]);
//                 sallary = stoi(data[9]);
//                 AddData(name, gmail, password, role, department, program, roll_no, semester, gpa, sallary);
//             }
//         }
//         file.close();
//     }

//     bool isEmailExists(string gmail)
//     {
//         Node *ptr = head;
//         while (ptr != NULL)
//         {
//             if (ptr->gmail == gmail)
//             {
//                 return true;
//             }
//             ptr = ptr->next;
//         }
//         return false;
//     }

//     void printHardCopyDetailOfStudent(string gmail, string filename)
//     {
//         ofstream file(filename);
//         Node *ptr = head;
//         if (ptr->role == "Student")
//             file << "Roll No,Name,Gmail,Department,Program,Semester,GPA\n";
//         else if (ptr->role == "Teacher")
//             file << "Name,Gmail,Department,Program,Sallary\n";
//         while (ptr != NULL)
//         {
//             if (ptr->gmail == gmail)
//             {
//                 if (ptr->role == "Student")
//                 {
//                     file << ptr->roll_no << "," << ptr->name << "," << ptr->gmail << "," << ptr->department << "," << ptr->program << "," << ptr->semester << "," << ptr->gpa << endl;
//                     cout << "Detail of Student of Gmail: " << ptr->gmail << " are printed\n";
//                     cout << "Goto [Student_academic_detail] folder to print Details3:\n";
//                 }
//                 else if (ptr->role == "Teacher")
//                 {
//                     file << ptr->name << "," << ptr->gmail << "," << ptr->department << "," << ptr->program << "," << ptr->sallary << endl;
//                     cout << "Detail of Teacher of Gmail: " << ptr->gmail << " are printed\n";
//                     cout << "Goto [Teacher_academic_detail] folder to print Details:\n";
//                 }
//                 return;
//             }
//             ptr = ptr->next;
//         }
//         file.close();
//     }
// };

// class student : public linkedList
// {
// public:
// };

// class Teacher : public linkedList
// {
// };

// void StudentSection(string role,string filename)
// {
//     HashTable hash;
//     student student;
//     student.LoadData(filename, "Student");
//     int option;
//     string gmail;
//     do
//     {
//         cout << "--------------------Student Section----------------\n";
//         cout << "1- Display All Student\n2- print Academic detail of student by gmail\n3- print academic detail of student by range[Roll no]\n0- Back\n";
//         cout << "---------------------------------------------------\n";
//         option = hash.safe_int("Choose Option: ");
//         switch (option)
//         {
//         case 1:
//             student.DisplayData("Student");
//             break;
//         case 2:
//             gmail = hash.safeString("Enter Gmail of student to print detail: ");
//             if (hash.isValidEmail(gmail))
//             {
//                 if (student.isEmailExists(gmail))
//                 {
//                     student.printHardCopyDetailOfStudent(gmail, "database/studentdata.csv");
//                 }
//                 else
//                 {
//                     cout << "Account on this Gmail Does not exists:\n";
//                 }
//             }
//             else
//             {
//                 cout << "Invalid Gmail:\n";
//             }
//             break;
//         case 3:
//             break;
//         case 0:
//             return;
//         default:
//             cout << "Invalid option:\n";
//         }

//     } while (option != 0);
// }

// void TeacherSection(string role,string filename)
// {
//     HashTable hash;
//     hash.loadData(role,filename);
//     int option;
//     string gmail;
//     do
//     {
//         cout << "----------------------------Teacher Section--------------------------\n";
//         cout << "1- Display All Teacher\n2- print Academic detail of teacher by gmail\n0- Back\n";
//         cout << "----------------------------------------------------------------------\n";
//         option = hash.safe_int("Choose Option: ");
//         switch (option)
//         {
//         case 1:
//             .DisplayData("Teacher");
//             break;
//         case 2:
//             gmail = hash.safeString("Enter Gmail of Teacher to print detail: ");
//             if (hash.isValidEmail(gmail))
//             {
//                 if (teacher.isEmailExists(gmail))
//                 {
//                     teacher.printHardCopyDetailOfStudent(gmail, "database/teacherdata.csv");
//                 }
//                 else
//                 {
//                     cout << "Account on this Gmail Does not exists:\n";
//                 }
//             }
//             else
//             {
//                 cout << "Invalid Gmail:\n";
//             }
//             break;
//         case 3:
//             break;
//         case 0:
//             return;
//         default:
//             cout << "Invalid option:\n";
//         }

//     } while (option != 0);
// }

// void AdminControl()
// {
//     HashTable auth;
//     int option;
//     string gmail;
//     do
//     {
//         cout << "--------------------------------\n";
//         cout << "1- Student Section\n2- Teacher Section\n3- Display All student\n4- Display All teacher\n0- Back\n";
//         cout << "--------------------------------\n";
//         option = auth.safe_int("Choose Option: ");
//         switch (option)
//         {
//         case 1:
//             StudentSection("Student",StudentFile);
//             break;
//         case 2:
//             TeacherSection("Teacher",TeacherFile);
//             break;
//         case 3:
//             break;
//         case 4:
//             break;
//         case 0:
//             return;
//         default:
//             cout << "Invalid Option:\n";
//         }
//     } while (option != 0);
// }

void startProram()
{
    HashTable hash;
    int option;
    string gmail, password, admingmail, adminpassword;
    do
    {
        cout << "-------------------Start Section-------------------\n";
        cout << "1- Login as Admin:\n2- User as Student:\n3- User as Teacher\n0- Exit\n";
        cout << "---------------------------------------------------\n";
        option = hash.safe_int("Choose Option: ");
        switch (option)
        {
        case 1:
            cout << "---------------Login As Admin---------------\n";
            admingmail = "admin@gmail.com";
            adminpassword = "admin123";
            gmail = hash.safeString("Enter Gmail: ");
            if (hash.isValidEmail(gmail))
            {
                password = hash.safeString("Enter Password: ");
                if (gmail == admingmail && password == adminpassword)
                {
                    // AdminControl();
                }
                else if (gmail == admingmail && password != adminpassword)
                {
                    cout << "Incorrect Password:\n";
                }
                else if (gmail != admingmail && password == adminpassword)
                {
                    cout << "Incorrect Gmail:\n";
                }
            }
            else
            {
                cout << "Invalid Email!\n1- Email Must Have [@gmail.com].\n2- Have No Space between.\n";
            }
            break;
        case 2:
            cout << "---------------User As Student----------------\n";
            Userspecifier("Student", StudentFile);
            break;
        case 3:
            cout << "---------------User As Teacher----------------\n";
            Userspecifier("Teacher", TeacherFile);
            break;
        case 0:
            cout << "-----------------Program End-------------------\n";
            break;
        default:
            cout << "Invalid Option:\n";
        }

    } while (option != 0);
}

int main()
{
    startProram();
    return 0;
}