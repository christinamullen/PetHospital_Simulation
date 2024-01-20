//
// Created by drcnm on 12/6/2022.
//

#ifndef FINAL_PET_H
#define FINAL_PET_H

#include <string>
using namespace std;


class Pet {
public:
    int severity; //priority
    double arrivalT;
    double estPickupT;//time when owner shows up to waiting room
    double pickupT; //time when released to owner

    string name;
    string breed;

    Pet() {};

    Pet(double arrivalt) {
        severity = 1 + (rand() % 6);
        name = generateName();
        breed = generateBreed();
        arrivalT = arrivalt;
        estPickupT = arrivalT + (severity * .5);
    }

    void updateSeverity() {//after seeing vet
        severity = 1 + (rand() % 6);;
    }

    void setPickupT(int time) {
        pickupT = time + (severity * .5);
    }

    void petData_toString() {
        // cout << severity << "\t" << name << "\t" << breed << "\t" << arrivalT << endl;
    }


// ------------------------------------------ Random Pet data methods --------------------------------------------
//
// _______________________________________________________________________________________________________________

    string generateName() {
        string first[] = {"Sir ", "Mr. ", "Ms. ", "Baby ", "Lil ", "Lucky ", "Senor ", "Senora ", "Big "}; //9
        string last[] = {"Kibbles", "Fluffy", "Buddy", "Bean", "Pepe", "Sal", "Sobi", "Rami", "Lola", "Mishi", "Romeo",
                         "Andy"}; //12

        return first[rand() % 9] + last[rand() % 12];
    }

    string generateBreed() {
        string breed[] = {"Cat", "Dog", "Bird"};
        return breed[rand() % 3];
    }
};
#endif //FINAL_PET_H
