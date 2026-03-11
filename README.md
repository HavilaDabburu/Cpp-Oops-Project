# Digital Declutter System

## Overview

The **Digital Declutter System** is a C++ application designed to analyze file properties and provide intelligent recommendations for managing digital storage efficiently.

The system evaluates files based on attributes such as file size, usage frequency, recency of access, file type importance, sensitivity of data, and cloud backup availability.

Using these attributes, the system classifies files and generates clear recommendations such as:

* **Keep** – Important and frequently used files
* **Archive** – Files not used recently but still valuable
* **Compress** – Large files that can be reduced in size
* **Delete** – Unnecessary files occupying storage space
* **Review** – Files that require manual inspection

This helps users clean and organize their digital storage safely and efficiently.

---

## Features

* Intelligent file classification system
* Smart storage optimization recommendations
* Rule-based recommendation engine
* Modular implementation using C++
* Demonstration of Object-Oriented Programming principles

---

## OOP Concepts Used

This project demonstrates the following Object-Oriented Programming concepts:

* **Classes and Objects**
* **Encapsulation**
* **Abstraction**
* **Modular design using header and source files**

---

## Technologies Used

* **Language:** C++
* **Programming Paradigm:** Object-Oriented Programming
* **Compiler:** g++

---

## Project Structure

Digital-Declutter-System

├── main.cpp   → Entry point of the program
├── sc.cpp     → Implementation of functions and core logic
├── sc.h       → Header file containing declarations and class definitions
└── README.md  → Project documentation

---

## How to Run

1. Clone the repository

git clone https://github.com/HavilaDabburu/Cpp-Oops-Project.git

2. Navigate to the project folder

cd Cpp-Oops-Project

3. Compile the program

g++ main.cpp sc.cpp -o declutter

4. Run the program

./declutter

---

## Example Output

Welcome to File Analyzer App
Enter number of files to add initially: 2

Adding file 1:
Enter type (1=Document, 2=Photo, 3=PersonalPhoto, 4=App, 5=CloudBackupApp): 1
Enter file name: Havi
Enter size in MB: 253
Enter days since last access: 2
Enter usage count: 1
Is it important? (1=yes, 0=no): 0
Add a tag? (y/n): n

Adding file 2:
Enter type (1=Document, 2=Photo, 3=PersonalPhoto, 4=App, 5=CloudBackupApp): 1
Enter file name: hehe
Enter size in MB: 34
Enter days since last access: 2
Enter usage count: 5
Is it important? (1=yes, 0=no): 0
Add a tag? (y/n): n

=== MENU ===
1. Analyze files
2. Compare two files
3. Show recommended deletions
4. Delete a file
5. Add another file
6. List brief info of all files
7. Copy a file
0. Exit
Enter choice: 1

Analysis Results (sorted by importance score)
hehe => Recommendation: REVIEW | Reason: Ambiguous - manual review recommended. | Score: 0.185
Havi => Recommendation: COMPRESS | Reason: Large file and low-to-medium importance - compression recommended. | Score: -0.511

---

## Team Members

This project was developed as part of a group project.

* Havila Dabburu
* Sathwika Valluru
* Maneesha S
* Rishitha Kommukuri

---

## My Contribution

* Implemented the file analysis logic
* Developed the Analysis system for file management
* Implemented Exception Handling
* Contributed to modular project design and testing

---

## Testing
<img width="1293" height="1077" alt="Screenshot from 2026-03-11 15-43-47" src="https://github.com/user-attachments/assets/7fd46668-f7f7-40a5-b4a6-78aed769cb48" />

---

## Future Improvements

* Integration with real file system scanning
* Graphical User Interface (GUI)

* Automated file cleanup suggestions
* Cloud storage integration

---

## License

This project is intended for educational and academic purposes.

