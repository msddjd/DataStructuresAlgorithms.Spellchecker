using namespace std;
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "dictionary.h"
#include <stdlib.h>
#include "BinarySearchTree.h"
using namespace boost;

void load_dict(Dictionary <string> & cambridge);
void check_field(vector <string> &fields, Dictionary <string> & cambridge,ofstream &outfile);
void print(vector <string> & v);
bool inhash(Dictionary <string> & cambridge, string word);
void add_2_tree(string word);
void menu(string word,Dictionary <string> & cambridge, ofstream &outfile);
void swap(string word,Dictionary <string> & cambridge);
int linenumb = 1;

//Struct for adding misspelled words
struct BNode
{
	string word;
	int lineNum;
	bool operator < (BNode otherStruct) const
	{
		return ( word < otherStruct.word );
	}
};
typedef BinarySearchTree<BNode> BST;

BNode bN = {"",0}; //Creating Dummy Node
BST myBST(bN); //Initializing myBST with Dummy Node
Dictionary <string> ignored_words ("ZZZ",1000); //For Ignored Words

int main(int argc, char *argv[] )
{

		

	if(argc<2)
	{cout << "Invalid Entry" << endl;
	return 0;}
	
	ifstream infile;
	ofstream outfile;
	outfile.open("dict.txt", ios_base::app); //Opens the file without deleting it
	infile.open( argv[1] );	
	string content = "";
	vector <string> fields;	
	Dictionary <string> cambridge ("ZZZ",1000);
	load_dict(cambridge);

	getline(infile,content);	
	while(infile)
	{
		split( fields, content, is_any_of(" ") );
		check_field(fields,cambridge,outfile);
		getline(infile,content);
		linenumb++;

	}	
	infile.close();

	//Write Notfound Words to notfound.txt
	outfile.open("notfound.txt");
	myBST.printTree( outfile );
	outfile.close();

	cout << "\n********Program has reached the end of the file********\n\nNow Exiting" << endl;
	
	return 0;
}

void load_dict(Dictionary <string> & cambridge)
{
	ifstream infile;
	//Open Dictionary File
	infile.open("dict.txt");
	string content = "";
	

	getline(infile,content);
	while(infile)
	{
		
		cambridge.insert(content); //Inserts each word into the dictionary
		getline(infile,content);

	}

	infile.close();
}

void check_field(vector <string> &fields, Dictionary <string> & cambridge, ofstream &outfile)
{
	string stemp = "";
	bool isword = false;
	char f_char,l_char;

	for(size_t n = 0; n < fields.size(); n++)
	{
		stemp = fields[n];

		cout << "Line:" << linenumb  << "\tField :\t\t" << fields[n] << "\t\tCase:\t";
	
		//string must have a length greater than or equal to 1 to be a word
		if(stemp.length() >= 1 )
		{

				f_char = stemp.at(0); 			//Taking first char
				l_char = stemp.at(stemp.length()-1); 	//Taking the last char
				if( ( f_char <= 'z' && f_char >= 'a' ) || ( f_char <= 'Z' && f_char >= 'A' ) ) //Checking if first char is a letter
				 {
					isword = true;
					cout << "Is Word:\t\t"; //If both conditions are true its word
					
					if( ( l_char <= 'z' && l_char >= 'a' ) || ( l_char <= 'Z' && l_char >= 'A' ) ) //Checking if last char is a letter
					{	isword = true;	}
					else	 //If the first char is a letter but the last is not chop off last char
					{	 
						stemp = stemp.substr(0,stemp.length()-1); //Are temp word is now a word
						isword = true;
						cout << "New Word:\t" << stemp << "\t";
					}	
				 }
				else //If first char was not a letter check last char
				if( ( l_char <= 'z' && l_char >= 'a' ) || ( l_char <= 'Z' && l_char >= 'A' ) ) //if first char is not a letter but last char is a letter chop off first char
				{

						stemp = stemp.substr(1,stemp.length()-1); //Are temp word is now a word
						isword = true;
						cout << "New Word:\t" << stemp << "\t";

				}

			if(!isword) 
			{cout << "Not Word" << endl;
			}


			//If the temp string is a word, check if its in the hash
			if(isword)
			{
			//Convert the string to lowercase before checking the hash
			stemp = to_lower_copy(stemp);
	
			 if(inhash(cambridge,stemp))
				cout << "found in hash" << endl;
			  else
				{	cout << "Not found in hash" << endl;
					add_2_tree(stemp);	 //Adding not found word to binary search tree
					menu(stemp,cambridge,outfile);
				}
			}
			isword = false;	//Reset Condition for next word
		
		}
		else
		cout << "Not Word" << endl;



	
	}
}

void add_2_tree(string word)
{
	BNode bN = {word, linenumb};
	myBST.insert(bN);
}




void print(vector <string> & v)
{
	for(size_t n = 0; n < v.size(); n++)
	cout << "\"" << v[n] << "\"\n";
	cout << endl;
}

bool inhash(Dictionary <string> & cambridge, string word) //Takes in a dictionary and the word to be checked
{
	
	if( ( cambridge.lookup(word) == word ) || ( ignored_words.lookup(word) == word) )//If in our hash
	return true;
	else
	return false;

}

void menu(string word,Dictionary <string> & cambridge, ofstream &outfile)
{

	system("clear"); 

	cout << "---------\tTHE SPELL CHECKER PROGRAM\t---------\n" << endl;
	
	cout << word << " On Line " << linenumb << " Was Not Found In Dictionary\n" << endl;

	cout <<  "A)\tAdd the Word To Dictionary\n"
	     	 "I)\tIgnore Word, and Skip Future References\n"
		 "G)\tGo On To Next Word\n"
		 "S)\tSearch For A Suggested Spelling\n"
		 "Q)\tQuit Spell Checking File\n\n";

	cout << "Selection : ";

	char choice;
	choice = cin.get();
	choice = toupper(choice);
	cin.get();
	

	switch( choice )
	{
		case 'A':
		cambridge.insert(word);
		outfile << word << endl;	
		cout << "\nThe Word " << word << " Has Been Successfully Inserted" << endl;		
		cout << "Press Enter to Continue..." << endl;
		cin.get();
		break;
	
		case 'I':
		ignored_words.insert(word);
		cout << "\nThe Word was Flagged" << endl;		
		cout << "Press Enter to Continue..." << endl;
		cin.get();
		break;

		case 'G':
		break;

		case 'S':
		swap(word,cambridge);
		cin.get();
		menu(word,cambridge,outfile);
		break;

		case 'Q':
		cout << "\nNow Exiting, Adios Flamingo...\n" << endl;
		exit(1);
		break;

		default : 
		cout << "Selection Error -- Please Try Again" << endl;
		cin.get();
		menu(word,cambridge,outfile);
		break;

	}

 
}

void swap(string word, Dictionary <string> & cambridge)
{
	string temp = word;
	string c;
	int i;
	bool found = false;

	for(i =0; i < word.length()-1; i++)
	{	c = word.at(i); 	//Take first char
		temp.erase(i,1);	//delete first char in temp string
//		cout << temp << endl;
		temp.insert(i+1,c); 	//Insert the char at right spot
//		cout << temp << endl;
//		cout << endl;

		//Check if newword is in hash
		if(inhash( cambridge,temp) )
		{	cout << "Suggested Spelling :\t" << temp  << endl << endl;
			i = word.length();	//Force Loop to stop
			found = true;	
		}
		
		temp = word; //Resetting temp
 	}

	if(!found)
	{ cout << "No Correct Spelling was found...\n" << endl;
	}

	  cout << "Press Enter to Return to Menu..." << endl;
	
}
