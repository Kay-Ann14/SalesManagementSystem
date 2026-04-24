#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_ITEMS 3
#define CORP_DISCOUNT 0.10
#define HOME_DISCOUNT 0.05
#define INDIV_DISCOUNT 0.077
#define TAX_RATE 0.15

// ===== Function Prototypes =====
int login();
int getCustomerType();
int getDayOfWeek();
int getNumItems();
int getPaymentMethod();
void processPurchase(int customerType, int dayOfWeek, double totalSales[6][4],
                     double totalDiscounts[6][4], int totalTransactions[6][4]);
void generateReport(double totalSales[6][4], double totalDiscounts[6][4],
                    int totalTransactions[6][4], int filterType);
void adminMenu(double totalSales[6][4], double totalDiscounts[6][4],
               int totalTransactions[6][4]);
void guestMenu(double totalSales[6][4], double totalDiscounts[6][4],
               int totalTransactions[6][4]);

// ===== Main =====
int main() {
    int role;
    double totalSales[6][4] = {0};
    double totalDiscounts[6][4] = {0};
    int totalTransactions[6][4] = {0};

    printf("========== SALES MANAGEMENT SYSTEM ==========\n");

    while (1) {
        role = login();

        if (role == 1) { // Admin
            printf("\nWelcome, Corporate Admin!\n");
            adminMenu(totalSales, totalDiscounts, totalTransactions);
        } else if (role == 2) { // Guest
            printf("\nWelcome, Guest!\n");
            guestMenu(totalSales, totalDiscounts, totalTransactions);
        } else if (role == 0) {
            printf("\nExiting system. Goodbye!\n");
            break;
        }
    }
    return 0;
}

// ===== LOGIN SYSTEM =====
int login() {
    char username[50], password[50];
    int attempts = 3;

    while (attempts > 0) {
        printf("\nLogin\nUsername (or type 'exit' to quit): ");
        scanf("%s", username);
        if (strcmp(username, "exit") == 0)
            return 0;

        printf("Password: ");
        scanf("%s", password);

        if (strcmp(username, "corpadmin") == 0 && strcmp(password, "admin123") == 0)
            return 1; // Admin
        else if (strcmp(username, "homeguest") == 0 && strcmp(password, "home123") == 0)
            return 2; // Guest
        else if (strcmp(username, "indguest") == 0 && strcmp(password, "ind123") == 0)
            return 2; // Guest
        else {
            attempts--;
            printf("Invalid credentials. Attempts remaining: %d\n", attempts);
        }
    }
    printf("Too many failed attempts. Returning to login menu.\n");
    return -1;
}

// ===== CORE FUNCTIONS =====
int getCustomerType() {
    int customerType;
    do {
        printf("\nEnter customer type (1=Corporate, 2=Home Offices, 3=Individuals, 0=Cancel): ");
        scanf("%d", &customerType);
        if (customerType == 0) return 0;
        if (customerType < 1 || customerType > 3)
            printf("Invalid customer type. Try again.\n");
    } while (customerType < 1 || customerType > 3);
    return customerType;
}

int getDayOfWeek() {
    int dayOfWeek;
    do {
        printf("Enter day (1=Mon, 2=Tue, 3=Wed, 4=Thu, 5=Fri, 0=Cancel): ");
        scanf("%d", &dayOfWeek);
        if (dayOfWeek == 0) return 0;
        if (dayOfWeek < 1 || dayOfWeek > 5)
            printf("Invalid day. Try again.\n");
    } while (dayOfWeek < 1 || dayOfWeek > 5);
    return dayOfWeek;
}

int getNumItems() {
    int numItems;
    do {
        printf("Enter number of items (min %d): ", MIN_ITEMS);
        scanf("%d", &numItems);
        if (numItems < MIN_ITEMS)
            printf("Must purchase at least %d items.\n", MIN_ITEMS);
    } while (numItems < MIN_ITEMS);
    return numItems;
}

int getPaymentMethod() {
    int method;
    do {
        printf("Enter payment method (1=Cash, 2=Card, 3=Online): ");
        scanf("%d", &method);
        if (method < 1 || method > 3)
            printf("Invalid payment method. Try again.\n");
    } while (method < 1 || method > 3);
    return method;
}

void processPurchase(int customerType, int dayOfWeek, double totalSales[6][4],
                     double totalDiscounts[6][4], int totalTransactions[6][4]) {
    int numItems = getNumItems();
    double *itemPrices = malloc(numItems * sizeof(double));
    if (!itemPrices) {
        printf("Memory allocation failed.\n");
        return;
    }

    double purchaseAmount = 0.0, discount = 0.0;
    for (int i = 0; i < numItems; i++) {
        printf("Enter price of item %d: $", i + 1);
        scanf("%lf", &itemPrices[i]);
        purchaseAmount += itemPrices[i];

        if (customerType == 3 && (i + 1) % 3 == 0) {
            double itemDiscount = INDIV_DISCOUNT * itemPrices[i];
            discount += itemDiscount;
            purchaseAmount -= itemDiscount;
        }
    }

    double baseDiscount = 0.0;
    if (customerType == 1 && purchaseAmount >= 10000)
        baseDiscount = CORP_DISCOUNT * purchaseAmount;
    else if (customerType == 2 && purchaseAmount >= 5000)
        baseDiscount = HOME_DISCOUNT * purchaseAmount;

    discount += baseDiscount;
    purchaseAmount -= baseDiscount;

    double tax = TAX_RATE * purchaseAmount;
    purchaseAmount += tax;

    printf("\nTotal cost (after discounts + tax): $%.2lf\n", purchaseAmount);
    printf("Discount applied: $%.2lf | Tax: $%.2lf\n", discount, tax);

    int paymentMethod = getPaymentMethod();
    const char *paymentName = (paymentMethod == 1) ? "Cash" :
                              (paymentMethod == 2) ? "Card" : "Online";

    if (paymentMethod == 1) {
        double cashPaid;
        do {
            printf("Enter cash amount: $");
            scanf("%lf", &cashPaid);
            if (cashPaid < purchaseAmount)
                printf("Insufficient funds. Please enter at least $%.2lf\n", purchaseAmount);
        } while (cashPaid < purchaseAmount);

        double change = cashPaid - purchaseAmount;
        printf("Change to return: $%.2lf\n", change);
    }

    printf("Payment method: %s\n", paymentName);

    totalSales[dayOfWeek][customerType] += purchaseAmount;
    totalDiscounts[dayOfWeek][customerType] += discount;
    totalTransactions[dayOfWeek][customerType]++;

    FILE *csv = fopen("transactions.csv", "a");
    if (csv != NULL) {
        time_t now = time(NULL);
        char timeStr[26];
        ctime_r(&now, timeStr);
        timeStr[strcspn(timeStr, "\n")] = 0;
        fprintf(csv, "%s,%d,%d,%.2lf,%.2lf,%s\n",
                timeStr, customerType, dayOfWeek, purchaseAmount, discount, paymentName);
        fclose(csv);
    }
    free(itemPrices);
}

// ===== REPORT SYSTEM =====
void generateReport(double totalSales[6][4], double totalDiscounts[6][4],
                    int totalTransactions[6][4], int filterType) {
    const char *days[] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    const char *types[] = {"", "Corporate", "Home Offices", "Individuals"};

    FILE *fp = fopen("weekly_report.txt", "a");
    if (!fp) {
        printf("Error opening report file.\n");
        return;
    }

    time_t now = time(NULL);
    fprintf(fp, "\n===== WEEKLY SALES REPORT =====\nGenerated on: %s\n", ctime(&now));
    printf("\n===== WEEKLY SALES REPORT =====\nGenerated on: %s\n", ctime(&now));

    double totalWeekSales = 0.0, totalWeekDiscounts = 0.0;
    int totalWeekTransactions = 0;

    printf("%-10s %-15s %-15s %-15s %-15s\n", "Day", "Type", "Transactions", "Sales($)", "Discounts($)");
    printf("--------------------------------------------------------------------------\n");

    for (int d = 1; d <= 5; d++) {
        for (int t = 1; t <= 3; t++) {
            if ((filterType == 0 || filterType == t) && totalTransactions[d][t] > 0) {
                printf("%-10s %-15s %-15d %-15.2lf %-15.2lf\n",
                       days[d], types[t], totalTransactions[d][t],
                       totalSales[d][t], totalDiscounts[d][t]);

                fprintf(fp, "%-10s %-15s %-15d %-15.2lf %-15.2lf\n",
                        days[d], types[t], totalTransactions[d][t],
                        totalSales[d][t], totalDiscounts[d][t]);

                totalWeekSales += totalSales[d][t];
                totalWeekDiscounts += totalDiscounts[d][t];
                totalWeekTransactions += totalTransactions[d][t];
            }
        }
    }

    printf("--------------------------------------------------------------------------\n");
    printf("TOTALS: %d transactions | $%.2lf sales | $%.2lf discounts\n",
           totalWeekTransactions, totalWeekSales, totalWeekDiscounts);

    fprintf(fp, "--------------------------------------------------------------------------\n");
    fprintf(fp, "TOTALS: %d transactions | $%.2lf sales | $%.2lf discounts\n",
            totalWeekTransactions, totalWeekSales, totalWeekDiscounts);
    fprintf(fp, "===== END OF REPORT =====\n\n");

    fclose(fp);
    printf("\nReport saved to 'weekly_report.txt'.\n");
}

// ===== MENUS =====
void adminMenu(double totalSales[6][4], double totalDiscounts[6][4],
               int totalTransactions[6][4]) {
    int choice;
    while (1) {
        printf("\nAdmin Menu:\n");
        printf("1. Enter a new transaction\n");
        printf("2. Generate full report\n");
        printf("3. Generate Corporate report\n");
        printf("4. Generate Home Offices report\n");
        printf("5. Generate Individuals report\n");
        printf("6. Switch user\n");
        printf("7. Exit system\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int customerType = getCustomerType();
            if (customerType == 0) continue;
            int dayOfWeek = getDayOfWeek();
            if (dayOfWeek == 0) continue;
            processPurchase(customerType, dayOfWeek, totalSales, totalDiscounts, totalTransactions);
        } else if (choice >= 2 && choice <= 5) {
            generateReport(totalSales, totalDiscounts, totalTransactions, choice - 2);
        } else if (choice == 6) {
            printf("Switching user...\n");
            return;
        } else if (choice == 7) {
            printf("Exiting system. Goodbye!\n");
            exit(0);
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
}

void guestMenu(double totalSales[6][4], double totalDiscounts[6][4],
               int totalTransactions[6][4]) {
    int choice;
    while (1) {
        printf("\nGuest Menu:\n");
        printf("1. Enter a new transaction\n");
        printf("2. Switch user\n");
        printf("3. Exit system\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int customerType = getCustomerType();
            if (customerType == 0) continue;
            int dayOfWeek = getDayOfWeek();
            if (dayOfWeek == 0) continue;
            processPurchase(customerType, dayOfWeek, totalSales, totalDiscounts, totalTransactions);
        } else if (choice == 2) {
            printf("Switching user...\n");
            return;
        } else if (choice == 3) {
            printf("Exiting system. Goodbye!\n");
            exit(0);
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
}
