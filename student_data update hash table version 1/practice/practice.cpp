#include <iostream>
#include <string>
#include <fstream>
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

void data()
{
    ofstream file("data1.txt");
    if (!file.is_open())
    {
        cout << "file does not exits:\n";
        return;
    }
    string gmail = "Saifullahkhan08@gmail.com";
    string name = "saifullah";
    string no = "03405475560";
    string f_name = "khalid";
file << "OFFICE OF THE PRINCIPAL GOVT: DEGREE COLLEGE CHARBAGH SWAT\n";
file << "Gmail: " <<gmail <<" Ph. "<<no<<"\n";

file<<"HOPE CERTIFICATE\n";
file<<"This is to certify that "<<name<<", S/O "<<f_name<<", is a bona fide student\n of Government Degree College Charbagh, Swat.\n He has successfully completed all academic requirements of the Intermediate\n (HSSC) – Part II for the academic session and has appeared in all board \nexaminations conducted by the concerned education board.\n";
file<<"His Intermediate Part-II result is currently awaited, and based\n on his academic performance in internal assessments and Part-I\n examinations, the college is confident that he will successfully pass the HSSC \n(Intermediate) Examination with good standing.  This certificate is being\n issued upon the request of the student for submission to relevant authorities\n. We wish him all the best in his future academic endeavors.\n";
file<<"Principal\n";
file<<"Head of Institution Government Degree\n"; 
file<<"College Charbagh, Swat\n"; 
file<<"Date: _______________________\n";
    file.close();
}


int main()
{
    data();
    return 0;
}