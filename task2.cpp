#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;


class Card {
public:
    enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
    enum Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
private:
    Suit suit;
    Rank rank;
public:
    Card(Suit s, Rank r) : suit(s), rank(r) {}
    Suit getSuit() const { return suit; }
    string toString() const {
        string r;
        if (rank == ACE) r = "A";
        else if (rank == JACK) r = "J";
        else if (rank == QUEEN) r = "Q";
        else if (rank == KING) r = "K";
        else if (rank == TEN) r = "10";
        else r = to_string(rank);
        string s = suit == HEARTS ? "♥" : suit == DIAMONDS ? "♦" : suit == CLUBS ? "♣" : "♠";
        return r + s;
    }
};

// Клас колоди
class Deck {
protected:
    vector<Card> cards;
public:
    Deck(bool shuffle = false) {
        for (int s = Card::HEARTS; s <= Card::SPADES; ++s)
            for (int r = Card::ACE; r <= Card::KING; ++r)
                cards.emplace_back(static_cast<Card::Suit>(s), static_cast<Card::Rank>(r));
        if (shuffle) this->shuffle();
    }
    void shuffle() {
        static mt19937 rng(static_cast<unsigned>(time(nullptr)));
        std::shuffle(cards.begin(), cards.end(), rng);
    }
    bool isEmpty() const { return cards.empty(); }
    size_t size() const { return cards.size(); }
    void display() const {
        for (const auto& card : cards)
            cout << card.toString() << " ";
        cout << endl;
    }
};

// Клас пасьянсу
class Solitaire : public Deck {
public:
    Solitaire(bool shuffle = true) : Deck(shuffle) {}
    void play() {
        cout << "Starting solitaire with " << cards.size() << " cards:\n";
        display();
        for (int pass = 1; pass <= 3 && cards.size() >= 3; ++pass) {
            cout << "\nPass " << pass << ":\n";
            bool removed = false;
            for (size_t i = 0; i < cards.size() - 2; ++i) {
                bool leftRed = (cards[i].getSuit() == Card::HEARTS || cards[i].getSuit() == Card::DIAMONDS);
                bool rightRed = (cards[i + 2].getSuit() == Card::HEARTS || cards[i + 2].getSuit() == Card::DIAMONDS);
                if (leftRed == rightRed) {
                    cout << "Removing: " << cards[i].toString() << " and " << cards[i + 2].toString() << endl;
                    cards.erase(cards.begin() + i + 2);
                    cards.erase(cards.begin() + i);
                    removed = true;
                    i--;
                }
                if (cards.size() < 3) break;
            }
            if (!removed) {
                cout << "No more cards to remove.\n";
                break;
            }
            cout << "Remaining:\n";
            display();
        }
        cout << "\nFinal result: " << cards.size() << " cards left\n";
        display();
    }
};

int main() {
    char choice;
    do {
        cout << "\n1 - Show ordered deck\n2 - Show shuffled deck\n3 - Play solitaire\nq - Quit\nChoice: ";
        cin >> choice;
        if (choice == '1') {
            Deck deck(false);
            deck.display();
        } else if (choice == '2') {
            Deck deck(true);
            deck.display();
        } else if (choice == '3') {
            Solitaire game;
            game.play();
        } else if (choice != 'q') {
            cout << "Invalid choice.\n";
        }
    } while (choice != 'q');
    return 0;
}
