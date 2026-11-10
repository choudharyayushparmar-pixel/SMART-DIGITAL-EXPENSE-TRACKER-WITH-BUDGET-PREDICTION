/*
    Project: Smart Digital Expense Tracker with Budget Prediction
    Course: BCA I Semester (Session 2026-2027)
    Language: C++ (C++17)

    Educational note:
    This is a beginner-friendly console project. User passwords are stored as
    plain text in users.txt, so this design is for classroom demonstration only.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <ctime>

using namespace std;

struct User {
    string username;
    string password;
};

struct Expense {
    int id;
    string username;
    string date;       // YYYY-MM-DD
    string category;
    string description;
    double amount;
};

struct Budget {
    string username;
    string month;      // YYYY-MM
    double amount;
};

const string USERS_FILE = "users.txt";
const string EXPENSES_FILE = "expenses.txt";
const string BUDGETS_FILE = "budgets.txt";

string cleanField(string value) {
    // Prevent the | separator from breaking our simple text-file format.
    replace(value.begin(), value.end(), '|', '/');
    return value;
}

double readAmount(const string& prompt) {
    double amount;
    while (true) {
        cout << prompt;
        if (cin >> amount && amount > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return amount;
        }
        cout << "Please enter a valid amount greater than zero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int readInteger(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

bool validDate(const string& date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
    }
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

string monthFromDate(const string& date) {
    return date.substr(0, 7);
}

vector<User> loadUsers() {
    vector<User> users;
    ifstream file(USERS_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        User user;
        if (getline(ss, user.username, '|') && getline(ss, user.password)) {
            users.push_back(user);
        }
    }
    return users;
}

vector<Expense> loadExpenses() {
    vector<Expense> expenses;
    ifstream file(EXPENSES_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Expense e;
        string amountText;
        if (getline(ss, amountText, '|') &&
            getline(ss, e.username, '|') &&
            getline(ss, e.date, '|') &&
            getline(ss, e.category, '|') &&
            getline(ss, e.description, '|')) {
            try {
                e.id = stoi(amountText);
                string finalAmount;
                if (getline(ss, finalAmount)) {
                    e.amount = stod(finalAmount);
                    expenses.push_back(e);
                }
            } catch (...) {
                // Ignore an incorrectly formatted line.
            }
        }
    }
    return expenses;
}

vector<Budget> loadBudgets() {
    vector<Budget> budgets;
    ifstream file(BUDGETS_FILE);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Budget b;
        string amountText;
        if (getline(ss, b.username, '|') &&
            getline(ss, b.month, '|') &&
            getline(ss, amountText)) {
            try {
                b.amount = stod(amountText);
                budgets.push_back(b);
            } catch (...) {}
        }
    }
    return budgets;
}

void saveExpenses(const vector<Expense>& expenses) {
    ofstream file(EXPENSES_FILE);
    for (const auto& e : expenses) {
        file << e.id << '|' << e.username << '|' << e.date << '|'
             << e.category << '|' << cleanField(e.description) << '|'
             << fixed << setprecision(2) << e.amount << '\n';
    }
}

void saveBudgets(const vector<Budget>& budgets) {
    ofstream file(BUDGETS_FILE);
    for (const auto& b : budgets) {
        file << b.username << '|' << b.month << '|'
             << fixed << setprecision(2) << b.amount << '\n';
    }
}

int nextExpenseId(const vector<Expense>& expenses) {
    int highest = 0;
    for (const auto& e : expenses) highest = max(highest, e.id);
    return highest + 1;
}

bool registerUser() {
    cout << "\n--- User Registration ---\n";
    string username = cleanField(readLine("Choose username: "));
    string password = cleanField(readLine("Choose password: "));
    if (username.empty() || password.empty()) {
        cout << "Username and password cannot be empty.\n";
        return false;
    }
    vector<User> users = loadUsers();
    for (const auto& user : users) {
        if (user.username == username) {
            cout << "That username already exists. Try another one.\n";
            return false;
        }
    }
    ofstream file(USERS_FILE, ios::app);
    if (!file) {
        cout << "Could not open the user file. Check folder permissions.\n";
        return false;
    }
    file << username << '|' << password << '\n';
    cout << "Registration successful. You can now log in.\n";
    return true;
}

bool login(string& loggedInUser) {
    cout << "\n--- Login ---\n";
    string username = readLine("Username: ");
    string password = readLine("Password: ");
    vector<User> users = loadUsers();
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            loggedInUser = username;
            cout << "Login successful. Welcome, " << username << "!\n";
            return true;
        }
    }
    cout << "Incorrect username or password.\n";
    return false;
}

void showExpense(const Expense& e) {
    cout << left << setw(6) << e.id
         << setw(13) << e.date
         << setw(15) << e.category
         << setw(28) << e.description.substr(0, 26)
         << right << fixed << setprecision(2) << e.amount << '\n';
}

void addExpense(const string& username, vector<Expense>& expenses) {
    cout << "\n--- Add Daily Expense ---\n";
    string date;
    while (true) {
        date = readLine("Date (YYYY-MM-DD): ");
        if (validDate(date)) break;
        cout << "Date format is not valid. Example: 2026-11-10\n";
    }

    cout << "Categories: Food, Travel, Shopping, Education, Bills, Other\n";
    string category = cleanField(readLine("Category: "));
    string description = cleanField(readLine("Description: "));
    double amount = readAmount("Amount (Rs.): ");

    Expense e;
    e.id = nextExpenseId(expenses);
    e.username = username;
    e.date = date;
    e.category = category.empty() ? "Other" : category;
    e.description = description;
    e.amount = amount;
    expenses.push_back(e);
    saveExpenses(expenses);
    cout << "Expense saved successfully. Expense ID: " << e.id << '\n';
}

void viewHistory(const string& username, const vector<Expense>& expenses) {
    cout << "\n--- Expense History ---\n";
    cout << left << setw(6) << "ID" << setw(13) << "Date"
         << setw(15) << "Category" << setw(28) << "Description"
         << "Amount (Rs.)\n";
    cout << string(75, '-') << '\n';
    bool found = false;
    for (const auto& e : expenses) {
        if (e.username == username) {
            showExpense(e);
            found = true;
        }
    }
    if (!found) cout << "No expenses recorded yet.\n";
}

double monthlyTotal(const string& username, const string& month,
                    const vector<Expense>& expenses) {
    double total = 0;
    for (const auto& e : expenses) {
        if (e.username == username && monthFromDate(e.date) == month)
            total += e.amount;
    }
    return total;
}

double getBudget(const string& username, const string& month,
                 const vector<Budget>& budgets) {
    for (const auto& b : budgets) {
        if (b.username == username && b.month == month) return b.amount;
    }
    return 0;
}

void setMonthlyBudget(const string& username, vector<Budget>& budgets) {
    cout << "\n--- Set Monthly Budget ---\n";
    string month;
    while (true) {
        month = readLine("Month (YYYY-MM): ");
        if (month.size() == 7 && month[4] == '-' &&
            isdigit(static_cast<unsigned char>(month[0])) &&
            isdigit(static_cast<unsigned char>(month[1])) &&
            isdigit(static_cast<unsigned char>(month[2])) &&
            isdigit(static_cast<unsigned char>(month[3])) &&
            isdigit(static_cast<unsigned char>(month[5])) &&
            isdigit(static_cast<unsigned char>(month[6])) &&
            stoi(month.substr(5, 2)) >= 1 && stoi(month.substr(5, 2)) <= 12)
            break;
        cout << "Please use YYYY-MM format, for example 2026-11.\n";
    }
    double amount = readAmount("Enter budget (Rs.): ");
    bool updated = false;
    for (auto& b : budgets) {
        if (b.username == username && b.month == month) {
            b.amount = amount;
            updated = true;
            break;
        }
    }
    if (!updated) budgets.push_back({username, month, amount});
    saveBudgets(budgets);
    cout << "Budget saved for " << month << ".\n";
}

void categoryReport(const string& username, const string& month,
                    const vector<Expense>& expenses) {
    vector<pair<string, double>> totals;
    for (const auto& e : expenses) {
        if (e.username != username || monthFromDate(e.date) != month) continue;
        auto it = find_if(totals.begin(), totals.end(),
                          [&](const pair<string,double>& p) { return p.first == e.category; });
        if (it == totals.end()) totals.push_back({e.category, e.amount});
        else it->second += e.amount;
    }
    sort(totals.begin(), totals.end(),
         [](const pair<string,double>& a, const pair<string,double>& b) {
             return a.second > b.second;
         });
    cout << "\nCategory-wise spending for " << month << ":\n";
    if (totals.empty()) {
        cout << "No expenses found for this month.\n";
        return;
    }
    for (const auto& item : totals)
        cout << left << setw(20) << item.first << "Rs. "
             << fixed << setprecision(2) << item.second << '\n';
}

void monthlyReport(const string& username, const vector<Expense>& expenses,
                   const vector<Budget>& budgets) {
    cout << "\n--- Monthly Expense Report ---\n";
    string month = readLine("Enter month (YYYY-MM): ");
    if (month.size() != 7 || month[4] != '-') {
        cout << "Please enter month in YYYY-MM format.\n";
        return;
    }
    double total = monthlyTotal(username, month, expenses);
    double budget = getBudget(username, month, budgets);
    cout << "Month: " << month << "\nTotal spending: Rs. "
         << fixed << setprecision(2) << total << '\n';
    if (budget > 0) {
        cout << "Budget: Rs. " << budget << '\n';
        cout << "Remaining: Rs. " << budget - total << '\n';
        if (total > budget) cout << "Status: Budget exceeded!\n";
        else cout << "Status: Within budget.\n";
    } else {
        cout << "No budget has been set for this month.\n";
    }
    categoryReport(username, month, expenses);
}

void budgetWarningAndPrediction(const string& username,
                                const vector<Expense>& expenses,
                                const vector<Budget>& budgets) {
    cout << "\n--- Budget Warning & Expense Prediction ---\n";
    string month = readLine("Enter month (YYYY-MM): ");
    if (month.size() != 7 || month[4] != '-') {
        cout << "Please enter month in YYYY-MM format.\n";
        return;
    }
    double budget = getBudget(username, month, budgets);
    if (budget <= 0) {
        cout << "No budget found. Please set a budget first.\n";
        return;
    }
    double spent = monthlyTotal(username, month, expenses);
    cout << "Spent so far: Rs. " << fixed << setprecision(2) << spent << '\n';
    cout << "Monthly budget: Rs. " << budget << '\n';
    if (spent >= budget) cout << "WARNING: Your budget has been reached or exceeded.\n";
    else if (spent >= budget * 0.80) cout << "WARNING: You have used 80% or more of your budget.\n";
    else cout << "You are currently within your budget.\n";

    int year = stoi(month.substr(0, 4));
    int monthNumber = stoi(month.substr(5, 2));
    static const int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int days = daysInMonth[monthNumber - 1];
    bool leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    if (monthNumber == 2 && leap) days = 29;

    int latestDay = 0;
    for (const auto& e : expenses) {
        if (e.username == username && monthFromDate(e.date) == month)
            latestDay = max(latestDay, stoi(e.date.substr(8, 2)));
    }
    if (latestDay == 0) {
        cout << "Add an expense before making a prediction.\n";
        return;
    }
    double predicted = (spent / latestDay) * days;
    cout << "Prediction based on average daily spending: Rs. "
         << predicted << '\n';
    if (predicted > budget)
        cout << "Prediction: You may exceed your monthly budget.\n";
    else
        cout << "Prediction: You are likely to stay within your budget.\n";
    cout << "Note: This is a simple estimate, not an AI prediction.\n";
}

void searchExpense(const string& username, const vector<Expense>& expenses) {
    cout << "\n--- Search Expense ---\n";
    string keyword = readLine("Enter date, category or description keyword: ");
    bool found = false;
    for (const auto& e : expenses) {
        if (e.username != username) continue;
        string combined = e.date + " " + e.category + " " + e.description;
        string lowerCombined = combined, lowerKeyword = keyword;
        transform(lowerCombined.begin(), lowerCombined.end(), lowerCombined.begin(),
                  [](unsigned char c) { return static_cast<char>(tolower(c)); });
        transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(),
                  [](unsigned char c) { return static_cast<char>(tolower(c)); });
        if (lowerCombined.find(lowerKeyword) != string::npos) {
            cout << "ID: " << e.id << " | " << e.date << " | " << e.category
                 << " | " << e.description << " | Rs. " << e.amount << '\n';
            found = true;
        }
    }
    if (!found) cout << "No matching expense found.\n";
}

void deleteExpense(const string& username, vector<Expense>& expenses) {
    cout << "\n--- Delete Expense ---\n";
    int id = readInteger("Enter expense ID to delete: ");
    auto it = find_if(expenses.begin(), expenses.end(),
        [&](const Expense& e) { return e.id == id && e.username == username; });
    if (it == expenses.end()) {
        cout << "Expense ID not found in your account.\n";
        return;
    }
    expenses.erase(it);
    saveExpenses(expenses);
    cout << "Expense deleted successfully.\n";
}

void userMenu(const string& username) {
    vector<Expense> expenses = loadExpenses();
    vector<Budget> budgets = loadBudgets();
    while (true) {
        cout << "\n========== SMART DIGITAL EXPENSE TRACKER ==========\n";
        cout << "Logged in as: " << username << "\n";
        cout << "1. Add Daily Expense\n";
        cout << "2. View Expense History\n";
        cout << "3. Category-wise Expense\n";
        cout << "4. Monthly Expense Report\n";
        cout << "5. Set Monthly Budget\n";
        cout << "6. Budget Warning and Prediction\n";
        cout << "7. Search Expense\n";
        cout << "8. Delete Expense\n";
        cout << "9. Logout\n";
        int choice = readInteger("Choose an option: ");
        switch (choice) {
            case 1: addExpense(username, expenses); break;
            case 2: viewHistory(username, expenses); break;
            case 3: {
                string month = readLine("Enter month (YYYY-MM): ");
                categoryReport(username, month, expenses);
                break;
            }
            case 4: monthlyReport(username, expenses, budgets); break;
            case 5: setMonthlyBudget(username, budgets); break;
            case 6: budgetWarningAndPrediction(username, expenses, budgets); break;
            case 7: searchExpense(username, expenses); break;
            case 8: deleteExpense(username, expenses); break;
            case 9: cout << "Logged out successfully.\n"; return;
            default: cout << "Invalid choice. Select 1 to 9.\n";
        }
    }
}

int main() {
    cout << "===============================================\n";
    cout << "   SMART DIGITAL EXPENSE TRACKER\n";
    cout << "   Budget Monitoring and Prediction System\n";
    cout << "===============================================\n";
    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\n";
        int choice = readInteger("Choose an option: ");
        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            string username;
            if (login(username)) userMenu(username);
        } else if (choice == 3) {
            cout << "Thank you for using the Expense Tracker. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Select 1, 2 or 3.\n";
        }
    }
    return 0;
}
