#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <cctype>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

struct Contact {
    string firstName;
    string lastName;
    string phoneNumber;
    Contact* next;

    Contact(const string& fname, const string& lname, const string& phone) : 
        firstName(fname), lastName(lname), phoneNumber(phone), next(nullptr) {}
};

class ContactList {
private:
    Contact* head;
    vector<string> phoneNumbers;

public:
    ContactList() : head(nullptr) {}


    void addContact(const string& fname, const string& lname, const string& phone) {
        Contact* newContact = new Contact(fname, lname, phone);
        if (head == nullptr) {
            head = newContact;
        } else {
            Contact* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newContact;
        }
        cout << "\n\tContact added successfully!\n";
    }


    void editContacts(const string& keyword, const string& newDetails) {
        if (head == nullptr) {
            cout << "\n\tNo contacts found!";
            return;
        }

        Contact* current = head;
        bool found = false;
        while (current != nullptr) {
            if (current->firstName == keyword || current->lastName == keyword || current->phoneNumber == keyword) {
                found = true;
                cout << "\n\tContact Details:";
                cout << "\n\tFirst Name: " << current->firstName;
                cout << "\n\tLast Name: " << current->lastName;
                cout << "\n\tPhone Number: " << current->phoneNumber;

                cout << "\n\tDo you want to edit this contact? (Y/N): ";
                char choice;
                cin >> choice;
                if (toupper(choice) == 'Y') {
                    size_t space1 = newDetails.find(' ');
                    size_t space2 = newDetails.find(' ', space1 + 1);
                    if (space1 != string::npos && space2 != string::npos) {
                        current->firstName = newDetails.substr(0, space1);
                        current->lastName = newDetails.substr(space1 + 1, space2 - space1 - 1);
                        current->phoneNumber = newDetails.substr(space2 + 1);
                        cout << "\n\tContact Edited Successfully!\n";
                    } else {
                        cout << "\n\tPlease enter correct details in the format: 'First Name Last Name Phone'\n";
                    }
                } else {
                    cout << "\n\tContact not edited.\n";
                }
                break;
            }
            current = current->next;
        }

        if (!found) {
            cout << "\n\tNo matching contact found! Please enter correct details.\n";
        }
    }


    void searchContact(const string& keyword) {
        if (head == nullptr) {
            cout << "\n\tNo contacts found!";
            return;
        }

        Contact* current = head;
        bool found = false;
        while (current != nullptr) {
            if (current->firstName.find(keyword) != string::npos ||
                current->lastName.find(keyword) != string::npos ||
                current->phoneNumber.find(keyword) != string::npos) {
                if (!found) {
                    cout << "\n\t\t================================\n\t\t\tSEARCH RESULTS\n\t\t================================\n\n";
                    found = true;
                }
                cout << "\t" << current->firstName << " " << current->lastName << " " << current->phoneNumber << '\n';
            }
            current = current->next;
        }

        if (!found) {
            cout << "\n\tNo matching contact found!\n";
        }
    }


    void displayContacts() {
        if (head == nullptr) {
            cout << "\n\tNo contacts found!";
            return;
        }

        Contact* i = head;
        while (i != nullptr) {
            Contact* j = i->next;
            while (j != nullptr) {
                if ((i->firstName + " " + i->lastName) > (j->firstName + " " + j->lastName)) {
                    swap(i->firstName, j->firstName);
                    swap(i->lastName, j->lastName);
                    swap(i->phoneNumber, j->phoneNumber);
                }
                j = j->next;
            }
            i = i->next;
        }

        cout << "\n\t\t================================\n\t\t\tLIST OF CONTACTS\n\t\t================================\n\n";
        Contact* current = head;
        while (current != nullptr) {
            // Print first name and last name up to 20 characters
            cout << "\t" << current->firstName.substr(0, 20) << " " << current->lastName.substr(0, 20);

            // Calculate the number of spaces needed to align phone numbers
            int numSpaces = 40 - current->firstName.length() - current->lastName.length();

            // Print spaces
            for (int i = 0; i < numSpaces; ++i) {
                cout << " ";
            }

            // Print phone number
            cout << current->phoneNumber << '\n';
            cout<<"\n";
            current = current->next;
        }
    }


    void deleteContacts(const string& keyword) {
        if (head == nullptr) {
            cout << "\n\tNo contacts found!";
            return;
        }

        Contact* current = head;
        Contact* previous = nullptr;
        while (current != nullptr) {
            if (current->firstName == keyword || current->lastName == keyword) {
                if (previous == nullptr) {
                    head = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                cout << "\n\tContact Deleted Successfully!\n";
                return;
            }
            previous = current;
            current = current->next;
        }
        cout << "\n\tNo Such Contact Found\n";
    }


    void saveContactsToFile() {
        ofstream outputFile("numbers.txt");
        if (!outputFile.is_open()) {
            cout << "\n\tError opening file for writing!\n";
            return;
        }

        Contact* current = head;
        while (current != nullptr) {
            outputFile << current->firstName << " " << current->lastName << " " << current->phoneNumber << "\n";
            current = current->next;
        }

        outputFile.close();
        //cout << "\n\tContacts saved to numbers.txt!\n";
    }


    void readContactsFromFile() {
        ifstream inputFile("numbers.txt");
        if (!inputFile.is_open()) {
            cout << "\n\tError opening file for reading!\n";
            return;
        }

        while (!inputFile.eof()) {
            string fname, lname, phone;
            inputFile >> fname >> lname >> phone;
            addContact(fname, lname, phone);
        }

        inputFile.close();
        // cout << "\n\tContacts loaded from numbers.txt!\n";
    }

    // Function to check if a phone number is spam using K-means clustering
    bool is_spam(const string& phoneNumber) {
        // For simplicity, let's consider two clusters: normal and spam
        const int k = 2;

        // Initialize centroids randomly (you may replace this with a more sophisticated method)
        vector<string> centroids = {"1234567890", "9876543210"};

        // Maximum number of iterations (to avoid infinite loop)
        const int maxIterations = 100;

        for (int iter = 0; iter < maxIterations; ++iter) {
            // Assign data points to clusters
            vector<vector<string>> clusters(k);
            for (const string& number : phoneNumbers) {
                int closestCentroidIndex = findClosestCentroid(number, centroids);
                clusters[closestCentroidIndex].push_back(number);
            }

            // Update centroids
            vector<string> newCentroids(k);
            for (int i = 0; i < k; ++i) {
                if (!clusters[i].empty()) {
                    newCentroids[i] = calculateMean(clusters[i]);
                }
            }

            // Check for convergence
            if (centroids == newCentroids) {
                // Converged, decide if the input number belongs to the spam cluster
                return isSpamCluster(phoneNumber, centroids);
            }

            centroids = newCentroids;
        }

        // Maximum iterations reached, unable to determine
        return false;
    }


    // Helper function to find the index of the closest centroid for a given number
    int findClosestCentroid(const string& number, const vector<string>& centroids) {
        int closestIndex = 0;
        double minDistance = numeric_limits<double>::max();

        for (int i = 0; i < centroids.size(); ++i) {
            double distance = calculateDistance(number, centroids[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestIndex = i;
            }
        }

        return closestIndex;
    }

    // Helper function to calculate the Euclidean distance between two phone numbers
    double calculateDistance(const string& num1, const string& num2) {
        // For simplicity, let's assume both numbers have the same length
        double distance = 0.0;
        for (int i = 0; i < num1.length(); ++i) {
            distance += pow(num1[i] - num2[i], 2);
        }
        return sqrt(distance);
    }

    // Helper function to calculate the mean of a cluster
    string calculateMean(const vector<string>& cluster) {
        if (cluster.empty()) {
            return "";
        }

        // Sum the corresponding digits of all numbers in the cluster
        vector<int> sum(10, 0);
        for (const string& number : cluster) {
            for (int i = 0; i < number.length(); ++i) {
                sum[i] += number[i] - '0';
            }
        }

        // Calculate the mean
        stringstream mean;
        for (int digitSum : sum) {
            mean << digitSum / cluster.size();
        }

        return mean.str();
    }

    // Helper function to decide if a given phone number belongs to the spam cluster
    bool isSpamCluster(const string& phoneNumber, const vector<string>& centroids) {
        // Decide based on the distance to the spam centroid
        double distanceToSpamCentroid = calculateDistance(phoneNumber, centroids[1]);
        
        // You may need to adjust this threshold based on your actual data
        const double spamThreshold = 10.0;

        return distanceToSpamCentroid < spamThreshold;
    }
    
    ~ContactList() {
        while (head != nullptr) {
            Contact* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

void addContact(ContactList& contacts) {
    string fname, lname, phoneNum;
    cout << "\n\n\tEnter First Name: ";
    cin >> fname;
    cout << "\tEnter Last Name: ";
    cin >> lname;
    cout << "\tEnter 10-Digit Phone Number: ";
    cin >> phoneNum;

    contacts.addContact(fname, lname, phoneNum);
}

void displayContact(ContactList& contacts) {
    contacts.displayContacts();
}

void deleteContact(ContactList& contacts) {
    string keyword;
    cout << "\n\tEnter Name To Delete: ";
    cin >> keyword;

    contacts.deleteContacts(keyword);
}

int main() {
    ContactList contacts;
    contacts.readContactsFromFile();

    short int choice = 0;
    cout << "\n\n\n\t\t\tContact Management.";

    while (choice != 8) {
        cout << "\n\n\t1. Add Contact\n\t2. Edit Contact\n\t3. Search Contact\n\t4. Display Contact\n\t5. Delete Contact\n\t6. Save Contacts to File\n\t7. Check for Spam\n\t8. Exit\n\t\n\t Please Enter your choice:  ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            addContact(contacts);
            break;
        case 2: {
            string keyword, newDetails;
            cout << "\n\tEnter Name or Number to Edit: ";
            cin >> keyword;

            cout << "\tEnter New Details (First Name Last Name Phone): ";
            cin.ignore();
            getline(cin, newDetails);

            contacts.editContacts(keyword, newDetails);
            break;
        }
        case 3: {
            string keyword;
            cout << "\n\tEnter Name or Number to Search: ";
            cin >> keyword;

            contacts.searchContact(keyword);
            break;
        }
        case 4:
            displayContact(contacts);
            break;
        case 5: {
            string keyword;
            cout << "\n\tEnter Name to Delete: ";
            cin >> keyword;

            contacts.deleteContacts(keyword);
            break;
        }
        case 6:
            contacts.saveContactsToFile();
            break;
        case 7: {
            string phoneNumber;
            cout << "\n\tEnter a phone number to check for spam: ";
            cin >> phoneNumber;

            bool isSpam = contacts.is_spam(phoneNumber);
            if (isSpam) {
                cout << "\n\tThe phone number is spam.\n";
            }
            else {
                cout << "\n\tThe phone number is not spam.\n";
            }
            break;
        }
        case 8:
            cout << "\n\n\n\t\tThank You For Using!\n\n\n";
            break;
        default:
            cout << "\nInvalid Input! Please enter a number between 1 and 8.\n";
            break;
        }
    }

    return 0;
}