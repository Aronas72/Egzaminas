#ifndef FUNKCIJOS_H
#define FUNKCIJOS_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include <regex>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

struct ZodzioInfo {
    int kiekis = 0;
    set<int> eilutes;};

void nuskaitytiFailus(stringstream& pagr_tekstas, stringstream& papild_nuorodos, vector<string>& galunes);
void ieskotiNuorodu(stringstream& saltinis, vector<string>& rezultatas, const vector<string>& galunes, bool valytiTeksta = false, stringstream* isvalytasTekstas = nullptr);
void analizuotiZodzius(stringstream& tekstas, unordered_map<string, ZodzioInfo>& statistika);
void spausdintiRezultatus(const unordered_map<string, ZodzioInfo>& statistika, const vector<string>& nuorodos, int riba);

#endif
