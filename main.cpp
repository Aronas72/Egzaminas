#include "funkcijos.h"

int main() {
    stringstream pagrindinis, papildomas, isvalytas;
    vector<string> galunes, visos_nuorodos;
    unordered_map<string, ZodzioInfo> zodziu_statistika;

    cout << "Skaitomi failai" << endl;
    nuskaitytiFailus(pagrindinis, papildomas, galunes);

    cout << "Vykdoma nuorodu paieska" << endl;
    ieskotiNuorodu(pagrindinis, visos_nuorodos, galunes, true, &isvalytas);
    ieskotiNuorodu(papildomas, visos_nuorodos, galunes);

    cout << "Zodziu analize" << endl;
    analizuotiZodzius(isvalytas, zodziu_statistika);

    cout << "Vyksta spausdinimas ir rikiavimas" << endl;
    spausdintiRezultatus(zodziu_statistika, visos_nuorodos, 1);

    cout << "Darbas baigtas. Rezultatai failuose." << endl;
    return 0;}
