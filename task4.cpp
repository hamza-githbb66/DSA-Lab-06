#include <iostream>
using namespace std;

#define MAX 50   // waiting room can hold 50 patients max

class WaitingRoom {
    int queue[MAX];   // stores patient ids
    int frontIndex;
    int rearIndex;
    int count;
    int nextId;       // id for the next patient, starts from 1

public:
    WaitingRoom() {
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
        nextId = 1;
    }

    // gives new id to patient and adds him in the queue
    void RegisterPatient() {
        if (count == MAX) {
            cout << "Waiting room is full, cannot register more patients!" << endl;
            return;
        }

        rearIndex = (rearIndex + 1) % MAX;   // circular so space is reused
        queue[rearIndex] = nextId;
        count++;

        cout << "Patient registered. Patient ID = " << nextId << endl;
        nextId++;
    }

    // calls the patient who came first
    void ServePatient() {
        if (count == 0) {
            cout << "No patient is waiting." << endl;
            return;
        }

        int id = queue[frontIndex];
        frontIndex = (frontIndex + 1) % MAX;
        count--;

        cout << "Patient " << id << " please come in, the doctor is ready." << endl;
    }

    // cancels everybody 
    void CancelAll() {
        if (count == 0) {
            cout << "Nobody is waiting, nothing to cancel." << endl;
            return;
        }

        cout << count << " appointments cancelled. Doctor can go to lunch!" << endl;
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
    }

    bool CanDoctorGoHome() {
        return count == 0;   // true if no one is waiting
    }

    // shows ids in sorted order
    void ShowAllPatient() {
        if (count == 0) {
            cout << "No patients are waiting." << endl;
            return;
        }

        // first copy the ids in a temp array, we dont want to disturb the queue
        int temp[MAX];
        for (int i = 0; i < count; i++) {
            temp[i] = queue[(frontIndex + i) % MAX];
        }

        // bubble sort 
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (temp[j] > temp[j + 1]) {
                    int t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }

        cout << "Waiting patients (" << count << "): ";
        for (int i = 0; i < count; i++) {
            cout << temp[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    WaitingRoom room;
    int choice;

    do {
        cout << "\n===== Emergency Ward Waiting Room =====" << endl;
        cout << "1. Register Patient" << endl;
        cout << "2. Serve Patient" << endl;
        cout << "3. Cancel All Appointments" << endl;
        cout << "4. Can Doctor Go Home?" << endl;
        cout << "5. Show All Waiting Patients" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            room.RegisterPatient();
        }
        else if (choice == 2) {
            room.ServePatient();
        }
        else if (choice == 3) {
            room.CancelAll();
        }
        else if (choice == 4) {
            if (room.CanDoctorGoHome()) cout << "Yes, no one is waiting. Doctor can go home." << endl;
            else cout << "No, patients are still waiting." << endl;
        }
        else if (choice == 5) {
            room.ShowAllPatient();
        }
        else if (choice != 0) {
            cout << "Wrong choice, try again." << endl;
        }

    } while (choice != 0);

    cout << "Bye!" << endl;
    return 0;
}