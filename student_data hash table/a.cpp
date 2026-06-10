#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

string filename = "database/userdata.csv";

class Node
{
public:
    string roll_no;
    string name;
    string gmail;
    string role;
    string password;
    string department;
    string program;
    string semester;
    int sallary;
    float gpa;
    Node *next;
    Node *prev;

    Node(string name, string gmail, string password, string role)
    {
        this->name = name;
        this->gmail = gmail;
        this->password = password;
        this->role = role;
        next = prev = NULL;
    }
};

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
            cout << "Incorrect! Please Enter Number Type:\n";
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
            cout << "Invalid! please Enter Float Number:\n";
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
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail)
            {
                cout << "User Already Exists\n";
                return;
            }
            ptr = ptr->next;
        }
        Node *newnode = new Node{name, gmail, password, role};
        newnode->next = table[index];
        table[index] = newnode;
    }

    void LoginAccount(string gmail, string password, string role)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        if (ptr == NULL)
        {
            cout << "Account does not exists:\n";
            return;
        }
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail && ptr->password == password)
            {
                cout << "Login Successfully:\n";
                user(gmail, password, role);
                return;
            }
            else if (ptr->gmail == gmail && ptr->password != password)
            {
                cout << "Incorrect Password:\n";
            }
            else if (ptr->gmail != gmail)
            {
                cout << "Account not found:\n";
            }
            ptr = ptr->next;
        }
    }

    void DisplayUsers(string role)
    {
        cout << "Role :" << role << endl;
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

    void SaveData(string filename)
    {
        ofstream file(filename);
        file << "Name,Gmail,Password,Role,Department,Program,Roll_no,Semester,GPA,Sallary\n";
        for (int i = 0; i < size; i++)
        {
            Node *ptr = table[i];
            while (ptr != NULL)
            {
                stringstream ss;
                ss << ptr->name << "," << ptr->gmail << "," << ptr->password << "," << ptr->role << "," << ptr->department << "," << ptr->program << ",";
                if (ptr->role == "Teacher")
                {
                    ss << (ptr->roll_no = "") << "," << (ptr->semester = "") << "," << (ptr->gpa = 0.0) << "," << ptr->sallary;
                }
                else if (ptr->role == "Student")
                {
                    ss << ptr->roll_no << "," << ptr->semester << "," << ptr->gpa << "," << (ptr->sallary = 0);
                }
                file << ss.str() << endl;
                ptr = ptr->next;
            }
        }
        file.close();
    }

    void loadData(string filename)
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

            if (data.size() > 3)
            {
                string name = data[0];
                string gmail = data[1];
                string password = data[2];
                string role = data[3];
                string department = data[4];
                string program = data[5];
                string roll_no = "";
                string semester = "";
                int sallary = 0;
                float gpa = 0.0;
                if (role == "Student")
                {
                    roll_no = data[6];
                    semester = data[7];
                    gpa = stof(data[8]);
                    sallary = stoi(data[9]);
                }
                else if (role == "Teacher")
                {
                    roll_no = data[6];
                    semester = data[7];
                    gpa = stof(data[8]);
                    sallary = stoi(data[9]);
                }
                createAccount(name, gmail, password, role);
                updateProfile(name, gmail, password, department, program, role, roll_no, semester, gpa, sallary);
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
                    cout << "       - Teacher of Organization-       \n";
                    cout << "Name: " << ptr->name << "\nGmail: " << ptr->gmail << "\nPassword: " << ptr->password << endl;
                    if (!ptr->department.empty() && !ptr->program.empty() && !ptr->role.empty())
                        cout << "Department: " << ptr->department << "\nProgram: " << ptr->program << "\nSallary: " << ptr->sallary << endl;
                }
                else if (ptr->role == "Student")
                {
                    cout << "       - Student of Organization-       \n";
                    cout << "Roll No: " << ptr->roll_no << "\nName: " << ptr->name << "\nGmail: " << ptr->gmail << "\nPassword: " << ptr->password << endl;
                    if (!ptr->department.empty() && !ptr->program.empty() && !ptr->role.empty())
                        cout << "Department: " << ptr->department << "\nProgram: " << ptr->program << "\nSemester: " << ptr->semester << "\nGPA: " << ptr->gpa << endl;
                }
            }
            ptr = ptr->next;
        }
    }

    void updateProfile(string name, string gmail, string password, string department, string program, string role, string roll_no, string semester, float gpa, int sallary)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail && ptr->password == password)
            {
                name = ptr->name;
                ptr->department = department;
                ptr->program = program;
                ptr->role = role;
                if (ptr->role == "Teacher" || ptr->role == "teacher" || ptr->role == "TEACHER")
                {
                    ptr->sallary = sallary;
                }
                else if (ptr->role == "Student" || ptr->role == "student" || ptr->role == "STUDENT")
                {
                    ptr->roll_no = roll_no;
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

    void ChangeEmail(string gmail, string password, string filename)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        string newroll_no, newname, newgmail, newpassword, newdept, newprogram, newrole, newsemester;
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
                if (newrole == "Student" || newrole == "student" || newrole == "STUDENT")
                {
                    newroll_no = ptr->roll_no;
                    newsemester = ptr->semester;
                    newgpa = ptr->gpa;
                }
                else if (newrole == "Teacher" || newrole == "teacher" || newrole == "TEACHER")
                {
                    newsallary = ptr->sallary;
                }
                DeleteAccount(gmail, password);
                createAccount(newname, newgmail, newpassword, newrole);
                updateProfile(newname, newgmail, newpassword, newdept, newprogram, newrole, newroll_no, newsemester, newgpa, newsallary);
                SaveData(filename);
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

    bool isValidEmail(string gmail)
    {
        for (char ch : gmail)
        {
            if (ch == '@')
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
    void user(string gmail, string password, string role)
    {
        int option;
        string roll_no, name, department, program, semester;
        int sallary;
        float gpa;
        do
        {
            cout << "-------------------------------------------------\n";
            cout << "1- My account details\n2- Update profile\n3- Change Password\n4- Change Gmail/Email\n5- Change Your Name\n6- Delete My Account\n0- Logout\n";
            cout << "-------------------------------------------------\n";
            option = safe_int("Choose Option: ");
            switch (option)
            {
            case 1:
                cout << "----------------------------------\n";
                cout << "         -My Account Detail-      \n";
                cout << "----------------------------------\n";
                AccountDetail(gmail, password, role);
                break;
            case 2:
                cout << "----------------------------------\n";
                cout << "           -Update Profile-       \n";
                cout << "----------------------------------\n";
                department = safeString("Enter Your Department Name: ");
                program = safeString("Enter Your Program Name: ");
                if (role == "Teacher" || role == "teacher" || role == "TEACHER")
                {
                    sallary = safe_int("Enter Sallary: ");
                }
                else if (role == "Student" || role == "student" || role == "STUDENT")
                {
                    roll_no = safeString("Enter Roll no: ");
                    semester = safeString("Enter Semester: ");
                    gpa = safe_float("Enter GPA: ");
                }
                else
                {
                    cout << "Enter role As [Teacher/Student]:\n";
                    break;
                }
                updateProfile(name, gmail, password, department, program, role, roll_no, semester, gpa, sallary);
                SaveData(filename);
                cout << "Acccount Updated Successfully:\n";
                break;
            case 3:
                cout << "------------------------------\n";
                cout << "        -Change Password-     \n";
                cout << "------------------------------\n";
                ChangePassword(gmail, password);
                SaveData(filename);
                cout << "Your Account Password Change Successfully:\n";
                cout << "Login Again with new password:\n";
                return;
            case 4:
                cout << "-------------------------------\n";
                cout << "         -Change Gmail-        \n";
                cout << "-------------------------------\n";
                ChangeEmail(gmail, password, filename);
                cout << "Email Changed Successfully:\n";
                cout << "Login now with new Gmail:\n";
                return;
            case 5:
                cout << "-------------------------------\n";
                cout << "          -Change Name-        \n";
                cout << "-------------------------------\n";
                ChangeName(gmail, password);
                SaveData(filename);
                cout << "Name Cahnge Successfully:\n";
                cout << "Login Again To Update Your name:\n";
                return;
            case 6:
                cout << "-------------------------------\n";
                cout << "        -Delete Account-       \n";
                cout << "-------------------------------\n";
                DeleteAccount(gmail, password);
                SaveData(filename);
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

void Userspecifier(string role)
{
    HashTable auth;
    auth.loadData(filename);
    int option;
    string name, gmail, password;
    do
    {
        cout << "-----------------------------------------\n";
        cout << "1- Create Account\n2- Login to Your Account\n3- Forgot Password\n4- Display " << role << " detail\n0- Back\n";
        cout << "-----------------------------------------\n";
        option = auth.safe_int("Choose Option: ");
        switch (option)
        {
        case 1:
            cout << "------------------------------------\n";
            cout << "           -Create Account-         \n";
            cout << "------------------------------------\n";
            gmail = auth.safeString("Enter Gmail: ");
            if (auth.isValidEmail(gmail))
            {
                if (!auth.isEmailExists(gmail))
                {
                    name = auth.safeString("Enter Name: ");
                    password = auth.safeString("Enter Password: ");
                    auth.createAccount(name, gmail, password, role);
                    auth.SaveData(filename);
                    cout << "Account Created Successfully:\n";
                }
                else
                {
                    cout << "Gmail Already Taken:\n";
                }
            }
            else if (!auth.isValidEmail(gmail))
            {
                cout << "Invalid Gmail: Must Have [@gmail.com]\n";
            }
            break;
        case 2:
            cout << "------------------------------------\n";
            cout << "          -Login Account-           \n";
            cout << "------------------------------------\n";
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
                    cout << "Account Does not Exists on corresponding Email:\n";
                }
            }
            else
            {
                cout << "Invalid Gmail: Must Have [@gmail.com]\n";
            }
            break;
        case 3:
            cout << "------------------------------------\n";
            cout << "          -Forgot password-         \n";
            cout << "------------------------------------\n";
            cout << "For creating new password please enter Name and Gmail:\n";
            gmail = auth.safeString("Enter Gmail: ");
            if (auth.isValidEmail(gmail))
            {
                if (auth.isEmailExists(gmail))
                {
                    name = auth.safeString("enter Name: ");
                    auth.forgotPassword(name, gmail);
                    auth.SaveData(filename);
                }
                else
                {
                    cout << "Account on Corresponding Gmail does not Exists:\n";
                }
            }
            else
            {
                cout << "Invalid Gmail: Must have [@gmail.com]\n";
            }
            break;
        case 4:
            cout << "------------------------------------\n";
            cout << "          -All " << role << "-   \n";
            cout << "------------------------------------\n";
            auth.DisplayUsers(role);
            break;
        case 0:
            return;
        default:
            cout << "Invalid Option:\n";
        }

    } while (option != 0);
}

class linkedList
{
    Node *head;
    Node *tail;

public:
    linkedList()
    {
        head = tail = NULL;
    }

    void AddData(string name, string gmail, string password, string role, string department, string program, string roll_no, string semester, float gpa, int sallary)
    {
        Node *newnode = new Node(name, gmail, password, role);
        newnode->department = department;
        newnode->program = program;
        if (role == "Student")
        {
            newnode->roll_no = roll_no;
            newnode->semester = semester;
            newnode->gpa = gpa;
        }
        else if (role == "Teacher")
        {
            newnode->sallary = sallary;
        }

        if (head == NULL)
        {
            head = tail = newnode;
            return;
        }
        newnode->prev = tail;
        tail->next = newnode;
        tail = newnode;
    }

    void DisplayData(string role)
    {
        Node *ptr = head;
        if (head == NULL)
        {
            cout << "No Data Added Yet:\n";
            return;
        }

        if (role == "Student")
            cout << "------------------------------------------------------------------------------Student Data--------------------------------------------------\n";
        else if (role == "Teacher")
            cout << "------------------------------------------------------------------------------Teacher Data----------------------------------------------\n";
        bool found = false;
        int count = 1;
        while (ptr != NULL)
        {
            if (ptr->role == role && role == "Student")
            {
                found = true;
                cout << "------------------------------------------------------------------------[" << role << "- " << count++ << "]-------------------------------------------------------------\n";
                cout << "Roll No: " << ptr->roll_no << "\t\tName: " << ptr->name << "\t\tGmail: " << ptr->gmail << "\t\tPassword: " << ptr->password << "\tDepartment: " << ptr->department << endl;
                cout << "Program: " << ptr->program << "\tSemester: " << ptr->semester << "\tGPA: " << ptr->gpa << endl;
                cout << "------------------------------------------------------------------------------------------------------------------------------------\n";
            }
            else if (ptr->role == role && role == "Teacher")
            {
                found = true;
                cout << "------------------------------------------------------------------------[" << role << "- " << count++ << "]--------------------------------------------------------------\n";
                cout << "Name: " << ptr->name << "\t\tGmail: " << ptr->gmail << "\t\tPassword: " << ptr->password << "\tDepartment: " << ptr->department << endl;
                cout << "Program: " << ptr->program << "\tSallary: " << ptr->sallary << endl;
                cout << "----------------------------------------------------------------------------------------------------------------------------------------------\n";
            }
            ptr = ptr->next;
        }

        if (!found)
        {
            cout << "Student Data Not Found:\n";
        }
    }

    void LoadData(string filename, string Role)
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
            vector<string> data;
            stringstream ss(line);
            string temp;
            while (getline(ss, temp, ','))
            {
                data.push_back(temp);
            }
            string name = data[0];
            string gmail = data[1];
            string password = data[2];
            string role = data[3];
            string department = data[4];
            string program = data[5];
            string roll_no = "";
            string semester = "";
            float gpa = 0.0;
            int sallary = 0;
            if (role == Role)
            {
                roll_no = data[6];
                semester = data[7];
                gpa = stof(data[8]);
                sallary = stoi(data[9]);
                AddData(name, gmail, password, role, department, program, roll_no, semester, gpa, sallary);
            }
            else if (role == Role)
            {
                roll_no = data[6];
                semester = data[7];
                gpa = stof(data[8]);
                sallary = stoi(data[9]);
                AddData(name, gmail, password, role, department, program, roll_no, semester, gpa, sallary);
            }
        }
        file.close();
    }

};

class student : public linkedList{
    
};

class Teacher : public linkedList{
    

};

void AdminControl()
{
    HashTable auth;
    Teacher teacher;
    student student;
    teacher.LoadData(filename, "Teacher");
    student.LoadData(filename, "Student");
    int option;
    do
    {
        cout << "--------------------------------\n";
        cout << "1- Display All student\n2- Display All teacher\n0- Back\n";
        cout << "--------------------------------\n";
        option = auth.safe_int("Choose Option: ");
        switch (option)
        {
        case 1:
            student.DisplayData("Student");
            break;
        case 2:
            teacher.DisplayData("Teacher");
            break;
        case 0:
            return;
        default:
            cout << "Invalid Option:\n";
        }
    } while (option != 0);
}

void startProram()
{
    HashTable auth;
    int option;
    string gmail, password, admingmail, adminpassword;
    do
    {
        cout << "--------------------------------------\n";
        cout << "1- Login as Admin:\n2- User as Student:\n3- User as Teacher\n0- Exit\n";
        cout << "--------------------------------------\n";
        option = auth.safe_int("Choose Option: ");
        switch (option)
        {
        case 1:
            cout << "-------------------------------\n";
            cout << "         -Login As Admin-      \n";
            cout << "-------------------------------\n";
            admingmail = "admin@gmail.com";
            adminpassword = "admin123";
            gmail = auth.safeString("Enter Gmail: ");
            if (auth.isValidEmail(gmail))
            {
                password = auth.safeString("Enter Password: ");
                if (gmail == admingmail && password == adminpassword)
                {
                    AdminControl();
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
                cout << "Invalid Email! Emailo Must Have [@gmail.com]:\n";
            }
            break;
        case 2:
            cout << "---------------------------------------\n";
            cout << "           -User As Student-          \n";
            cout << "---------------------------------------\n";
            Userspecifier("Student");
            break;
        case 3:
            cout << "---------------------------------------\n";
            cout << "           -User As Teacher-          \n";
            cout << "---------------------------------------\n";
            Userspecifier("Teacher");
            break;
        case 0:
            cout << "---------------------------------------\n";
            cout << "             -Program End-             \n";
            cout << "---------------------------------------\n";
            break;
        default:
            cout << "Invalid Option:\n";
        }

    } while (option != 0);
}

int main()
{
    startProram();
}