#include "Pizza.h"

Pizza::Pizza()
{
    ID = "";
    boyut = "";
    kalori = 0;
    hacim = 0;
}

Pizza::Pizza(string id, string _boyut, int _kalori, int _hacim)
{
    ID = id;
    boyut = _boyut;
    kalori = _kalori;
    hacim = _hacim;
}
