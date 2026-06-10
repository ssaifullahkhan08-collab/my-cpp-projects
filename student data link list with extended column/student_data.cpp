#include <iostream>
#include <string>
#include <limits>
using namespace std;

class rownode
{
public:
    string data;
    int value;
    rownode *next;
    rownode *prev;
    rownode(string d)
    {
        data = d;
        next = prev = NULL;
    }
};

class Column
{
public:
    string col_name;
    rownode *rowhead;
    rownode *rowtail;
    Column *next;
    Column *prev;

    Column(string name)
    {
        col_name = name;
        rowhead = rowtail = NULL;
        next = prev = NULL;
    }

    void addRowData(string data)
    {
        rownode *newrow = new rownode(data);
        if (rowhead == NULL)
        {
            rowhead = rowtail = NULL;
        }
        else
        {
            rowtail->next = newrow;
            newrow->prev = rowtail;
            rowtail = newrow;
        }
    }

    void deleteRowData(string value)
    {
        if (rowhead == NULL)
        {
            cout << "Row is empty:\n";
            return;
        }

        rownode *ptr = rowhead;
        while (ptr != NULL)
        {
            if (ptr->data == value)
            {
                // delete data if head
                if (ptr == rowhead)
                {
                    rowhead = ptr->next;
                    rowhead->prev = NULL;
                }
                // delete data if tail
                else if (ptr == rowtail)
                {
                    rowtail = ptr->prev;
                    rowtail->next = NULL;
                }
                else
                {
                    ptr->prev->next = ptr->next;
                    ptr->next->prev = ptr->prev;
                }
                delete ptr;
                return;
            }
            ptr = ptr->next;
        }
        cout << "Data not found:\n";
    }
};

class Table
{
private:
    Column *head;
    Column *tail;

public:
    Table()
    {
        head = tail = NULL;
    }

    void add_column(string name)
    {
        Column *newcolumn = new Column(name);
        if (head == NULL)
        {
            head = tail = newcolumn;
            return;
        }
        else
        {
            tail->next = newcolumn;
            newcolumn->prev = tail;
            tail = newcolumn;
        }
        cout << "Column successfully Added:\n";
    }

    void safeString(string message)
    {
        string value;

        while (true)
        {
            cout << message;
            cin >> value;

            if (!cin.fail())
            {
                return;
            }

            cout << "Invalid: Enter String type: ";
            cin.clear();

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        return;
    }

    void addRow()
    {
        Column *ptr = head;
        string value;
        cout << "Enter Row Data:\n";
        while (ptr != NULL)
        {
            cout << ptr->col_name << ": ";
            cin >> value;
            ptr->addRowData(value);
            ptr = ptr->next;
        }
    }

    void delete_col_name(string name)
    {
        if (head == NULL)
        {
            cout << "No column to be Deleted:\n";
            return;
        }

        Column *ptr = head;
        while (head != NULL)
        {
            if (ptr->col_name == name)
            {
                // delete first column if it is head
                if (ptr == head)
                {
                    head = ptr->next;
                    if (head != NULL)
                        head->prev = NULL;
                }
                // delete last column tail
                else if (ptr == tail)
                {
                    tail = ptr->next;
                    tail->next = NULL;
                }
                // delete column at middle;
                else
                {
                    ptr->next->prev = ptr->prev;
                    ptr->prev->next = ptr->next;
                }
                delete ptr;
                cout << "Column deleted Successfully:\n";
                return;
            }
            ptr = ptr->next;
        }
        cout << "Column not found:\n";
    }

    void display()
    {
        if (head == NULL)
        {
            cout << "No Data yet:\n";
            return;
        }

        Column *ptr = head;
        while (ptr != NULL)
        {
            cout << ptr->col_name << "\t\t";
            ptr = ptr->next;
        }
        cout << endl;

        int row = 0;
        rownode *r = head->rowhead;
        while (r != NULL)
        {
            row++;
            r = r->next;
        }
        cout << "row: " << row << endl;

        for (int i = 0; i < row; i++)
        {
            ptr = head;
            while (ptr != NULL)
            {
                rownode *rn = ptr->rowhead;
                for (int j = 0; j < i; j++)
                {
                    rn = rn->next;
                    cout << rn->data << "\t\t";
                    ptr = ptr->next;
                }
            }
            cout << endl;
        }
    }

    void display_data()
    {
        if (head == NULL)
        {
            cout << "Empty: \n";
            return;
        }

        Column *ptr = head;
        cout << "Row: \n";
        while (ptr != NULL)
        {
            rownode *r = head->rowhead;
            while (r != NULL)
            {
                cout << ptr->col_name << ": " << r->data << endl;
                r = r->next;
            }
            ptr = ptr->next;
        }
    }
};

int main()
{
    Table t;
    t.add_column("RollNo");
    t.add_column("Name");
    t.add_column("GPA");

    t.addRow();
    t.addRow();

    t.display_data();
    return 0;
}