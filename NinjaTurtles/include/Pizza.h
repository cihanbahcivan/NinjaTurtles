#pragma once
using namespace std;

#include <string>
#include <fstream>

class Pizza
{
public:
    Pizza();
    Pizza(string, string, int, int);

    string ID;
    string boyut;
    int kalori;
    int hacim;
};
