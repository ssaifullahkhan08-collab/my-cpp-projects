#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
using namespace std;

/* ================= ENUMS ================= */

enum class UserType { NONE = 0, ADMIN, STUDENT, TEACHER };
enum class FileType { STUDENT_FILE, TEACHER_FILE };
enum class Menu {CREATE = 1,LOGIN,DISPLAY,EXIT};

/* ================= FILE PATHS ================= */

string getFilePath(FileType type) {
    if (type == FileType::STUDENT_FILE)
        return "database/student.csv";
    return "database/teacher.csv";
}

/* ================= USER MODEL ================= */

struct User {
    int id;
    string name;
    string email;
    string password;
    UserType type;
    string department;
    string program;
    string semester;
    float gpa;
    int salary;
};

/* ================= UTILITIES ================= */

int safeInt(string msg) {
    int x;
    while (true) {
        cout << msg;
        cin >> x;
        if (!cin.fail()) return x;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number!\n";
    }
}

string safeString(string msg) {
    string s;
    cout << msg;
    getline(cin >> ws, s);
    return s;
}

/* ================= USER MANAGER ================= */

class UserManager {
    vector<User> users;
    int lastId = 0;

public:
    void load(FileType file, UserType type) {
        users.clear();
        ifstream fin(getFilePath(file));
        string line;
        if (!fin.is_open()) return;

        getline(fin, line); // header
        while (getline(fin, line)) {
            stringstream ss(line);
            User u;
            string temp;

            getline(ss, temp, ','); u.id = stoi(temp);
            getline(ss, u.name, ',');
            getline(ss, u.email, ',');
            getline(ss, u.password, ',');
            getline(ss, u.department, ',');
            getline(ss, u.program, ',');

            if (type == UserType::STUDENT) {
                getline(ss, u.semester, ',');
                getline(ss, temp, ','); u.gpa = stof(temp);
            } else {
                getline(ss, temp, ','); u.salary = stoi(temp);
            }

            u.type = type;
            users.push_back(u);
            lastId = max(lastId, u.id);
        }
        fin.close();
    }

    void save(FileType file, UserType type) {
        ofstream fout(getFilePath(file));
        if (type == UserType::STUDENT)
            fout << "ID,Name,Email,Password,Department,Program,Semester,GPA\n";
        else
            fout << "ID,Name,Email,Password,Department,Program,Salary\n";

        for (auto &u : users) {
            fout << u.id << "," << u.name << "," << u.email << "," << u.password
                 << "," << u.department << "," << u.program << ",";
            if (type == UserType::STUDENT)
                fout << u.semester << "," << u.gpa;
            else
                fout << u.salary;
            fout << "\n";
        }
        fout.close();
    }

    void createUser(UserType type) {
        User u;
        u.id = ++lastId;
        u.type = type;

        u.name = safeString("Enter Name: ");
        u.email = safeString("Enter Email: ");
        u.password = safeString("Enter Password: ");
        u.department = safeString("Enter Department: ");
        u.program = safeString("Enter Program: ");

        if (type == UserType::STUDENT) {
            u.semester = safeString("Enter Semester: ");
            u.gpa = stof(safeString("Enter GPA: "));
        } else {
            u.salary = safeInt("Enter Salary: ");
        }

        users.push_back(u);
        cout << "Account Created | ID = " << u.id << endl;
    }

    void display(UserType type) {
        for (auto &u : users) {
            cout << "\nID: " << u.id
                 << "\nName: " << u.name
                 << "\nEmail: " << u.email
                 << "\nDepartment: " << u.department
                 << "\nProgram: " << u.program << endl;

            if (type == UserType::STUDENT)
                cout << "Semester: " << u.semester << " GPA: " << u.gpa << endl;
            else
                cout << "Salary: " << u.salary << endl;
        }
    }
};

/* ================= MAIN MENU ================= */

void userSection(UserType type, FileType file) {
    UserManager manager;
    manager.load(file, type);

    int choice;
    do {
        cout << "\n1. Create Account\n2. Display All\n3. Exit\n";
        choice = safeInt("Choose: ");

        switch ((Menu)choice) {
            case Menu::CREATE:
                manager.createUser(type);
                manager.save(file, type);
                break;

            case Menu::DISPLAY:
                manager.display(type);
                break;

            case Menu::EXIT:
                manager.save(file, type);
                return;

            default:
                cout << "Invalid option\n";
        }
    } while (true);
}

/* ================= PROGRAM START ================= */

int main() {
    int choice;
    do {
        cout << "\n1. Student Section\n2. Teacher Section\n0. Exit\n";
        choice = safeInt("Choose: ");

        if (choice == 1)
            userSection(UserType::STUDENT, FileType::STUDENT_FILE);
        else if (choice == 2)
            userSection(UserType::TEACHER, FileType::TEACHER_FILE);

    } while (choice != 0);

    cout << "Program Ended\n";
    return 0;
}
