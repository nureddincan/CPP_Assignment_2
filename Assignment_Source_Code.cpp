/****************************************************************************
**							     SAKARYA UNIVERSITY
**					FACULTY OF COMPUTER AND INFORMATION SCIENCES
**						 DEPARTMENT OF COMPUTER ENGINEERING
**						 INTRODUCTION TO PROGAMMING COURSE
**							  2024-2025 FALL SEMESTER
**
**					ASSIGNMENT NUMBER.: 2
**					STUDENT NAME......: Nureddin Can ERDEÐER
**					STUDENT NUMBER....: B231210041
**					COURSE GROUP......: C
*****************************************************************************/

#include <iostream>
#include <cstdlib>		// For random numbers and system functions (e.g., rand()).
#include <ctime>		// For time-related functions (e.g., time(), srand()).
#include <iomanip>		// For formatting output (e.g., setprecision).

using namespace std;

// Chicken class representing each chicken on the farm
class Chicken {
private:
    int daysWithoutLaying;     // Number of consecutive days without laying an egg
    int daysSinceArrival;      // Number of days since the chicken arrived at the farm
    int totalEggCount;         // Total number of eggs laid by the chicken

public:
    // Constructor to initialize the chicken's attributes
    Chicken() : daysWithoutLaying(0), daysSinceArrival(0), totalEggCount(0) {}

    // Function for daily feed consumption (in grams)
    int eatFeed() {
        return 100 + rand() % 21; // Random feed consumption between 100 and 120 grams
    }

    // Function for daily egg-laying
    int layEgg() {
        if (daysSinceArrival < 3) { // Does not lay eggs for the first 3 days
            daysSinceArrival++;
            return 0;
        }

        int eggs = rand() % 3; // Random number of eggs: 0, 1, or 2
        if (eggs == 0) {
            daysWithoutLaying++; // If it doesn't lay eggs, the counter is incremented
        } else {
            daysWithoutLaying = 0; // The counter is reset when the egg is laid
        }

        totalEggCount += eggs; // Updates the total number of eggs
        return eggs;
    }

    // Function to check if it's time for slaughter
    bool isTimeForSlaughter() const {
        return daysWithoutLaying >= 3 || totalEggCount >= 100;
    }

    // Function to reset the chicken's attributes (used when replacing a chicken)
    void reset() {
        daysWithoutLaying = 0;
        daysSinceArrival = 0;
        totalEggCount = 0;
    }	
};

// Farm class representing the entire chicken farm
class Farm {
private:
    double capital;            // Available capital (money)
    int chickenCount;          // Number of chickens on the farm
    double feedKgPrice;        // Price per kilogram of chicken feed
    double eggPrice;           // Price per egg
    double chickenPurchasePrice; // Purchase price for a new chicken
    int remainingFeed;         // Remaining feed in grams
    Chicken chickens[500];     // Array to hold up to 500 chickens

public:
    // Constructor to initialize the farm's attributes
    Farm(double capital, int chickenCount, double feedKgPrice, double eggPrice, double chickenPurchasePrice)
        : capital(capital), chickenCount(chickenCount), feedKgPrice(feedKgPrice),
          eggPrice(eggPrice), chickenPurchasePrice(chickenPurchasePrice), remainingFeed(700000) {}

    // Function to simulate the farm operations over a given number of days
    void runSimulation(int days) {

        // Print table headers
        cout << left << setw(5) << "Day"
             << setw(18) << "Feed Consumption"
             << setw(6) << "Eggs"
             << setw(22) << "Slaughtered Chickens"
             << setw(14) << "Daily Income"
             << setw(15) << "Daily Expense"
             << setw(16) << "Remaining Feed"
             << setw(16) << "Remaining Capital" << endl;

        // Print table header separator
        cout << left << setw(5) << "---"
             << setw(18) << "----------------"
             << setw(6) << "----"
             << setw(22) << "--------------------"
             << setw(14) << "------------"
             << setw(15) << "-------------"
             << setw(16) << "--------------"
             << setw(16) << "-----------------" << endl;

        // Loop through each day of the simulation
        for (int day = 1; day <= days; ++day) {
            int totalFeedConsumption = 0;
            int totalEggs = 0;
            int slaughteredChickenCount = 0;
            double expense = 0;
            double income = 0;

            // Loop through each chicken
            for (int i = 0; i < chickenCount; ++i) {
                totalFeedConsumption += chickens[i].eatFeed();
                totalEggs += chickens[i].layEgg();

                // Check if the chicken should be sent for slaughter
                if (chickens[i].isTimeForSlaughter()) {
                    slaughteredChickenCount++;
                    income += chickenPurchasePrice / 2; // Income from slaughter
                    expense += chickenPurchasePrice; // Cost of buying a new chicken
                    chickens[i].reset(); // Replace with a new chicken
                }
            }

            // Deduct the total daily feed consumption from remaining feed
            remainingFeed -= totalFeedConsumption;

            // Calculate income from selling eggs
            income += totalEggs * eggPrice;
            capital += income;

            // Check feed stock and replenish if needed
            if (remainingFeed < 70000) {
                expense += 700 * feedKgPrice;
                remainingFeed += 700000; // Replenish with 700 kilograms of feed
            }

            // Deduct daily expenses from capital
            capital -= expense;

            // Print the day's results in table format
            cout << left << setw(5) << day
                 << setw(18) << totalFeedConsumption
                 << setw(6) << totalEggs
                 << setw(22) << slaughteredChickenCount
                 << setw(14) << fixed << setprecision(2) << income
                 << setw(15) << fixed << setprecision(2) << expense
                 << setw(16) << remainingFeed
                 << setw(16) << fixed << setprecision(2) << capital << endl;

            // End simulation if capital goes below zero
            if (capital < 0) {
                cout << "You have gone bankrupt..." << endl;
                break;
            }
        }
    }
};

// Function that takes initial values from the user.
double getInitialValues(const string& prompt, double maxValue = -1) {
    double value;
    do {
        cout << prompt;
        cin >> value;
        if (value <= 0 || (maxValue > 0 && value > maxValue)) {
            cout << "Please enter a number greater than 0";
            if (maxValue > 0)
                cout << " and up to " << maxValue;
            cout << "." << endl;
        }
    } while (value <= 0 || (maxValue > 0 && value > maxValue));
    return value;
}

// Main function to execute the farm simulation
int main() {
	srand(time(NULL));

    double capital = getInitialValues("Initial capital: ");
    int chickenCount = static_cast<int>(getInitialValues("Number of chickens (maximum 500): ", 500));
    double feedKgPrice = getInitialValues("Feed price per kilogram: ");
    double eggPrice = getInitialValues("Egg selling price: ");
    double chickenPurchasePrice = getInitialValues("Chicken purchase price: ");
    int simulationDays = static_cast<int>(getInitialValues("Number of days for the simulation: "));

    // Initial costs for purchasing chickens and feed
    capital -= chickenCount * chickenPurchasePrice; // Deduct cost for chickens
    capital -= 700 * feedKgPrice;                   // Deduct cost for 700 kilograms of feed

    // Check if the initial capital is sufficient
    if (capital < 0) {
        cout << "Insufficient initial capital. The program is terminating..." << endl;
        return 0;
    }

    // Create the farm object with the user-specified parameters
    Farm farm(capital, chickenCount, feedKgPrice, eggPrice, chickenPurchasePrice);
    farm.runSimulation(simulationDays);

    return 0;
}
