#include <iostream>
#include <fstream>       //for files
#include <limits>        //for to get safe input
#include <sstream>       //for to get or load data
#include <vector>        //for to create list or table
#include <openssl/evp.h> //for password hashing
#include <iomanip>       //for output formatting
using namespace std;

string SafeString(string message);

//---------------Hashing password using openssl library--------------//

#define hash_length 32
#define Iteration 10000

string HashPassword()
{
    string password = SafeString("Enter password: ");
    string salt = "!@hasher!@";
    unsigned char hash[hash_length];
    PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), reinterpret_cast<const unsigned char *>(salt.c_str()), salt.length(), Iteration, EVP_sha256(), hash_length, hash);
    stringstream ss;
    for (int i = 0; i < hash_length; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

//-----------------------Enumarate class-----------------//
enum class User
{
    None,
    Admin = 1,
    Student,
    Teacher,
    Exit = 0
};

enum class filePath
{
    StudentData,
    TeacherData
};

enum class Menu
{
    CreateUser = 1,
    LoginAccount,
    ForgotPassword,
    DisplayUser,
    Exit = 0
};

enum class UserPersonalMenu
{
    AccountDetail = 1,
    ChangeName,
    ChangeEmail,
    ChangePassword,
    UpdateData,
    DeleteAccount,
    Logout = 0
};

//---------------------File Path-----------------------//
string filename(filePath type)
{
    if (type == filePath::StudentData)
        return "database/StudentData.csv";
    else
        return "database/TeacherData.csv";
}

//------------------Usefull Function------------------//
int SafeInt(string message)
{
    int value;
    while (true)
    {
        cout << message;
        cin >> value;
        if (!cin.fail())
            return value;
        cin.clear();
        cout << "Invalid Input:\nPlease Enter Number Type:\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

bool IsValidGpa(float gpa)
{
    if (gpa >= 0.0 && gpa <= 4.0)
        return true;
    return false;
}

float SafeFlaot(string message)
{
    float value;
    while (true)
    {
        cout << message;
        cin >> value;
        if (!cin.fail() && IsValidGpa(value))
            return value;
        cin.clear();
        cout << "Invalid Input:\nPlease Float Type:\nGPA must be in Range[0.0:4.0]\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

bool GmailHaveSpace(string Gmail)
{
    for (char ch : Gmail)
    {
        if (ch == ' ')
            return true;
    }
    return false;
}

bool IsValidGmail(string Gmail)
{
    for (char ch : Gmail)
    {
        if (ch == '@' && !GmailHaveSpace(Gmail))
        {
            int index = Gmail.find("@");
            string a = Gmail.substr(index);
            if (a == "@gmail.com")
                return true;
            return false;
        }
    }
    return false;
}

string SafeString(string message)
{
    string value;
    cout << message;
    getline(cin >> ws, value);
    return value;
}

// string PasswordHasher(string password)
// {
//     unsigned long hash = 2345;
//     for (char ch : password)
//     {
//     }
//     return;
// }

//-------------------Data Storing Node------------------//
class Node
{
public:
    int id;
    string name;
    string gmail;
    string password;
    User Role;
    string department;
    string program;
    string semester;
    float gpa;
    int sallary;
    Node *next;
    Node *prev;
    Node()
    {
        next = prev = NULL;
    }
};

//----------------Class to store defferent type of data-------------//
class UserManager
{
    vector<Node> data;
    int lastId = 0;

public:
    void LoadData(filePath type, User Role)
    {
        data.clear();
        ifstream file(filename(type));
        if (!file.is_open())
            return;
        string line;

        getline(file, line);
        while (getline(file, line))
        {
            stringstream ss(line);
            string temp;
            Node u;
            getline(ss, temp, ',');
            u.id = stoi(temp);
            getline(ss, u.name, ',');
            getline(ss, u.gmail, ',');
            getline(ss, u.password, ',');
            getline(ss, u.department, ',');
            getline(ss, u.program, ',');
            if (Role == User::Student)
            {
                getline(ss, u.semester, ',');
                getline(ss, temp, ',');
                u.gpa = stof(temp);
            }
            else if (Role == User::Teacher)
            {
                getline(ss, temp, ',');
                u.sallary = stoi(temp);
            }
            u.Role = Role;
            data.push_back(u);
            lastId = max(lastId, u.id);
        }
        file.close();
    }

    void SaveData(filePath type, User Role)
    {
        ofstream file(filename(type));
        if (Role == User::Student)
            file << "Roll No,Name,Gmail,Password,Department,Program,Semester,GPA\n";
        else if (Role == User::Teacher)
            file << "Teacher ID,Name,Gmail,Password,Department,Program,Semester,GPA\n";
        for (auto &u : data)
        {
            file << u.id << "," << u.name << "," << u.gmail << "," << u.password << "," << u.department << "," << u.program << ",";
            if (Role == User::Student)
            {
                file << u.semester << "," << u.gpa;
            }
            else if (Role == User::Teacher)
            {
                file << u.sallary;
            }
            file << endl;
        }
        file.close();
    }

    bool IsUserExits(string gmail)
    {
        for (auto &d : data)
        {
            if (d.gmail == gmail)
                return true;
        }
        return false;
    }

    void CreateUser(User Role)
    {
        Node u;
        u.gmail = SafeString("Enter Gmail: ");
        if (IsValidGmail(u.gmail))
        {
            if (!IsUserExits(u.gmail))
            {
                u.id = ++lastId;
                cout << "User ID: " << u.id << endl;
                u.name = SafeString("Enter Name: ");
                u.password = HashPassword();
                u.department = SafeString("Enter Department: ");
                u.program = SafeString("Enter program: ");
                if (Role == User::Student)
                {
                    u.semester = SafeString("Enter Semester: ");
                    u.gpa = SafeFlaot("Enter GPA: ");
                }
                else if (Role == User::Teacher)
                {
                    u.sallary = SafeInt("Enter Salary: ");
                }
                data.push_back(u);
            }
            else
                cout << "User Already Exists:\n";
        }
        else
        {
            cout << "Invalid Gmail:\n1- Must have '@gmail.com'\n2- Have no Space Between\n";
        }
    }

    void DisplayUser(User role)
    {
        if (role == User::Student)
        {
            cout << "-------------------------------------------------------------All Student Data------------------------------------------------------------------\n";
            cout << setw(4) << left << "ID" << setw(15) << left << "Name" << setw(25) << left << "Gmail" << setw(40) << left << "Department" << setw(25) << left << "Program" << setw(10) << left << "Semester" << setw(5) << left << "GPA" << endl;
        }
        else if (role == User::Teacher)
        {
            cout << "----------------------------------All Teacher Data---------------------------------\n";
            cout << setw(4) << left << "ID" << setw(15) << left << "Name" << setw(25) << left << "Gmail" << setw(40) << left << "Department" << setw(25) << left << "Program" << setw(8) << left << "Salary" << endl;
        }

        if (data.empty())
            cout << "No data Added Yet:\n";

        for (auto &u : data)
        {
            cout << "-----------------------------------------------------------------------------------------------------------------------------------------------\n";
            if (role == User::Student)
                cout << setw(4) << left << u.id << setw(15) << left << u.name << setw(25) << left << u.gmail << setw(40) << left << u.department << setw(25) << left << u.program << setw(10) << left << u.semester << setw(5) << left << u.gpa << endl;
            else if (role == User::Teacher)
                cout << setw(4) << left << u.id << setw(15) << left << u.name << setw(25) << left << u.gmail << setw(40) << left << u.department << setw(25) << left << u.program << setw(8) << left << u.sallary << endl;
        }
    }

    void MyAccountDetails(filePath type, User Role, string gmail)
    {
        for (auto &a : data)
        {
            if (a.gmail == gmail)
            {
                cout << "------------- -Welcome " << a.name << "!---------------\n";
                if (Role == User::Student)
                    cout << "Roll No: " << a.id << "\nName: " << a.name << "\nGmail: " << a.gmail << "\nPassword: " << a.password << "\nDepartment: " << a.department << "\nProgram: " << a.program << "\nSemester: " << a.semester << "\nGPA: " << a.gpa << endl;
                else if (Role == User::Teacher)
                    cout << "Teacher ID: " << a.id << "\nName: " << a.name << "\nGmail: " << a.gmail << "\nPassword: " << a.password << "\nDepartment: " << a.department << "\nProgram: " << a.program << "\nSalary: " << a.sallary << endl;
            }
        }
    }

    void ChangeName(filePath type, User role, string gmail)
    {
        for (auto &u : data)
        {
            if (u.gmail == gmail)
            {
                string newname = SafeString("Enter new Name: ");
                u.name = newname;
                cout << "name Change successfully:\n";
                cout << "Login Again:\n";
                return;
            }
        }
    }

    void ChangeEmail(filePath type, User role, string gmail)
    {
        for (auto &u : data)
        {
            if (u.gmail == gmail)
            {
                string newgmail = SafeString("Enter New Gmail: ");
                u.gmail = newgmail;
                cout << "Gmail change Successfuly:\n";
                cout << "Login Again with new Gmail and Password:\n";
                return;
            }
        }
    }

    void ChangePassword(filePath type, User role, string gmail)
    {
        for (auto &u : data)
        {
            if (u.gmail == gmail)
            {
                string newpassword = HashPassword();
                u.password = newpassword;
                cout << "Password Change Successfully:\n";
                cout << "Login Again to update data:\n";
                return;
            }
        }
    }

    // bool Isupdatedselected()
    // {
    // }

    void UpdateData(filePath type, User role, string gmail)
    {
        cout << "-----------------------Updating Section---------------------\n";
        cout << "1- Department\t2- Program\t3- Semester\t4- GPA\t5- Salary\t0- Back\n";
        cout << "------------------------------------------------------------\n";
        string option = SafeString("Choose Option with Comma between To update Data: ");
        vector<int> UpdatingMenu;
        stringstream ss(option);
        string temp;
        int index = 0;
        while (true)
        {
            getline(ss, temp, ',');
            index = stoi(temp);
            UpdatingMenu.push_back(index);
            index++;
        }
        for (auto u : UpdatingMenu)
        {
            cout << u << endl;
        }
    }

    void DeleteAccount(filePath type, User role, string gmail)
    {
    }

    void personalUserArea(filePath Type, User Role, string gmail)
    {
        LoadData(Type, Role);
        int option;
        do
        {
            if (Role == User::Student)
                cout << "--------------------Student--------------------------\n";
            else if (Role == User::Teacher)
                cout << "--------------------Teacher--------------------------\n";
            cout << "1- My account Detail\n2- Change Name\n3- Change Gmail\n4- Change Password\n5- Update Data\n6- Delete my Account\n0- Logout\n";
            cout << " ------------------------------------------------------\n";
            option = SafeInt("Choose Option: ");
            switch ((UserPersonalMenu)option)
            {
            case UserPersonalMenu::AccountDetail:
                MyAccountDetails(Type, Role, gmail);
                break;
            case UserPersonalMenu::ChangeName:
                ChangeName(Type, Role, gmail);
                SaveData(Type, Role);
                return;
            case UserPersonalMenu::ChangeEmail:
                ChangeEmail(Type, Role, gmail);
                SaveData(Type, Role);
                return;
            case UserPersonalMenu::ChangePassword:
                ChangePassword(Type, Role, gmail);
                SaveData(Type, Role);
                return;
            case UserPersonalMenu::UpdateData:
                cout<<"---------------Update Data---------------\n";
                UpdateData(Type,Role,gmail);
                return;
            case UserPersonalMenu::DeleteAccount:
                return;
            case UserPersonalMenu::Logout:
                return;
            default:
                cout << "Invalid Option:\n";
            }
        } while (option != 0);
    }

    void ForgotPassword(filePath type, User Role, string gmail, string name)
    {
        LoadData(type, Role);
        for (auto &u : data)
        {
            if (u.gmail == gmail && u.name == name)
            {
                string newpassword = SafeString("Enter newpassword: ");
                u.password = newpassword;
                SaveData(type, Role);
                cout << "Password Forgetted Successfully:\nLogin again with new cridential\n";
                return;
            }
            else if (u.gmail == gmail && u.name != name)
            {
                cout << "Incorrect Name:\n";
                return;
            }
        }
    }
};

class HashTable
{
    const static int size = 10;
    Node *Table[size];

    int HashFunction(string Gmail)
    {
        int Hash = 0;
        for (char ch : Gmail)
        {
            Hash += ch;
        }
        return Hash % size;
    }

public:
    HashTable()
    {
        for (int i = 0; i < size; i++)
        {
            Table[i] = NULL;
        }
    }

    void LoadDataHashTable(filePath type, User Role)
    {
        ifstream file(filename(type));
        if (!file.is_open())
            return;
        string line;
        getline(file, line);
        while (getline(file, line))
        {
            stringstream ss(line);
            int id = 0;
            string name, gmail, password, department, program, semester;
            int sallary = 0;
            float gpa = 0.0;
            string temp;
            getline(ss, temp, ',');
            id = stoi(temp);
            getline(ss, name, ',');
            getline(ss, gmail, ',');
            getline(ss, password, ',');
            getline(ss, department, ',');
            getline(ss, program, ',');
            if (Role == User::Student)
            {
                getline(ss, semester, ',');
                getline(ss, temp, ',');
                stof(temp);
            }
            else if (Role == User::Teacher)
            {
                getline(ss, temp, ',');
                sallary = stoi(temp);
            }
            AddUserToHashTable(id, name, gmail, password, department, program, semester, gpa, sallary);
        }
        file.close();
    }

    void AddUserToHashTable(int id, string name, string gmail, string password, string department, string program, string semester, float gpa, int sallary)
    {
        int index = HashFunction(gmail);
        Node *newnode = new Node{};
        newnode->id = id;
        newnode->name = name;
        newnode->gmail = gmail;
        newnode->password = password;
        newnode->department = department;
        newnode->program = program;
        newnode->semester = semester;
        newnode->gpa = gpa;
        newnode->sallary = sallary;
        newnode->next = Table[index];
        Table[index] = newnode;
    }

    bool IsEmailExists(string Gmail)
    {
        int index = HashFunction(Gmail);
        Node *ptr = Table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == Gmail)
                return true;
            ptr = ptr->next;
        }
        return false;
    }

    void LoginAccount(filePath Type, User Role, string Gmail, string Password)
    {
        UserManager user;
        int index = HashFunction(Gmail);
        Node *ptr = Table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == Gmail && ptr->password == Password)
            {
                user.personalUserArea(Type, Role, ptr->gmail);
                return;
            }
            else if (ptr->gmail == Gmail && ptr->password != Password)
            {
                cout << "Incorrect Password:\n";
                return;
            }
            ptr = ptr->next;
        }
    }
};

void UserSection(filePath type, User Role)
{
    UserManager user;
    HashTable Hash;
    user.LoadData(type, Role);
    string Gmail, Password;
    int option;
    do
    {
        if (Role == User::Student)
            cout << "----------------Student Section-------------------\n";
        else if (Role == User::Teacher)
            cout << "----------------Teacher Section-------------------\n";
        cout << "1- Create User\n2- Login\n3- Forgot Password\n4- Display\n0- Back\n";
        cout << "---------------------------------------------------\n";
        option = SafeInt("Choose Option: ");
        switch ((Menu)option)
        {
        case Menu::CreateUser:
            user.CreateUser(Role);
            user.SaveData(type, Role);
            break;
        case Menu::LoginAccount:
            cout << "-----------------Login------------------\n";
            Hash.LoadDataHashTable(type, Role);
            Gmail = SafeString("Enter Gmail: ");
            if (IsValidGmail(Gmail))
            {
                if (Hash.IsEmailExists(Gmail))
                {
                    Password = HashPassword();
                    Hash.LoginAccount(type, Role, Gmail, Password);
                }
                else
                {
                    cout << "Account Does not Exists on this Gmail\n";
                }
            }
            else
            {
                cout << "Invalid Gmail!\n1- Must Have 'Gmail.com'\n2- Have No space between\n";
            }
            break;
        case Menu::ForgotPassword:
            cout << "------------Forgot Password-----------------\n";
            cout << "To Forget Password. Enter Name And Gmail:\n";
            Gmail = SafeString("Enter Gmail: ");
            if (IsValidGmail(Gmail))
            {
                if (user.IsUserExits(Gmail))
                {
                    string name = SafeString("Enter Name: ");
                    user.ForgotPassword(type, Role, Gmail, name);
                }
                else
                {
                    cout << "Account Does not exists on this Gmail:\n";
                }
            }
            else
            {
                cout << "Invalid Gmail:\n1- Must have '@gmail.com'.\n2- Have no space between\n";
            }
            break;
        case Menu::DisplayUser:
            user.DisplayUser(Role);
            break;
        case Menu::Exit:
            return;
        default:
            cout << "Invalid Option:\n";
        }
    } while (option != 0);
}

void AdminSection()
{
    cout<<"---------------------Admin Section-------------------\n";
    cout<<"1- Display All Teacher\n2- Display All Student\n0- Exit\n ";
    cout<<"-----------------------------------------------------\n";
    cout << "Admin Section:\n";
}

void StartProgram()
{
    int option;
    do
    {
        cout << "---------------------Start Program-----------------\n";
        cout << "1- Admin Control Panel\n2- User as Student\n3- User as Teacher\n0- Exit\n";
        cout << "---------------------------------------------------\n";
        option = SafeInt("Choose Option: ");
        switch ((User)option)
        {
        case User::Admin:
            AdminSection();
            break;
        case User::Student:
            UserSection(filePath::StudentData, User::Student);
            break;
        case User::Teacher:
            UserSection(filePath::TeacherData, User::Teacher);
            break;
        case User::Exit:
            cout << "---------------Program End-----------------\n";
            return;
        default:
            cout << "Invalid OPtion:\n";
        }
    } while (option != 0);
}

int main()
{
    StartProgram();

    return 0;
}