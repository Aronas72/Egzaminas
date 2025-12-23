#include "funkcijos.h"

string toLowerUTF8(string s) {
    static const unordered_map<string, string> lt_keitykla = {
        {"Ą", "ą"}, {"Č", "č"}, {"Ę", "ę"}, {"Ė", "ė"}, {"Į", "į"},
        {"Š", "š"}, {"Ų", "ų"}, {"Ū", "ū"}, {"Ž", "ž"}};
    string rezultatas = "";
    for (int i = 0; i < s.length(); ++i) {
        unsigned char c = s[i];
        if (c >= 'A' && c <= 'Z') {
            rezultatas += (char)(c + 32);}
        else if (c >= 128) {
            if (i + 1 < s.length()) {
                string utf_simbolis = s.substr(i, 2);
                if (lt_keitykla.count(utf_simbolis)) {
                    rezultatas += lt_keitykla.at(utf_simbolis);}
                else{
                    rezultatas += utf_simbolis;}
                i++;}}
        else {
            rezultatas += c;}}
    return rezultatas;}

void nuskaitytiFailus(stringstream& pagr_tekstas, stringstream& papild_nuorodos, vector<string>& galunes) {
    ifstream f1("Lietuva.txt");
    if (!f1) cerr << "KLAIDA: Nepavyko atidaryti Lietuva.txt!" << endl;
    else { pagr_tekstas << f1.rdbuf(); f1.close(); }

    ifstream f2("nuorodos.txt");
    if (f2) { papild_nuorodos << f2.rdbuf(); f2.close(); }

    ifstream f3("nuorodu_listas.txt");
    if (!f3) cerr << "KLAIDA: Nepavyko atidaryti nuorodu_listas.txt!" << endl;
    else { string g; while (f3 >> g) if(!g.empty()) galunes.push_back(g); f3.close(); }}

void ieskotiNuorodu(stringstream& saltinis, vector<string>& rezultatas, const vector<string>& galunes, bool valytiTeksta, stringstream* isvalytasTekstas) {
    saltinis.clear();
    saltinis.seekg(0);
    if (galunes.empty()) {
        if (valytiTeksta && isvalytasTekstas) *isvalytasTekstas << saltinis.rdbuf();
        return;}

    string galu_str = "(";
    for (const auto& g : galunes) galu_str += g + "|";
    galu_str.pop_back();
    galu_str += ")";

    string sablonas_str = R"(((http(s)?://)?(www\.)?([\w-]+\.)+)" + galu_str + R"(\b[^ \[\)\(\s\r\n\t\f]*))";
    regex url_regex(sablonas_str, regex::icase);

    string eilute;
    while (getline(saltinis, eilute)) {
        auto pradzia = sregex_iterator(eilute.begin(), eilute.end(), url_regex);
        auto pabaiga = sregex_iterator();

        for (auto i = pradzia; i != pabaiga; ++i) {
            rezultatas.push_back(i->str());}

        if (valytiTeksta && isvalytasTekstas) {
            *isvalytasTekstas << regex_replace(eilute, url_regex, " ") << "\n";}}}

void analizuotiZodzius(stringstream& tekstas, unordered_map<string, ZodzioInfo>& statistika) {
    tekstas.clear();
    tekstas.seekg(0);
    regex zodzio_regex(R"([a-zA-ZąĄčČęĘėĖįĮšŠųŲūŪžŽ]+)");
    string eilute;
    int eilutes_nr = 1;

    while (getline(tekstas, eilute)) {
        auto pradzia = sregex_iterator(eilute.begin(), eilute.end(), zodzio_regex);
        auto pabaiga = sregex_iterator();

        for (auto i = pradzia; i != pabaiga; ++i) {
            string originalus = i->str();
            string raktas = toLowerUTF8(originalus);

            if (statistika.find(raktas) == statistika.end() || (originalus == raktas)) {
                statistika[raktas].originalusZodis = originalus;}

            statistika[raktas].kiekis++;
            statistika[raktas].eilutes.insert(eilutes_nr);}
        eilutes_nr++;}}

void spausdintiRezultatus(const unordered_map<string, ZodzioInfo>& statistika, const vector<string>& nuorodos, int riba) {
    vector<pair<string, ZodzioInfo>> surikiuoti(statistika.begin(), statistika.end());

    sort(surikiuoti.begin(), surikiuoti.end(), [](const auto& a, const auto& b) {
        if (a.second.kiekis != b.second.kiekis) return a.second.kiekis > b.second.kiekis;
        return a.first < b.first;});

    ofstream fZ("RezultataiZodziai.txt");
    if (fZ.is_open()) {
        fZ << left << setw(25) << "ŽODIS" << setw(15) << "KIEKIS" << "EILUTĖS" << endl;
        fZ << string(70, '-') << endl;

        for (const auto& p : surikiuoti) {
            if (p.second.kiekis > riba) {
                int lt_baitai = 0;
                for (unsigned char c : p.second.originalusZodis) if (c >= 128) lt_baitai++;
                
                fZ << left << setw(25 + lt_baitai/2) << p.second.originalusZodis
                   << setw(15) << p.second.kiekis;
                
                bool pirmas = true;
                for (int nr : p.second.eilutes) {
                    if (!pirmas) fZ << ", ";
                    fZ << nr;
                    pirmas = false;}
                fZ << endl;}}
        fZ.close();}

    set<string> unikalios(nuorodos.begin(), nuorodos.end());
    ofstream fN("RezultataiNuorodos.txt");
    if (fN.is_open()) {
        fN << "RASTOS NUORODOS:" << endl;
        fN << string(30, '-') << endl;
        for (const auto& n : unikalios) fN << n << endl;
        fN.close();}}
