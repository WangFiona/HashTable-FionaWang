#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

/*
 * Author: Fiona Wang
 * Date: March, 16, 2022
 * This program uses a hash table to store a database of students
 */

//Struct for student
struct Student {
  //Student variables
  char fname[15];
  char lname[15];
  int id;
  float gpa;
public:
  //Constructors
  Student(){}
  Student(char* newFName, char* newLName, int newID, float newGPA){
    strcpy(fname, newFName);
    strcpy(lname, newLName);
    id = newID;
    gpa = newGPA;
  }
};

//Class for the nodes used in the hash table
class HNode {
  Student* data;
  HNode* next;
  
public:
  //Constructor
  HNode(Student* newData){
    data = newData;
    next = NULL;
  }

  //Set and get functions
  void setNext(HNode* newNext){
    next = newNext;
  }

  HNode* getNext(){
    return next;
  }

  void setData(Student* newStudent){
    data = newStudent;
  }

  Student* getStudent(){
    return data;
  }
};

//Initializing functions
void rehash(HNode** &hTable, int &size);
void print(HNode* head, int &printC);
void generate(int* &idArray, int number, int &random, int &idRand, int &size, HNode** &hTable);
int hashFunction(int data, int size);
void addStudent(int &size, Student* student, HNode** head, HNode** &hTable);
void deleteStudent(int size, int id, HNode** &hTable);

int main(){
  //Initializing variables
  int size = 100;
  int id = 0;
  int random = 0;
  HNode** hTable = new HNode*[size];
  int* idArray = new int[1000];

  //Clearing out arrays for keeping track of IDs and students
  for(int i=0; i<1000; i++){
    idArray[i] = 0;
  }
  for(int i=0; i<size; i++){
    hTable[i] = NULL;
  }

  //More variables
  bool running = true;
  bool found=false;
  char command[10];
  char ADD[] = "ADD";
  char PRINT[] = "PRINT";
  char DELETE[] = "DELETE";
  char GENERATE[] = "GENERATE";
  char QUIT[] = "QUIT";
  bool ifRehash = false;
  
  //Ask the user for a command
  cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
  while (running == true) {
    cin >> command;

    //Allow both upper and lower case
    for(int i=0; i < strlen(command); i++) {
      command[i] = toupper(command[i]);
    }

    //If the user wants to add
    if(strcmp(command, ADD) == false){
      char* newF = new char[15];
      cout << "Enter a first name:" << endl;
      cin >> newF;

      char* newL = new char[15];
      cout << "Enter a last name:" << endl;
      cin >> newL;

      int nID = 0;
      cout << "Enter a unique ID:" << endl;
      cin >> nID;
      //Check if the ID is unique
      while(idArray[nID]==1){
	cout << "Enter a different ID:" << endl;
	cin >> nID;
      }
      idArray[nID] = 1;

      float nGPA = 0;
      cout << "Enter a GPA:" << endl;
      cin >> nGPA;

      //Add the student into the database
      Student* student = new Student(newF, newL, nID, nGPA);
      addStudent(size, student, &(hTable[hashFunction(student->id, size)]), hTable);
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
    }
    //If the user wants to print the database
    else if(strcmp(command, PRINT) == false){
      int printC=0;
      for(int i=0; i<size; i++){
	print(hTable[i],printC);
      }
      cout << "Number of Students: " << printC << endl;
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
    }
    //If the user wants to delete a student
    else if(strcmp(command, DELETE) == false){
      cout << "Enter the ID you want to delete:" << endl;
      int target = 0;
      cin >> target;
      deleteStudent(size, target, hTable);
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
    }
    //If the user wants to generate random students
    else if(strcmp(command, GENERATE) == false){
      generate(idArray, 0, random, id, size, hTable);
      cout << "Enter a command (ADD, PRINT, DELETE, GENERATE, or QUIT):" << endl;
    }
    //If the user wants to quit the program
    else if(strcmp(command, QUIT) == false){
      running = false;
    }
    else{
      cout << "Please enter a valid command:" << endl;
    }
  }
  return 0;
}

//Function for rehashing the table
void rehash(HNode** &hTable, int &size){
  int count=0;
  //Create a new table, double the size
  HNode** newHTable = new HNode*[size*2];
  for(int i=0; i<size*2; i++){
    newHTable[i] = NULL;
  }
  //Go through the old table and rehash into the new
  for(int i=0; i < size; i++){
    HNode* temp = hTable[i];
    while(temp != NULL){
      addStudent(size, temp->getStudent(),
	     &(newHTable[hashFunction(temp->getStudent()->id, size*2)]), hTable);
      temp = temp->getNext();
      count++;
    }
  }
  //Set the new table to the old one
  size *= 2;
  for(int i=0; i<size/2; i++){
    hTable[i]->~HNode();
  }
  hTable = new HNode*[size];
  hTable = newHTable;
  cout << "Rehashed!" << endl;
}

//Function for printing the database
void print(HNode* head, int &printC){
  //If there are students in the table, print
  if(head){
    cout << head->getStudent()->fname << " " << head->getStudent()->lname
	 << ", ID: " << head->getStudent()->id;
    cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    cout.precision(2);
    cout << ", GPA: " << head->getStudent()->gpa << endl;
    printC++;
    //Recursion
    print(head->getNext(), printC);
  }
}

//Function to generate random students
void generate(int* &idArray, int number, int &random,  int &idRand, int &size, HNode** &hTable){
  //Determine how many students to generate
  srand(time(NULL));
  cout << "How many students do you want to generate?" << endl;
  cin >> number;
  int	lineNum	= 0;

  //Create random students
  for(int i=0; i<number; i++){
    fstream FNfile;
    fstream LNfile;
    FNfile.open("fname.txt");
    LNfile.open("lname.txt");

    lineNum = 0;
    int oldR = random;
    while(oldR == random){
      random = rand() % 1001; 
    }
    Student* randStudent = new Student();
    char* firstN = new char[50];
    char* lastN = new char[50];
    while(FNfile >> randStudent->fname && LNfile >> randStudent->lname){
      if(lineNum == random) break;
      lineNum++;
    }
    idRand++;
    while(idArray[idRand]==1){
      idRand++;
    }
    idArray[idRand]=1;
    randStudent->id = idRand;
    double gpaRand = (double)(rand()%500-0)/100;
    randStudent->gpa = gpaRand;
    //Add student to the database
    addStudent(size, randStudent, &(hTable[hashFunction(randStudent->id, size)]), hTable); 
  }
  cout << "Generated!" << endl;
}

//Simple function to hash the students
int hashFunction(int data, int size){
  return data % size;
}

//Function for adding into the database
void addStudent(int &size, Student* student, HNode** head, HNode** &hTable){
  HNode* temp = NULL;
  temp = new HNode(student);
  temp->setNext(*head);
  *head = temp;

  //Check and rehash if neccessary
  if((*head)->getNext() != NULL && (*head)->getNext()->getNext() != NULL
     && (*head)->getNext()->getNext()->getNext() != NULL){
    rehash(hTable, size);
  }
}

//Function for deleting students
void deleteStudent(int size, int id, HNode** &hTable){
  //Search for the right ID
  for(int i=0; i<size; i++){
    if(hTable[i]){
      HNode* prev = NULL;
      HNode* head = hTable[i];
      HNode* first = hTable[i];
      //If the student is at the beginning of the list
      if(first->getStudent()->id == id){
	HNode* temp = first;
	first = first->getNext();
	temp->~HNode();
	hTable[i] = first;
      }
      else{
	while(head->getNext() != NULL && head->getStudent()->id != id){
	  prev = head;
	  head = head->getNext();
	}
	//If the student is anywhere else
	if(id == head->getStudent()->id){
	  prev->setNext(head->getNext());
	  head->~HNode();
	}
      }
    }
  }
  cout << "Deleted!" << endl;
}
