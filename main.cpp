#include <iostream>
#include "dbmanager.hpp"

using namespace std;

int main()
{
    DBManager manager;

    manager.load_raw_data("personas", "data.csv", '|');

    manager.create_index("personas", "phone");

    Record record = manager.find("personsa", 427929150);
    cout << record.nombre << endl;
    return 0;
}