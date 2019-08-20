#include "SinglyLinkedList.h" 
#include "Stacks.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


int main(int argc, char *argv[]) {
	string filename;
	if (argc >= 2) {
		filename = argv[1]; // command line arguments are given. The first argument is the filename
	}
	else {
		cout << "Enter name of file to check: ";
		cin >> filename;
	}
	cout << "File to check: " << filename << endl;

	ifstream infile(filename);
	if (!infile) {
		cout << "That file does not exist.\n";
		return (-1); // end the program
	}

	LinkedStack<char> Stack;

	int line_num = 1;
	string line;
	char c;
	bool no_mismatch = true;

	while (getline(infile, line)) {
		size_t found_comment = line.find("//");
		size_t found_quote = line.find_first_of('"');
		size_t brace_pos1 = line.find_last_of('}');
		size_t brace_pos2 = line.find_last_of(']');
		size_t brace_pos3 = line.find_last_of(')');
		//Found comment before end of string so truncate comment
		if (found_comment != string::npos) {
			//cout << "Comment found on line " << line_num << " at position " << found_comment << "\nResizing string\n";
			line.resize(found_comment);
			//cout << line << endl;
		}
		//Found " before end of string
		if (found_quote != string::npos) {
			//cout << "Quote found on line " << line_num << " at position " << found_quote << "\n";
			line.erase(found_quote, (line.find_last_of('"') - found_quote + 1)); //Erase characters from start of " until end quote + 1 character since last character isnt deleted
			//Display if whole line wasn't comment
			//cout << line << endl;
		}
		int index = 0; //Char index for string
		while (index < line.length()+1) {
			c = line[index];
			if ('{' == c || '[' == c || '(' == c) {
				Stack.push(c);
				//cout << "Pushed: " << c << "\n";
			}
			if ('}' == c) {
				if ('{' == Stack.peek()) {
					assert(!Stack.is_empty());
					Stack.pop();
					//cout << "Popped: " << c << "\n";
				}
				else {
					no_mismatch = false;
					//cout << static_cast<int>(line.length()) << endl; //Display line length
					cout << "There is a mismatched " << c << " on line " << line_num << " at character " << brace_pos1 + 1 << ".\n";
				}
			}
			if (']' == c) {
				if ('[' == Stack.peek()) {
					assert(!Stack.is_empty());
					Stack.pop();
					//cout << "Popped: " << c << "\n";
				}
				else {
					no_mismatch = false;
					//cout << static_cast<int>(line.length()) << endl; //Display line length
					cout << "There is a mismatched " << c << " on line " << line_num << " at character " << brace_pos2 + 1 << ".\n";
				}
			}
			if (')' == c) {
				if ('(' == Stack.peek()) {
					assert(!Stack.is_empty());
					Stack.pop();
					//cout << "Popped: " << c << "\n";
				}
				else {
					no_mismatch = false;
					//cout << static_cast<int>(line.length()) << endl; //Display line length
					cout << "There is a mismatched " << c << " on line " << line_num << " at character " << brace_pos3 + 1 << ".\n";
				}
			}
			index++; //Increment index to get next char in string
		}
		line_num++; //Increment current line number
	}
	if (Stack.is_empty() && no_mismatch) {
		cout << "All braces were matched and stack is empty.\n";
	}
	else if(!Stack.is_empty()){
		cout << "Stack has a mismatched brace.\n" << "The stack has \n";
		while (!Stack.is_empty()) {
			cout << Stack.peek() << " ";
			Stack.pop();
		}
		cout << "\nleft on it.\n";
	}
	infile.close();

	return 0;
}
