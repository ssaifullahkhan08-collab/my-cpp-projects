#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <unordered_map>
using namespace std;

struct Date
{
    int Day;
    int Month;
    int Year;
};
Date Today = {6, 6, 2026};

enum Role
{
    None = 1,
    AdminRole,
    StudentRole,
    TeacherRole,
    EmployeRole,
    User
};

enum FileSystem
{
    UsersFile = 0,
    UserAcademicFile,
    StudentFile,
    TeacherFile,
    EmployeFile,
    DptProgramFile
};

int SafeInt(string message)
{
    int value;
    while (true)
    {
        cout << message;
        cin >> value;
        if (!cin.fail())
            return value;
        cout << "Please Enter Numeric Data:\n";
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
        cout << "Please Enter Numeric Float Data:\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

string SafeString(string message)
{
    string value;
    cout << message;
    getline(cin >> ws, value);
    return value;
}

vector<string> ProgramList = {"None", "BS Computer Science", "BS Informartion Technology", "BS Data Science", "BS Software Engineering", "BS Artificial Intillegence"};
void ShowProgram()
{
    cout << setfill('-') << setw(150) << "-" << endl;
    int counter = 0;
    for (auto el : ProgramList)
    {
        cout << counter++ << "- " << el << "\t";
    }
    cout << endl;
    cout << setfill('-') << setw(150) << "-" << endl;
}

//--------------------Department And Program Section----------------
struct Program
{
    int ProgramId;
    int DepartmentId;
    string ProgramName;
    int Seats;
};

struct DepartmentData
{
    int DepartmentId;
    string DepartmentName;
    vector<Program> ProgramList;
};

class DepartmentManager
{
public:
    unordered_map<int, string> mapDeptlist;
    unordered_map<int, string> mapPrgmlist;

    void LoadDepartments(vector<DepartmentData> &dptList)
    {
        ifstream file("Database/Departments.csv");
        if (!file.is_open())
        {
            cout << "Deprtments File Not Found:\n";
            return;
        }

        string line;
        string temp;
        getline(file, line);
        while (getline(file, line))
        {
            DepartmentData dpt;
            stringstream ss(line);
            getline(ss, temp, ',');
            dpt.DepartmentId = stoi(temp);
            getline(ss, dpt.DepartmentName);
            dptList.push_back(dpt);
        }
        file.close();
    }

    void LoadPrograms(vector<DepartmentData> &dptList)
    {
        ifstream file("Database/Programs.csv");
        if (!file.is_open())
        {
            cout << "Programs File Not Found:\n";
            return;
        }

        string line;
        getline(file, line);
        string temp;
        while (getline(file, line))
        {
            Program prgm;
            stringstream ss(line);
            getline(ss, temp, ',');
            prgm.ProgramId = stoi(temp);
            getline(ss, temp, ',');
            prgm.DepartmentId = stoi(temp);
            getline(ss, prgm.ProgramName, ',');
            getline(ss, temp);
            prgm.Seats = stoi(temp);

            for (auto &dept : dptList)
            {
                if (dept.DepartmentId == prgm.DepartmentId)
                {
                    dept.ProgramList.push_back(prgm);
                    break;
                }
            }
        }
        file.close();
    }

    void ShowDepartAndPrograms(vector<DepartmentData> &dptList)
    {
        for (auto &dpt : dptList)
        {
            cout << "------------------------------------------------------------------------------------------\n";
            cout << "               " << dpt.DepartmentId << "- [" << dpt.DepartmentName << "]\n";
            cout << "------------------------------------------------------------------------------------------\n";
            cout << left << setw(8) << "P. ID" << setw(40) << "Program Name" << endl;
            for (auto &prgm : dpt.ProgramList)
            {
                cout << left << setw(8) << prgm.ProgramId << setw(40) << prgm.ProgramName << endl;
            }
        }
    }

    void ShowDepartment(vector<DepartmentData> &dptList)
    {

        cout << "------------------------ [Departments] ---------------------------\n";
        cout << left << setw(8) << "D.ID" << setw(20) << "Department Name" << endl;
        for (auto &dpt : dptList)
        {
            cout << left << setw(8) << dpt.DepartmentId << setw(20) << dpt.DepartmentName << endl;
        }
        cout << "------------------------------------------------------------------\n";
    }

    string SelectDepartment(vector<DepartmentData> &dptList, string message)
    {

        string Department;
        int DeptCode;
        for (auto &dpt : dptList)
        {
            mapDeptlist.insert({dpt.DepartmentId, dpt.DepartmentName});
        }
        ShowDepartment(dptList);
        DeptCode = SafeInt(message);

        bool found = false;
        auto it = mapDeptlist.find(DeptCode);
        if (it != mapDeptlist.end())
        {
            found = true;
            Department = it->second;
        }
        if (!found)
        {
            cout << "Invalid Option for Department:\n";
        }
        return Department;
    }

    void ShowPrograms(vector<DepartmentData> &deptList, string department)
    {
        for (auto &dept : deptList)
        {
            if (dept.DepartmentName == department)
            {
                cout << "-------------------------" << dept.DepartmentName << "-----------------------\n";
                cout << left << setw(8) << "P. ID" << setw(20) << "Program Name" << endl;
                for (auto &prgm : dept.ProgramList)
                {
                    cout << left << setw(8) << prgm.ProgramId << setw(20) << prgm.ProgramName << endl;
                }
                break;
            }
        }
    }

    string SelectProgram(vector<DepartmentData> &deptList, string department, string message)
    {
        int PrgmCode;
        string Program;
        for (auto &dept : deptList)
        {
            if (dept.DepartmentName == department)
            {
                for (auto &prgm : dept.ProgramList)
                {
                    mapPrgmlist.insert({prgm.ProgramId, prgm.ProgramName});
                }
                break;
            }
        }

        ShowPrograms(deptList, department);
        PrgmCode = SafeInt(message);
        bool found = false;
        auto it = mapPrgmlist.find(PrgmCode);
        if (it != mapPrgmlist.end())
        {
            Program = it->second;
            found = true;
        }

        if (!found)
        {
            cout << "Invalid Program Code:\n";
        }
        return Program;
    }
};

//---------------------------------------------------------------

//--------------------Valid Gmail and Password Input-------------
bool IsGmailValid(string Gmail)
{
    string Email = "@gmail.com";
    if (Gmail.length() <= Email.length())
        return false;
    return Gmail.substr(Gmail.length() - Email.length()) == Email;
}

string SafeGmail(string Message)
{
    string Gmail;
    while (true)
    {
        Gmail = SafeString(Message);
        bool Space = false;
        for (char ch : Gmail)
        {
            if (isspace(ch))
            {
                Space = true;
            }
        }
        if (IsGmailValid(Gmail) && !Space)
            return Gmail;
        cout << "Invalid Gmail. Please Try Again:\n";
        cin.clear();
    }
    return Gmail;
}

bool IsStrongPassword(const string &Password)
{
    bool Upper;
    bool Lower;
    bool Digit;
    bool Special;
    if (Password.length() <= 8)
        return false;
    for (char ch : Password)
    {
        if (isupper(ch))
            Upper = true;
        else if (islower(ch))
            Lower = true;
        else if (isdigit(ch))
            Digit = true;
        else
            Special = true;
    }
    return Upper && Lower && Digit && Special;
}

string SafePassword(string message)
{
    string Password;
    while (true)
    {
        Password = SafeString(message);
        if (IsStrongPassword(Password))
            return Password;
        cout << "------------Alert! Invalid Password---------------\n";
        cout << "Create Strong Password Which Include:\n 1-At least 8 Charcater\n2- Uppercase letter\n3- LowerCase letter\n4- Digit\n5- special Charcter(@,#,%,&)]\n";
        cout << "--------------------------------------------------\n";
        cin.clear();
    }
    return Password;
}
//-----------------------------------------------------------------------


//------------------------------User Data Section------------------------
class UserData
{
public:
    struct Name
    {
        string FirstName;
        string LastName;
    };
    Name name;

    string FullName;
    string FatherHusbandName;
    string CNIC;
    string Gmail;
    string Password;
    Date DoB;
    int Age;

    struct Adress
    {
        string city;
        string Division;
        string State;
        string Country;
    };
    Adress adress;

    string FullAdress;
    string Religion;
    string Gender;

    struct ContactInfo
    {
        string MobileNo;
        string PhoneNo;
        string EmergencyContactNo;
    };
    ContactInfo contactinfo;

    Role role;
    string Rolestr;

    struct DepartmentPreference
    {
        string Choice1;
        string Choice2;
        string Choice3;
    };
    DepartmentPreference deptPreference;

    struct ProgramPreference
    {
        string Choice1;
        string Choice2;
        string Choice3;
    };
    ProgramPreference programpreference;

    struct AcademicDetail
    {
        string degree;
        string program;
        string PassingYear;
        string Institute;
        string Board;
        int TotalMarks;
        int ObtMarks;
        float Percentage;
        float CGPA;
    };
    AcademicDetail academicdetail;

    vector<AcademicDetail> AcademicDetailsVector;
};


class UserManager
{
public:
    void CreateAccount1(vector<UserData> &users, const string &gmail)
    {
        UserData user;
        string CNIC = SafeString("CNIC witout Dashes: ");
        CNIC.insert(5, 1, '-');
        CNIC.insert(13, 1, '-');
        user.CNIC = CNIC;
        cout << "-----------------------Full Name--------------------\n";
        UserData::Name name = {SafeString("First Name: "), SafeString("Last Name: ")};
        user.name = name;
        user.FullName = name.FirstName + " " + name.LastName;
        user.FatherHusbandName = SafeString("Enter Father/Husband Name: ");
        user.Gmail = gmail;
        user.Password = SafePassword("Enter Password: ");
        cout << "------------------Date of Birth--------------------\n";
        Date DoB = {SafeInt("Day: "), SafeInt("Month: "), SafeInt("Year: ")};
        user.DoB = DoB;
        user.Age = AgeCalculate(DoB, Today);
        user.academicdetail.TotalMarks = 0;
        user.academicdetail.ObtMarks = 0;
        user.academicdetail.CGPA = 0;
        user.academicdetail.Percentage;
        users.push_back(user);
        SaveUsers(users, FileSystem::UsersFile);
    }

    void UpdateData(vector<UserData> &users, vector<DepartmentData> &deptList, const string &gmail, const string &password, Role role)
    {
        for (auto &user : users)
        {
            if (user.Gmail == gmail && user.Password == password)
            {
                if (user.Gender != "" || user.Religion != "")
                {
                    cout << "Your Admission Application Submitted Successfully:\n";
                    return;
                }
                DepartmentManager dept;
                user.Gmail = gmail;
                user.Password = password;
                cout << "-----------------------Adress-----------------------\n";
                UserData::Adress adress = {SafeString("city: "), SafeString("Division: "), SafeString("State/Province: "), SafeString("Country: ")};
                user.adress = adress;
                user.FullAdress = adress.city + " " + adress.Division + " " + adress.State + " " + adress.Country;
                user.Religion = SafeString("Enter Religion Name: ");
                user.Gender = SafeString("Enter Male/Female: ");
                cout << "-------------------Contact Info----------------------\n";
                string ContactInitial = "No: +92";
                UserData::ContactInfo contactinfo = {ContactInitial + SafeString("Enter Mobile No: ").erase(0, 1), ContactInitial + SafeString("Enter Phone No: ").erase(0, 1), ContactInitial + SafeString("Enter Emergency Contact No: ").erase(0, 1)};
                user.contactinfo = contactinfo;
                string Rolestr;
                if (role == Role::StudentRole)
                {
                    user.Rolestr = "Student";
                    cout << "Program Preference 1:\n";
                    user.deptPreference.Choice1 = dept.SelectDepartment(deptList, "Enter Depertment ID: ");
                    user.programpreference.Choice1 = dept.SelectProgram(deptList, user.deptPreference.Choice1, "Enter Program ID: ");
                    cout << "Program Preference 2:\n";
                    user.deptPreference.Choice2 = dept.SelectDepartment(deptList, "Enter Depertment ID: ");
                    user.programpreference.Choice2 = dept.SelectProgram(deptList, user.deptPreference.Choice2, "Enter Program ID: ");
                    cout << "Program Preference 3:\n";
                    user.deptPreference.Choice3 = dept.SelectDepartment(deptList, "Enter Depertment ID: ");
                    user.programpreference.Choice3 = dept.SelectProgram(deptList, user.deptPreference.Choice3, "Enter Program ID: ");
                    AcademicDetail(user);
                }
                else if (role == Role::TeacherRole)
                {
                    user.Rolestr = "Teacher";
                    AcademicDetail(user);
                    user.programpreference = {"", "", ""};
                }
                else if (role == Role::EmployeRole)
                {
                    user.Rolestr = "Employe";
                    AcademicDetail(user);
                    user.programpreference = {"", "", ""};
                }
                SaveUsers(users, FileSystem::UsersFile);
                SaveUsers(users, FileSystem::UserAcademicFile);
                cout << "Your Amission Application is submitted Successfully:\n";
            }
        }
    }

    int SafeObtMarks(string message, int TotalMarks)
    {
        int ObtMarks;
        while (true)
        {
            ObtMarks = SafeInt(message);
            if (ObtMarks < TotalMarks)
            {
                return ObtMarks;
            }
            cout << "Obtain Marks Must Be Less than Total Marks [" << ObtMarks << "].\n";
            cin.clear();
        }
        return ObtMarks;
    }

    float SafeCGPA(string message)
    {
        float CGPA;
        while (true)
        {
            CGPA = SafeFloat(message);
            if (CGPA <= 4.0)
            {
                return CGPA;
            }
            cout << "CGPA Must Be Less Than [4.0].\n";
            cin.clear();
        }
        return CGPA;
    }

    void AcademicDetail(UserData &user)
    {
        UserData::AcademicDetail AcademicDetail;
        float percentage;
        enum degree
        {
            None,
            HSC,
            HSSC,
            BS,
            Mphil,
            Phd
        };

        int option;
        do
        {
            cout << "------------------Academic Detail--------------------\n";
            cout << "1- Add HSC Degree Detail\n2- Add HSSC Degree Detail\n3- Add BS Degree Detail\n4- Add MS/Mphil Degree Detail\n5- Add Phd Degree Detail\n0- Degree Details Already Added[Exit]:\n";
            cout << "-----------------------------------------------------\n";
            option = SafeInt("Choose option: ");
            switch (option)
            {
            case degree::HSC:
                AcademicDetail = {"HSC", SafeString("Program Name: "), SafeString("Passing Year: "), SafeString("College/University: "), SafeString("Board: "), SafeInt("Total Marks: "), 0, 0.0, 0.0};
                AcademicDetail.ObtMarks = SafeObtMarks("Obtain Marks: ", AcademicDetail.TotalMarks);
                AcademicDetail.Percentage = (float)AcademicDetail.ObtMarks / AcademicDetail.TotalMarks * 100;
                break;
            case degree::HSSC:
                AcademicDetail = {"HSSC", SafeString("Program Name: "), SafeString("Passing Year: "), SafeString("College/University: "), SafeString("Board: "), SafeInt("Total Marks: "), 0, 0.0, 0.0};
                AcademicDetail.ObtMarks = SafeObtMarks("Obtain Marks: ", AcademicDetail.TotalMarks);
                AcademicDetail.Percentage = (float)AcademicDetail.ObtMarks / AcademicDetail.TotalMarks * 100;
                break;
            case degree::BS:
                AcademicDetail = {"BS", SafeString("Program Name: "), SafeString("Graduate Year: "), SafeString("College/University: "), "", 0, 0, 0.0, SafeCGPA("CGPA: ")};
                break;
            case degree::Mphil:
                AcademicDetail = {"Mphil", SafeString("Program Name: "), SafeString("Graduate Year: "), SafeString("College/University: "), "", 0, 0, 0.0, SafeCGPA("CGPA: ")};
                break;
            case degree::Phd:
                AcademicDetail = {"Phd", SafeString("Program Name: "), SafeString("Graduate Year: "), SafeString("College/University: "), "", 0, 0, 0.0, SafeCGPA("CGPA: ")};
                break;
            case 0:
                return;
            }
            user.academicdetail = AcademicDetail;
            user.AcademicDetailsVector.push_back(AcademicDetail);
        } while (option != 0);
    }

    int AgeCalculate(const Date &D_O_Birth, const Date &Today)
    {
        int age = Today.Year - D_O_Birth.Year;
        if (Today.Month < D_O_Birth.Month || Today.Month == D_O_Birth.Month && Today.Day < D_O_Birth.Day)
            age--;
        return age;
    }

    void TableFormat(Role role)
    {
        if (role == Role::StudentRole)
        {
            cout << left << setw(5) << "No#" << setw(20) << "Name" << setw(20) << "Father Name:" << setw(5) << "Age" << setw(8) << "Gender" << setw(30) << "Program Choice 1" << setw(30) << "Program Choice 2" << setw(30) << "Program Choice 3" << endl;
            cout << setfill('-') << setw(150) << "-" << endl;
        }
        else if (role == Role::TeacherRole || role == Role::EmployeRole)
        {
            cout << left << setw(5) << "No#" << setw(20) << "Name" << setw(20) << "Father Name:" << setw(5) << "Age" << setw(8) << "Gender" << endl;
            cout << setfill('-') << setw(60) << "-" << endl;
        }
        cout << setfill(' ');
    }

    void ShowData(vector<UserData> &Users, Role role)
    {
        int counter = 1;
        if (role == Role::StudentRole)
            cout << "-----------------Student Applicants-----------------\n";
        else if (role == Role::TeacherRole)
            cout << "-----------------Teacher Applicants-----------------\n";
        else if (role == Role::EmployeRole)
            cout << "-----------------Employe Applicants-----------------\n";

        TableFormat(role);
        if (Users.empty())
        {
            cout << "No Data Yet:\n";
            return;
        }

        for (auto &el : Users)
        {
            if (el.Rolestr == "Student" && role == Role::StudentRole)
            {
                cout << left << setw(5) << counter++ << setw(20) << el.FullName << setw(20) << el.FatherHusbandName << setw(5) << el.Age << setw(8) << el.Gender << setw(30) << el.programpreference.Choice1 << setw(30) << el.programpreference.Choice2 << setw(30) << el.programpreference.Choice3 << endl;
            }
            else if (el.Rolestr == "Teacher" && role == Role::TeacherRole)
            {
                cout << left << setw(5) << counter++ << setw(20) << el.FullName << setw(20) << el.FatherHusbandName << setw(5) << el.Age << setw(8) << el.Gender << endl;
            }
            else if (el.Rolestr == "Employe" && role == Role::EmployeRole)
            {
                cout << left << setw(5) << counter++ << setw(20) << el.FullName << setw(20) << el.FatherHusbandName << setw(5) << el.Age << setw(8) << el.Gender << endl;
            }
        }
    }

    void SaveUsers(const vector<UserData> &users, FileSystem filesystem)
    {
        if (filesystem == FileSystem::UsersFile)
        {
            ofstream file("database/user.csv");
            file << "F.Name,L.Name,FullName,Fathername,CNIC,Gmail,Password,DOB[Day],DoB[Month],DoB[Year],Age,City,Division,State,Country,FullADress,Religion,Gender,Mobile No,Phone No,Emergency No,Role,Department Choice1,Program Choice1,Department Choice2,Program Choice2,Department Choice3,Program Choice3\n";
            for (const auto &user : users)
            {
                file << user.name.FirstName << "," << user.name.LastName << "," << user.FullName << "," << user.FatherHusbandName << "," << user.CNIC << "," << user.Gmail << "," << user.Password << "," << user.DoB.Day << "," << user.DoB.Month << "," << user.DoB.Year << "," << user.Age << "," << user.adress.city << "," << user.adress.Division << "," << user.adress.State << "," << user.adress.Country << "," << user.FullAdress << "," << user.Religion << "," << user.Gender << "," << user.contactinfo.MobileNo << "," << user.contactinfo.PhoneNo << "," << user.contactinfo.EmergencyContactNo << "," << user.Rolestr << "," << user.deptPreference.Choice1 << "," << user.programpreference.Choice1 << "," << user.deptPreference.Choice2 << "," << user.programpreference.Choice2 << "," << user.deptPreference.Choice3 << "," << user.programpreference.Choice3 << endl;
            }
            file.close();
        }
        else if (filesystem == FileSystem::UserAcademicFile)
        {
            ofstream file("Database/UserAcademic.csv");
            file << "CNIC,Degree Name,Program Name,Paasing Year,Institute,Board,Total Marks,Obt.Marks,percentage,CGPA\n";
            for (const auto &user : users)
            {
                for (const auto &academic : user.AcademicDetailsVector)
                {
                    file << user.CNIC << "," << academic.degree << "," << academic.program << "," << academic.PassingYear << "," << academic.Institute << "," << academic.Board << "," << academic.TotalMarks << "," << academic.ObtMarks << "," << academic.Percentage << "," << academic.CGPA << endl;
                }
            }
            file.close();
        }
    }

    void LoadUsers(vector<UserData> &users, FileSystem filesystem)
    {
        if (filesystem == FileSystem::UsersFile)
        {
            ifstream file("Database/user.csv");
            if (!file.is_open())
            {
                cout << "File not Found:\n";
                return;
            }
            string line;
            getline(file, line);
            while (getline(file, line))
            {
                stringstream ss(line);
                UserData user;
                string temp;
                getline(ss, user.name.FirstName, ',');
                getline(ss, user.name.LastName, ',');
                getline(ss, user.FullName, ',');
                getline(ss, user.FatherHusbandName, ',');
                getline(ss, user.CNIC, ',');
                getline(ss, user.Gmail, ',');
                getline(ss, user.Password, ',');
                getline(ss, temp, ',');
                user.DoB.Day = stoi(temp);
                getline(ss, temp, ',');
                user.DoB.Month = stoi(temp);
                getline(ss, temp, ',');
                user.DoB.Year = stoi(temp);
                getline(ss, temp, ',');
                try
                {
                    user.Age = stoi(temp);
                }
                catch (...)
                {
                    user.Age = 0;
                }
                getline(ss, user.adress.city, ',');
                getline(ss, user.adress.Division, ',');
                getline(ss, user.adress.State, ',');
                getline(ss, user.adress.Country, ',');
                getline(ss, user.FullAdress, ',');
                getline(ss, user.Religion, ',');
                getline(ss, user.Gender, ',');
                getline(ss, user.contactinfo.MobileNo, ',');
                getline(ss, user.contactinfo.PhoneNo, ',');
                getline(ss, user.contactinfo.EmergencyContactNo, ',');
                getline(ss, user.Rolestr, ',');
                if (user.Rolestr == "Student")
                {
                    getline(ss, user.deptPreference.Choice1, ',');
                    getline(ss, user.programpreference.Choice1, ',');
                    getline(ss, user.deptPreference.Choice2, ',');
                    getline(ss, user.programpreference.Choice2, ',');
                    getline(ss, user.deptPreference.Choice3, ',');
                    getline(ss, user.programpreference.Choice3);
                }
                users.push_back(user);
            }
            file.close();
        }
        else if (filesystem == FileSystem::UserAcademicFile)
        {
            ifstream file("Database/UserAcademic.csv");
            if (!file.is_open())
            {
                cout << "File Not Found:\n";
                return;
            }
            string line;
            getline(file, line);
            while (getline(file, line))
            {
                UserData::AcademicDetail academic;
                string temp;
                string cnic;
                stringstream ss(line);
                getline(ss, cnic, ',');
                getline(ss, academic.degree, ',');
                getline(ss, academic.program, ',');
                getline(ss, academic.PassingYear, ',');
                getline(ss, academic.Institute, ',');
                getline(ss, academic.Board, ',');
                getline(ss, temp, ',');
                academic.TotalMarks = stoi(temp);
                getline(ss, temp, ',');
                academic.ObtMarks = stoi(temp);
                getline(ss, temp, ',');
                academic.Percentage = stof(temp);
                getline(ss, temp);
                academic.CGPA = stof(temp);
                for (auto &el : users)
                {
                    if (el.CNIC == cnic)
                    {
                        el.AcademicDetailsVector.push_back(academic);
                        el.academicdetail = academic;
                        break;
                    }
                }
            }
            file.close();
        }
    }
};
//----------------------------------------------------------------------------------------

//------------------------------------Merit Data Section------------------------------------
struct StudentMeritData
{
    UserData user;
    float merit;
};

class StudentMeritManager
{
public:
    float CalculateMerit(vector<StudentMeritData> &MeritList, const UserData &user, Role role)
    {
        bool found = false;
        float merit = 0;
        for (const auto &record : user.AcademicDetailsVector)
        {
            if (role == Role::StudentRole)
            {
                if (record.degree == "HSSC")
                {
                    found = true;
                    merit = record.Percentage;
                }
            }
        }
        if (!found)
        {
            return merit = 0;
        }
        return merit;
    }

    void AddStudentToMeritList(vector<StudentMeritData> &MeritList, const vector<UserData> &users, Role role, string program)
    {
        for (const auto &user : users)
        {
            if (role == Role::StudentRole && user.Rolestr == "Student" && user.programpreference.Choice1 == program || user.programpreference.Choice2 == program || user.programpreference.Choice3 == program)
            {
                StudentMeritData m;
                m.user = user;
                m.merit = CalculateMerit(MeritList, user, role);
                MeritList.push_back(m);
            }
        }
        SortMeritList(MeritList);
    }

    void SortMeritList(vector<StudentMeritData> &MeritList)
    {
        sort(MeritList.begin(), MeritList.end(), [](const StudentMeritData &a, const StudentMeritData &b)
             { return a.merit > b.merit; });
    }

    void ShowMeritList(vector<StudentMeritData> &MeritList)
    {
        if (MeritList.empty())
        {
            cout << "No Aplicants Yet:\n";
            return;
        }

        cout << left << setw(5) << "No#" << setw(20) << "Name" << setw(20) << "Father Name:" << setw(5) << "Age" << setw(8) << "Gender" << setw(10) << "Merit percentage" << endl;
        cout << setfill('-') << setw(80) << "-" << endl;
        cout << setfill(' ');

        int counter = 1;
        for (auto &el : MeritList)
        {
            cout << left << setw(5) << counter++ << setw(20) << el.user.FullName << setw(20) << el.user.FatherHusbandName << setw(5) << el.user.Age << setw(8) << el.user.Gender << setw(10) << el.merit << endl;
        }
    }
};

//------------------------------------------------------------------------


//----------------------------Student manager Section------------------------------
class StudentData : public UserData
{
public:
    UserData user;
    int RollNo;
    float CGPA;
    string program;

    struct CurrentStudying
    {
        int semester_counter;
        string semester;
        string session;
        float GPA;
        struct Subject
        {
            string SubjectName;
            string SubjectTeacher;
            const int TotalMark = 100;
            int ObtMarks;
            float CreditHour;
            float ObtGradePoint;
        };
        vector<Subject> Subject;
    };
    vector<CurrentStudying> CurrentStudyingDetail;
};

class StudentManager
{
public:
    void AdmitStudent(vector<StudentData> &students, vector<StudentMeritData> &MeritList)
    {
        StudentData student;
        int counter = 100;
        student.RollNo = ++counter;
        student.FullName = MeritList[0].user.FullName;
        student.FatherHusbandName = MeritList[0].user.FatherHusbandName;
        student.Age = MeritList[0].user.Age;
        student.Gender = MeritList[0].user.Gender;
        student.CNIC = MeritList[0].user.CNIC;
        student.program = MeritList[0].user.programpreference.Choice1;
        students.push_back(student);
    }

    void TableFormat()
    {
        cout << left << setw(5) << "No#" << setw(20) << "CNIC" << setw(20) << "Name" << setw(20) << "Father Name:" << setw(5) << "Age" << setw(8) << "Gender" << setw(30) << "Program Name" << endl;
        cout << setfill('-') << setw(150) << "-" << endl;
        cout << setfill(' ');
    }

    void ShowStudents(vector<StudentData> &students, string program)
    {
        TableFormat();
        int counter = 1;
        for (const auto &student : students)
        {
            if (student.program == program)
            {
                cout << left << setw(5) << counter++ << setw(20) << student.CNIC << setw(20) << student.FullName << setw(20) << student.FatherHusbandName << setw(5) << student.Age << setw(8) << student.Gender << setw(30) << student.program << endl;
            }
        }
    }

    void LoadStudent(vector<StudentData> &students, FileSystem filename)
    {
        ifstream file("Database/Students.csv");
        string line;
    }

    void SaveStudent(vector<StudentData> &student, FileSystem filename)
    {
        ofstream file("Database/Students.csv");
    }
};
//--------------------------------------------------------------------------


//--------------------------------Teacher Section-----------------------
// class TeacherData : public UserData
// {
// public:
//     int TeacherID;
// };

//---------------------------------------------------------------------


//------------------------- Employe Section------------------------------
// class EmployeData : public UserData
// {
// public:
//     int EmployeID;
// };

//----------------------------------------------------------------------



//--------------------------------Authentication------------------------------
class Authentication
{
public:
    unordered_map<string, string> RegisterUsers;
    unordered_map<string, string> RegisterStudents;

    void AddRegisterDataToMap(vector<UserData> &users, vector<StudentData> &students, Role role)
    {
        if (role == Role::User)
        {

            for (auto &user : users)
            {
                RegisterUsers.insert({user.Gmail, user.Password});
            }
        }
        else if (role == Role::StudentRole)
        {
            for (auto &student : students)
            {
                RegisterStudents.insert({student.Gmail, student.Password});
            }
        }
    }

    void ShowUsersByMap(vector<UserData> &users, vector<StudentData> &students, Role role)
    {
        AddRegisterDataToMap(users, students, role);
        int count = 1;
        cout << "----------------Total Register Users in System----------------\n";
        cout << left << setw(5) << "No#" << setw(30) << "User Gmail" << setw(20) << "User Password" << endl;
        if (role == Role::User)
        {
            for (auto &user : RegisterUsers)
            {
                cout << left << setw(5) << count++ << setw(30) << user.first << setw(20) << user.second << endl;
            }
        }
        else if (role == Role::StudentRole)
        {
            for (auto &student : RegisterStudents)
            {
                cout << left << setw(5) << count++ << setw(30) << student.first << setw(20) << student.second << endl;
            }
        }
    }

    bool IsUserExists(string Gmail, vector<UserData> &users, vector<StudentData> &students, Role role)
    {
        AddRegisterDataToMap(users, students, role);
        auto it = RegisterUsers.find(Gmail);
        if (it != RegisterUsers.end())
        {
            return true;
        }
        return false;
    }

    bool IsStudentExists(string Gmail, vector<UserData> &users, vector<StudentData> &students, Role role)
    {
        AddRegisterDataToMap(users, students, role);
        auto it = RegisterStudents.find(Gmail);
        if (it != RegisterStudents.end())
        {
            return true;
        }
        return false;
    }

    bool Login(vector<UserData> &users, vector<StudentData> &students, string gmail, string password, Role role)
    {
        AddRegisterDataToMap(users, students, role);
        if (role == Role::User)
        {
            if (IsUserExists(gmail, users, students, role))
            {
                auto it = RegisterUsers.find(gmail);
                if (it != RegisterUsers.end())
                {
                    if (it->second == password)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            else
            {
                cout << "User Does not Exits:\n";
                return false;
            }
        }
        else if (role == Role::StudentRole)
        {
            if (IsStudentExists(gmail, users, students, role))
            {
                cout << "Student Exists\n";
                return true;
            }
            else
            {
                cout << "Student Does not Exists:\n";
                return false;
            }
        }
        return true;
    }
};

//-----------------------------------------------------------------------------

//-------------------------------DataBase section-------------------------------
class DataBase
{
public:
    //----------------USER------------------
    UserManager userManager;
    vector<UserData> Users;
    void loadUser()
    {
        userManager.LoadUsers(Users, FileSystem::UsersFile);
        userManager.LoadUsers(Users, FileSystem::UserAcademicFile);
    }
    void SaveUser()
    {
        userManager.SaveUsers(Users, FileSystem::UsersFile);
        userManager.SaveUsers(Users, FileSystem::UserAcademicFile);
    }
    //--------------------------------------

    //---------------Merit List-------------
    vector<StudentMeritData> MeritList;
    StudentMeritManager studentMeritManager;
    //--------------------------------------

    //--------------Department--------------
    vector<DepartmentData> departmentList;
    DepartmentManager departmentManager;
    void loadDepartment()
    {
        departmentManager.LoadDepartments(departmentList);
        departmentManager.LoadPrograms(departmentList);
    }
    //--------------------------------------

    //--------------Authentication----------
    Authentication auth;
    //--------------------------------------

    //----------------Student---------------
    vector<StudentData> students;
    StudentManager studentManager;
    void LoadStudent()
    {
        studentManager.LoadStudent(students, FileSystem::StudentFile);
    }

    void SaveStudent()
    {
        studentManager.SaveStudent(students, FileSystem::StudentFile);
    }
    //--------------------------------------

    // vector<TeacherData> teachers;
    // vector<EmployeData> employes;
};
//------------------------------------------------------------------------------


//----------------------------------Admin manage Section--------------------------------
void AdminSection(DataBase &db)
{
    int option;
    int SubOption;
    string department, program;
    do
    {
        cout << "----------------Admin Section-----------------\n";
        cout << "1- Applicants Section\n2- Merit List\n3- Student Section\n4- Teacher Section\n5- Employe Section\n0- Back\n";
        cout << "----------------------------------------------\n";
        option = SafeInt("Choose OPtion: ");
        switch (option)
        {
        case 1:
            do
            {
                cout << "----------------Applicants Section--------------\n";
                cout << "1- Student Applicants Section\n2- Teacher Applicants Section\n3- Employe Applicants Section\n4- Total Applicants in System\n0- Back\n";
                cout << "----------------Applicants Section--------------\n";
                SubOption = SafeInt("Choose Option: ");
                switch (SubOption)
                {
                case 1:
                    db.userManager.ShowData(db.Users, Role::StudentRole);
                    break;
                case 2:
                    db.userManager.ShowData(db.Users, Role::TeacherRole);
                    break;
                case 3:
                    db.userManager.ShowData(db.Users, Role::EmployeRole);
                    break;
                case 4:
                    db.auth.ShowUsersByMap(db.Users, db.students, Role::User);
                    break;
                case 0:
                    break;
                default:
                    cout << "Invalid Option:\n";
                }
            } while (SubOption != 0);
            break;
        case 2:
            cout << "------------------------Merit List------------------------\n";
            department = db.departmentManager.SelectDepartment(db.departmentList, "Select department: ");
            program = db.departmentManager.SelectProgram(db.departmentList, department, "Select Program: ");
            db.MeritList.clear();
            db.studentMeritManager.AddStudentToMeritList(db.MeritList, db.Users, Role::StudentRole, program);
            db.studentMeritManager.ShowMeritList(db.MeritList);
            break;
        case 3:
            do
            {
                cout << "---------------------Student Section--------------------\n";
                cout << "1- Admit Students\n2- Show Students\n3- Search Student\n0- Back\n";
                cout << "---------------------Student Section--------------------\n";
                SubOption = SafeInt("Choose Option: ");
                switch (SubOption)
                {
                case 1:
                    db.studentManager.AdmitStudent(db.students, db.MeritList);
                    break;
                case 2:
                    department = db.departmentManager.SelectDepartment(db.departmentList, "Select Department: ");
                    program = db.departmentManager.SelectDepartment(db.departmentList, "Select Program: ");
                    db.studentManager.ShowStudents(db.students, program);
                    break;
                case 3:
                    break;
                case 0:
                    return;
                default:
                    cout << "Invalid Option:\n";
                }
            } while (SubOption != 0);
            break;
        case 4:
            break;
        case 0:
            break;
        default:
            cout << "Invalid Option:\n";
        }
    } while (option != 0);
}
//----------------------------------------------------------------------

//--------------------------Student manage Function------------------------------
void StudentSection(DataBase &db)
{
    int option;
    do
    {
        cout << "-----------------Student Section-------------------\n";
        cout << "1- My account\n0- Exit\n";
        cout << "---------------------------------------------------\n";
    } while (option != 0);
}
//---------------------------------------------------------------------


//----------------------------Teacher Manage Function-------------------------
// void TeacherSection()
// {
//     cout << "Teacher\n";
// }

//-------------------------------------------------------------------------



//---------------------------Employe Manage Function----------------------------
// void EmployeSection()
// {
//     cout << "Employe\n";
// }

//-----------------------------Apply to Admission Section-------------------------
void ApplyForAdmissionSection(DataBase &db, const string &gmail, const string &password)
{
    int SubOption;
    do
    {
        cout << "----------------Admission Section--------------\n";
        cout << "1- Apply For Studying\n2- Apply For Teaching\n3- Apply for Working\n0- Home Page\n";
        cout << "-----------------------------------------------\n";
        SubOption = SafeInt("Choose option: ");
        switch (SubOption)
        {
        case 1:
            db.userManager.UpdateData(db.Users, db.departmentList, gmail, password, Role::StudentRole);
            return;
        case 2:
            db.userManager.UpdateData(db.Users, db.departmentList, gmail, password, Role::TeacherRole);
            return;
        case 3:
            db.userManager.UpdateData(db.Users, db.departmentList, gmail, password, Role::EmployeRole);
            return;
        case 0:
            return;
        default:
            cout << "Invalid Option\n";
        }
    } while (SubOption != 0);
}
//-----------------------------------------------------------------------------------

//------------------------------Start Program section--------------------------------
void StartProgram(DataBase &db)
{
    int option;
    int SubOption;
    string Gmail, Password = "";
    do
    {
        cout << "--------------University Of Cham----------------\n";
        cout << "1- Create Account\n2- Admin Login\n3- Student Login\n4- Teacher Login\n5- Employe Login\n6- Login For Apply To Admission Application\n0- Exit\n";
        cout << "------------------------------------------------\n";
        option = SafeInt("Choose Option: ");
        switch (option)
        {
        case 1:
            Gmail = SafeGmail("Enter Gmail: ");
            if (db.auth.IsUserExists(Gmail, db.Users, db.students, Role::User) || db.auth.IsStudentExists(Gmail, db.Users, db.students, Role::User))
            {
                cout << "User Already Exists\n";
                break;
            }
            else
            {
                cout << "-------------Create Account----------------\n";
                db.userManager.CreateAccount1(db.Users, Gmail);
                cout << "Account Created Successfully:\n";
            }
            break;
        case 6:
            cout << "-------------------Login Account---------------\n";
            Gmail = SafeGmail("Enter Gmail: ");
            if (db.auth.IsUserExists(Gmail, db.Users, db.students, Role::User) || db.auth.IsStudentExists(Gmail, db.Users, db.students, Role::User))
            {
                Password = SafePassword("Enter Password: ");
                if (db.auth.Login(db.Users, db.students, Gmail, Password, Role::User))
                {
                    ApplyForAdmissionSection(db, Gmail, Password);
                }
                else
                {
                    cout << "Incorrect Gmail or Password:\n";
                }
            }
            else
            {
                cout << "User Does Not Exists:\n";
                cout << "Create Account First:\n";
            }
            break;
        case Role::AdminRole:
            AdminSection(db);
            break;
        case Role::StudentRole:
            StudentSection(db);
            break;
        case Role::TeacherRole:
            // TeacherSection();
            break;
        case Role::EmployeRole:
            // EmployeSection();
            break;
        case 0:
            cout << "Program Exited:\n";
            break;
        default:
            cout << "Invalid Option:\n";
        }
    } while (option != 0);
}

//--------------------------------------------------------------------------------

int main()
{
    DataBase db;
    db.loadUser();
    db.loadDepartment();
    db.LoadStudent();
    StartProgram(db);
    db.SaveUser();
    db.SaveStudent();

    return 0;
}