#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string Filename = "database/data.csv";
string completeAppointFile = "database/completAppoint.csv";

class Node
{
public:
    int AppointId;
    string id;
    string name;
    string gmail;

    Node *next;
    Node *prev;

    Node(int a, string i, string n, string g)
    {
        AppointId = a;
        id = i;
        name = n;
        gmail = g;
        next = prev = NULL;
    }
};

class Appointment
{
    Node *front;
    Node *rear;
    static const int size = 10;
    Node *table[size];

public:
    Appointment()
    {
        front = rear = NULL;
    }

    void bookAppoint(int appointId, string id, string name, string gmail)
    {
        Node *newnode = new Node(appointId, id, name, gmail);
        if (front == NULL)
        {
            front = rear = newnode;
            return;
        }
        newnode->next = front;
        front->prev = newnode;
        front = newnode;
    }

    void doneAppoint()
    {
        if (front == NULL)
        {
            cout << "No more Appointment in the list:\n";
            return;
        }

        Node *ptr = rear;
        if (front == rear)
        {
            front = rear = NULL;
        }
        else
        {
            rear = rear->prev;
            rear->next = NULL;
        }

        delete ptr;
    }

    void cancelAppointment(string id)
    {
        if (front == NULL)
        {
            cout << "No Appointment Yet:\n";
            return;
        }

        bool found = false;

        Node *ptr = front;
        while (ptr != NULL)
        {
            if (ptr->id == id)
            {
                found = true;
                Node *del = ptr;
                ptr = ptr->next;

                if (front == rear)
                {
                    front = rear = NULL;
                    return;
                }

                if (del == front)
                {
                    front = del->next;
                    if (front != NULL)
                        front->prev = NULL;
                }
                else if (del == rear)
                {
                    rear = del->prev;
                    if (rear != NULL)
                        rear->next = NULL;
                }
                else
                {
                    del->prev->next = del->next;
                    if (del->next != NULL)
                        del->next->prev = del->prev;
                }
                delete del;
            }
            else
            {
                ptr = ptr->next;
            }
        }

        if (found)
        {
            cout << "Appointment cancelled Successfully:\n";
        }
        else
        {
            cout << "Appointment not Found:\n";
        }
    }

    void displayAppoint()
    {
        Node *ptr = rear;
        int count = 1;
        if (rear == NULL)
        {
            cout << "No Appointment yet:\n";
            return;
        }
        cout << "Appointment List:\n";
        cout << "_______________________________________________________________________________________________________________\n";
        cout << "No#\t\tAppoint Id\t\tID No:\t\tName\t\t\t\tGmail\n";
        while (ptr != NULL)
        {
            cout << "______________________________________________________________________________________________________________\n";
            cout << count++ << "\t\t" << ptr->AppointId << "\t\t\t" << ptr->id << "\t\t" << ptr->name << "\t\t\t" << ptr->gmail << "\n";
            ptr = ptr->prev;
        }
        cout << "_______________________________________________________________________________________________________________\n";
    }

    void searchApoint(string id)
    {
        if (front == NULL)
        {
            cout << "No Appointment Yet:\n";
            return;
        }

        Node *ptr = rear;
        bool found = false;
        int count = 1;
        cout << "Appointment Detail:\n";
        cout << "_____________________________________________________________________________________________________________\n";
        cout << "No#\t\tAppoint ID:\t\tID No:\t\tName\t\t\t\tGmail\n";
        while (ptr != NULL)
        {
            if (ptr->id == id)
            {
                found = true;
                cout << "____________________________________________________________________________________________________________\n";
                cout << count++ << "\t\t" << ptr->AppointId << "\t\t\t" << ptr->id << "\t\t" << ptr->name << "\t\t" << ptr->gmail << "\n";
            }
            ptr = ptr->prev;
        }
        cout << "____________________________________________________________________________________________________________\n";

        if (!found)
        {
            cout << "Appointment Not Found:\n";
        }
    }

    void searchApoint_name(string name)
    {
        if (front == NULL)
        {
            cout << "No Appointment Yet:\n";
            return;
        }

        Node *ptr = rear;
        bool found = false;
        int count = 1;
        cout << "Appointment Detail:\n";
        cout << "_____________________________________________________________________________________________________________\n";
        cout << "No#\t\tAppoint ID:\t\tID No:\t\tName\t\t\t\tGmail\n";
        while (ptr != NULL)
        {
            if (ptr->name == name)
            {
                found = true;
                cout << "____________________________________________________________________________________________________________\n";
                cout << count++ << "\t\t" << ptr->AppointId << "\t\t\t" << ptr->id << "\t\t" << ptr->name << "\t\t" << ptr->gmail << "\n";
            }
            ptr = ptr->prev;
        }
        cout << "____________________________________________________________________________________________________________\n";

        if (!found)
        {
            cout << "Appointment Not Found:\n";
        }
    }

    void saveData(string filename)
    {
        ofstream file(filename);
        Node *ptr = rear;
        while (ptr != NULL)
        {
            stringstream ss;
            ss << ptr->AppointId << "," << ptr->id << "," << ptr->name << "," << ptr->gmail;
            file << ss.str() << endl;
            ptr = ptr->prev;
        }
        file.close();
    }

    void loadData(string filename)
    {
        ifstream file(filename);
        string line;

        if (!file.is_open())
        {
            cout << "File Not Found:\n";
            return;
        }

        while (front != NULL)
        {
            doneAppoint();
        }

        while (getline(file, line))
        {
            if (line.empty())
                continue;

            int AppointId;
            string id, name, gmail;
            stringstream ss(line);

            string temp;
            getline(ss, temp, ',');
            AppointId = stoi(temp);

            getline(ss, id, ',');

            getline(ss, name, ',');

            getline(ss, gmail, ',');

            bookAppoint(AppointId, id, name, gmail);
        }

        file.close();
    }

    void rear_()
    {
        if (front == NULL)
        {
            cout << "No Data:\n";
            return;
        }

        int appointId;
        string id, name, gmail;
        Node *ptr = rear;
        cout << "___________________________________________________________________________\n";
        cout << "appoint ID: " << ptr->AppointId << "\t ID:" << ptr->id << "\t Name: " << ptr->name << "\t Gmail: " << ptr->gmail << endl;
        cout << "___________________________________________________________________________\n";
    }

    void front_()
    {
        if (front == NULL)
        {
            cout << "No Data:\n";
            return;
        }

        int appointId;
        string id, name, gmail;
        Node *ptr = front;
        cout << "___________________________________________________________________________\n";
        cout << "appoint ID: " << ptr->AppointId << "\t ID:" << ptr->id << "\t Name: " << ptr->name << "\t Gmail: " << ptr->gmail << endl;
        cout << "___________________________________________________________________________\n";
    }

    int appoint_id()
    {
        static int counter;
        if (front == NULL)
        {
            counter = 0;
        }
        else
        {
            counter = front->AppointId;
        }
        return counter;
    }

    int comAppoint_id(int apointId)
    {
        apointId = rear->AppointId;
        return apointId;
    }

    string comAppointId(string id)
    {
        id = rear->id;
        return id;
    }

    string comAppointName(string name)
    {
        name = rear->name;
        return name;
    }

    string comApointGmail(string gmail)
    {
        gmail = rear->gmail;
        return gmail;
    }

    bool is_empty()
    {
        if (front == NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
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
        cout << "Invalid! Please Enter number: ";
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

void start_program()
{
    Appointment a, a1;
    a.loadData(Filename);
    a1.loadData(completeAppointFile);
    int option;
    int apointId;
    string id, name, gmail;
    cout << "------------------------------\n";
    cout << "          -Appointment-       \n";
    do
    {
        cout << "------------------------------\n";
        cout << "1- Book Appointment:\n2- Cancell Apointment:\n3- Appointment Completed:\n4- Show Appointment List:\n5- Search Appointment:\n6- List of Completed Appointment:\n7- Search Appontment detail in completed Appointment list:\n0- Exit:\n";
        cout << "------------------------------\n";
        option = safe_int("Choose Option: ");
        switch (option)
        {
        case 1:
            cout << "------------------------------\n";
            cout << "    -Appointment booking-     \n";
            cout << "------------------------------\n";
            if (a1.is_empty())
            {
                apointId = a.appoint_id();
            }
            else if (!a.is_empty())
            {
                apointId = a.appoint_id();
            }
            else if (a.is_empty())
            {
                apointId = a1.appoint_id();
            }
            id = safeString("Enter ID No: ");
            name = safeString("Enter Name: ");
            gmail = safeString("Enter Gmail: ");
            apointId++;
            a.bookAppoint(apointId, id, name, gmail);
            a.saveData(Filename);
            cout << "Appointment Booked Successfully:\n";
            break;
        case 2:
            cout << "------------------------------\n";
            cout << "        -Cancel Apointment-   \n";
            cout << "------------------------------\n";
            id = safeString("Enter ID No: to cancel Apointment: ");
            a.cancelAppointment(id);
            a.saveData(Filename);
            break;
        case 3:
            cout << "------------------------------\n";
            cout << "   -Appointment completed-    \n";
            cout << "------------------------------\n";
            if (a.is_empty())
            {
                cout << "No Appointment Yet:\n";
                break;
            }
            apointId = a.comAppoint_id(apointId);
            id = a.comAppointId(id);
            name = a.comAppointName(name);
            gmail = a.comApointGmail(gmail);
            a1.bookAppoint(apointId, id, name, gmail);
            a1.saveData(completeAppointFile);
            a.doneAppoint();
            a.saveData(Filename);
            cout << "Appointment completed Successfully:\n";
            break;
        case 4:
            cout << "------------------------------\n";
            cout << "       -Appointment List-     \n";
            cout << "------------------------------\n";
            a.displayAppoint();
            break;
        case 5:
            cout << "--------------------------------\n";
            cout << "        -Search Appointment-    \n";
            cout << "--------------------------------\n";
            id = safeString("Enter ID No To search Appointment: ");
            a.searchApoint(id);
            break;
        case 6:
            cout << "--------------------------------\n";
            cout << "  -Completed Appointment List-  \n";
            cout << "--------------------------------\n";
            a1.displayAppoint();
            break;
        case 7:
            cout << "---------------------------------\n";
            cout << "-Search Appointment in Completed-\n";
            cout << "---------------------------------\n";
            name = safeString("Enter Name to Search: ");
            a1.searchApoint_name(name);
            break;
        case 0:
            cout << "------------------------------\n";
            cout << "            -Exit-            \n";
            cout << "------------------------------\n";
            cout << "         Exiting....          \n";
            cout << "         Program End          \n";
            break;
        default:
            cout << "Invalid Option:\n";
        }
    } while (option != 0);
}

int main()
{
    start_program();
}