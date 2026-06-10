#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Node
{
public:
    string name;
    string gmail;
    string password;
    Node *next;
    Node *prev;

    Node(string n, string g, string p)
    {
        name = n;
        gmail = g;
        password = p;
        next = prev = NULL;
    }
};

class Authentication
{
    static const int size = 10;
    Node *table[size];

    // Hash function to find index to where store the gmail
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
    Authentication()
    {
        for (int i = 0; i < size; i++)
        {
            table[i] = NULL;
        }
    }

    void signUp(string name, string gmail, string password)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        while (ptr != NULL)
        {
            if (ptr->gmail == gmail)
            {
                cout << "User Already Exists:\n";
                return;
            }
            ptr = ptr->next;
        }
        Node *newnode = new Node{name, gmail, password};
        newnode->next = table[index];
        table[index] = newnode;
    }

    void login(string gmail, string password)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        bool found = false;
        if (ptr == NULL)
        {
            cout << "User does not exists:\n";
        }
        else
        {
            while (ptr != NULL)
            {
                if (ptr->gmail == gmail && ptr->password == password)
                {
                    cout << "You are Login Successfully:\n";
                    found = true;
                    return;
                }
                else if (ptr->gmail != gmail || ptr->password != password)
                {
                    cout << "Incorect User Gmail or Password:\n";
                    return;
                }
                ptr = ptr->next;
            }

            if (!found)
            {
                cout << "User Does not Exists:\n";
            }
        }
    }

    void diplay_users()
    {
        for (int i = 0; i < size; i++)
        {
            cout << "Index: " << i << " --> ";
            Node *ptr = table[i];
            if (ptr == NULL)
            {
                cout << "NULL";
            }
            else
            {
                while (ptr != NULL)
                {
                    cout << "[" << ptr->name << "," << ptr->gmail << "," << ptr->password << "] --> ";
                    ptr = ptr->next;
                }
                cout << "NULL";
            }
            cout << endl;
        }
    }

    void delete_account(string gmail, string password)
    {
        login(gmail, password);
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

        if (ptr == NULL)
        {
            cout << "Account not Found:\n";
            return;
        }

        prev->next = ptr->next;
        delete ptr;
    }

    void save_info(string filename)
    {
        ofstream file(filename);
        for (int i = 0; i < size; i++)
        {
            Node *ptr = table[i];
            while (ptr != NULL)
            {
                stringstream ss;
                ss << ptr->name << "," << ptr->gmail << "," << ptr->password;
                file << ss.str() << endl;
                ptr = ptr->next;
            }
        }
        file.close();
    }

    string safe_string(string message)
    {
        string value;
        cout << message;
        getline(cin >> ws, value);
        return value;
    }

    void change_password(string gmail, string password)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        bool found = false;
        if (ptr == NULL)
        {
            cout << "Account Does Not Exists:\n";
            return;
        }

        while (ptr != NULL)
        {
            if (ptr->gmail == gmail && ptr->password == password)
            {
                found = true;
                password = safe_string("Enter new Password: ");
                ptr->password = password;
                cout << "Password Changed Successfully:\n";
                return;
            }
            else if(ptr->gmail == gmail && ptr->password != password)
            {
                cout << "Incorrect Password:\n";
                cout << "Please Try Again:\n";
                return;
            }
            ptr = ptr->next;
        }

        if (!found)
        {
            cout << "Account Not Found:\n";
        }
    }

    void changeEmail(string gmail, string password, string filename)
    {
        int index = HashFunction(gmail);
        Node *ptr = table[index];
        bool found = false;
        string temp_name, temp_pass;

        if (ptr == NULL)
        {
            cout << "Account Does Not Exists:\n";
            return;
        }

        while (ptr != NULL)
        {
            if (ptr->gmail == gmail && ptr->password == password)
            {
                found = true;
                temp_name = ptr->name;
                temp_pass = ptr->password;
                delete_account(gmail, password);
                gmail = safe_string("Enter New Gmail: ");
                signUp(temp_name, gmail, temp_pass);
                save_info(filename);
                cout << "Gmail Changed Successfully:\n";
                return;
            }
            else if (ptr->gmail == gmail && ptr->password != password)
            {
                cout << "Incorrect password:\n";
                cout << "Please Try Again....\n";
                return;
            }
            ptr = ptr->next;
        }

        if (!found)
        {
            cout << "Account Not Found:\n";
        }
    }

    void load_data(string filename)
    {
        ifstream file(filename);
        string line;
        if (!file.is_open())
        {
            cout << "User File Missing:\n";
            return;
        }

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            string name, gmail, password;
            stringstream ss(line);

            if (!getline(ss, name, ',') || !getline(ss, gmail, ',') || !getline(ss, password, ','))
                continue;

            if (name.empty() || gmail.empty() || password.empty())
                continue;

            signUp(name, gmail, password);
        }
        file.close();
    }

    void disply_user_in_table()
    {
        cout << "______________________________________________________________________________________________________\n";
        cout << "No#\t\tName\t\t\t\t\tGmail\t\t\t\t\t\tPassword\n";
        cout << "______________________________________________________________________________________________________\n";
        int count = 1;
        for (int i = 0; i < size; i++)
        {
            Node *ptr = table[i];
            if(ptr == NULL){
                cout<<"NULL Indexing\n";
            }

            while (ptr != NULL)
            {
                cout << count++ << "\t\t" << ptr->name << "\t\t\t\t" << ptr->gmail << "\t\t\t\t\t" << ptr->password << "\n";
                ptr = ptr->next;
            }
            cout<<endl;
        }
        cout << "__________________________________________________________________________________________________________\n";
    }
};

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

        cout << "Invalid! Enter Number Type: ";
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

        cout << "Invalid! Enter Number Type: ";
        cin.clear();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

string safe_string(string message)
{
    string value;
    cout << message;
    getline(cin >> ws, value);
    return value;
}

void start_program()
{
    Authentication a;
    string filename = "database/users_detail.csv";
    a.load_data(filename);
    int option;
    string name, gmail, password;
    cout << "----------------------------------\n";
    cout << "        -Authentication-          \n";
    cout << "----------------------------------\n";
    do
    {
        cout << "----------------------------------\n";
        cout << "1- Create Account:\n2- Login To Account:\n3- Display Users:\n4- Delete Account:\n5- Change Password:\n6- Change Email:\n7- Display Data in Table\n0- Exit:\n";
        cout << "----------------------------------\n";
        option = safe_int("Choose Option: ");

        switch (option)
        {
        case 1:
            cout << "--------------------------------\n";
            cout << "            Sign Up             \n";
            cout << "--------------------------------\n";
            name = safe_string("Enter Name: ");
            gmail = safe_string("Enter Gmail/Mail: ");
            password = safe_string("Enter Password: ");
            a.signUp(name, gmail, password);
            a.save_info(filename);
            cout << "Your Account Created Successfully:\n";
            break;
        case 2:
            cout << "--------------------------------\n";
            cout << "            Login               \n";
            cout << "--------------------------------\n";
            gmail = safe_string("Enter Gmail/mail: ");
            password = safe_string("Enter Password: ");
            a.login(gmail, password);
            break;
        case 3:
            cout << "--------------------------------\n";
            cout << "        -Users Details:-        \n";
            cout << "--------------------------------\n";
            a.diplay_users();
            cout << "--------------------------------\n";
            break;
        case 4:
            cout << "--------------------------------\n";
            cout << "         -Delete Account-       \n";
            cout << "--------------------------------\n";
            cout << "For Deleting Account Please Login First:\n";
            gmail = safe_string("Enter Gmail to Delete Account: ");
            password = safe_string("Enter Password: ");
            a.delete_account(gmail, password);
            a.save_info(filename);
            cout << "Account Deleted Successfully:\n";
            break;
        case 5:
            cout << "--------------------------------\n";
            cout << "         -Change Password-      \n";
            cout << "--------------------------------\n";
            cout << "For Changing Password Please Login first:\n";
            gmail = safe_string("Enter Gmail: ");
            password = safe_string("Enter Password: ");
            a.change_password(gmail, password);
            a.save_info(filename);
            break;
        case 6:
            cout << "--------------------------------\n";
            cout << "         -Change Gmail-         \n";
            cout << "--------------------------------\n";
            cout << "For Changing Gmail Please Login First:\n";
            gmail = safe_string("Enter Gmail: ");
            password = safe_string("Enter Password: ");
            a.changeEmail(gmail, password, filename);
            break;
        case 7:
            cout << "--------------------------------\n";
            cout << "     -Display data in table-    \n";
            cout << "--------------------------------\n";
            a.disply_user_in_table();
            break;
        case 0:
            cout << "--------------------------------\n";
            cout << "          program end           \n";
            cout << "--------------------------------\n";
            break;
        default:
            cout << "Invalid! Please Choose Valid Option: ";
        }
    } while (option != 0);
}

int main()
{
    start_program();
}