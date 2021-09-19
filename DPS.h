//
// Created by BedTed on 5/5/2021.
//

#ifndef STRING_DPS_H
#define STRING_DPS_H


#include <string>

using namespace std;

class DPS {
public:
    DPS* last_; // указатель на предыдуший блок
    DPS* next_; // указатель на следующий блок
    int now_;   // кол-во символов в векторе
    char* vector_; // вектор символов

    DPS (int max) { // создание первого блока
        now_ = 0;
        last_ = nullptr;
        next_ = nullptr;
        vector_ = new char[max];
    }

    DPS () {
            now_ = 0;
            last_ = nullptr;
            next_ = nullptr;
    }

    ~DPS () {
        if (vector_ != nullptr) {
            delete[] vector_;
        }
        now_ = 0;
        last_ = nullptr;
        next_ = nullptr;
    }

    DPS *getLast() const {
        return last_;
    }

    DPS *getNext() const {
        return next_;
    }

    int getNow() const {
        return now_;
    }

    char *getVector() const {
        return vector_;
    }

};


#endif //STRING_DPS_H
