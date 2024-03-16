#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

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

// Function to handle user signup
void signup() {
    string email, password;
    cout << "Enter your email: ";
    cin >> email;
    cout << "Would you like to generate a random password? (1 for yes, 0 for no): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
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

    if (found) {
        cout << "Login successful." << endl;
    }
    else {
        cout << "Invalid email or password." << endl;
    }
}

int main() {
    srand(time(nullptr)); // Seed for random number generation

    while (true) {
        cout << "Welcome to the authentication system.\n"
            << "1. Signup\n"
            << "2. Login\n"
            << "3. Exit\n"
            << "Choose an option: ";
        int option;
        cin >> option;

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
