//
// Created by drcnm on 12/6/2022.
//
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>
using namespace std;

#include <cstring>
#include "PriorityQueue.h"
#include "DynamicArray.h"
#include "Pet.h"
#include "BinarySearchTree.h"

struct Appointment {
    int vetIndex;
    int endTime;
};

struct VetInfo{
    bool status;
    string name;

    Pet pet;
};

struct Owner{
    Pet pet;
    double pickupT;
};

bool operator < (const Pet& a, const Pet& b)
{
    return !(a.severity < b.severity);
};
bool operator < (const Appointment& a, const Appointment& b)
{
    return !(a.endTime < b.endTime);
};

void programmers();
int startSimulationMenu(DynamicArray<VetInfo>&, int&, int&);
void intake(PriorityQueue<Pet>&, BinarySearchTree<string, int>&, DynamicArray<Owner>&, double, int);
void seeVet(DynamicArray<VetInfo>&, PriorityQueue<Pet>&, PriorityQueue<Appointment>&, double, double);
void pickup(BinarySearchTree<string, int>, DynamicArray<Owner>, double);
int readNum();
int getRandomNumberOfArrivals(double);
int randTimeAdd(int, int);
double severityToCost(int);
string vetName();

int main(){
    programmers();

    PriorityQueue<Pet> holding; // waiting, still has to be priority by severity?
    DynamicArray<VetInfo> vets;
    //Binary Tree for our pet data? search for a pet?
    PriorityQueue<Appointment> beingSeen;
    BinarySearchTree<string, int> petDatabase;
    DynamicArray<Owner> owners;

    int simulationHours = 0;
    int vets_on_site = 0;

    int max_appts_today = startSimulationMenu(vets, simulationHours, vets_on_site);

    for (double time = 0;; time++) {
        if (time < simulationHours) {
            intake(holding, petDatabase, owners, time, max_appts_today);
        }

        seeVet(vets, holding, beingSeen, time, vets_on_site);
        pickup(petDatabase, owners, time);

        bool allVetsCleared = false;
        int count = 0;
        if (time > simulationHours) { // Checks if time limit has been reached
            for (count = 0; count < vets_on_site;
                 count++) { // Checks if all vets are clear
                if (vets[count].status == true) {
                    break;
                }
            }

            if (count == vets_on_site) { // if we get here then all vets are cleared
                allVetsCleared = true;
            }

            if (allVetsCleared) { // if all vets are cleared, end the simulation
                cout << endl;
                cout << "Simulation is over! " << endl;
                break;
            }
        }

        cout << "Press Enter To Continue...\n";
        cin.get();
    }
}

// ---------------------------- Displayed at beginning of simulation ---------------------
//need to account for user entering string or non int
//what happens if user enters 0
//need loop for press Q to exit
//how many patients per vet per day 2-3/ hour?
// ________________________________________________________________________________________
int startSimulationMenu(DynamicArray<VetInfo> &vets, int &simulationHours, int &vets_on_site) {

    int max_appts_today = 0;
    int min_appts_today = 0;
    cout << "\n~+~+~+~+~+~+~+~+~+~+~+~+~ Walk-in Pet Hospital ~+~+~+~+~+~+~+~+~+~+~+~+~\n";
    cout << "Veterinarians on staff today: ";
    vets_on_site = readNum();
    cout << "How many hours will this simulation run: ";
    simulationHours = readNum();

    max_appts_today = vets_on_site * (simulationHours * 3);
    min_appts_today = vets_on_site * (simulationHours * 2);

    //summary
    cout << "Number of Veterinarians: " << vets_on_site << endl;
    cout << "Number of Appointment hours in the day: " << simulationHours << endl;
    cout << "Max pets that can be seen today based on available providers " << vets_on_site << ": " << max_appts_today
         << endl << endl;

    for (int i = 0; i < vets_on_site; i++) {
        vets[i].status = false;
        vets[i].name = vetName();
    }
    return max_appts_today;
}

// ---------------------------- intake() --------------------------------------------
//generate pets per getRandomNumber Of Arrivals
// actual pickup time may be later if pet severity change
// ________________________________________________________________________________________
void intake(PriorityQueue<Pet> &holding,  BinarySearchTree<string, int> &petDatabase,DynamicArray<Owner> &owners, double time, int mat){
    int arrivalRate = 3; //do we need this? does it come from file or random?
    int newArrivals = getRandomNumberOfArrivals(arrivalRate);
    static int ownerCount = 0;
    cout << "~+~+~+~+~+~+~+~+~+~+~+~+~ Pet Intake ~+~+~+~+~+~+~+~+~+~+~+~+~\n";
    cout << endl;
    cout << " Current Time: " << time << endl;
    cout << " New Pets in Intake: " <<  newArrivals << endl;
    cout << " Total Seen of Max: " << (ownerCount + newArrivals) << "/" << mat << endl;
    cout << setfill('-')<<setw(57)<<"-"<<endl;
    cout << "|  Pet/Patient   |    Severity   |   Est.Pickup Time   |\n";
    cout << setfill('-')<<setw(57)<<"-"<<endl;
    for (int i = 0; i < newArrivals; i++) {
        Pet pet(time);
        petDatabase[pet.name];
        Owner owner; // used at pickup
        owner.pet = pet;
        owners[ownerCount] = owner;
        cout << setfill(' ')<<setw(5) << pet.name << setw(26-pet.name.size()) << pet.severity
             << setw(20) << pet.estPickupT << "\n";

        if (pet.severity > 0) { //if severity 0 they get pickedup
            holding.push(pet);
        }
        ownerCount++;
    }
    cout << setfill('-')<<setw(75)<<"-"<<endl;
}

// ---------------------------- seeVet() --------------------------------------------
//assign pets from holding to vets
//update severity
//assign new pickuptime
// ________________________________________________________________________________________
void seeVet(DynamicArray<VetInfo> &vets, PriorityQueue<Pet> &holding, PriorityQueue<Appointment> &beingSeen, double time, double vets_on_site){

    cout << "\n+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ Seeing Vet ~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+\n";
    cout << setfill('-')<<setw(96)<<"-"<<endl;
    cout << "| Veterinarian | Currently Treating | (intake) Severity | (updated) Severity |  New Est Pickup |\n";
    cout << setfill('-')<<setw(96)<<"-"<<endl;

    for (int i = 0; i < vets_on_site; i++) {
        if(!vets[i].status && !holding.empty()) {
            vets[i].pet = holding.top();
            holding.pop();
            vets[i].status = true;
            Appointment app;

            int oldSeverity = vets[i].pet.severity;
            vets[i].pet.updateSeverity();
            vets[i].pet.setPickupT(time); //set pickup time based on new severity
            app.endTime = vets[i].pet.pickupT;
            app.vetIndex = i;
            beingSeen.push(app);
            int vn = vets[i].name.size();
            cout << "Dr. " << vets[i].name << setfill(' ')<<setw(28-vn) << vets[i].pet.name;
            cout << setw(15) << oldSeverity << setw(20) << vets[i].pet.severity;
            cout << setw(20) << vets[i].pet.pickupT << endl;
        }
    }
    //cout << setfill('-')<<setw(96)<<"-"<<endl;

    for (int i = 0; i < vets_on_site; i++){
        if(!beingSeen.empty() && (beingSeen.top().endTime >= time)){
            vets[beingSeen.top().vetIndex].status = false;
            vets[beingSeen.top().vetIndex].pet.severity = 0; // they have been seen and cured!
            beingSeen.pop();
        }
    }
}

// ---------------------------- pickup() --------------------------------------------
//owners in waiting room = pet.estPickupTime < time
//
//
// ________________________________________________________________________________________
void pickup(BinarySearchTree<string, int> petDatabase,DynamicArray<Owner> owners, double time){
    double cost = 0;
    double dailyTotal = 0;
    cout << "\n~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+ Accounting ~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+\n";
    cout << setfill('-')<<setw(96)<<"-"<<endl;
    cout << setfill(' ')<<setw(5) << " Pet Name           |       Amount Due" << endl;
    cout << setfill('-')<<setw(96)<<"-"<<endl;
    for (int i = 0; i < owners.capacity(); i++) {
        if (owners[i].pet.estPickupT <= time && owners[i].pet.severity !=0){
            cost = severityToCost(owners[i].pet.severity);
            cout <<setfill(' ')<< owners[i].pet.name <<setw(33-owners[i].pet.name.size())<< "$" << cost  << endl;
            dailyTotal += cost;
        }

        if (owners[i].pet.estPickupT <= time && owners[i].pet.pickupT <= time) {
            petDatabase.deleteKey(owners[i].pet.name); //remove pet from database
            owners[i].pet.estPickupT = 1000; //cant figure out how to remove element from dynamic array owners
        }
    }
    cout << setfill('-')<<setw(40)<<"-"<<endl;
    cout << "Total Collections:              $" << dailyTotal << endl;
}
// ---------------------------- EndOfSimSummary() --------------------------------------------
//when simulationHours == end

// ________________________________________________________________________________________
#if 0
void endOfSimSummary(){
    cout << setfill('-')<<setw(57)<<"-"<<endl;
    cout << setfill(' ')<<setw(60) << "End of Day Summary \n";
    cout << setfill('-')<<setw(57)<<"-"<<endl;

    //summary output
    cout << "Patient Arrival rate: " << avRate << " per minute, for " << clockStopTime << " minutes" << endl;
    cout << "Average Wait time: " << avWaitTime << endl;
    cout << "Average Cost per visit: " << avCost << endl;
}

#endif
// ---------------------------- severityToCost() --------------------------------------------

// ________________________________________________________________________________________
double severityToCost(int sev){
    switch(sev){
        case 0: return 0.00;
        case 1: return 125.00;
        case 2: return 350.00;
        case 3: return 575.00;
        case 4: return 750.00;
        case 5: return 1125.00;
        case 6: return 2235.00;
        default: return 0.00;
    }
}


// ---------------------------- readFileData() --------------------------------------------
// IF we use a simulation.txt file
//reads the file, outputs file data
// ________________________________________________________________________________________
void readFileData(){
    fstream file;
    string fileArray[5];

    string line;
    file.open("simulation.txt");
    if(!file.good()){
        cout << "Could not open file\n";
    }
    for (int i = 0; i < 6; i++) {
        getline(file, line);
        fileArray[i] = line;
    }
    file.close();

    int waitMaxLength = 0, minApptTime = 0, maxApptTime = 0, clockStopTime = 0;
    double avRate = 0.0;
    int turnedAway = 0;
    //nServers = atoi(fileArray[0].c_str());
    avRate = atof(fileArray[1].c_str());
    waitMaxLength  = atoi(fileArray[2].c_str());
    minApptTime  = atoi(fileArray[3].c_str());
    maxApptTime  = atoi(fileArray[4].c_str());
    clockStopTime = atoi(fileArray[5].c_str());

    //summary output
    cout << "Patient Arrival rate: " << avRate << " per minute, for " << clockStopTime << " minutes" << endl;
    cout << "Maximum wait time: " << waitMaxLength << endl;
    cout << "Minimum Appointment time: " << minApptTime<< endl;
    cout << "Maximum Appointment time: " << maxApptTime << endl;

}
// --------------------------------- readNum() --------------------------------------------
// User input
// accounts for getline weirdness
// ________________________________________________________________________________________

int readNum(){
    char buffer[20];
    cin.getline(buffer, sizeof(buffer));
    return atoi(buffer);
}

// ---------------------------- getRandomNumberOfArrivals() --------------------------------------------
// random arrivals
// _____________________________________________________________________________________________________

int getRandomNumberOfArrivals(double averageArrivalRate){
    int arrivals = 0;
    double probOfnArrivals = exp(-averageArrivalRate);
    for (double randomValue = (double)rand() / RAND_MAX; (randomValue -= probOfnArrivals) > 0.0; probOfnArrivals *= averageArrivalRate / static_cast<double>(++arrivals));
    return arrivals;
}

// ------------------------------------------ randTimeAdd() --------------------------------------------
//
// _____________________________________________________________________________________________________

int randTimeAdd(const int a, const int b){
    return a + (rand() % b);
}

string vetName() {
    string first[] = {"Tucker", "Victor", "Christina", "Megan", "Youseff", "Mari", "Mauricio", "Marisa", "Dennis"};

    return first[rand() % 9];
}

void programmers(){
    cout << "Authors:  Christina, Tucker, Victor" << endl;
    cout << "file: "<<__FILE__<<"\n\n";
}
