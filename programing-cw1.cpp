#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits> // Include the <limits> header for numeric_limits

using namespace std;

// Forward declaration of the signup function
void signup();

// Function to encrypt plaintext using Caesar Cipher
string encrypt(string text, int shift) {
    string result = "";
    for (char& c : text) {
        if (isalpha(c)) {
            char shifted = (isupper(c)) ? 'A' : 'a';
            result += char((int(c - shifted + shift) % 26) + shifted);
        }
        else {
            result += c;
        }
    }
    return result;
}

// Function to decrypt ciphertext using Caesar Cipher
string decrypt(string text, int shift) {
    return encrypt(text, 26 - shift);
}

// Function to generate a random password
string generatePassword(int length) {
    const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+";
    string password = "";
    for (int i = 0; i < length; ++i) {
        password += charset[rand() % charset.length()];
    }
    return password;
}

// Function to validate password requirements
bool validatePassword(const string& password) {
    bool hasUpperCase = false;
    bool hasSpecialChar = false;
    if (password.length() < 8)
        return false;
    for (char c : password) {
        if (isupper(c))
            hasUpperCase = true;
        if (!isalnum(c) && !isspace(c))
            hasSpecialChar = true;
    }
    return hasUpperCase && hasSpecialChar;
}

// Function to validate email format
bool validateEmail(const string& email) {
    size_t atPosition = email.find('@');
    size_t dotPosition = email.rfind('.');
    return (atPosition != string::npos && dotPosition != string::npos && dotPosition > atPosition);
}

// Structure to hold a password entry
struct PasswordEntry {
    string website;
    string username;
    string password;
};

// Function to add a new password entry
void addPassword(vector<PasswordEntry>& passwords) {
    PasswordEntry newEntry;
    cout << "Enter website: ";
    cin >> newEntry.website;
    cout << "Enter username: ";
    cin >> newEntry.username;
    cout << "Enter password: ";
    cin >> newEntry.password;
    passwords.push_back(newEntry);
    cout << "Password added successfully." << endl;
}

// Function to view all password entries
void viewPasswords(const vector<PasswordEntry>& passwords) {
    if (passwords.empty()) {
        cout << "No passwords stored." << endl;
        return;
    }

    cout << "Your passwords:" << endl;
    for (int i = 0; i < passwords.size(); ++i) {
        cout << i + 1 << ". Website: " << passwords[i].website << ", Username: " << passwords[i].username << ", Password: " << passwords[i].password << endl;
    }
}

// Function to modify a password entry
void modifyPassword(vector<PasswordEntry>& passwords) {
    viewPasswords(passwords);
    int choice;
    cout << "Enter the number of the password you want to modify: ";
    cin >> choice;
    if (choice < 1 || choice > passwords.size()) {
        cout << "Invalid choice." << endl;
        return;
    }
    cout << "Enter new password: ";
    cin >> passwords[choice - 1].password;
    cout << "Password modified successfully." << endl;
}

// Function to handle user login
void login() {
    string email, password;
    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter your password: ";
    cin >> password;

    ifstream infile("credentials.txt");
    if (!infile) {
        cout << "No users signed up yet." << endl;
        return;
    }

    string storedEmail, storedPassword;
    bool found = false;
    while (infile >> storedEmail >> storedPassword) {
        if (decrypt(storedEmail, 3) == email && decrypt(storedPassword, 3) == password) {
            found = true;
            break;
        }
    }

    infile.close();

    if (!found) {
        cout << "Invalid email or password." << endl;
        return;
    }

    // User authenticated, provide password management options
    vector<PasswordEntry> passwords;
    int option;
    while (true) {
        cout << "\nPassword Management:\n"
            << "1. Add New Password\n"
            << "2. View Passwords\n"
            << "3. Modify Password\n"
            << "4. Exit\n"
            << "Choose an option: ";
        cin >> option;

        if (cin.fail()) { // Check if input extraction failed
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            continue; // Restart the loop to prompt again
        }

        switch (option) {
        case 1:
            addPassword(passwords);
            break;
        case 2:
            viewPasswords(passwords);
            break;
        case 3:
            modifyPassword(passwords);
            break;
        case 4:
            return;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

// Function to handle user signup
void signup() {
    string email, password;
    cout << "Enter your email: ";
    cin >> email;

    if (!validateEmail(email)) {
        cout << "Invalid email format." << endl;
        return;
    }

    cout << "Do you want to generate a password? (1 for yes, 0 for no): ";
    int generateChoice;
    cin >> generateChoice;

    if (cin.fail() || (generateChoice != 0 && generateChoice != 1)) { // Check for invalid input
        cout << "Invalid input. Please enter either 0 or 1." << endl;
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        return;
    }

    if (generateChoice == 1) {
        password = generatePassword(12); // Generating a random password of length 12
        cout << "Generated password: " << password << endl;
    }
    else {
        cout << "Enter your password: ";
        cin >> password;

        if (!validatePassword(password)) {
            cout << "Password does not meet requirements (minimum 8 characters, at least one uppercase and one special character)." << endl;
            return;
        }
    }

    ofstream outfile("credentials.txt", ios::app);
    if (!outfile) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    // Encrypt email and password before storing
    string encryptedEmail = encrypt(email, 3); // Caesar Cipher shift 3
    string encryptedPassword = encrypt(password, 3); // Caesar Cipher shift 3

    outfile << encryptedEmail << " " << encryptedPassword << endl;
    outfile.close();
    cout << "Signup successful." << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed for random number generation

    while (true) {
        cout << "Welcome to the authentication system.\n"
            << "1. Signup\n"
            << "2. Login\n"
            << "3. Exit\n"
            << "Choose an option: ";
        int option;
        cin >> option;

        if (cin.fail()) { // Check if input extraction failed
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
            continue; // Restart the loop to prompt again
        }

        switch (option) {
        case 1:
            signup();
            break;
        case 2:
            login();
            break;
        case 3:
            exit(0);
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
