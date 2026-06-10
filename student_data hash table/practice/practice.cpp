#include <iostream>
#include <string>
using namespace std;

bool isValidEmail(string gmail)
{
    bool found = false;
    for (char ch : gmail)
    {
        if (ch == '@')
        {
            found = true;
        }
    }
    if (found)
    {
        int index = gmail.find('@');
        string a = gmail.substr(index);
        if (a == "@gmail.com")
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

int main()
{
    string gmail = "saif@jml";

    // cout<<gmail.capacity();
    // cout<<gmail.back();
    // gmail.append("kal");
    // cout<<gmail<<endl;
    // cout<<gmail.front();
    // string a = gmail.substr(3);
    // cout << a << endl;

    // gmail.append("2");
    // cout<<gmail;
    if (isValidEmail(gmail))
        cout << "Valid Gmail:\n";
    else if (!isValidEmail(gmail))
        cout << "Invalid Gmail:\n";
    return 0;
}