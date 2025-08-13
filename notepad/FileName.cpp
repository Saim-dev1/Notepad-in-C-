#include <conio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Stack {
private:
	struct Node {
		string data;
		Node* next;
	};

	Node* top;

public:
	Stack() {
		top = nullptr;
	}

	void push(string value) {
		Node* newNode = new Node;
		newNode->data = value;
		newNode->next = top;
		top = newNode;
	}

	void pop() {
		if (top != nullptr) {
			Node* temp = top;
			top = top->next;
			delete temp;
		}
	}

	string peek() {
		if (top != nullptr) {
			return top->data;
		}

		cout << "Stack is empty";
		return "";
	}

	bool isEmpty() {
		return !top;
	}

	void clear() {
		while (top != nullptr) {
			pop();
		}
	}
};

class TextEditor {
private:
	Stack undoStack;
	Stack redoStack;
	string data;
	int cursorPos;
	int last_inserted_text_size;
	string currentFilename; 

public:
	TextEditor() {
		cursorPos = 0;
		last_inserted_text_size = 0;
	}

	void displayText();
	void insertCharacter(string c);
	void deleteCharacter();
	void undo();
	void redo();
	void searchQuery(string query);
	void replace(string query, string replacement);
	void saveFile(string filename);
	void openFile(string filename);
	void newFile();
	void editCurrentFile();
	void deleteCurrentFile();
	void manageScreenSize();
	void copyFile(const string& sourceFilename, const string& destinationFilename);
	void moveCursorLeft(); 
	void moveCursorRight(); 

	~TextEditor() {}
};

void TextEditor::displayText() {
	cout << data;
	cout << endl;
	for (int i = 0; i < cursorPos; i++) {
		cout << " ";
	}
	cout << "^";
}

void TextEditor::insertCharacter(string c) {
	string text_before_cursor = data.substr(0, cursorPos);
	string text_after_cursor = data.substr(cursorPos, data.length() - cursorPos);
	data = text_before_cursor + c + text_after_cursor;
	cursorPos += c.length();

	last_inserted_text_size = c.length();

	undoStack.push(data);
	redoStack.clear();
}

void TextEditor::deleteCharacter() {
	if (cursorPos == 0) {
		return;
	}

	string before = data.substr(0, cursorPos - last_inserted_text_size);
	string after = data.substr(cursorPos, data.length() - cursorPos);
	data = before + after;
	cursorPos = cursorPos - last_inserted_text_size;

	undoStack.push(data);
	redoStack.clear();
}

void TextEditor::openFile(string filename) {
	ifstream file(filename);
	if (file.is_open()) {
		getline(file, data);
		cursorPos = data.length();
		undoStack.clear();
		redoStack.clear();
		last_inserted_text_size = 0;
		currentFilename = filename;
		cout << "\n\n\tFile opened successfully ..." << endl;
		file.close();
	}
	else {
		cout << "\n\nUnable to open a file\n";
	}
}

void TextEditor::newFile() {
	data = "";
	cursorPos = 0;
	undoStack.clear();
	redoStack.clear();
	last_inserted_text_size = 0;
	currentFilename = ""; 
}

void TextEditor::saveFile(string filename) {
	ofstream out(filename);
	out << data;
	out.close();
	currentFilename = filename;
	cout << "\n\n\tFile saved successfully." << endl;
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tPress any key to continue ... ";
	_getch();
}

void TextEditor::replace(string query, string replacement) {
	size_t found = data.find(query);

	if (found != string::npos) {
		data.replace(found, query.length(), replacement);

		cursorPos = data.length();

		undoStack.push(data);
		redoStack.clear();
		cout << "\n\n\tReplacement Successful ... ";
	}
	else {
		cout << "\n\n\tReplacement Failed ... ";
	}
}

void TextEditor::searchQuery(string query) {
	cursorPos = 0;

	size_t found = data.find(query, cursorPos);
	if (found != string::npos) {
		cursorPos = found + query.length();
		cout << "\n\n\tFound at position " << cursorPos << endl;
	}
	else {
		cout << "\n\n\tNot Found" << endl;
	}

	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tPress any key to continue ... ";
	_getch();
}

void TextEditor::undo() {
	if (!undoStack.isEmpty()) {
		redoStack.push(data);
		data = undoStack.peek();
		undoStack.pop();
		cursorPos = data.length();
	}
}

void TextEditor::redo() {
	if (!redoStack.isEmpty()) {
		undoStack.push(data);
		data = redoStack.peek();
		redoStack.pop();
		cursorPos = data.length();
	}
}

void TextEditor::editCurrentFile() {
	char option;
	do {
		cout << "\nFile Editing Menu:\n";
		cout << "1. Insert Character\n";
		cout << "2. Delete Character\n";
		cout << "3. Move Cursor Left\n";
		cout << "4. Move Cursor Right\n";
		cout << "5. Display Text\n";
		cout << "6. Exit Editing\n";
		cout << "Enter option: ";
		cin >> option;

		switch (option) {
		case '1': {
			string text;
			cout << "Enter text to insert: ";
			cin.ignore();
			getline(cin, text);
			insertCharacter(text);
			break;
		}
		case '2':
			deleteCharacter();
			break;
		case '3':
			moveCursorLeft();
			break;
		case '4':
			moveCursorRight();
			break;
		case '5':
			displayText();
			break;
		case '6':
			cout << "Exiting File Editing.\n";
			break;
		default:
			cout << "Invalid option. Please try again.\n";
		}

	} while (option != '6');
}

void TextEditor::deleteCurrentFile() {
	if (!currentFilename.empty()) {
		if (remove(currentFilename.c_str()) == 0) {
			cout << "File deleted: " << currentFilename << endl;
			newFile();
		}
		else {
			cout << "Unable to delete the file." << endl;
		}
	}
	else {
		cout << "No current file to delete." << endl;
	}
}

void TextEditor::manageScreenSize() {

	// This may not work on all systems and environments
	system("CLS");
	cout << "Screen cleared." << endl;
}

void TextEditor::copyFile(const string& sourceFilename, const string& destinationFilename) {
	ifstream sourceFile(sourceFilename);
	ofstream destinationFile(destinationFilename);
	if (sourceFile.is_open() && destinationFile.is_open()) {
		char ch;
		while (sourceFile.get(ch)) {
			destinationFile << ch;
		}
		sourceFile.close();
		destinationFile.close();
		cout << "File copied from " << sourceFilename << " to " << destinationFilename << endl;
	}
	else {
		cerr << "Unable to open the files for copying." << endl;
	}
}

void TextEditor::moveCursorLeft() {
	if (cursorPos > 0) {
		cursorPos--;
	}
}

void TextEditor::moveCursorRight() {
	if (cursorPos < data.length()) {
		cursorPos++;
	}
}

int main() {
	TextEditor textEditor;

	do {
		system("cls");
		int choice;
		cout << "---------------------------------------------------------------------------------------------------------------------\n\n";
		cout << "                                                  Text Editor\n\n";
		cout << "---------------------------------------------------------------------------------------------------------------------\n\n";
		textEditor.displayText();
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tPress any key to continue ... ";
		_getch();
		system("cls");
		cout << "---------------------------------------------------------------------------------------------------------------------\n\n";
		cout << "                                      Welcome to the text editor main menu\n\n";
		cout << "---------------------------------------------------------------------------------------------------------------------\n\n\n\n";
		cout << "                                          1. Insert Character\n";
		cout << "                                          2. Delete Character\n";
		cout << "                                          3. Undo\n";
		cout << "                                          4. Redo\n";
		cout << "                                          5. Save text to file\n";
		cout << "                                          6. Open text to file\n";
		cout << "                                          7. Create a new empty file\n";
		cout << "                                          8. Query Replacement\n";
		cout << "                                          9. Search Query\n";
		cout << "                                          10. Edit Current File\n";
		cout << "                                          11. Delete Current File\n";
		cout << "                                          12. Manage Screen Size\n";
		cout << "                                          13. Copy File\n";
		cout << "                                          14. Quit\n\n\n";
		cout << "---------------------------------------------------------------------------------------------------------------------\n";
		cout << "Please enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1: {
			system("cls");
			cout << "------------------------------------------------------------------------------------------------------------------------\n\n\n";
			cout << "                                                 Insert Character\n\n\n";
			cout << "------------------------------------------------------------------------------------------------------------------------\n\n\n";
			cout << "\n\n\tPlease enter the character you want to insert: ";
			cin.ignore();
			string inputC;
			getline(cin, inputC);
			textEditor.insertCharacter(inputC);
			system("cls");
			textEditor.displayText();
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tPress any key to continue ... ";
			_getch();
			break;
		}
		case 2:
			textEditor.deleteCharacter();
			break;
		case 3:
			textEditor.undo();
			break;
		case 4:
			textEditor.redo();
			break;
		case 5: {
			system("cls");
			string filename;
			cout << "\n\n\tPlease enter the filename to save: ";
			cin >> filename;
			textEditor.saveFile(filename);
			break;
		}
		case 6: {
			system("cls");
			string filename;
			cout << "\n\n\tPlease enter the filename to open: ";
			cin >> filename;
			textEditor.openFile(filename);
			textEditor.displayText();
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tPress any key to continue ... ";
			_getch();
			break;
		}
		case 7:
			textEditor.newFile();
			break;
		case 8: {
			string query, replacement;
			system("cls");
			cout << "\n\n\tPlease enter the query to replace: ";
			cin >> query;
			cout << "\n\n\tPlease enter the replacement: ";
			cin.ignore();
			getline(cin, replacement);
			textEditor.replace(query, replacement);
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tPress any key to check Replacement ... ";
			_getch();
			system("cls");
			textEditor.displayText();
			break;
		}
		case 9: {
			string srchQuery;
			system("cls");
			cout << "\n\n\tPlease enter your query: ";
			cin >> srchQuery;
			textEditor.searchQuery(srchQuery);
			break;
		}
		case 10:
			textEditor.editCurrentFile();
			break;
		case 11:
			textEditor.deleteCurrentFile();
			break;
		case 12:
			textEditor.manageScreenSize();
			break;
		case 13: {
			string source, destination;
			system("cls");
			cout << "\n\n\tEnter source filename: ";
			cin >> source;
			cout << "\n\n\tEnter destination filename: ";
			cin >> destination;
			textEditor.copyFile(source, destination);
			break;
		}
		case 14:
			cout << "Program closed.\n";
			exit(0);
			break;
		default:
			cout << "Invalid option. Please try again.\n";
		}

	} while (true);

	system("pause");
	return 0;
}
