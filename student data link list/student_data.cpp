#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <sstream>
#include <stack>
using namespace std;

// get safe string
string get_safe_string(string message)
{
    string value;
    while (true)
    {
        cout << message;
        cin >> value;
        if (!cin.fail())
        {
            return value;
        }
        cout << "Invalid! please enter string: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return value;
}

// creating csv file
string file_name1()
{
    string file = get_safe_string("Enter New file or Existing File name: ");
    file = "database/" + file + ".csv";
    return file;
}

// Csv file name
string filename = file_name1();

// stack for undo and redo operation
struct Action
{
    int roll;
    string old_name;
    float old_gpa;
    string new_name;
    float new_gpa;
};

// this is class to represent Node
class Node
{
public:
    int roll_no;
    string name;
    float gpa;
    Node *next;
    Node *prev;
    Node(int roll_no, string name, float gpa)
    {
        this->roll_no = roll_no;
        this->gpa = gpa;
        this->name = name;
        next = NULL;
        prev = NULL;
    }
};

// class to represent link list structure
class link_list
{
    Node *head;
    Node *tail;
    stack<Action> undoStack;
    stack<Action> redoStack;

public:
    link_list()
    {
        head = NULL;
        tail = NULL;
    }

    // class to add data to start
    void add_data_start(int roll_no, string name, float gpa)
    {
        Node *newnode = new Node(roll_no, name, gpa);
        newnode->roll_no = roll_no;
        newnode->gpa = gpa;
        newnode->name = name;
        if (head == NULL)
        {
            head = tail = newnode;
            return;
        }
        newnode->next = head;
        head->prev = newnode;
        head = newnode;
    }

    // class to add data to end
    void add_data_end(int roll_no, string name, float gpa)
    {
        Node *newnode = new Node(roll_no, name, gpa);
        newnode->roll_no = roll_no;
        newnode->gpa = gpa;
        newnode->name = name;
        if (tail == NULL)
        {
            head = tail = newnode;
            return;
        }
        newnode->prev = tail;
        tail->next = newnode;
        tail = newnode;
    }

    // class to add data at position
    void insert_at_pos(int roll_no, string name, float gpa, int position)
    {
        Node *newnode = new Node(roll_no, name, gpa);
        newnode->roll_no = roll_no;
        newnode->gpa = gpa;
        newnode->name = name;

        // insert if pos = 1: start
        if (position == 1)
        {
            add_data_start(roll_no, name, gpa);
            return;
        }

        // go to node located ad pos-1
        int count = 1;
        Node *ptr = head;
        while (ptr != NULL && count < position - 1)
        {
            ptr = ptr->next;
            count++;
        }

        // if user enter position is not present
        if (ptr == NULL)
        {
            cout << "Position invalid:\n";
            delete newnode;
            return;
        }

        // insert at position which is in end of list
        if (ptr->next == NULL)
        {
            add_data_end(roll_no, name, gpa);
            return;
        }

        // case: insert at position which is in middle
        newnode->next = ptr->next;
        newnode->prev = ptr;
        ptr->next->prev = newnode;
        ptr->next = newnode;
    }

    // forward display of list
    void display_forward()
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }
        Node *ptr = head;
        int count = 1;
        cout << "List of student:" << endl;
        while (ptr != NULL)
        {
            cout << "-------------------------------\n";
            cout << "Data of student:" << count++ << endl;
            cout << "ROLL NO: " << ptr->roll_no << endl;
            cout << "Name: " << ptr->name << endl;
            cout << "GPA: " << ptr->gpa << endl;
            ptr = ptr->next;
        }
    }

    // back ward display of list
    void diplay_reverse()
    {
        if (tail == NULL)
        {
            cout << "List is empty:\n";
            return;
        }
        Node *ptr = tail;
        int count = 1;
        cout << "List of Students:\n";
        while (ptr != NULL)
        {
            cout << "--------------------\n";
            cout << "Data of Student:" << count++ << endl;
            cout << "ROLL NO: " << ptr->roll_no << endl;
            cout << "Name: " << ptr->name << endl;
            cout << "GPA: " << ptr->gpa << endl;
            ptr = ptr->prev;
        }
    }

    // find data in user enter roll no range
    void range_roll_no(int low, int high)
    {
        if (head == NULL)
        {
            cout << "List is null:\n";
            return;
        }
        if (low > high)
            swap(low, high);

        Node *ptr = head;
        bool found = false;

        int count = 1;
        cout << "Data in range of ROLL NO[" << low << ", " << high << "]\n";
        while (ptr != NULL)
        {
            if (ptr->roll_no >= low && ptr->roll_no <= high)
            {
                cout << "--------------------\n";
                cout << "Data of Student:" << count++ << endl;
                cout << "ROLL NO: " << ptr->roll_no << endl;
                cout << "Name: " << ptr->name << endl;
                cout << "GPA: " << ptr->gpa << endl;
                found = true;
            }
            ptr = ptr->next;
        }

        if (!found)
        {
            cout << "No Data Present in this Range:\n";
            return;
        }
    }

    // data of gpa present in user enter range
    void range_by_gpa(float low, float high)
    {
        if (head == NULL)
        {
            cout << "List is Empty:\n";
            return;
        }

        if (low > high)
        {
            swap(low, high);
        }

        Node *ptr = head;
        bool found = false;
        int count = 1;
        cout << "Data of Student in Range GPA [" << low << ", " << high << "]:\n";
        while (ptr != NULL)
        {
            if (ptr->gpa >= low && ptr->gpa <= high)
            {
                cout << "----------------------------\n";
                cout << "Data of Student: " << count++ << endl;
                cout << "ROLL NO: " << ptr->roll_no << endl;
                cout << "Name: " << ptr->name << endl;
                cout << "GPA: " << ptr->gpa << endl;
                found = true;
            }
            ptr = ptr->next;
        }

        if (!found)
        {
            cout << "No Data present in this range:\n";
        }
    }

    // search by roll no
    void search_roll_no(int roll_no)
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }

        Node *ptr = head;
        int count = 1;
        while (ptr != NULL)
        {
            if (ptr->roll_no == roll_no)
            {
                break;
            }
            else
            {
                ptr = ptr->next;
            }
        }

        if (ptr == NULL)
        {
            cout << "Sorry! Data not Found:\n";
        }
        else
        {
            cout << "----------------------------\n";
            cout << "Data of Student: " << count++ << endl;
            cout << "ROLL NO: " << ptr->roll_no << endl;
            cout << "Name: " << ptr->name << endl;
            cout << "GPA: " << ptr->gpa << endl;
        }
    }

    // search by name
    void search_name(string name)
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }

        Node *ptr = head;
        bool found = false;
        int count = 1;
        while (ptr != NULL)
        {
            if (ptr->name == name)
            {
                cout << "----------------------------\n";
                cout << "Data of Student: " << count++ << endl;
                cout << "ROLL NO: " << ptr->roll_no << endl;
                cout << "Name: " << ptr->name << endl;
                cout << "GPA: " << ptr->gpa << endl;
                found = true;
            }
            ptr = ptr->next;
        }
        if (!found)
        {
            cout << "Data not Found:\n";
            return;
        }
    }

    // search by gpa
    void search_gpa(float gpa)
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }

        Node *ptr = head;
        bool found = false;
        int count = 1;
        while (ptr != NULL)
        {
            if (ptr->gpa == gpa)
            {
                cout << "----------------------------\n";
                cout << "Data of Student: " << count++ << endl;
                cout << "ROLL NO: " << ptr->roll_no << endl;
                cout << "Name: " << ptr->name << endl;
                cout << "GPA: " << ptr->gpa << endl;
                found = true;
            }
            ptr = ptr->next;
        }
        if (!found)
        {
            cout << "Data not Found:\n";
            return;
        }
    }

    // delete data from start
    void delete_start()
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }
        Node *ptr = head;
        head = head->next;
        delete ptr;
    }

    // delete data fro end of list
    void delete_last()
    {
        if (tail == NULL)
        {
            cout << "List is empty:\n";
            return;
        }
        Node *ptr = tail;
        if (head == tail)
        {
            head = tail = NULL;
        }
        else
        {
            tail = tail->prev;
            tail->next = NULL;
        }
        delete ptr;
    }

    // delete user enter roll no from list
    void delete_roll_no(int roll_no)
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }

        Node *ptr = head;
        bool found = false;

        while (ptr != NULL)
        {
            if (ptr->roll_no == roll_no)
            {
                found = true;
                Node *del = ptr;
                ptr = ptr->next;

                if (del == head)
                {
                    head = del->next;
                    if (head != NULL)
                        head->prev = NULL;
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
            cout << "--------------------------------------------------------------------------\n";
            cout << "All record of existing roll no :" << roll_no << ": are deleted Successfully:\n";
            cout << "--------------------------------------------------------------------------\n";
        }
        else
        {
            cout << "Roll no not found:\n";
        }
    }

    // sorted list by roll no in ascending order
    void sorted_roll_no_a()
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }
        for (Node *i = head; i != NULL; i = i->next)
        {
            for (Node *j = i->next; j != NULL; j = j->next)
            {
                if (i->roll_no > j->roll_no)
                {
                    int ptr = i->roll_no;
                    i->roll_no = j->roll_no;
                    j->roll_no = ptr;
                }
            }
        }
        cout << "List sorted ascending wise:\n";
    }

    // sorted list by roll no in descending order
    void sorted_roll_no_d()
    {
        if (head == NULL)
        {
            cout << "List is empty:\n";
            return;
        }
        for (Node *i = head; i != NULL; i = i->next)
        {
            for (Node *j = i->next; j != NULL; j = j->next)
            {
                if (i->roll_no < j->roll_no)
                {
                    int ptr = i->roll_no;
                    i->roll_no = j->roll_no;
                    j->roll_no = ptr;
                }
            }
        }
        cout << "List sorted descending wise:\n";
    }

    // save data to the data base system
    void save_data(string filename)
    {
        ofstream file(filename, ios::trunc);
        file << "Roll No:,Name,GPA\n";

        Node *ptr = head;
        while (ptr != NULL)
        {
            file << ptr->roll_no << "," << ptr->name << "," << ptr->gpa << endl;
            ptr = ptr->next;
        }
        file.close();
    }

    // get safe integer
    int getSafeint(string message)
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

            cout << "Invalid input! please enter a number type: ";
            cin.clear();

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // get safe float input
    float safeFloat(string message)
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

            cout << "Invalid input:\nplease enter float number: ";
            cin.clear();

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // get string input
    string get_string(string message)
    {
        string value;

        while (true)
        {
            cout << message;
            cin >> value;

            if (!cin.fail())
            {
                return value;
            }

            cout << "Inalid! Please Enter String value: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }

    // input safe data
    void input_data(int &roll_no, string &name, float &gpa)
    {
        roll_no = getSafeint("Enter Roll No: ");
        name = get_string("Enter name: ");
        gpa = safeFloat("Enter GPA: ");
    }

    // load data from csv file
    void load_data(string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            ofstream file(filename);
            cout << "File created successfully:\n";
            return;
        }

        // clear existing list first
        while (head != NULL)
        {
            delete_start();
        }

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            int roll;
            float gpa;
            string name;

            stringstream ss(line);
            string temp;

            // roll
            getline(ss, temp, ',');
            roll = stoi(temp);

            // name
            getline(ss, name, ',');

            // gpa
            getline(ss, temp, ',');
            gpa = stof(temp);

            add_data_end(roll, name, gpa);
        }

        file.close();
        cout << "data loaded from csv successfully:\n ";
    }

    // update data
    void update_data(int roll_no, string newname, float newgpa)
    {
        if (head == NULL)
        {
            cout << "List ie empty:\n";
            return;
        }

        Node *ptr = head;
        while (ptr != NULL)
        {
            if (ptr->roll_no == roll_no)
            {
                ptr->name = newname;
                ptr->gpa = newgpa;
                cout << "Student record updated successully:\n";
                save_data(filename);
                break;
            }
            ptr = ptr->next;
        }
        cout << "Student with :" << roll_no << ": roll no not found:\n";
    }

    void undo()
    {
        if (undoStack.empty())
        {
            cout << "Nothing to undo:\n";
            return;
        }

        Action act = undoStack.top();
        undoStack.pop();

        Node *ptr = head;
        while (ptr)
        {
            if (ptr->roll_no == act.roll)
            {
                redoStack.push(act);

                ptr->name = act.old_name;
                ptr->gpa = act.old_gpa;

                cout << "Undo performed:\n";
                return;
            }
            ptr = ptr->next;
        }
    }

    void redo()
    {
        if (redoStack.empty())
        {
            cout << "Nothing to redo:\n";
            return;
        }

        Action act = redoStack.top();
        redoStack.pop();

        Node *ptr = head;
        while (ptr)
        {
            if (ptr->roll_no == act.roll)
            {
                undoStack.push(act);

                ptr->name = act.new_name;
                ptr->gpa = act.new_gpa;
                cout << "Redo performd:\n";
                return;
            }
            ptr = ptr->next;
        }
    }

    void delete_list()
    {
        if (head == NULL)
        {
            cout << "List ie empty:\n";
            return;
        }

        Node *ptr = head;
        while (ptr != NULL)
        {
            ptr = ptr->next;
        }
    }
};

// start program
void start_program()
{
    link_list list;
    list.load_data(filename);
    int choice;
    do
    {
        cout << "----------------------------------------\n";
        cout << "----------------------------------------\n";
        cout << "Student Database:\n";
        cout << "----------------------------------------\n";
        cout << " 1- Add Data to Start:\n 2- Add Data to End:\n 3- Add Data at position:\n 4- Delete Data from start:\n 5- Delete data from end:\n 6- Delete Roll no:\n 7- search Roll no:\n 8- search by gpa:\n 9- search by name\n 10- Sorted ascending order:\n 11- sorted descending order:\n 12- display forward:\n 13: Display reverse:\n 14: Range by roll no:\n 15: Range by gpa:\n 16- Update Data:\n 17- Save Data\n 18- undo:\n 19- redo:\n 0- Exit:\n";
        cout << "----------------------------------------\n";
        choice = list.getSafeint("Enter Choice: ");

        int roll_no, position;
        string name;
        float gpa;

        switch (choice)
        {
        case 1:
            list.input_data(roll_no, name, gpa);
            list.add_data_start(roll_no, name, gpa);
            list.save_data(filename);
            cout << "---------------------------------------\n";
            cout << "Data Added to start successfully!:\n";
            cout << "---------------------------------------\n";
            break;
        case 2:
            list.input_data(roll_no, name, gpa);
            list.add_data_end(roll_no, name, gpa);
            list.save_data(filename);
            cout << "---------------------------------------\n";
            cout << "Data Added to End successfully!:\n";
            cout << "---------------------------------------\n";
            break;
        case 3:
            list.input_data(roll_no, name, gpa);
            cout << "Enter Position: ";
            cin >> position;
            list.insert_at_pos(roll_no, name, gpa, position);
            list.save_data(filename);
            cout << "---------------------------------------\n";
            cout << "Data Added At position successfully!:\n";
            cout << "---------------------------------------\n";
            break;
        case 4:
            list.delete_start();
            list.save_data(filename);
            cout << "Data remove from start successfully!:\n";
            break;
        case 5:
            list.delete_last();
            list.save_data(filename);
            cout << "Data Remove from end successfully!:\n";
            break;
        case 6:
            cout << "Enter Roll no to delete: ";
            cin >> roll_no;
            list.delete_roll_no(roll_no);
            list.save_data(filename);
            break;
        case 7:
            cout << "Enter Roll no to search: ";
            cin >> roll_no;
            list.search_roll_no(roll_no);
            break;
        case 8:
            cout << "Enter GPA to find:";
            cin >> gpa;
            list.search_gpa(gpa);
            break;
        case 9:
            cout << "Enter name to find: ";
            cin >> name;
            list.search_name(name);
            break;
        case 10:
            list.sorted_roll_no_a();
            list.save_data(filename);
            cout << "List sorted Ascending order:\n";
            break;
        case 11:
            list.sorted_roll_no_d();
            list.save_data(filename);
            cout << "List sorted descending order:\n";
            break;
        case 12:
            list.display_forward();
            break;
        case 13:
            list.diplay_reverse();
            break;
        case 14:
            int start_roll;
            int end_roll;
            cout << "Enter start Roll no: ";
            cin >> start_roll;
            cout << "Enter ending roll no: ";
            cin >> end_roll;
            list.range_roll_no(start_roll, end_roll);
            break;
        case 15:
            float start_gpa;
            float end_gpa;
            cout << "Enter starting GPA: ";
            cin >> start_gpa;
            cout << "Enter ending GPA: ";
            cin >> end_gpa;
            list.range_by_gpa(start_gpa, end_gpa);
            break;
        case 16:
            roll_no = list.getSafeint("Enter Roll no to update data: ");
            name = list.get_string("Enter new name: ");
            gpa = list.safeFloat("Enter new GPA:");
            list.update_data(roll_no, name, gpa);
            list.save_data(filename);
            break;
        case 17:
            list.save_data(filename);
            cout << "Data saved successfully:\n";
            break;
        case 18:
            list.undo();
            list.save_data(filename);
            break;
        case 19:
            list.redo();
            list.save_data(filename);
            break;
        case 0:
            cout << "Exiting...\n";
            cout << "Program Exit\n";
            break;
        default:
            cout << "Invalid Choice:\n";
        }
    } while (choice != 0);
}

// Authentication
void signup()
{
    string email = get_safe_string("Enter Email: ");
    string password = get_safe_string("Enter password: ");
    ofstream file("database/Signup.csv");
    file << "Email,Password\n";
    file << email << "," << password << endl;
    file.close();
}

void login()
{
    ifstream file("database/Signup.csv");
}

int main()
{
    start_program();
    return 0;
}
