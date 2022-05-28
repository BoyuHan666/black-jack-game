#include <iostream>

using namespace std;

int current_id = -1; // record the new store id

class Node{
public:
    int id;
    int ssize;
    int* store_data;
    Node* next;
};

Node* head = NULL; // record the head store for iterating the linked list(datastore) later
Node* tail = NULL; // record the tail store which help to add a new store in the lined list(datastore)

// Question 1
int newstore(int ssize){
    if(ssize < 0){
        return -1;
    }
    // initial the Node and its attribute
    Node* newNode = new Node();
    current_id ++;
    newNode->id = current_id;
    newNode->ssize = ssize;
    newNode->store_data = new int[ssize]();
    // if the store is the first store, assign it to head store and it is also the tail store
    if(head == NULL){
        head = newNode;
        tail = newNode;
        return current_id;
    }
    // else assign it to tail->next and update the tail store
    else{
        tail->next = newNode;
        tail = newNode;
        return current_id;
    }

}

// Question 2
int add_element_to_store(int id, int val, int idx=-1){
    Node* pointer = head;
    // iterate the linked list(datastore)
    while(pointer != NULL){
        // check if the id is what we want
        if(pointer->id == id) {
            // check id the idx is valid
            if (idx >= pointer->ssize) {
                return -1;
            }
            // check if idx parameter id given and the idx position is empty
            if (idx != -1 and pointer->store_data[idx] == 0) {
                pointer->store_data[idx] = val;
                return 0;
            }
            if (idx == -1) {
                // find the last empty position
                for (int i = 0; i < pointer->ssize; i++) {
                    if (pointer->store_data[i] == 0) {
                        pointer->store_data[i] = val;
                        return 0;
                    }
                }
                return -1;
            }
        }
        pointer = pointer->next;
    }

    return -1;
}

// Question 3
void print_debug(){
    // print the number of total used store elements and the size of datastore
    int num_elements = 0;
    int cur_size_of_datastore = 0;
    Node* a = head;
    // iterate the linked list(datastore)
    while(a!=NULL){
        for(int i=0;i<a->ssize;i++) {
            // check if the element is not empty on the ith position
            if (a->store_data[i] != 0) {
                num_elements++;
            }
        }
        cur_size_of_datastore += a->ssize;
        a = a->next;
    }
    cout << "the number of total used store elements: " << num_elements << endl;
    cout << "the size of the datastore: " << cur_size_of_datastore << endl;

    // print the current datastore
    cout << "datastore   : ";
    Node* b = head;
    // iterate the linked list(datastore)
    while(b!=NULL){
        for(int i=0;i<b->ssize;i++) {
            cout << b->store_data[i] << " ";
        }
        b = b->next;
    }
    cout << endl;

    // print elements in each store
    Node* p = head;
    // iterate the linked list(datastore)
    while(p!=NULL){
        cout << "store " << p->id << ": ";
        for(int i=0;i<p->ssize;i++){
            cout << p->store_data[i] << " ";
        }
        cout << endl;
        p = p->next;
    }
    cout << "########################################" << endl;


}

// Question 4
void delete_element_from_store_by_value(int id, int val){
    Node* p = head;
    // iterate the linked list(datastore)
    while(p!=NULL){
        // check if the id is what we want
        if(p->id == id){
            for(int i=0;i<p->ssize;i++){
                // check if there is a same value
                if(p->store_data[i] == val){
                    p->store_data[i] = 0;
                    break;
                }
            }
        }
        p = p->next;
    }

}

void delete_element_from_store_by_index(int id, int idx){
    Node* p = head;
    // iterate the linked list(datastore)
    while(p!=NULL){
        // check if the id is what we want
        if(p->id == id){
            for(int i=0;i<p->ssize;i++){
                // check if the idx is valid
                if(i == idx){
                    p->store_data[i] = 0;
                    break;
                }
            }
        }
        p = p->next;
    }

}

// Question 5
void which_stores_have_element(int val){
    Node* p = head;
    bool flag = false; // this indicates if we find the same value in at least one store
    bool first_id = true; // check if it is the first time we find the same value, just help to print the message
    // iterate the linked list(datastore)
    while(p!=NULL){
        // iterate the store and check if there is the same value
        for(int i=0;i<p->ssize;i++){
            if(p->store_data[i] == val){
                if(first_id){
                    cout << "Element " << val << " is in store IDs: ";
                    cout << p->id;
                    first_id = false;
                }
                else{
                    cout << ", " << p->id;
                }
                flag = true;
            }
        }
        p = p->next;
    }
    if(!flag){
        cout << "Element " << val << " is not available in any store";
    }
    cout << endl;
}

// Question 6
void delete_store(int id){
    Node* p = head;
    // check if the head is the store that we want to delete
    if(p->id == id){
        head = head->next; // change the head
        delete p; // delete the previous store on the head
    }
    // iterate the linked list(datastore)
    while(p!=NULL and p->next!=NULL){
        // check if the id of the next store is what we want
        if(p->next->id == id){
            p->next = p->next->next; // concatenate the linked list
            delete p->next; // delete that store
        }
        p = p->next;
    }
}

// Question 7
int resize_store(int id, int newsize){
    Node* p = head;
    // iterate the linked list(datastore)
    while(p!=NULL){
        // check if the id is what we want
        if(p->id == id){
            // determine if we need more space or less space
            if(newsize<p->ssize){
                // only keep the elements with the range of new size, others will reassign to 0
                for(int i=newsize;i<p->ssize;i++){
                    p->store_data[i] = 0;
                }
                p->ssize = newsize; // change size
                return 0;
            }
            else{
                // initial the element to 0 on the new positions
                for(int i=p->ssize;i<newsize;i++){
                    p->store_data[i] = 0;
                }
                p->ssize = newsize; // change size
                return 0;
            }
        }
        p = p->next;
    }
    return -1;
}

int main(){
    int s1 = newstore(3); // create new empty data store of size 3
    int s2 = newstore(5); // create new empty data store of size 5

    if (s1 != -1)
    {
        add_element_to_store(s1, 13);
        add_element_to_store(s1, 15);
        add_element_to_store(s1, 21);
        add_element_to_store(s1, 42); // this should return -1
    }

    if (s2 != -1)
    {
        add_element_to_store(s2, 7, 2);
        add_element_to_store(s2, 15, 0);
        add_element_to_store(s2, 22, 1);
    }

    print_debug();

    delete_element_from_store_by_value(s1, 13);
    delete_element_from_store_by_value(s1, 71);
    delete_element_from_store_by_index(s2, 2);
    delete_element_from_store_by_index(s1, 5);
    print_debug();

    which_stores_have_element(15);
    which_stores_have_element(29);

    delete_store(s1);
    print_debug();

    resize_store(s2, 20);
    int s3 = newstore(40);
    print_debug();

    s3 = newstore(30);
    add_element_to_store(s3, 7, 29);
    print_debug();
}
