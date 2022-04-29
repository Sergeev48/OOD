#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

void readFile(vector<double> *firstTen, vector<double> *secondTwenty, vector<double> *allX) {
    string filename = "input1.txt", line;
    ifstream file(filename);
    if (file.is_open()) {
        if (getline(file, line)) {
            int i = 0;
            double x_i, f_i;
            while (getline(file, line)) {
                istringstream iss(line);
                iss >> x_i >> f_i;
                (*allX).push_back(x_i);
                if (i < 10) {
                    (*firstTen).push_back(x_i);
                } else if (i > 9 && i < 30){
                    (*secondTwenty).push_back(x_i);
                }
                i++;
            }
        }
    }
}

// мат ожидание
double expectedValue(vector<double> x) {
    double sum = 0;
    for (int i = 0; i < x.size(); i++) {
        sum += x[i] / x.size();
    }
    return sum;
}

// математическое ожидание ДжекНайф
double expectedValueForEachSample(vector<double> x, double expectedVal, double *dispersionExpVal) {
    double sum = 0, resSum = 0;
    vector<double> resExpValForSamp, resPseudoEstimates;

    for (int i = 0; i < x.size(); i++) {
        sum += x[i];
    }

    // мат ожидание для каждой выборки
    for (int i = 0; i < x.size(); i++) {
        resExpValForSamp.push_back((sum - x[i]) / (x.size() - 1));
        resSum += ((sum - x[i]) / (x.size() - 1));
    }

    // псевдооценки
    for (int i = 0; i < x.size(); i++) {
        resPseudoEstimates.push_back(
                x.size() * expectedVal - (x.size() - 1) * resExpValForSamp[i]
        );
    }

    // дисперсии мат. ожиданий
    for (int i = 0; i < x.size(); i++) {
        (*dispersionExpVal) += (resPseudoEstimates[i] - expectedVal) * (resPseudoEstimates[i] - expectedVal) /
                               (x.size() * (x.size() - 1));
    }

    return resSum / x.size();
}

// дисперсия
double dispersionValue(vector<double> x, double expectedVal) {
    double sum = 0;
    for (int i = 0; i < x.size(); i++) {
        sum += (x[i] - expectedVal) * (x[i] - expectedVal);
    }

    return 1.0 / (x.size() - 1.0) * sum;
}

// Дисперсия (Джек Найф)
void dispersionJackNaif(vector<double> x, double disperse, double *result, double *disperseOfDisperse) {
    int count = 0;
    while (count < x.size()) {
        vector<double> v;
        double average = 0, averageDisperse = 0;
        for (int i = 0; i < x.size(); i++) {
            if (i == count) continue;
            v.push_back(x[i]);
            average += x[i] / (x.size() - 1.0);
        }
        for (int i = 0; i < x.size(); i++) {
            if (i == count) continue;
            // D^_un-дисперсия
            averageDisperse += ((average - x[i]) * (average - x[i])) * (1.0 / (x.size() - 2.0));
        }
        // псевдооценки
        (*result) += ((x.size() * disperse - (x.size() - 1.0) * averageDisperse) / x.size());
        (*disperseOfDisperse) += (((x.size() * disperse - (x.size() - 1.0) * averageDisperse) - disperse) *
                                  ((x.size() * disperse - (x.size() - 1.0) * averageDisperse) - disperse)) /
                                 (x.size() * (x.size() - 1));
        count++;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<double> firstTen, secondTwenty, allX;
    readFile(&firstTen, &secondTwenty, &allX);

    // мат ожидание
    double expectedValFor10 = expectedValue(firstTen);

    // дисперсия
    double dispersionFor10 = dispersionValue(firstTen, expectedValFor10);

    // дисперсия мат ожидания
    double dispersionExpVal = 0;

    // математическое ожидание для каждой выборки, псевдооценки, дисперсии мат. ожиданий
    double expectedValJack = expectedValueForEachSample(firstTen, expectedValFor10, &dispersionExpVal);

    // Дисперсия - Джек Найф
    double dispersionJackNaif10 = 0, disperseOfDisperse10 = 0;
    dispersionJackNaif(firstTen, dispersionFor10, &dispersionJackNaif10, &disperseOfDisperse10);

    cout
            << "Выборка = 10:\n\tМатематическое ожидание:\n\t" << expectedValFor10
            << "\n\tМатематическое ожидание (ДжекНайф):\n\t" << expectedValJack
            << "\n\tДисперсия мат. ожидания:\n\t" << dispersionExpVal
            << "\n\tДисперсия:\n\t" << dispersionFor10
            << "\n\tДисперсия (ДжекНайф):\n\t" << dispersionJackNaif10
            << "\n\tДисперсия дисперсии:\n\t" << disperseOfDisperse10;

    // мат ожидание
    double expectedValFor20 = expectedValue(secondTwenty);

    // дисперсия
    double dispersionFor20 = dispersionValue(secondTwenty, expectedValFor20);

    // дисперсия мат ожидания
    double dispersionExpVal20 = 0;

    // математическое ожидание для каждой выборки, псевдооценки, дисперсии мат. ожиданий
    double expectedValJack20 = expectedValueForEachSample(secondTwenty, expectedValFor20, &dispersionExpVal20);

    // Дисперсия - Джек Найф
    double dispersionJackNaif20 = 0, disperseOfDisperse20 = 0;
    dispersionJackNaif(secondTwenty, dispersionFor20, &dispersionJackNaif20, &disperseOfDisperse20);

    cout
            << "\nВыборка = 20:\n\tМатематическое ожидание:\n\t" << expectedValFor20
            << "\n\tМатематическое ожидание (ДжекНайф):\n\t" << expectedValJack20
            << "\n\tДисперсия мат. ожидания:\n\t" << dispersionExpVal20
            << "\n\tДисперсия:\n\t" << dispersionFor20
            << "\n\tДисперсия (ДжекНайф):\n\t" << dispersionJackNaif20
            << "\n\tДисперсия дисперсии:\n\t" << disperseOfDisperse20;


    // мат ожидание
    double expectedValForAll = expectedValue(allX);

    // дисперсия
    double dispersionForAll = dispersionValue(allX, expectedValForAll);

    cout << "\nВыборка = все:\n\tМатематическое ожидание:\n\t" << expectedValForAll
         << "\n\tДисперсия:\n\t" << dispersionForAll;
    return 0;
}
