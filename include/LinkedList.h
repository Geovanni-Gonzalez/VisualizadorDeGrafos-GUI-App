#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
};

template <typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Copy constructor to avoid dangling pointers when returning by value
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node<T>* curr = other.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    // Disable assignment for simplicity in this project
    LinkedList& operator=(const LinkedList&) = delete;

    ~LinkedList() {
        clear();
    }

    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
        }
        count++;
    }

    T& front() { return head->data; }
    const T& front() const { return head->data; }
    T& back() { return tail->data; }
    const T& back() const { return tail->data; }
    
    void pop_back() {
        if (!tail) return;
        Node<T>* toDelete = tail;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete toDelete;
        count--;
    }

    bool remove(const T& value) {
        Node<T>* current = head;
        while (current) {
            if (current->data == value) {
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }

                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }

                delete current;
                count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void clear() {
        Node<T>* current = head;
        while (current) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        count = 0;
    }

    size_t size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    // Iterator support
    class iterator {
        Node<T>* current;
    public:
        iterator(Node<T>* node) : current(node) {}

        T& operator*() { return current->data; }
        iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
        bool operator==(const iterator& other) const {
            return current == other.current;
        }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

private:
    Node<T>* head;
    Node<T>* tail;
    size_t count;
};

#endif // LINKEDLIST_H
