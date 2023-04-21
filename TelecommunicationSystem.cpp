/*
IMPORTANT NOTES :

    -> The graph is of size 15 x 15 in which first 5 are towers to send and recieve information and the minimum cost has to be calculated 
        till these coordinates only
    -> The Information of Customers is Stored in a Linked List 
    -> Every Customer is Linked to BST for contacts he/she saved
    -> The Queue of Messages is used to store messages until they are conveyed
    
*/

#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#define V 15
#define INF __INT_MAX__

// The Charges for every message or every second of call
#define callCharges 0.05
#define smsCharges 0.5

using namespace std;

// Graph Arrays
// Initial Array for the Towers
int arrayGraph[V][V] = {
    {0, 4, 12, 10, 20,     INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {4, 0, 3, 5, 6,        INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {12, 3, 0, 8, 17,      INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {10, 5, 8, 0, 9,       INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {20, 6, 17, 9, 0,      INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, 0, INF, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, 0, INF, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, 0, INF, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, 0, INF, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, 0, INF, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 0, INF, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 0, INF, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 0, INF, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 0, INF},
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 0},
};


// Houses of Customers 
// MAximum 10
int houses[2][10] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};


// Find the Index of last slot available for houses
int lastIndex()
{

    for (int i = 0; i < V; i++)
    {
        if (arrayGraph[0][i] == INF)
        {
            return i;
        }
    }
    return -1;
}

// Check Houses by printing them
void printHouses()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cout << houses[i][j] << "\t";
        }

        cout << endl;
    }
}

// Structs
struct Contacts
{
    string name;
    string phone;

    Contacts *left = NULL;
    Contacts *right = NULL;
};

struct Customer
{
    string name;
    string phone;
    string cnic;
    int street;
    int house;
    int id;
    double balance = 0;
    Contacts *phoneContacts = NULL;

    Customer *next = NULL;
    Customer *prev = NULL;
};

struct message
{

    string recieverPhone;
    string senderPhone;
    string m;
};

// Classes
class Call  // USED FOR CALL TIME CALCULATION
{
public:
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    void displayClock()
    {
        // system call to clear the screen
        system("CLS");

        cout << setfill(' ') << setw(55) << "	O N G O I N G   C A L L  \n";
        cout << setfill(' ') << setw(55) << "\t -----------------------------------------------------\n";
        cout << setfill(' ') << setw(29);
        cout << "| " << setfill('0') << setw(2) << hours << " hrs | ";
        cout << setfill('0') << setw(2) << minutes << " min | ";
        cout << setfill('0') << setw(2) << seconds << " sec |" << endl;
        cout << setfill(' ') << setw(55) << "\t ----------------------------------------------------\n";
    }

    int timer()
    {
        int duration = 0;
        
        while (!_kbhit())
        {

            // display the timer
            displayClock();

            cout << "\t\tEnter Any Button to End Call" << endl;

            
            // for 1 second
            sleep(1);

            // increment seconds
            seconds++;
            duration++;

            // if seconds reaches 60
            if (seconds == 60)
            {

                // increment minutes
                minutes++;

                // if minutes reaches 60
                if (minutes == 60)
                {

                    // increment hours
                    hours++;
                    minutes = 0;
                }
                seconds = 0;
            }
            // kbhit();
        }
        return duration;
    }
};

class LinkedList
{
private:
    int FourDigitRandom()
    {
        return (int)rand() % (9999 + 1);
    }

public:
    Customer *first, *last;

    bool isEmpty()
    {

        if (first == NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    string insert(string n, string cnic, int street, int house)
    {

        Customer *current = new Customer();

        current->name = n;
        // cout << "here" << endl;

        if (SearchUsingCNIC(cnic) == NULL)
        {
            current->cnic = cnic;
        }

        int id;

        // Assign Random ID to Customer
        do
        {
            id = (rand() % 1000) + 1;
        } while (IdExists(id));
        current->id = id;
        // cout << id <<endl;
        int num;
        string s;
        do
        {
            num = FourDigitRandom() + (FourDigitRandom() * 10000);

            s = to_string(num);
            // cout << "s  :: " << s <<endl;
        } while (num > 10000000 && num < 10000000 && searchUsingPhone("030" + s) != NULL);

        current->phone = "030" + s;
        // cout << current->phone << endl;
        if (isEmpty())
        {
            first = last = current;
        }
        else
        {
            last->next = current;
            current->prev = last;
            last = current;
        }

        int j = INF;
        for (int i = 0; i < 15; i++)
        {
            if (houses[0][i] == 0)
            {
                j = i;
                break;
            }
        }


        // Random Distances from towers to cover up the graph
        for (int i = 0; i < j + 5; i++)
        {
            int distance = (rand() % 20) + 1;
            arrayGraph[j + 5][i] = distance;
            arrayGraph[i][j + 5] = distance;
        }

        houses[0][j] = house;
        houses[1][j] = street;
        current->house = house;
        current->street = street;

        //        cout << "......... Value Addded ........." << endl;
        return current->phone;
    }

    string insert(Customer c)
    {

        Customer *current = &c;

        
        if (isEmpty())
        {
            first = last = current;
        }
        else
        {
            last->next = current;
            current->prev = last;
            last = current;
        }

        int j = INF;
        for (int i = 0; i < 15; i++)
        {
            if (houses[0][i] == 0)
            {
                j = i;
                break;
            }
        }

        for (int i = 0; i < j + 5; i++)
        {
            int distance = (rand() % 20) + 1;
            arrayGraph[j + 5][i] = distance;
            arrayGraph[i][j + 5] = distance;
        }

        houses[0][j] = c.house;
        houses[1][j] = c.street;

        //        cout << "......... Value Addded ........." << endl;
        return current->phone;
    }

    bool deleteSpecific(string num)
    {

        Customer *p = first;
        while (stoi(p->phone.substr(3, 11)) != stoi(num.substr(3, 11)) && p != NULL)
        {
            p = p->next;
        }

        if (p != NULL)
        {
            if (p == first)
            {
                if (first == last)
                {
                    delete first;
                    first = last = NULL;
                }
                else
                {
                    first = first->next;
                    delete first->prev;
                    first->prev = NULL;
                }
            }
            else if (p == last)
            {
                if (first == last)
                {
                    delete first;
                    first = last = NULL;
                }
                else
                {
                    last = last->prev;
                    delete last->next;
                    last->next = NULL;
                }
            }
            else
            {
                p->prev->next = p->next;
                p->next->prev = p->prev;

                delete p;
                return true;
                // cout << "......... Deleted Successfully ........." << endl;
            }
        }
        else
        {
            return false;
            // cout << "Invalid Number Entered to Delete......." << endl;
        }
    }

    bool IdExists(int i)
    {
        Customer *p = first;
        while (p != NULL)
        {
            if (i == p->id)
            {
                return true;
            }

            p = p->next;
        }
        return false;
    }
    Customer *searchUsingPhone(string n)
    {
        Customer *p = first;
        while (p != NULL)
        {
            if (n == p->phone)
            {
                return p;
            }

            p = p->next;
        }
        return NULL;
    }
    Customer *SearchUsingCNIC(string i)
    {
        Customer *p = first;
        while (p != NULL)
        {
            if (i == p->cnic)
            {
                return p;
            }

            p = p->next;
        }
        return NULL;
    }

    void displayInfo()
    {
        Customer *p = first;
        if (p == NULL)
        {
            cout << "\t\t\tNO CUSTOMERS TO DISPLAY " << endl;
        }
        else
        {
            cout << "\t\tID \t\t | Name \t\t | Phone Number \t\t |  CNIC NUMBER \t\t |  Balance \t\t |  " << endl;
            cout << "\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
            while (p != NULL)
            {
                cout << "\t\t" << p->id << " \t\t | " << p->name << " \t\t | " << p->phone << " \t\t\t | " << p->cnic << " \t\t | " << p->balance << " \t\t | " << endl;
                p = p->next;
            }

            cout << "\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
        }
    }
};

class queue
{

public:
    int front = -1, rear = -1;
    int size = 100;
    message array[100];
    queue()
    {

    }
    

    bool isEmpty()
    {
        if (front == -1 && rear == -1)
        {
            return true;
        }
        return false;
    }

    bool isFull()
    {
        if (front == 0 && rear == size - 1)
        {
            return true;
        }
        return false;
    }

    void leftShift(message array[], int endvalue)
    {
        for (int i = front; i <= endvalue; i++)
        {
            array[i - 1] = array[i];
        }
        front--;
    }

    // Right Shift


    void enqueue(message x)
    {
        if (isFull())
        {
            cout << "\t---------- No Space ---------- " << endl;
        }
        else
        {
            if (isEmpty())
            {
                front = rear = 0;
                array[rear] = x;
            }
            // else if (rear == size-1)
            // {
            //     leftShift(array,rear);
            //     array[size-1]= x;
            // }
            else
            {
                array[++rear] = x;
            }

            // cout << "\t---------- Element Added ---------- " << endl;
        }
    }

    message dequeue()
    {
        if (isEmpty())
        {
            cout << "\t---------- No Element To Delete ---------- " << endl;
            message m;
            return m;
        }
        else
        {
            message x = array[front];
            if (front == rear)
            {
                front = -1;
                rear = -1;
            }
            else
            {
                front++;
                leftShift(array, rear);
                rear--;
            }
            return x;
        }
    }

    // void display()
    // {
    //     if (isEmpty())
    //     {
    //         cout << "\t---------- Queue Is Empty ---------- " << endl;
    //     }
    //     else
    //     {
    //         cout << "\t---------- D I S P L A Y ---------- " << endl;

    //         for (int i = front; i <= rear; i++)
    //         {
    //             cout << ">> " << array[i] << endl;
    //         }
    //     }
    // }
};

class BST
{

public:
    Contacts *root = NULL;

    BST(Contacts *p)
    {
        root = p;
    }

    void inOrder(Contacts *p)
    {

        if (p != NULL)
        {
            inOrder(p->left);                                              // Left
            cout << "\t\t\t\t" << p->name << "\t\t\t" << p->phone << endl; // Print
            inOrder(p->right);                                             // Right
        }
    }

    Contacts *insert(string num, string name)
    {

        Contacts *curr = new Contacts();

        curr->phone = num;
        curr->name = name;

        if (root == NULL)
        {
            root = curr;
        }
        else
        {
            Contacts *p = root;
            Contacts *p2 = root;

            while (p != NULL)
            {
                p2 = p;
                if (stoi(curr->phone.substr(3, 11)) >= stoi(p->phone.substr(3, 11)))  // Phone - String  to convert into int 
                {
                    p = p->right;
                }
                else
                {
                    p = p->left;
                }
            }

            if (stoi(curr->phone.substr(3, 11)) >= stoi(p2->phone.substr(3, 11)))
            {
                // cout << "TO THE RIGHT" <<endl;
                p2->right = curr;
            }
            else
            {
                // cout << "TO THE LEFT" <<endl;
                p2->left = curr;
            }
        }

        return curr;
    }

    

    
};

class Graph
{

public:
    int finalCost[V];
    int visit[V];

    int getCustomerIndex(Customer *s)
    {

        for (int i = 0; i < V; i++)
        {
            if (s->house == houses[0][i] && s->street == houses[1][i])
            {
                return i + 5;
            }
        }
        return INF;
    }

    void printResult()
    {
        for (int i = 0; i < V; i++)
        {
            cout << finalCost[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < V; i++)
        {
            cout << visit[i] << " ";
        }
        cout << "\n-----------------------------------" << endl;
    }

    //  0 8 4 9 4

    void initialize()
    {
        for (int i = 0; i < V; i++)
        {

            finalCost[i] = __INT_MAX__;
            visit[i] = false;
        }
    }

    void dijkstra(int edge)
    {

        initialize();
        // Putting Edge itself to finalCosts
        for (int k = 0; k < V; k++)
        {
            finalCost[k] = arrayGraph[edge][k];
        }
        visit[edge] = true;

        for (int i = 0; i < V - 1; i++)
        {

            int min = __INT_MAX__;
            int index = 0;
            for (int j = 0; j < V; j++)
            {
                if (finalCost[j] < min && !visit[j] && finalCost[j] != 0)
                {
                    min = finalCost[j];
                    index = j;
                }
            }
            visit[index] = true;

            for (int j = 0; j < V; j++)
            {

                if ((arrayGraph[index][j] + min) < finalCost[j] && arrayGraph[index][j] != INF && !visit[j] && arrayGraph[index][j] != 0 && j < lastIndex())
                {

                    finalCost[j] = arrayGraph[index][j] + min;
                }
            }
        }
    }
};

// Data Structures
LinkedList customers;
queue messagesQ;
Graph towers;

// Methods
Customer *login();
string signup();
void customer(Customer *);
void admin();

int main()
{
    

    Customer c;
    c.name = "hafif";
    c.cnic = "3660314502649";
    c.balance = 1000;
    c.id = 12;
    c.phone = "03045153334";
    c.street = 12;
    c.house = 3;

    Customer c2;
    c2.name = "saad";
    c2.cnic = "1234567890123";
    c2.balance = 500;
    c2.id = 52;
    c2.phone = "03006530081";
    c2.street = 21;
    c2.house = 32;

    customers.insert(c);
    customers.insert(c2);

    cout << lastIndex() << endl;


    int choice;
    do
    {

        cout << "\t\t--------------------------------------------------------------------------------------------" << endl;
        cout << "\t\t                     T E L E C O M M U N I C A T I O N     S Y S T E M                      " << endl;
        cout << "\t\t--------------------------------------------------------------------------------------------" << endl;

        cout << "\n\n\t\t\tEnter [0] to Exit \n"
             << "\t\t\tEnter [1] to Login \n"
             << "\t\t\tEnter [2] to Login as Admin \n"
                "\t\t\tEnter [3] to Signup :  "
             << endl;
        cout << "\n\t\tEnter your choice : ";
        // bool t = true;
        cin >> choice;
        if (choice == 1)
        {
            Customer *c = login(); // Login Returns the node of That customer  
            if (c != NULL)
            {
                customer(c);
            }
        }
        else if (choice == 2)   // Get to Admin
        {

            cout << "\n\t\tEnter user name:" << endl;
            cout << "\t\t----------------\n\t\t";
            string un;
            cin >> un;
            cout << "\n\t\tEnter Password:" << endl;
            cout << "\t\t----------------\n\t\t";
            string pass;
            cin >> pass;
            if (un == "admin" && pass == "admin")
            {

                admin();
            }
        }
        else if (choice == 3)   // Signupp
        {

            string p = signup();
            cout << "\t\t\t\tYour phone number is  ' " << p << " '" << endl;
            cout << "\t\t\tCongrats For Signing Up!!! \n\t\t\tNow Login to your Account \n"
                 << endl;
        }

    } while (choice != 0);
}

Customer *login()
{
    cout << "\n\n"
         << endl;
    cout << "\t\t\t----------------------------------------------------" << endl;
    cout << "\t\t\t                     L O G I N                      " << endl;
    cout << "\t\t\t----------------------------------------------------" << endl;

    while (true)
    {
        cout << "\n\t\tEnter the Phone Number : " << endl;
        cout << "\t\t-------------------------\n\t\t";
        string p;
        cin >> p;

        cout << "\n\t\tEnter the CNIC Number  : " << endl;
        cout << "\t\t-------------------------\n\t\t";
        string c;
        cin >> c;

        Customer *l = customers.searchUsingPhone(p);  // Search the phone in the linked list if there is that contact or not

        if (l != NULL)
        {
            if (l->cnic == c) // Check if the cnic corresponds to that number entered that is entered first or not
            {
                return l;
            }
        }
        cout << "\n\n\t\t\t!!!! Invlaid Login Information !!!!" << endl;
        cout << "\t\t\tEnter 1 to Try Again OR \n\t\t\tEnter Anything else to return to Main Menu : \n\t\t";
        int choice;
        cin >> choice;
        if (choice != 1)
        {
            break;
        }
    }

    return NULL;
}

string signup()
{

    /*
        Get the information from the customer to add login or signup
        Checks :::
            --> If CNIC is present previously then it is invalid
            --> Else enter the values like name, address
            --> Rest will be assigned automatically During Insertion
    */
    cout << "\n\n"
         << endl;
    cout << "\t\t\t----------------------------------------------------" << endl;
    cout << "\t\t\t                     S I G N U P                      " << endl;
    cout << "\t\t\t----------------------------------------------------" << endl;

    cout << "\n\t\tEnter the CNIC Number  : " << endl;
    cout << "\t\t-------------------------\n\t\t";
    string c;
    while (true)
    {

        cin >> c;

        if (c.length() == 13 && customers.SearchUsingCNIC(c) == NULL)
        {
            break;
        }
        else
        {
            cout << "\n\t\t\tInvalid CNIC : \n\t\t\tEnter CNIC  Again : " << endl;
        }
    }

    cout << "\n\t\tEnter your Name  : " << endl;
    cout << "\t\t--------------------\n\t\t";
    string n;
    cin >> n;

    cout << "\n\t\tEnter your Street Number  : " << endl;
    cout << "\t\t-----------------------------\n\t\t";
    int s;
    cin >> s;
    cout << "\n\t\tEnter your House Number  : " << endl;
    cout << "\t\t-----------------------------\n\t\t";
    int h;
    cin >> h;

    string p = customers.insert(n, c, s, h);  // Get the Phone  number that is automatically Generated in the insert method which  is returned later
    return p;
}

void admin()
{

    int choice;
    do
    {
        cout << "\n\n"
             << endl;
        cout << "\t\t\t----------------------------------------------------" << endl;
        cout << "\t\t\t                     A D M I N                      " << endl;
        cout << "\t\t\t----------------------------------------------------" << endl;

        cout << "\n\t\t\tEnter [0] to Exit : \n"
             << "\t\t\tEnter [1] to add Customer : \n"
             << "\t\t\tEnter [2] to delete Customer : \n"
             << "\t\t\tEnter [3] to update Balance of Customer : \n"
             << "\t\t\tEnter [4] to view all Customers : \n"
             << "\t\t\tEnter [5] to view Graph State : \n"
             << endl;
        cout << "\n\t\tEnter your choice : \n\t\t";
        cin >> choice;

        if (choice == 1)
        { // Add Customer

            /*
                Add Customer is Similar as Signup (written on line 874) and is an approach for admin to add a customer
            */
            cout << "\n\t\tEnter the CNIC Number  : " << endl;
            cout << "\t\t-------------------------\n\t\t";
            string c;
            while (true)
            {

                cin >> c;

                if (c.length() == 13)
                {
                    break;
                }
                else
                {
                    cout << "\n\t\t\tInvalid CNIC : \n\t\t\tEnter CNIC  Again : " << endl;
                }
            }

            cout << "\n\t\tEnter Name of Customer : " << endl;
            cout << "\t\t-------------------------\n\t\t";
            string n;
            cin >> n;

            cout << "\n\t\tEnter Street Number of Customer : " << endl;
            cout << "\t\t-----------------------------------\n\t\t";
            int s;
            cin >> s;
            cout << "\n\t\tEnter House Number of Customer : " << endl;
            cout << "\t\t----------------------------------\n\t\t";
            int h;
            cin >> h;

            string p = customers.insert(n, c, s, h);

            cout << "\t\tPhone Number of this Customer will be : " << endl;
            cout << "\t\t\t\t" << p << endl;
        }

        else if (choice == 2) // Delete Customer

        {
            /*
                In order to delete a customer, you can take the phone number and search it in the file;
                If the contact exists, it is deleted else shows an error prompt
            */
            cout << "\n\t\tEnter Phone Number of Customer to Delete : " << endl;
            cout << "\t\t----------------------------------------------\n\t\t";
            string p;
            cin >> p;

            Customer *c = customers.searchUsingPhone(p);

            if (c != NULL)
            {
                
                bool deleted = customers.deleteSpecific(p);
                if (deleted)
                {
                    cout << "\t\t--------------- D E L E T E D  -------------\n\t\t" << endl;
                }
                else
                {
                    cout << "\t\t--------------- CAN NOT BE DELETED  -------------\n\t\t";
                }
            }
            else{
                 cout << "\n\t\t\tInvalid Number Entered !!! \n\t\t\t!!!! Try Again !!!! " << endl;   
            }
        }

        else if (choice == 3) // Update Balance of Customer
        {

            /*
                To update the balance of the user  , first search it's phone number and add the amount of balance to the  
                Node which is returned after searching the phone number 
                The amount added will be added to the existing balance of user
            */
            cout << "\n\t\tEnter Phone Number of Customer to Update Balance : " << endl;
            cout << "\t\t---------------------------------------------------\n\t\t";
            string p;
            cin >> p;

            Customer *c = customers.searchUsingPhone(p);
            if (c != NULL)
            {
                cout << "\n\t\tEnter Amount of Balance to be added : " << endl;
                cout << "\t\t----------------------------------------------\n\t\t";
                int balance;
                cin >> balance;
                c->balance = c->balance + balance;
            }
            else
            {
                cout << "\n\t\t\tInvalid Number Entered !!! \n\t\t\t!!!! Try Again !!!! " << endl;
            }
        }

        else if (choice == 4)
        { // Display all Customers Information
            // call the display function in linked List
            
            customers.displayInfo();
        }

        else if (choice == 5)
        { // Display Graph State
            // Printing the 2D Array 
            for (int i = 0; i < V; i++)
            {
                for (int j = 0; j < V; j++)
                {
                    cout << arrayGraph[i][j] << "\t";
                }
                cout << endl;
            }
            cout << "\n\n\n"
                 << endl;
                 // Printing the Houses
            printHouses();
        }

    } while (choice != 0);
}

void customer(Customer *person)
{
    int c;
    do
    {
        cout << "\n\n"
             << endl;
        cout << "\t\t\t----------------------------------------------------------" << endl;
        cout << "\t\t\t                     C U S T O M E R                      " << endl;
        cout << "\t\t\t----------------------------------------------------------" << endl;

        cout << "\n\t\t\tEnter [0] to exit : \n"
             << "\t\t\tEnter [1] to add Contacts :  \n"
            
             << "\t\t\tEnter [2] to See Contacts: \n"
             << "\t\t\tEnter [3] to Call Anyone : \n"
             << "\t\t\tEnter [4] for Messages Log : \n"
             << "\t\t\tEnter [5] to Check your Information : \n"
             << endl;
        cout << "\n\t\tEnter your choice : \n\t\t";
        cin >> c;

        if (c == 1)
        { // Add Contact

            /*
                Add Contact is like the contacts in our mobile phone,
                For this Purpose BST is used, which is arranged on the basis of phone Number, 
                                                    (Phone Number is in String but can be changed to int after 
                                                    forming a substring removing 030 from the list)
                The Contacts will have the number and the name as you prefer to store it in.
                BST.insert() function is used to add in the binary tree which will have root
                The root of Binary  tree will be stored inside the node of customer in linked list as Contact*phoneContacts
                i.e In simple Words, each BST is linked to its Customer Node in Linked List to form a connection
            */


            cout << "\n\t\tEnter Phone Number  : " << endl;
            cout << "\t\t-----------------------\n\t\t";
            string p;
            cin >> p;
            Customer *cu = customers.searchUsingPhone(p);

            if (cu != NULL)
            {
                cout << "\n\t\tEnter Name of Contact : " << endl;
                cout << "\t\t-------------------------\n\t\t";
                string n;
                cin >> n;

                Contacts *firstContact = person->phoneContacts;

                BST cont(firstContact);

                if (firstContact == NULL)
                {
                    person->phoneContacts = cont.insert(p, n);
                }
                else
                {
                    cont.insert(p, n);
                }
            }
            else
            {
                cout << "\n\t\t\tInvalid Number Entered !!! \n\t\t\t!!!! Try Again !!!! " << endl;
            }
        }
        
        else if (c == 2)
        {

            /*
                The elements in BST can be printed by Inorder, So we can call the inorder with root as the address in  phoneContacts of that
                customer node easily.
            */
            cout << "\t\t\t                   --------------------                    " << endl;
            cout << "\t\t\t                   | C O N T A C T S  |                    " << endl;
            cout << "\t\t\t                   --------------------                    " << endl;
            BST cont(person->phoneContacts);

            // Format  Requirred
            cout << "\t\t\t\tName\t\t\tContact Number" << endl;
            cout << "\t\t\t\t-------------------------------------------" << endl;
            cont.inOrder(cont.root);
        }



        else  if (c == 3) // For Calls
        {

            
            /*
                To call, we can display the Existing Contacts, and then the number from the list or not caan be dialied

                When the call forwards, a timer will be calculating the number of seconds which will be spent into the call
                and then that cost will be multiplied by the cost and the distance

                Calculating Distance ::>
                    -> First step, calculate the distance between caller to it's nearest Tower
                    -> Secondly, calculate the distance from reciever to it's nearest Tower
                    -> Thirdly, calculate distance from first tower to Second tower

                        -> For first step, apply dijikstra with the index of first person i.e sender  in graph array, 
                            then find the smallest among the first 5 indices which  represents the towers.
                            Store that cost in a variable as well as the index  of tower
                        
                        -> Second Step will be same apply dijisktra on index  of second person i.e reciever
                            giving cost2 i.e minimum cost for the reciever and the second tower index as well

                        -> Thirdly, the dijisktra will apply on the first tower and the cost of index of second tower will be taken out,
                            which will be the cost3
                
                The calculated cost will be deducted from it's balance
                
            */
            cout << "\t\t\t                   -------------                    " << endl;
            cout << "\t\t\t                   | C A L L S |                    " << endl;
            cout << "\t\t\t                   -------------                    " << endl;

            BST cont(person->phoneContacts);
            cout << "\n\n\t\t\t\tName\t\t\tContact Number" << endl;
            cout << "\t\t\t\t-------------------------------------------" << endl;
            cont.inOrder(cont.root);
            cout << "\n\t\tEnter Contact Number to Call : " << endl;
            cout << "\t\t---------------------------------\n\t\t";
            string p;
            cin >> p;

            Customer *cu = customers.searchUsingPhone(p);
            if (cu != NULL)
            {
                if (person->balance > 0)
                {
                    Call c;
                    int d = c.timer();
                    
                    // Calculate Call Charges

                    
                    int first = towers.getCustomerIndex(person);

                    int second = towers.getCustomerIndex(cu);

                    
                    towers.dijkstra(first);

                    

                    int cost1 = INF;
                    int index1;

                    for (int i = 0; i < 5; i++)
                    {
                        if (towers.finalCost[i] < cost1)
                        {
                            index1 = i;
                            cost1 = towers.finalCost[i];
                        }
                    }

                    towers.dijkstra(second);

                    

                    int cost2 = INF;
                    int index2;
                    for (int i = 0; i < 5; i++)
                    {
                        if (towers.finalCost[i] < cost2)
                        {
                            
                            index2 = i;
                            cost2 = towers.finalCost[i];
                        }
                    }

                    towers.dijkstra(index1);
    

                    int cost3 = towers.finalCost[index2];

                    

                    int finalCostPerCall = cost1 + cost2 + cost3;
                    finalCostPerCall = finalCostPerCall * d * callCharges;
                    person->balance -= finalCostPerCall;
                }
                else{
                    cout << "\n\t\t\tUn-Sufficient Balance !!! \n\t\t\t!!!! Try Again Later !!!! " << endl;
                }
                
            }
            else
            {
                cout << "\n\t\t\tInvalid Number Entered !!! \n\t\t\t!!!! Try Again !!!! " << endl;
            }
        }

        else if (c == 4)
        {
            /*
                You can either send messages or recieve the messages sent by other users
                Both are stated Below
            */
            int c2;
            do
            {
                cout << "\t\t\t                   -------------------              " << endl;
                cout << "\t\t\t                   | M E S S A G E S |              " << endl;
                cout << "\t\t\t                   -------------------              " << endl;

                cout << "\n\t\t\tEnter [0] to exit : \n"
                     << "\t\t\tEnter [1] to Check Messages : \n"
                     << "\t\t\tEnter [2] to Send Messages  : \n"
                     << endl;
                cout << "\n\t\tEnter your choice : \n\t\t";
                cin >> c2;

                if (c2 == 1) // Check New Messages
                 { 
                    /*
                        All the messages sent by different users will be in a queue.

                        In order to Collect all the messages sent to this user, there can be two additional queues.
                            -> First queue will collect the messages for other customers i.e extra messages
                            -> Second will collect messages for this customer stated as the inbox for this customer.

                        Now:
                        if,
                            the inbox queue is Empty there will be no Messages to show;
                        Else,
                            We can dequeue the messages for this User and just empty it as recieving of all messages

                        ~ How can you check if the message is for this user??
                            -> Every Message has a reciever number, sender number and Message to be conveyed
                               To check  the corresponding message is for this user, just match the reciever's phone equal to the original 
                               customer's phone to get to the inbox queue
                        
                        ~ What will be displayed ? 
                            -> The sender's number will be displayed with the message he sent.


                        At the end, the extra list will be replaced as original queue  with remaining messages
                    */
                    queue extra;  
                    queue inbox;

                    while (!messagesQ.isEmpty())
                    {
                        message mess = messagesQ.dequeue();
                        if (mess.recieverPhone == person->phone)
                        {
                            inbox.enqueue(mess);
                        }
                        else
                        {
                            extra.enqueue(mess);
                        }
                    }

                    cout << "\t\t\t                   N E W    M E S S A G E S        " << endl;
                    cout << "\t\t\t                   ------------------------        " << endl;
                    if (!inbox.isEmpty())
                    {

                        while (!inbox.isEmpty())
                        {
                            message mess = inbox.dequeue();
                            cout << "\t\t Sender's Phone : " << mess.senderPhone << endl;
                            cout << "\t\t Message : \n\t\t\t" << mess.m << endl;
                            cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
                        }
                    }
                    else
                    {
                        cout << "\t\t N O   N E W   M E S S A G E S \n"
                             << endl;
                    }

                    messagesQ = extra;
                }
                else if (c2 == 2) // Send Messages
                {

                    /*
                        To Send a Message, it's Similar to thee Call, 
                        Well the message object will be created, and the sender's and reciever's phone will be stored alongwith message  
                        and it will be enqueued to the main messages Queue.
                        
                        The  Cost of message will be calculated as the distance between sender and reciever multiplied by the cost of message

                        The distance between sender and reciever is similar to one in calls as follows ;

                        Calculating Distance ::>
                            -> First step, calculate the distance between caller to it's nearest Tower
                            -> Secondly, calculate the distance from reciever to it's nearest Tower
                            -> Thirdly, calculate distance from first tower to Second tower

                                -> For first step, apply dijikstra with the index of first person i.e sender  in graph array, 
                                then find the smallest among the first 5 indices which  represents the towers.
                                Store that cost in a variable as well as the index  of tower
                            
                                -> Second Step will be same apply dijisktra on index  of second person i.e reciever
                                 giving cost2 i.e minimum cost for the reciever and the second tower index as well

                                -> Thirdly, the dijisktra will apply on the first tower and the cost of index of second tower will be taken out,
                                    which will be the cost3
                        
                        The calculated cost will be deducted from it's balance

                    */
                    

                    BST cont(person->phoneContacts);
                    cout << "\t\t\t                   -------------------                    " << endl;
                    cout << "\t\t\t                   | C O N T A C T S |                    " << endl;
                    cout << "\t\t\t                   -------------------                    " << endl;

                    cout << "\n\n\t\t\t\tName\t\t\tContact Number" << endl;
                    cout << "\t\t\t\t-------------------------------------------" << endl;
                    cont.inOrder(cont.root);
                    cout << "\n\t\tEnter Contact Number to Send Text : " << endl;
                    cout << "\t\t----------------------------------------------\n\t\t";
                    string p;
                    cin >> p;

                    Customer *cu = customers.searchUsingPhone(p);
                    if (cu != NULL)
                    {

                        static char blank[100] = {0};
                        static char m[100] = {0};

                        cout << "\n\t\tEnter Message To Convey : " << endl;
                        cout << "\t\t----------------------------\n\t\t";
                        cin.getline(blank, 100);

                        cin.getline(m, 100);

                        message mess;

                        mess.recieverPhone = cu->phone;
                        mess.senderPhone = person->phone;

                        mess.m = m;

                        
                        double charges;
                        // Calculate SMS Charges
                        int first = towers.getCustomerIndex(person);
                        int second = towers.getCustomerIndex(cu);

                        towers.dijkstra(first);
                        double cost1 = INF;
                        int index1;
                        for (int i = 0; i < 5; i++)
                        {
                            if (towers.finalCost[i] < cost1)
                            {
                                
                                index1 = i;
                                cost1 = towers.finalCost[i];
                            }
                        }

                        towers.dijkstra(second);
                        double cost2 = INF;
                        int index2;
                        for (int i = 0; i < 5; i++)
                        {
                            if (towers.finalCost[i] < cost2)
                            {
                                
                                index2 = i;
                                cost2 = towers.finalCost[i];
                            }
                        }
                        towers.dijkstra(index1);
                        

                        double cost3 = towers.finalCost[index2];

                        charges = cost1 + cost2 + cost3;
                        charges = charges * smsCharges;
                        // person->balance -= charges;
                        if (charges<person->balance)
                        {
                            messagesQ.enqueue(mess);
                            person->balance -= charges;
                        }
                        else{
                            cout << "\n\t\t\tIn-Sufficient Balance !!! \n\t\t\t!!!! Try Again !!!! " << endl;
                        }
                        
                    }
                    else
                    {
                        cout << "\n\t\t\tInvalid Number Entered !!! \n\t\t\t!!!! Try Again !!!! " << endl;
                    }
                }

            } while (c2 == 0);
        }

        else if (c == 5)
        {

            /*
                To display the information of customer, just print out 
                the data members of struct 
            */

            Customer *p = person;
            cout << "\n\tID \t\t | Name \t\t | Phone Number \t\t |  CNIC NUMBER \t\t |  Balance \t\t |  House No./Street No." << endl;
            cout << "\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

            cout << "\t" << p->id << " \t\t | " << p->name << " \t\t | " << p->phone << " \t\t\t | " << p->cnic << " \t\t | " << p->balance << " \t\t | " << p->house << " / " << p->street << endl;
            p = p->next;

            cout << "\t---------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
        }

    } while (c != 0);
}

////////////////////////////////////////////////////// END HERE //////////////////////////////////////////////////////////////////////////////////////