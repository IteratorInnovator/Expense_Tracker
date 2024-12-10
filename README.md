# 💰 Expense Tracker

A simple and efficient tool to keep track of your expenses. This application allows you to record expenses, calculate totals for specific months or years, and view all transactions at a glance.

---

## ✨ Features

- 📄 **Add Expenses**: Record expenses with a description and amount.
- 🗑️ **Delete Expenses**: Remove specific expenses using their unique ID.
- 🔄 **Clear All Expenses**: Reset your expense list to start fresh.
- 📆 **Calculate Totals**: Get total expenses for a specified month and/or year.
- 🔍 **View All Expenses**: Display a complete list of all recorded expenses.
- 🚫 **No Budget Feature**: Focused solely on tracking, not budgeting.

---

## 📦 Requirements

- **Programming Language**: C++ (Recommended: C++17 or newer)
  - **Why C++17?**
    - Utilizing `std::optional` for managing optional values.
    - Using inline global variables for better modularity and clarity.
- **Compiler**: GCC, Clang, or any C++ compatible compiler that supports C++17
- **Libraries/Dependencies**:
  - [CLI11](https://github.com/CLIUtils/CLI11): A powerful and flexible library for parsing command-line arguments.

---

## 🚀 Getting Started

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/ExpenseTracker.git
   cd ExpenseTracker

2. **Run the Application**:
  ```bash
  ./expense-tracker
  ```

---

## 🗒️ Notes

-The expense-tracker executable is pre-built and included in the repository. If you encounter compatibility issues or wish to modify the source code, you can rebuild it using a C++17-compatible compiler. 
-If you're distributing the executable, ensure it's compiled in a way that maximizes compatibility (e.g., statically linking libraries or building on a standard environment). Let me know if you'd like to include additional details!
