#include "Dusman.h"

Dusman::Dusman()
{
    ID = "";
    dakika = 0;
    konum = "";
    kalori = 0;
}
Dusman::Dusman(string id, int _kalori, string _konum, int _dakika)
{
    ID = id;
    kalori = _kalori;
    konum = _konum;
    dakika = _dakika;
}
