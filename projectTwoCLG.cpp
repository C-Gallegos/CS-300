//Cristian Gallegos
//Oct. 18, 2025
//CS-300

#include <iostream>
#include <string>
#include<vector>
#include<sstream>
#include <fstream>
#include<cstring>
#include<algorithm>
using namespace std;

struct Course
{
	string courseId;
	string title;
	vector <string> prereq;
	Course() {																					//default constructor for courses
		courseId = "";
		title = "";
		prereq = {};
	}

};

struct Node {																					//stuct for treee nodes
	Course course;
	Node* left;
	Node* right;
	Node() {																					//defult node
		left = nullptr;
		right = nullptr;
	}

	Node(Course aCourse) :Node() {																//
		course = aCourse;
	}
};

class BST
{
public:
	BST();
	virtual ~BST();
	void InOrder();
	void Insert(Course course);
	Course Search(string courseId);
	void Destroy(Node* node);
	int size = 0;

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node);

};
void loadCourse(string fileName, BST* bst);
void displayCourse(Course course);
void displayMen();

BST::BST()																					//initialize Binary search tree variables
{
	root = nullptr;
	size = 0;
}

BST::~BST()																					//deconstruct free's up memory
{
	Destroy(root);																			//calls function to remove nodes from tree
}

void BST::InOrder()
{
	if (root==nullptr)																		//checks to see if the tree is empty
	{
		cout << "No courses loaded." << endl<<"Please load courses."<<endl;					//prints to user that there id no courses in tree
	}
	inOrder(root);
}

void BST::Insert(Course course)																//function to insert course to tree
{
	if (root == nullptr)																	//if initial node is empty set a new node with course info to root node
	{
		root = new Node(course);
		size++;
	}
	else
	{
		addNode(root, course);																//else calls add function to add node to the tree passing root node and the course to be added
		size++;
	}
}

Course BST::Search(string courseId)															//search function with the course id we wish to find
{
	Node* curr = root;																		
	while (curr != nullptr) {																//will loop while the current node is not null
		Course currCourse = curr->course;													//new course set to the current nodes course info
		if (currCourse.courseId==courseId)													//if the current course id matches the parameter course id
		{
			return currCourse;																//will return current course
		}
		else
		{																					//else
			if (courseId < currCourse.courseId) {											//if the parameter course id is less than the current course id 
				curr = curr->left;															//set the current node to the current node's left node

			}
			else																			//else
			
			{
				curr = curr->right;															//set current node to current node's right node
			}
		}
	}
	Course course;																			//create empty class
	return course;																			//return empty course if not found
}

void BST::Destroy(Node* node)																//destroy node function parameter is a node
{
	if (node != nullptr)																	//if the node is not null
	{
		Destroy(node->left);																//recursive transverse left node
		Destroy(node->right);																//recursive transverse right node
		delete node;																		// deletes node
	}
}

void BST::addNode(Node* node, Course course)												//add node function 
{
	if (node->course.courseId.compare(course.courseId) > 0)									//
	{
																							// if no left node
		if (node->left == nullptr)
		{
																							// this node becomes left
			node->left = new Node(course);

		}
																							// else recurse down the left node
		else
		{
			this->addNode(node->left, course);
		}
	}
																							// else
	else
	{


																							// if no right node
		if (node->right == nullptr)
		{
																							// this node becomes left
			node->right = new Node(course);

		}

		else
		{
			this->addNode(node->right, course);												//else recursively calls add node function down the right node
		}
		

	}
}

void BST::inOrder(Node* node)																//function to print all courses in order
{
	int itr = 0;																			//iterator for course prerequisite 
	if (node != nullptr)																	//if loop node pass down is not null
	{
		inOrder(node->left);																//recursive call inorder with node's left node
		cout << node->course.courseId << ", " << node->course.title << " | ";				//print out current node's course id and title
		if (node->course.prereq.empty())													// if the node's vector for prerequisite			
		{
			cout << "No prerequisites for course" << endl;									
		}
		else																				// else print off each prerequisite in the node's prerequisite
		{
			for (auto i = node->course.prereq.begin(); i < node->course.prereq.end(); i++)
			{
				cout << node->course.prereq[itr] << " "; itr++;
			}
			cout << endl;
		}
		inOrder(node->right);																//recursive call inorder with node's right node
	}
}

void loadCourse(string fileName, BST* bst)													//function to load csv courses into tree of course
{
	Course course;																			//create temp course vector 
	ifstream input;																			
	input.open(fileName);																	//opens file


	if (input.fail())																		//if  file fails to open gives user an error
	{
		cout << "Fail fail to open." << endl;
	}
	else																					//else
	{
		string line = "";																	//empty string variable 
		while (getline(input, line)) {														// while eof
			string lineCourseId = "";														//string variable for course id 
			string lineTitle = "";															//string variable for title
			string temp = "";																
			vector<string> linePrereq;														//vector for each lines prerequisites

			stringstream inputString(line);													//get line from file
			getline(inputString, lineCourseId, ',');										//get	course id from line
			course.courseId = lineCourseId;													//set the line course id into course struct course id
			getline(inputString, lineTitle, ',');											//get title from line
			course.title = lineTitle;														//set the line's title into the course title
			if (lineCourseId.compare("") == 0 || lineTitle.compare("") == 0)				//if less than two variables then print error and course not added
			{
				cout << endl << line << "Less than two values, course was not added." << endl;
				continue;
			}
			while (getline(inputString, temp, '\n')){										//while not new line
				stringstream pre(temp);														// saves prereq into temp
				while (getline(pre, temp, ',')) {											//spits temp eaach of its prereqs
					course.prereq.push_back(temp);											//pushes each prereq into the coures vector
				}
			}
			bst->Insert(course);															//calls the insert function passes the course into the tree

			line = "";																		//clears the line variable and vector
			course.prereq.clear();

		}
	}
	input.close();																			//closes file
}

void displayCourse(Course course)														//display function with course parameter
{
	int itr = 0;																		//iterator variable	
	cout << course.courseId << ", " << course.title << endl;							// print course id and title
	cout << "Prerequisites: ";
	if (course.prereq.empty())															//if to chek to see if prerequisite vector is empty 
	{
		cout << "course does not have prerequisite";
	}
	else																				//else prints each prerequisite in vector
	{
		for (auto i = course.prereq.begin(); i < course.prereq.end(); i++)
		{
			cout << course.prereq[itr] << " ";
			itr++;
		}
	}
	cout << endl << endl;
}
void displayMen() {																		//display menu function

	cout << "\t1. Load Data Structure." << endl <<
		"\t2. Print Course List." << endl <<
		"\t3.Print course info." <<endl<<
		"\t9.Exit program." << endl <<
		"\tEnter choice: ";
}
int main() {

	string csvPath, key;
	
	csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
	
	BST* bst;																						//crate new binary search tree
	bst = new BST();
	Course  course;																					//creates course variable

	int choice = 0;																					
	while (choice != 9) {																			//while the user does not select exit option

		displayMen();																				//display menu
		cin >> choice;																				//saves user choice
		cout << endl;
		switch(choice){																				//switch 
		case 1:																						//loads courses from the csv file
			loadCourse(csvPath, bst);
			cout << "Courses have been loaded." << endl<<endl;
			break;
		case 2:																						//print each course in order from tree
			bst->InOrder();
			break;
		case 3:																						//asks user for course 
			cout << "What course do you want to know about? ";
			cin >> key;
			cout << endl;
			transform(key.begin(), key.end(), key.begin(), toupper);								//makes sure the key is all upper case
			course = bst->Search(key);																//searches for user course

			if (!course.courseId.empty()) {															//if course is not empty print course info
				displayCourse(course);
			}
			else																					//else print course not found
			{
				cout << "Course " << key << " not found." << endl;
			}
			break;
		case 9:																						//exit option

			cout << "Goodbye." << endl;
			break;
		default:																					// all other choices not a valid choices
			cout << "Not a valid choice." << endl;

		}
	}
	return 0;
}