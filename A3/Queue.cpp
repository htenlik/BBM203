#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here
    front = -1; // Indicates the queue is empty
    rear = -1;  // Indicates the queue is empty
    size = 0;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province
    if (size == MAX_QUEUE_SIZE) {
        return;
    }
    if (isEmpty()) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
    }
    data[rear] = province;
    ++size;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here
    if (isEmpty()) {
        return -1;
    }
    int removedProvince = data[front];
    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % MAX_QUEUE_SIZE;
    }
    --size;
    return removedProvince;
}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    if (isEmpty()) {
        return -1;
    }
    return data[front];}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    return size == 0;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if (size == MAX_QUEUE_SIZE) {
        return;
    }
    if (isEmpty()) {
        enqueue(province);
        return;
    }
    front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    data[front] = province;
    ++size;
}
bool Queue::contains(int province) const {
    for (int i = front; i != rear; i = (i + 1) % MAX_QUEUE_SIZE) {
        if (data[i] == province) {
            return true;
        }
    }
    return false;
}
