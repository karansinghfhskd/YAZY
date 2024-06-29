#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// Function to roll a die
int rollDie() {
    return rand() % 6 + 1;
}

// Function to roll all five dice
vector<int> rollDice() {
    vector<int> dice(5);
    for (int &die : dice) {
        die = rollDie();
    }
    return dice;
}

// Function to print the dice
void printDice(const vector<int> &dice) {
    cout << "Dice: ";
    for (int die : dice) {
        cout << die << " ";
    }
    cout << endl;
}

// Function to reroll selected dice
void rerollDice(vector<int> &dice, const vector<int> &rerollPositions) {
    for (int pos : rerollPositions) {
        dice[pos - 1] = rollDie();
    }
}

// Function to score the dice based on a given category
int scoreDice(const vector<int> &dice, const string &category) {
    int score = 0;
    if (category == "ones") {
        score = count(dice.begin(), dice.end(), 1);
    } else if (category == "twos") {
        score = count(dice.begin(), dice.end(), 2) * 2;
    } else if (category == "threes") {
        score = count(dice.begin(), dice.end(), 3) * 3;
    } else if (category == "fours") {
        score = count(dice.begin(), dice.end(), 4) * 4;
    } else if (category == "fives") {
        score = count(dice.begin(), dice.end(), 5) * 5;
    } else if (category == "sixes") {
        score = count(dice.begin(), dice.end(), 6) * 6;
    } else if (category == "three of a kind") {
        for (int i = 1; i <= 6; ++i) {
            if (count(dice.begin(), dice.end(), i) >= 3) {
                score = accumulate(dice.begin(), dice.end(), 0);
                break;
            }
        }
    } else if (category == "four of a kind") {
        for (int i = 1; i <= 6; ++i) {
            if (count(dice.begin(), dice.end(), i) >= 4) {
                score = accumulate(dice.begin(), dice.end(), 0);
                break;
            }
        }
    } else if (category == "full house") {
        bool threeOfKind = false, pair = false;
        for (int i = 1; i <= 6; ++i) {
            if (count(dice.begin(), dice.end(), i) == 3) {
                threeOfKind = true;
            }
            if (count(dice.begin(), dice.end(), i) == 2) {
                pair = true;
            }
        }
        if (threeOfKind && pair) {
            score = 25;
        }
    } else if (category == "small straight") {
        vector<int> sortedDice = dice;
        sort(sortedDice.begin(), sortedDice.end());
        sortedDice.erase(unique(sortedDice.begin(), sortedDice.end()), sortedDice.end());
        if (includes(sortedDice.begin(), sortedDice.end(), vector<int>{1, 2, 3, 4}.begin(), vector<int>{1, 2, 3, 4}.end()) ||
            includes(sortedDice.begin(), sortedDice.end(), vector<int>{2, 3, 4, 5}.begin(), vector<int>{2, 3, 4, 5}.end()) ||
            includes(sortedDice.begin(), sortedDice.end(), vector<int>{3, 4, 5, 6}.begin(), vector<int>{3, 4, 5, 6}.end())) {
            score = 30;
        }
    } else if (category == "large straight") {
        vector<int> sortedDice = dice;
        sort(sortedDice.begin(), sortedDice.end());
        sortedDice.erase(unique(sortedDice.begin(), sortedDice.end()), sortedDice.end());
        if (includes(sortedDice.begin(), sortedDice.end(), vector<int>{1, 2, 3, 4, 5}.begin(), vector<int>{1, 2, 3, 4, 5}.end()) ||
            includes(sortedDice.begin(), sortedDice.end(), vector<int>{2, 3, 4, 5, 6}.begin(), vector<int>{2, 3, 4, 5, 6}.end())) {
            score = 40;
        }
    } else if (category == "yahtzee") {
        for (int i = 1; i <= 6; ++i) {
            if (count(dice.begin(), dice.end(), i) == 5) {
                score = 50;
                break;
            }
        }
    } else if (category == "chance") {
        score = accumulate(dice.begin(), dice.end(), 0);
    }
    return score;
}

int main() {
    srand(time(0));
    vector<string> categories = {"ones", "twos", "threes", "fours", "fives", "sixes", "three of a kind", "four of a kind", 
                                 "full house", "small straight", "large straight", "yahtzee", "chance"};
    vector<int> scores(13, -1); // -1 indicates that the category has not been used

    for (int round = 0; round < 13; ++round) {
        cout << "Round " << round + 1 << endl;
        vector<int> dice = rollDice();
        printDice(dice);

        for (int roll = 1; roll <= 2; ++roll) {
            cout << "Do you want to reroll? (y/n): ";
            char reroll;
            cin >> reroll;
            if (reroll == 'n') {
                break;
            }
            cout << "Enter the positions (1-5) of the dice you want to reroll, separated by spaces: ";
            vector<int> rerollPositions;
            int pos;
            while (cin >> pos) {
                rerollPositions.push_back(pos);
                if (cin.peek() == '\n') break;
            }
            rerollDice(dice, rerollPositions);
            printDice(dice);
        }

        cout << "Available categories: ";
        for (int i = 0; i < categories.size(); ++i) {
            if (scores[i] == -1) {
                cout << categories[i] << " ";
            }
        }
        cout << endl;
        cout << "Enter the category you want to use: ";
        string category;
        cin >> ws; // Ignore leading whitespace
        getline(cin, category);
        
        auto it = find(categories.begin(), categories.end(), category);
        if (it != categories.end() && scores[it - categories.begin()] == -1) {
            scores[it - categories.begin()] = scoreDice(dice, category);
        } else {
            cout << "Invalid category or category already used. Try again." << endl;
            --round;
            continue;
        }
    }

    int totalScore = accumulate(scores.begin(), scores.end(), 0);
    cout << "Your total score is: " << totalScore << endl;

    return 0;
}
