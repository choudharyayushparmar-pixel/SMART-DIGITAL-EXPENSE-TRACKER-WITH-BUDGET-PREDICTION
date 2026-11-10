# Smart Digital Expense Tracker with Budget Prediction

**Course:** BCA I Semester (Session 2026–2027)  
**Lab:** Lab Sheet-09  
**Language:** C++17  
**Project type:** Menu-driven console application

## 1. Project objective
This project helps a student record daily expenses, organize them by category, set a monthly budget, review monthly spending, receive budget warnings, and estimate total spending for the month.

## 2. Features
1. User registration
2. Login
3. Add daily expense
4. View expense history
5. Category-wise expense report
6. Monthly expense report
7. Set monthly budget
8. Budget warning at 80% and when budget is reached/exceeded
9. Estimate monthly spending from average daily spending so far
10. Search expenses by date, category, or description
11. Delete an expense by ID
12. Save information in text files

## 3. C++ concepts used
Variables, data types, loops, conditional statements, structures, vectors, functions, strings, searching, sorting, and file handling.

## 4. Requirements
- A C++ compiler that supports C++17 (for example, GCC / MinGW-w64)
- Visual Studio Code or another text editor (optional)

## 5. Compile and run

Open a terminal in this folder and run:

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o expense_tracker
```

On Windows, run:

```powershell
.\expense_tracker.exe
```

On Linux/macOS, run:

```bash
./expense_tracker
```

## 6. How to use
1. Choose `1` to register a username and password.
2. Choose `2` to log in.
3. Set a monthly budget using option `5` (format: `YYYY-MM`).
4. Add expenses using option `1` (date format: `YYYY-MM-DD`).
5. Use options `2` to `8` to review, search, predict, or delete records.
6. Choose `9` to log out.

## 7. Data files
The program creates these files in the current working folder:
- `users.txt` – registered usernames and passwords
- `expenses.txt` – expense records
- `budgets.txt` – monthly budgets

Do not delete these files if you want to keep your saved project data.

## 8. Prediction method
The program finds the total spending recorded for the selected month and the latest day number entered. It estimates the month-end total as:

`Predicted monthly spending = (Total spending so far / Latest day number recorded) × Days in that month`

This is a simple linear estimate for learning purposes. It is not an AI or machine-learning model.

## 9. Important limitations
- This is a classroom demonstration, not a production financial application.
- Passwords are stored as plain text. Do not use a real or reused password.
- Date validation checks the format and broad day/month ranges; it does not verify every calendar date.
- Prediction assumes the average daily spending so far continues for the rest of the month.

## 10. Suggested test
Create a test account, set a budget of 10000 for a month, add expenses such as Food 100, Travel 200, and Shopping 500, then check the monthly report and prediction.

## 11. Submission
Submit `main.cpp`, this README, and `report/Project-Report.pdf` to your teacher. Upload the complete project folder to your GitHub repository.
