#include "sc.h"
#include <limits>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

static FileItem* createFileInteractively() {
    cout << "Enter type (1=Document, 2=Photo, 3=PersonalPhoto, 4=App, 5=CloudBackupApp): ";
    int type;
    if (!(cin >> type)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputError("Type must be a number.");
    }
    if (type < 1 || type > 5) throw InvalidInputError("Invalid file type.");

    // common fields
    string name;
    size_t size;
    days_t days;
    int usage;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline
    cout << "Enter file name: ";
    getline(cin, name);

    cout << "Enter size in MB: ";
    if (!(cin >> size)) throw InvalidInputError("Size must be a positive number.");
    if (size == 0) throw InvalidInputError("Size cannot be 0.");

    cout << "Enter days since last access: ";
    if (!(cin >> days)) throw InvalidInputError("Days must be a number.");
    if (days < 0) throw InvalidInputError("Days cannot be negative.");

    cout << "Enter usage count: ";
    if (!(cin >> usage)) throw InvalidInputError("Usage count must be a number.");
    if (usage < 0) throw InvalidInputError("Usage count cannot be negative.");

    FileItem* item = nullptr;
    switch (type) {
        case 1: { // Document
            bool important;
            cout << "Is it important? (1=yes, 0=no): ";
            if (!(cin >> important)) throw InvalidInputError("Important must be 0 or 1.");
            Document* d = new Document(name, size, days, usage, important);

            // tags
            char addTagChoice;
            do {
                cout << "Add a tag? (y/n): ";
                cin >> addTagChoice;
                if (addTagChoice == 'y' || addTagChoice == 'Y') {
                    string tag;
                    cout << "Enter tag: ";
                    cin >> tag;
                    d->addTag(tag);
                }
            } while (addTagChoice == 'y' || addTagChoice == 'Y');

            item = d;
            break;
        }
        case 2: { // Photo
            bool highres;
            cout << "High resolution? (1=yes, 0=no): ";
            if (!(cin >> highres)) throw InvalidInputError("High resolution must be 0 or 1.");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string loc, date;
            cout << "Enter location: ";
            getline(cin, loc);
            cout << "Enter date taken: ";
            getline(cin, date);
            Photo* p = new Photo(name, size, days, usage, highres);
            p->setLocation(loc).setDateTaken(date);
            item = p;
            break;
        }
        case 3: { // PersonalPhoto
            bool highres;
            cout << "High resolution? (1=yes, 0=no): ";
            if (!(cin >> highres)) throw InvalidInputError("High resolution must be 0 or 1.");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string loc, date, reason;
            cout << "Enter location: ";
            getline(cin, loc);
            cout << "Enter date taken: ";
            getline(cin, date);
            cout << "Enter sensitivity reason: ";
            getline(cin, reason);
            if (reason.empty()) throw InvalidInputError("Sensitivity reason cannot be empty.");
            PersonalPhoto* pp = new PersonalPhoto(name, size, days, usage, highres, reason);
            pp->setLocation(loc).setDateTaken(date);
            item = pp;
            break;
        }
        case 4: { // App
            bool systemApp;
            cout << "Is it a system app? (1=yes, 0=no): ";
            if (!(cin >> systemApp)) throw InvalidInputError("System app flag must be 0 or 1.");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string version, category;
            cout << "Enter version: ";
            getline(cin, version);
            cout << "Enter category: ";
            getline(cin, category);
            App* a = new App(name, size, days, usage, systemApp, version, category);
            item = a;
            break;
        }
        case 5: { // CloudBackupApp
            bool systemApp, backup;
            cout << "Is it a system app? (1=yes, 0=no): ";
            if (!(cin >> systemApp)) throw InvalidInputError("System app flag must be 0 or 1.");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string version, category, provider;
            cout << "Enter version: ";
            getline(cin, version);
            cout << "Enter category: ";
            getline(cin, category);
            cout << "Has cloud backup? (1=yes, 0=no): ";
            if (!(cin >> backup)) throw InvalidInputError("Backup flag must be 0 or 1.");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter cloud provider: ";
            getline(cin, provider);
            CloudBackupApp* a = new CloudBackupApp(name, size, days, usage,
                                                   systemApp, version, category,
                                                   backup, provider);
            item = a;
            break;
        }
    }
    return item;
}

static void analyzeAndPrint(const Analyzer& analyzer, const vector<FileItem*>& files) {
    auto results = analyzer.analyzeBatch(files);
    cout << "\nAnalysis Results (sorted by importance score)\n";
    for (auto &pr : results) {
        cout << pr.first->getName()
             << " => Recommendation: " << pr.second.getRec()
             << " | Reason: " << pr.second.getReason()
             << " | Score: " << pr.second.getScore()
             << '\n';
    }
}

int main() {
    vector<FileItem*> files;
    Analyzer analyzer;

    cout << "Welcome to File Analyzer App\n";
    // initial population
    try {
        int n = safeInput<int>("Enter number of files to add initially: ");
        for (int i = 0; i < n; ++i) {
            cout << "\nAdding file " << (i+1) << ":\n";
            try {
                FileItem* f = createFileInteractively();
                files.push_back(f);
            } catch (const exception& e) {
                cout << "Failed to add file: " << e.what() << "\nRetry this entry.\n";
                --i;
            }
        }
    } catch (const exception& e) {
        cout << "Input aborted: " << e.what() << "\nStarting with empty list.\n";
    }

    // main menu
    int choice = -1;
    while (choice != 0) {
        cout << "\n=== MENU ===\n";
        cout << "1. Analyze files\n";
        cout << "2. Compare two files\n";
        cout << "3. Show recommended deletions\n";
        cout << "4. Delete a file\n";
        cout << "5. Add another file\n";
        cout << "6. List brief info of all files\n";
        cout << "7. Copy a file\n";
        cout << "0. Exit\n";
        choice = safeInput<int>("Enter choice: ");

        try {
            if (choice == 1) {
                analyzeAndPrint(analyzer, files);
            } else if (choice == 2) {
                if (files.size() < 2) { cout << "Need at least 2 files to compare.\n"; continue; }
                cout << "Choose index of first file (0.." << files.size()-1 << "): ";
                int a = safeInput<int>("Index: ");
                cout << "Choose index of second file (0.." << files.size()-1 << "): ";
                int b = safeInput<int>("Index: ");
                if (a < 0 || a >= (int)files.size() || b < 0 || b >= (int)files.size()) {
                    cout << "Invalid indices.\n"; continue;
                }
                cout << files[a]->getName() << " + " << files[b]->getName()
                     << " = " << (*files[a] + *files[b]) << " bytes\n";
                cout << files[a]->getName() << " > " << files[b]->getName()
                     << " ? " << ((*files[a] > *files[b]) ? "yes" : "no") << '\n';
            } else if (choice == 3) {
                auto results = analyzer.analyzeBatch(files);
                cout << "\nFiles recommended for DELETE:\n";
                bool any = false;
                for (auto &pr : results) {
                    if (pr.second.getRec() == Recommendation::DELETE) {
                        cout << " - " << pr.first->getName() << " : " << pr.second.getReason() << '\n';
                        any = true;
                    }
                }
                if (!any) cout << "None recommended for deletion.\n";
            }else if (choice == 4) {
                if (files.empty()) {
                    cout << "No files to delete.\n";
                    continue;
                }

                cout << "Files:\n";
                for (size_t i = 0; i < files.size(); ++i) {
                    cout << i << ": ";
                    files[i]->brief();
                }

                int idx = safeInput<int>("Enter index to delete (or -1 to cancel): ");

                if (idx < 0) {
                    cout << "Canceling delete.\n";
                    continue;
                }

                if ((size_t)idx >= files.size()) {
                    cout << "Invalid index.\n";
                    continue;
                }

                cout << "\nDeleting: ";
                files[idx]->brief();
                cout << "\n";

                delete files[idx];
                files.erase(files.begin() + idx);

                cout << "Deleted file at index " << idx << ".\n";

                if (files.empty()) {
                    cout << "No files remaining.\n";
                } else {
                    cout << "\nUpdated Files:\n";
                    for (size_t i = 0; i < files.size(); ++i) {
                        cout << i << ": ";
                        files[i]->brief();
                    }
                }

            } else if (choice == 5) {
                try {
                    FileItem* f = createFileInteractively();
                    files.push_back(f);
                    cout << "File added.\n";
                } catch (const exception& e) {
                    cout << "Failed to add file: " << e.what() << '\n';
                }
            } else if (choice == 6) {
                cout << "\nBrief Info of All Files\n";
                printBriefAll(files);
            }else if (choice == 7) {
                    if (files.empty()) {
                        cout << "No files available to copy.\n";
                        continue;
                    }

                    cout << "Files available to copy:\n";
                    for (size_t i = 0; i < files.size(); ++i) {
                        cout << i << ": ";
                        files[i]->brief();
                    }

                    int idx = safeInput<int>("Enter index of file to copy: ");
                    if (idx < 0 || (size_t)idx >= files.size()) {
                        cout << "Invalid index.\n";
                        continue;
                    }

                    FileItem* original = files[idx];
                    FileItem* copy = nullptr;

                    // Detect type and call appropriate copy constructor
                    if (Document* d = dynamic_cast<Document*>(original)) {
                        copy = new Document(*d);
                    } else if (PersonalPhoto* pp = dynamic_cast<PersonalPhoto*>(original)) {
                        copy = new PersonalPhoto(*pp);
                    } else if (Photo* p = dynamic_cast<Photo*>(original)) {
                        copy = new Photo(*p);
                    } else if (CloudBackupApp* cb = dynamic_cast<CloudBackupApp*>(original)) {
                        copy = new CloudBackupApp(*cb);
                    } else if (App* a = dynamic_cast<App*>(original)) {
                        copy = new App(*a);
                    } else {
                        cout << "Unknown file type. Cannot copy.\n";
                        continue;
                    }

                    files.push_back(copy);
                    cout << "File copied successfully. Original and copy:\n";
                    cout << "Original: "; original->brief();
                    cout << "Copy: "; copy->brief();
                }

             else if (choice == 0) {
                cout << "Exiting.\n";
            } else {
                cout << "Unknown choice.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    }

    // cleanup remaining files
    for (FileItem* f : files) delete f;
    files.clear();

    cout << "Program finished.\n";
    return 0;
}
