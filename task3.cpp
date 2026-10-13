#include <iostream>
using namespace std;

// one print request
struct PrintRequest {
    int id;
    int arrivalTime;
    int processingTime;
};

class CircularQueue {
    PrintRequest *arr;   // dynamic array so we can resize it
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;           // how many items are in the queue right now

public:
    CircularQueue(int cap) {
        capacity = cap;
        arr = new PrintRequest[capacity];
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
    }

    ~CircularQueue() {
        delete[] arr;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == capacity;
    }

    int getCapacity() { return capacity; }
    int getCount() { return count; }

    // makes a new array and copies everything in the right order
    void resize(int newCapacity) {
        cout << "   >> Resizing: capacity " << capacity << " -> " << newCapacity << endl;

        PrintRequest *newArr = new PrintRequest[newCapacity];

        // copy from front to rear (index wraps around with %)
        for (int i = 0; i < count; i++) {
            newArr[i] = arr[(frontIndex + i) % capacity];
        }

        delete[] arr;   // free the old array
        arr = newArr;
        capacity = newCapacity;
        frontIndex = 0;
        rearIndex = count - 1;   // after copying, items are at 0 to count-1
    }

    void enqueue(PrintRequest x) {
        // if full then double the size first
        if (isFull()) {
            resize(capacity * 2);
        }

        rearIndex = (rearIndex + 1) % capacity;
        arr[rearIndex] = x;
        count++;
    }

    PrintRequest dequeue() {
        PrintRequest x;
        x.id = -1;   // -1 means nothing was there
        x.arrivalTime = 0;
        x.processingTime = 0;

        if (isEmpty()) {
            cout << "Queue is empty, nothing to dequeue!" << endl;
            return x;
        }

        x = arr[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;

        // if only 1/4 or less of the space is used then make it half
        if (capacity > 4 && count <= capacity / 4) {
            resize(capacity / 2);
        }

        return x;
    }

    PrintRequest front() {
        if (isEmpty()) {
            PrintRequest x;
            x.id = -1;
            x.arrivalTime = 0;
            x.processingTime = 0;
            return x;
        }
        return arr[frontIndex];
    }

    // shows the queue state, prints every slot of the array
    void display() {
        cout << "   Queue state -> size: " << count << ", capacity: " << capacity
             << ", front index: " << frontIndex << ", rear index: " << rearIndex << endl;
        cout << "   Slots: ";

        for (int i = 0; i < capacity; i++) {
            // check if this slot has a real item or is empty
            bool used = false;
            for (int j = 0; j < count; j++) {
                if ((frontIndex + j) % capacity == i) {
                    used = true;
                }
            }

            if (used) cout << "[R" << arr[i].id << "] ";
            else cout << "[ - ] ";
        }
        cout << endl;
    }
};

// ---------- helpers for main ----------
CircularQueue q(4);   // starting capacity is 4

int totalWaiting = 0;     // sum of waiting times
int processedCount = 0;   // how many requests are processed
int currentTime = 0;      // the clock of the printer

void addRequest(int id, int arrival, int processing) {
    PrintRequest r;
    r.id = id;
    r.arrivalTime = arrival;
    r.processingTime = processing;

    cout << "\nEnqueue request R" << id << " (arrival=" << arrival
         << ", processing=" << processing << ")" << endl;
    q.enqueue(r);
    q.display();
}

void processRequest() {
    if (q.isEmpty()) {
        cout << "\nDequeue: queue is empty" << endl;
        return;
    }

    cout << "\nDequeue request" << endl;
    PrintRequest r = q.dequeue();

    // printer can only start when the request has arrived
    int startTime = currentTime;
    if (r.arrivalTime > startTime) {
        startTime = r.arrivalTime;
    }

    int waiting = startTime - r.arrivalTime;
    currentTime = startTime + r.processingTime;

    totalWaiting = totalWaiting + waiting;
    processedCount++;

    cout << "   Processed R" << r.id << ": started at " << startTime
         << ", waited " << waiting << ", finished at " << currentTime << endl;
    q.display();
}

int main() {
    cout << "===== Print Server Simulation =====" << endl;
    cout << "Starting capacity = 4" << endl;
    q.display();

    // 1) add 5 requests, the 5th one will make the queue full -> resize to 8
    addRequest(1, 0, 5);
    addRequest(2, 1, 3);
    addRequest(3, 2, 4);
    addRequest(4, 3, 2);
    addRequest(5, 4, 6);

    cout << "\nFront request is R" << q.front().id << endl;

    // 2) process some, queue will become sparse -> shrinks
    processRequest();
    processRequest();
    processRequest();
    processRequest();

    // 3) add more so that rear wraps around the array
    addRequest(6, 10, 3);
    addRequest(7, 11, 2);
    addRequest(8, 12, 4);

    // 4) process everything that is left
    while (!q.isEmpty()) {
        processRequest();
    }

    // try dequeue on an empty queue
    processRequest();

    cout << "\n===== Summary =====" << endl;
    cout << "Requests processed: " << processedCount << endl;
    cout << "Total waiting time: " << totalWaiting << endl;
    if (processedCount > 0) {
        cout << "Average waiting time: " << (double)totalWaiting / processedCount << endl;
    }

    return 0;
}