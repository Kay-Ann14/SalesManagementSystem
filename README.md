Sales Management System

A console-based sales management system built in C, simulating real-world business transaction workflows including role-based access, tiered discounts, tax calculation, and automated reporting.

Features


Role-Based Login: Separate access levels for Admin (Corporate) and Guest (Home Offices, Individuals) users, with limited login attempts for invalid credentials.
Multi-Customer Transaction Processing: Supports three customer types — Corporate, Home Offices, and Individuals — each with distinct discount rules.
Tiered Discount Logic:

Corporate customers receive a 10% discount on purchases of $10,000 or more.
Home Office customers receive a 5% discount on purchases of $5,000 or more.
Individual customers receive a 7.7% discount on every third item purchased.



Automatic Tax Calculation: A flat 15% tax is applied to the post-discount purchase total.
Payment Processing: Supports Cash, Card, and Online payment methods, including change calculation for cash transactions.
Input Validation: All user inputs (customer type, day of week, number of items, payment method) are validated with retry loops to prevent invalid data entry.
Persistent Data Storage: Every transaction is appended to a transactions.csv file, including timestamp, customer type, day, sales amount, discount, and payment method.
Automated Weekly Reporting: Generates a breakdown of transactions by day and customer type, including total sales, discounts, and transaction counts. Reports can be filtered by customer type or generated in full, and are saved to weekly_report.txt.


How It Works


On launch, the user logs in as either an Admin or Guest.
From the menu, the user can:

Enter a new transaction (selecting customer type, day of week, item quantities and prices, and payment method)
Generate a full or filtered sales report (Admin only sees all report options; Guests can still log transactions)
Switch users or exit the system



Each transaction automatically calculates discounts, applies tax, and (for cash payments) computes change.
All transactions are saved to CSV for record-keeping, and reports summarizing sales activity can be generated at any time.


Technical Details


Language: C
Data Structures: 2D arrays (totalSales, totalDiscounts, totalTransactions) indexed by day of week and customer type for aggregate reporting.
File Handling: Persistent storage via CSV export (transactions.csv) and plain-text weekly reports (weekly_report.txt).
Memory Management: Dynamic memory allocation (malloc/free) for storing item prices per transaction.


Sample Login Credentials

RoleUsernamePasswordAdmincorpadminadmin123Guesthomeguesthome123Guestindguestind123

Getting Started

Prerequisites


A C compiler (e.g., GCC)


Compile and Run

bashgcc sales_management_system.c -o sales_system
./sales_system

Author

Kay-Ann Green
github.com/Kay-Ann14
