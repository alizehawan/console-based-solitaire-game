#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iomanip>

using namespace std;

template <typename T>
class List {
private:
    // Nested Node class to represent each element in the list
    class Node {
    public:
        T data;        
        bool faceUp;   // Indicates if the card is face up (T) or face down (F)
        Node* next;    // Ptr to the next node in the list
        Node* prev;    // Ptr to the previous node in the list

        // Constructor to initialize the node with data and faceUp flag
        Node(T val, bool faceUp = false) : data(val), faceUp(faceUp), next(nullptr), prev(nullptr) {}
    };

    Node* head;   
    Node* tail;   
    int count;    

public:
    // Nested Iterator class to traverse the list
    class Iterator {
    private:
        Node* current;  
    public:
        
        Iterator(Node* node) : current(node) {}

        // Dereference operator to access the data of the current node
        T& operator*() { return current->data; }

        // Increment operator 
        Iterator& operator++() { current = current->next; return *this; }

        // Comparison operator 
        bool operator!=(const Iterator& other) { return current != other.current; }
    };

    // Constructor to initialize an empty list
    List() : head(nullptr), tail(nullptr), count(0) {}  // Initialize count to 0

    // Destructor to delete all nodes and free memory
    ~List() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Add a new node to the front of the list
    void pushFront(T val, bool faceUp = false) {
        Node* newNode = new Node(val, faceUp);  // Create a new node
        if (!head) {
            // If the list is empty, head and tail point to the new node
            head = tail = newNode;
        }
        else {
            
            newNode->next = head;  // Point new node's next to the current head
            head->prev = newNode;  
            head = newNode;        
        }
        count++;  // Increment the element count
    }

    // Remove and return the front element of the list
    T popFront() {
        if (!head) 
            cout << "List is empty!" << endl;
        T data = head->data;     // Store the data from the head node
        Node* toDelete = head;   // Node to be deleted
        head = head->next;      
        if (head) head->prev = nullptr;  // Update the new head's prev pointer
        else tail = nullptr;     // If the list is empty, set tail to nullptr
        delete toDelete;         // Free memory of the old head node
        count--;  
        return data;
    }

    // Return the number of elements in the list
    int size() const {
        return count;
    }

    // Get the card  at a specific position by index
    T& getCardAt(int index) {
        if (index < 0 || index >= count) {

            cout << "Error: Index out of range" << endl;
        }

        Node* current = head;    // Start from the head node
        int currentIndex = 0;

        // Traverse the list to reach the node at the given index
        while (currentIndex < index && current != nullptr) {
            current = current->next;
            currentIndex++;
        }

        return current->data;  // Return the data of the node at the specified index
    }

    // Clear the entire list, freeing all memory
    void clear() {
        // Delete all nodes in the list
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        // Reset head, tail, and count to represent an empty list
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    // Add a new node to the back of the list
    void pushBack(T val, bool faceUp = false) {
        Node* newNode = new Node(val, faceUp);  // Create a new node
        if (!tail) head = tail = newNode;       // If the list is empty, set head and tail
        else {
            // Adjust pointers to insert the new node at the back
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;  
    }

    // Remove and return the last element in the list
    T popBack() {
        if (!tail) {
            cout << "List is empty" << endl;
            return T(); 
        }
        T data = tail->data;  // Store the data from the tail node
        Node* toDelete = tail;  // Node to be deleted
        tail = tail->prev;  
        if (tail) tail->next = nullptr;  // Update the new tail's next pointer
        else head = nullptr;  // If the list is empty, reset head to nullptr
        delete toDelete;  
        count--;  
        return data; 
    }

    // Move a certain number of cards  from this list to another list
    void moveTo(List& destination, int numCards) {
        Node* current = tail;
        for (int i = 1; i < numCards && current; ++i) {
            current = current->prev;  // Traverse to the node representing the numCards limit
        }

        if (current && current->next) {
            // Separate the portion to be moved
            Node* temp = current->next;
            current->next = nullptr;

            // If destination list is empty move the nodes
            if (!destination.tail) {
                destination.head = temp;
                destination.tail = tail;
            }
            else {
                // Adjust pointers for merging the two lists
                destination.tail->next = temp;
                temp->prev = destination.tail;
                destination.tail = tail;
            }
            tail = current;  

            // Adjust counts for both lists
            int movedCount = 0;
            Node* tempCurrent = destination.tail;
            while (tempCurrent != current) {
                movedCount++;
                tempCurrent = tempCurrent->prev;
            }
            count -= movedCount;
            destination.count += movedCount;
        }
        else {
            cout << "Not enough cards to move!" << endl;
        }
    }

    // Check if the list is empty
    bool isEmpty() const {
        return head == nullptr;
    }

    // Access the data of the last element in the list
    T& back() const {
        if (!tail)
            cout << "List is empty!" << endl;
        return tail->data;
    }


    Iterator begin() const {
        return Iterator(head);
    }


    Iterator end() const {
        return Iterator(nullptr);
    }

    // Display the top card that is face up
    void displayTopCard() const {
        if (!tail) {
            std::cout << "[ ]";  // Empty list
        }
        else {
            Node* current = tail;
            while (current) {
                if (current->faceUp) {
                    current->data.printCard();  // Call the print function of the card
                    break;
                }
                current = current->prev;
            }
            if (!current)
                cout << "[Empty]";
        }
    }

    // Flip the top card to face up
    void flipTopCard() {
        if (tail) {
            tail->faceUp = true;
        }
    }

    // Count the number of cards (elements) that are face up
    int countFaceUp() const {
        int countFaceUp = 0;
        Node* current = tail;
        while (current && current->faceUp) {
            ++countFaceUp;
            current = current->prev;
        }
        return countFaceUp;
    }
};


class Card {
private:

    int suit; 
    int rank;  

public:
    // Constants to represent the suits
    static const int HEARTS = 0;
    static const int DIAMONDS = 1;
    static const int CLUBS = 2;
    static const int SPADES = 3;

    // Constants to represent the rank values
    static const int ACE = 1;    
    static const int KING = 13; 

    // Default constructor initializes the card to Ace of Hearts
    Card() : suit(HEARTS), rank(ACE) {}

    // Parameterized constructor to create a card with specified suit and rank
    Card(int s, int r) : suit(s), rank(r) {}

    // Getter method to get the suit of the card
    int getSuit() const { return suit; }

    // Getter method to get the rank of the card
    int getRank() const { return rank; }

    // Method to check if the card is red (Hearts or Diamonds)
    bool isRed() const {
    
        return suit == HEARTS || suit == DIAMONDS;
    }

    // Method to check if the card is black (Clubs or Spades)
    bool isBlack() const {
 
        return suit == CLUBS || suit == SPADES;
    }

    // Method to check if the card is a King
    bool isKing() const {
        // Returns true if the rank is equal to KING (13)
        return rank == KING;
    }

    // Method to print the card's rank and suit
    void printCard() const {
      
        const char* suits[] = { "\033[31m\x03\033[0m", "\033[31m\x04\033[0m", "\x05", "\x06" };  // Hearts, Diamonds, Clubs, Spades
        // Array of string representations for ranks
        const char* ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
        // Print the rank and suit of the card, using color codes for red suits
        cout << ranks[rank - 1] << suits[suit];
    }
};


template <typename T>
class Stack {
private:
    // Nested Node class to represent each element in the stack
    class Node {
    public:
        T data;         // Stores the data of type T
        Node* next;     // Pointer to the next node in the stack (the one below this node)

        // Constructor to initialize the node with data
        Node(T val) {
            data = val;
            next = nullptr;
        } 
    };

    Node* topNode;   // Pointer to the top node of the stack
    int count;       // Tracks the number of elements in the stack

public:
    // Constructor to initialize an empty stack
    Stack() : topNode(nullptr), count(0) {}

    // Destructor to delete all nodes and free memory
    ~Stack() {
        while (!isEmpty()) pop();  // Pop all elements until the stack is empty
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return topNode == nullptr;
    }

    // Add a new element to the top of the stack
    void push(T val) {
        Node* newNode = new Node(val);  
        newNode->next = topNode;        // Point newnode next to the current top node
        topNode = newNode;              // Move topNode to point to the new node
        count++;  
    }

    //nnumber of elements
    int size() const {
        return count;
    }

    // Remove all elements from the stack
    void clear() {
        while (!isEmpty()) {
            pop();  // pop elements until the stack is empty
        }
    }

    // Remove and return the top element of the stack
    T pop() {
        if (!topNode) {
            cout << "Error: Stack is empty!" << endl;
            return T();  
        }

        Node* toDelete = topNode;   // Store the node to be deleted (the top one)
        T data = topNode->data;     // Store the data from the top node
        topNode = topNode->next;    // Move topNode to the next node (one below)
        delete toDelete;            // Delete the old top node
        count--;  
        return data;  // Return the data from the removed node
    }

    // Access the top element of the stack without removing it
    T& top() const {
        if (!topNode) {
            std::cout << "Error: Stack is empty!" << std::endl;
            static T defaultObj;  // Static object to avoid dangling references in case the stack is empty
            return defaultObj;
        }
        return topNode->data;  // Return the data of the top node
    }

    // Method to check if the stack contains a complete sequence 
    bool isComplete() const {
        if (isEmpty()) return false;  

        Node* current = topNode;  // Start from the top node
        int elementCount = 0;
        int suit = current->data.getSuit();  

        // Traverse the stack to check for a complete sequence of 13 cards in the same suit and in order
        while (current != nullptr) {
            elementCount++;  // Inc element count as we traverse
            if (elementCount > 13 || current->data.getSuit() != suit) {
                return false; 
            }
            if (elementCount != current->data.getRank()) {
                return false;  // Cards are not in the correct order
            }
            current = current->next; 
        }

        return elementCount == 13;  //  complete stack = 13 cards
    }
};


class Command {
public:

    static const int DRAW = 1;                      // Draw command from stockpile
    static const int MOVE = 2;                      // Move command between locations
    static const int UNDO = 3;                      // Undo the last operation
    static const int WASTE_TO_TABLEAU = 4;         // Move card from wastepile to tableau
    static const int WASTE_TO_FOUNDATION = 5;      // Move card from wastepile to foundation
    static const int TABLEAU_TO_FOUNDATION = 6;    // Move card from tableau to foundation
    static const int TABLEAU_MOVE = 7;             // Move card between tableau columns
    static const int QUIT = 8;                      // Quit command
    static const int INVALID = 0;                   // Invalid command type

  
    int type;            // The type of command (e.g., MOVE, DRAW)
    int source;         // Source location for the command
    int destination;    // Destination location for the command
    int numCards;       // Number of cards involved in the command

    // Default constructor initializes an invalid command
    Command() {
        type = INVALID;
        source = -1;
        destination = -1;
        numCards = 0;

    } 

    // Parameterized constructor to create a command with specific details
    Command(int type, int source, int destination, int numCards) {
        type = type;
        source = source;
        destination = destination;
        numCards = numCards;
    }
      
    // Parses a location string & returns its corresponding index
    int parseLocation(const std::string& location) {
        if (location[0] == 'c') {
            // Parse column index (1-based), converting to 0-based by subtracting 1
            return location[1] - '1';
        }
        else if (location[0] == 'f') {
            // Parse foundation index (1-based) and offset it by 10
            return location[1] - '1' + 10;
        }
        else if (location[0] == 'w') {
            // Wastepile is a special case represented by -1
            return -1;
        }
        return -1;  // Invalid location
    }

    // Parses user input command
    void parseInput(const std::string& input) {
        // Reset command attributes to initial values
        type = INVALID;
        source = -1;
        destination = -1;
        numCards = 0;

        // Check if the input is a quit command
        if (input == "q") {
            type = QUIT;  
        }
        else if (input[0] == 's') {
            // Handle the command to draw from stockpile
            type = DRAW;
        }
        else if (input[0] == 'm') {
            // Handle move command: expects format 'm source,destination,numCards'
            std::string src, dest;
            size_t index = 2;  // Start after 'm '

            // Parse source location
            while (input[index] != ',' && index < input.size()) {
                src += input[index++];
            }
            index++;  // Skip the comma

            // Parse destination location
            while (input[index] != ',' && index < input.size()) {
                dest += input[index++];
            }
            index++;  // Skip the comma

            // Parse number of cards 
            if (index < input.size()) {
                numCards = input[index] - '0';
            }

            // Convert source and destination locations into internal representation
            source = parseLocation(src);
            destination = parseLocation(dest);

            // Validate the move: check if both source and destination are valid 
            if (source != -1 && destination != -1 && numCards > 0) {
                // Check if moving to foundation 
                if (destination >= 10 && destination <= 13) {
                    type = TABLEAU_TO_FOUNDATION;  // Moving from Tableau to Foundation
                }
                else {
                    type = TABLEAU_MOVE;  // Moving between Tableau columns
                }
            }
        }
        else if (input[0] == 'w') {
            // Handle wastepile commands
            if (input.size() == 4 && input[2] == 'f') {
                // Move from wastepile to foundation
                type = WASTE_TO_FOUNDATION;
                destination = parseLocation(input.substr(2, 2));  // Parse foundation destination
                numCards = 1;  // Moving one card from waste
            }
            else if (input.size() == 4 && input[2] == 'c') {
                // Move from wastepile to tableau
                type = WASTE_TO_TABLEAU;
                destination = parseLocation(input.substr(2, 2));  // Parse tableau destination
                numCards = 1;  // Moving one card from waste
            }
        }
        else if (input[0] == 'z') {
            // Handle undo command
            type = UNDO;
        }
    }

 
    void display() const {
        // Check the command type and print the corresponding message
        if (type == QUIT) {
            cout << "Command: QUIT " << std::endl;
            cout << "          " << "**********************" << endl;
            cout << "          " << "|    GAME ENDED :)   |" << endl;
            cout << "         " << " **********************" << endl;
            exit(0);  // End the game when quitting
        }
        else if (type == DRAW) {
            cout << "Command: DRAW from stockpile" << endl;
        }
        else if (type == TABLEAU_MOVE) {
            cout << "Command: MOVE " << numCards << " card(s) from column " << (source + 1);
            cout << " to column " << (destination + 1) << endl;  // Output is 1-based
        }
        else if (type == TABLEAU_TO_FOUNDATION) {
            cout << "Command: MOVE from column " << (source + 1) << " to foundation " << (destination - 10 + 1) << endl;  // Convert to 1-based
        }
        else if (type == WASTE_TO_TABLEAU) {
            cout << "Command: MOVE from wastepile to column " << (destination + 1) << endl;  // Output is 1-based
        }
        else if (type == WASTE_TO_FOUNDATION) {
            cout << "Command: MOVE from wastepile to foundation " << (destination - 10 + 1) << endl;  // Output is 1-based
        }
        else if (type == UNDO) {
            cout << "Command: UNDO last operation" << endl;
        }
        else {
            cout << "Command: INVALID" << endl;  // Handles any invalid command types
        }
    }
};


class Game {
private:
    List<Card> tableau[7];  // Array of 7 columns representing the tableau
    Stack<Card> foundation[4];  // Array of 4 stacks representing the foundations
    Stack<Card> stockpile;  // Stack for the stockpile (remaining cards)
    Stack<Card> wastepile;  // Stack for the wastepile (drawn cards from the stockpile)
    Stack<Command> undoStack;  // Stack to keep track of moves for undo operations
    Card lastMovedCards[7];  // Array to keep track of the last moved card for each tableau column

    // Private method to shuffle and deal the deck at the start of the game
    void shuffleAndDeal() {
        Card deck[52];  // Array to hold a standard deck of 52 cards
        int index = 0;

        // Populate the deck with cards
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 1; rank <= 13; ++rank) {
                deck[index++] = Card(suit, rank);  // Create each card with its suit and rank
            }
        }

        
        srand(time(nullptr));  // random number generator
        for (int i = 51; i > 0; --i) {
            int j = rand() % (i + 1);  // Random index from 0 to i
            std::swap(deck[i], deck[j]);  // Swap the current card with a random one
        }

        // Deal the cards to the tableau (face down except for the last card in each column)
        int deckIndex = 0;
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j <= i; ++j) {
                bool faceUp = (j == i);  // Last card in each column is face up
                tableau[i].pushBack(deck[deckIndex++], faceUp);  // Add card to the tableau
            }
        }

        // The remaining cards go into the stockpile
        for (; deckIndex < 52; ++deckIndex) {
            stockpile.push(deck[deckIndex]);  // Push the remaining cards into the stockpile
        }
    }

    // Check if a card can be legally placed on a tableau column
    bool canPlaceOnColumn(const Card& card, int destination) const {
        if (tableau[destination].isEmpty()) {
            // If the column is empty, only a King can be placed
            return card.getRank() == Card::KING;
        }

        // Get the top card of the destination column
        Card topCard = tableau[destination].back();

        // Check if the card is the opposite color and one rank lower
        bool isOppositeColor = (card.isRed() && topCard.isBlack()) || (card.isBlack() && topCard.isRed());
        bool isOneRankLower = (card.getRank() == topCard.getRank() - 1);

        return isOppositeColor && isOneRankLower;
    }

public:
    // Constructor initializes the game by shuffling and dealing the cards
    Game() {
        shuffleAndDeal();
    }

    // Method to draw a card from the stockpile to the wastepile
    void drawCard() {
        if (!stockpile.isEmpty()) {
            // Draw a card from the stockpile and push it to the wastepile
            Card drawnCard = stockpile.pop();
            wastepile.push(drawnCard);
            undoStack.push(Command());  // Record the draw action in the undo stack
        }
        else {
            // If the stockpile is empty, move all cards from the wastepile back to the stockpile
            if (!wastepile.isEmpty()) {
                while (!wastepile.isEmpty()) {
                    Card cardToMove = wastepile.pop();  // Move each card from the wastepile
                    stockpile.push(cardToMove);  // Push it back to the stockpile
                }

                // Now draw the last card from the stockpile
                Card drawnCard = stockpile.pop();
                wastepile.push(drawnCard);
                undoStack.push(Command());  // Record the draw action in the undo stack
            }
            else {
                // No cards left in both stockpile and wastepile
                cout << "No cards in stockpile or wastepile!" << std::endl;
            }
        }
    }

    // Method to display the current state of the game
    void display() const {
     
        cout << setw(8) << "Stock" << setw(20) << "Waste" << setw(20) << "Foundation 1"
            << setw(18) << "Foundation 2" << setw(18) << "Foundation 3"
            << setw(19) << "Foundation 4" << endl;

        // Display the stockpile: show the size or empty if there are no cards left
        cout << "[ ";
        if (stockpile.isEmpty()) {
            cout << " ] ";  // Empty stockpile
        }
        else {
            cout << "(" << stockpile.size() << " cards)] ";  // Show the number of cards left
        }

        // Display the wastepile: show the top card or empty if no cards
        cout << setw(12);
        if (!wastepile.isEmpty()) {
            wastepile.top().printCard();  // Print the top card in the wastepile
        }
        else {
            cout << "[ ]";  // Empty wastepile
        }

        // Display the top cards of each foundation pile
        for (int i = 0; i < 4; ++i) {
            cout << setw(10) << "[";
            if (!foundation[i].isEmpty()) {
                foundation[i].top().printCard();  
            }
            else {
                cout << " ";  // Empty foundation pile
            }
            cout << "] (" << foundation[i].size() << " cards)";  // Display the number of cards in the foundation
        }
        cout << endl << endl;


        for (int i = 0; i < 7; ++i) {
            cout << setw(13) << "Column " << i + 1; 
        }
        cout << endl << endl;

        // Display the number of cards in each tableau column
        for (int i = 0; i < 7; ++i) {
            cout << setw(4) << "(" << tableau[i].size() << " cards)";  // Show card count per column
        }
        cout << endl;

        // Display the top card of each tableau column or show empty
        for (int i = 0; i < 7; ++i) {
            cout << setw(8) << "[";
            if (!tableau[i].isEmpty()) {
                tableau[i].displayTopCard();  
            }
            else {
                cout << " ";  // Empty column
            }
            cout << "]";
        }
        cout <<endl;
    }

    // Destructor to clean up memory and clear all piles
    ~Game() {
        // Clear each tableau column
        for (int i = 0; i < 7; ++i) {
            tableau[i].clear();  // Clear all cards from each tableau column
        }

        // Clear each foundation stack
        for (int i = 0; i < 4; ++i) {
            foundation[i].clear();  // Clear all cards from each foundation stack
        }

     
        stockpile.clear(); 
        wastepile.clear();  

        // Clear the undo stack
        undoStack.clear();  
    }


    // Move a card from a tableau column to a foundation
    void moveColumnToFoundation(int source, int foundationIndex) {
        // Check for valid source column and foundation index
        if (source < 0 || source >= 7 || foundationIndex < 0 || foundationIndex >= 4) {
            cout << "Invalid column or foundation index!" << endl;
            return; 
        }

        // Ensure the source column has cards to move
        if (tableau[source].isEmpty()) {
            cout << "Source column is empty!" << endl;
            return;  
        }

        // Get the top card from the source column
        Card topCard = tableau[source].back();

       //check if the top card can go to the foundation
        if (canMoveToFoundation(topCard, foundationIndex)) {
            // Move the card to the foundation
            foundation[foundationIndex].push(tableau[source].popBack());

            // Log the move to enable undo functionality
            Command cmd;
            cmd.type = Command::MOVE;  // Set command type to MOVE
            cmd.source = source;  // Record the source column
            cmd.destination = foundationIndex + 10;  // Offset foundation index for logging
            cmd.numCards = 1;
            undoStack.push(cmd);  // Push command onto the undo stack

            // Flip the new top card in the source column if any cards remain
            if (!tableau[source].isEmpty()) {
                tableau[source].flipTopCard(); // Ensure the new top card is flipped
            }
        }
        else {
            cout << "Invalid move to foundation!" << endl;  // If the move isn't valid
        }
    }

    // Move a card from the wastepile to the foundation
    void wasteToFoundation(int foundationIndex) {
        // Check for valid foundation index
        if (foundationIndex < 0 || foundationIndex >= 4) {
            cout << "Invalid foundation index!" << endl;
            return;  // Exit if the index is invalid
        }

        // Check if the wastepile is not empty
        if (!wastepile.isEmpty()) {
            Card cardToMove = wastepile.top();  // Get the top card from the wastepile
            // Validate if the card can move to the specified foundation
            if (canMoveToFoundation(cardToMove, foundationIndex)) {
                // Move the card to the foundation
                foundation[foundationIndex].push(wastepile.pop());

              
                Command cmd;
                cmd.type = Command::MOVE;  // Set command type to MOVE
                cmd.source = -1;  // -1 indicates the source is the wastepile
                cmd.destination = foundationIndex + 10;  // Use offset for foundation logging
                cmd.numCards = 1;  // Only moving one card
                undoStack.push(cmd);  // Push command onto the undo stack
            }
            else {
                cout << "Invalid move to foundation!" << endl;  // If the move isn't valid
            }
        }
        else {
            cout << "Wastepile is empty!" << endl;  // Exit if the wastepile is empty
        }
    }

    // Move a card from a tableau column to a foundation
    void tableauToFoundation(int source, int foundationIndex) {
        // Check for valid column index
        if (source < 0 || source >= 7) {
            cout << "Invalid column index!" << endl;
            return;  
        }


        if (!tableau[source].isEmpty()) {
            Card cardToMove = tableau[source].back();  // Get the top card from the tableau

            if (canMoveToFoundation(cardToMove, foundationIndex)) {
                // Move the card to the foundation
                foundation[foundationIndex].push(tableau[source].popBack());
                Command cmd;
                cmd.type = Command::MOVE;  
                cmd.source = source; 
                cmd.destination = foundationIndex + 10;  // Offset foundation index
                cmd.numCards = 1;  // Only moving one card
                undoStack.push(cmd);  // Push command onto the undo stack

                
                if (!tableau[source].isEmpty()) {
                    tableau[source].flipTopCard();
                }
            }
            else {
                cout << "Invalid move to foundation!" << endl;  
            }
        }
        else {
            cout << "Tableau column is empty!" << endl;  
        }
    }

    // Display the moved card details
    void displayMovedCard(const Card& card, int destination) const {
        card.printCard();  
    }

    // Move a card from the wastepile to a tableau column
    void wasteToTableau(int destination) {
        // Check if the wastepile is not empty
        if (!wastepile.isEmpty()) {
            Card cardToMove = wastepile.top();  // Get the top card from the wastepile
            // Validate if the card can be placed on the specified tableau column
            if (canPlaceOnColumn(cardToMove, destination)) {
                tableau[destination].pushBack(wastepile.pop());  // Move the card to the tableau

              
                cout << "Moved Card to Column " << (destination + 1) << ": ";
                cardToMove.printCard();  
                cout << endl;  

                // Log the move for undo functionality
                Command cmd;
                cmd.type = Command::MOVE;  // Set command type to MOVE
                cmd.source = -1;  // -1 indicates the source is the wastepile
                cmd.destination = destination;  // Record the destination
                cmd.numCards = 1;  
                undoStack.push(cmd);  // Push command onto the undo stack
            }
            else {
                cout << "Invalid move to tableau!" << endl;  // If the move isn't valid
            }
        }
        else {
            cout << "Wastepile is empty!" << endl;  // Exit if the wastepile is empty
        }
    }

    // Move cards from one tableau column to another tableau column
    void tableauMove(int source, int destination, int numCards) {
        // Check for valid source and destination column indices
        if (source < 0 || source >= 7 || destination < 0 || destination >= 7) {
            cout << "Invalid column indices!" << endl;
            return;  
        }

        // Check if the source tableau column is not empty and numCards is valid
        if (!tableau[source].isEmpty() && numCards > 0) {
          
            if (numCards > tableau[source].size()) {
                cout << "Not enough cards in the source column to move!" << endl;
                return;  
            }

            // Get the card to move (from the specified number of cards)
            Card cardToMove = tableau[source].getCardAt(tableau[source].size() - numCards);

            //ckeck if the card can be placed on the dest col
            if (canPlaceOnColumn(cardToMove, destination)) {
                // Move num of cards from src to dest
                for (int i = 0; i < numCards; ++i) {
                    tableau[destination].pushBack(tableau[source].popBack());
                }

                
                cout << "Moved Card to Column " << (destination + 1) << ": ";
                cardToMove.printCard();  
                cout << endl;  

                // Log the move for undo functionality
                Command cmd;
                cmd.type = Command::MOVE;  // Set command type to MOVE
                cmd.source = source;  // Record the src col
                cmd.destination = destination;  // Record the dest col
                cmd.numCards = numCards;  // Number of cards moved
                undoStack.push(cmd);  // Push command on undo stack

                // Flip the new top card in the source column, if any
                if (!tableau[source].isEmpty()) {
                    tableau[source].flipTopCard();
                }
            }
            else {
                cout << "Cannot move to destination column!" << endl;  // If the move isn't valid
            }
        }
        else {
            cout << "Source column is empty or invalid number of cards!" << endl;  
        }
    }
    void undoMove() {
  
        if (undoStack.isEmpty()) {
            cout << "No moves to undo!" << endl;
            return;
        }

        // Get the last command from the undo stack
        Command lastCommand = undoStack.top();
        undoStack.pop();  

        //  undoing different types of moves based on the last command
        switch (lastCommand.type) {
        case Command::DRAW:
            // Undo a draw
            if (!wastepile.isEmpty()) {
                stockpile.push(wastepile.pop());
            }
            break;

        case Command::MOVE:
            //  move the card(s) back from the destination to the source
            moveCard(lastCommand.destination, lastCommand.source, lastCommand.numCards);
            break;

        case Command::WASTE_TO_TABLEAU:
            
            if (!tableau[lastCommand.destination].isEmpty()) {
                wastepile.push(tableau[lastCommand.destination].popBack());
            }
            break;

        case Command::WASTE_TO_FOUNDATION:
           
            if (!foundation[lastCommand.destination - 10].isEmpty()) {
                wastepile.push(foundation[lastCommand.destination - 10].pop());
            }
            break;

        case Command::TABLEAU_TO_FOUNDATION:
            
            if (!foundation[lastCommand.destination - 10].isEmpty()) {
                tableau[lastCommand.source].pushBack(foundation[lastCommand.destination - 10].pop());
            }
            break;

        case Command::TABLEAU_MOVE:
            // Undo a move between cols move cards back to the source tableau
            for (int i = 0; i < lastCommand.numCards; ++i) {
                tableau[lastCommand.source].pushBack(tableau[lastCommand.destination].popBack());
            }
            // Flip the new top card in the destination column (if any cards remain)
            if (!tableau[lastCommand.destination].isEmpty()) {
                tableau[lastCommand.destination].flipTopCard();
            }
            break;

        default:
            cout << "No valid command to undo!" << endl;
            break;
        }

        // Optionally, check if the game is over after undoing a move
        checkGameEnd();
    }

   
    void executeCommand(const Command& cmd) {
        switch (cmd.type) {
        case Command::DRAW:
            drawCard();  
            undoStack.push(cmd);  
            break;

        case Command::MOVE:
           
            moveCard(cmd.source, cmd.destination, cmd.numCards);
            undoStack.push(cmd); 
            break;

        case Command::UNDO:
            // Undo the last move
            undoMove();
            break;

        case Command::WASTE_TO_TABLEAU:
         
            wasteToTableau(cmd.destination);
            undoStack.push(cmd);  
            break;

        case Command::WASTE_TO_FOUNDATION:
        
            wasteToFoundation(cmd.destination - 10);
            undoStack.push(cmd); 
            break;

        case Command::TABLEAU_TO_FOUNDATION:
       
            tableauToFoundation(cmd.source, cmd.destination - 10);
            undoStack.push(cmd);  
            break;

        case Command::TABLEAU_MOVE:
            
            tableauMove(cmd.source, cmd.destination, cmd.numCards);
            undoStack.push(cmd); 
            break;

        default:
            cout << "Invalid command!" << endl;
            break;
        }

        // Check if the game is won after executing the command
        checkGameEnd();
    }

    // handles diff moves
    void moveCard(int source, int destination, int numCards) {
        if (source >= 0 && source < 7 && destination >= 0 && destination < 7) {
            // Move cards between columns
            tableauMove(source, destination, numCards);
        }
        else if (source == -1) {
            // Move a card from the wastepile to col
            wasteToTableau(destination);
        }
        else if (destination >= 10 && destination <= 13) {
            // Move a card from a column to a foundation pile
            tableauToFoundation(source, destination - 10);
        }
        else if (source == -1 && destination >= 10 && destination <= 13) {
            // Move a card from the wastepile to a foundation pile
            wasteToFoundation(destination - 10);
        }
        else {
    
            cout << "Invalid move!" << endl;
        }
    }

    // Ensure to check the win condition after each move
    void checkGameEnd() {
        if (checkWinCondition()) {
            cout << "*************************************" << endl;
            cout << "Congratulations! You've won the game!" << endl;
            cout << "*************************************" << endl;
        }
    }

    
    bool canMoveToFoundation(const Card& card, int foundationIndex) const {
        if (foundation[foundationIndex].isEmpty()) {
            // If the foundation is empty, only an Ace can be placed
            return card.getRank() == Card::ACE;
        }
        else {
            // Get the top card of the foundation pile
            const Card& topCard = foundation[foundationIndex].top();
            // The card can be placed if it is the same suit and one rank higher than the top card
            return card.getSuit() == topCard.getSuit() && card.getRank() == topCard.getRank() + 1;
        }
    }


    void undoDraw() {
        if (!undoStack.isEmpty()) {
            Command lastCommand = undoStack.pop();  // Get the last command
            if (lastCommand.type == Command::DRAW) {
                // Undo draw: Move the card from wastepile back to stockpile
                if (!wastepile.isEmpty()) {
                    Card drawnCard = wastepile.pop();  // Remove the top card from wastepile
                    stockpile.push(drawnCard);         // Restore the card back to stockpile
                }
                else {
                    cout << "Wastepile is empty, cannot undo draw!" << endl;
                }
            }
            else {
                cout << "Last command was not a draw!" << endl;
            }
        }
        else {
            cout << "No moves to undo!" << endl;
        }
    }

    // Undo the move of a card from waste to foundation
    void undoWasteToFoundation() {
        if (!undoStack.isEmpty()) {
            Command lastCommand = undoStack.pop();  // Get the last command
            if (lastCommand.type == Command::WASTE_TO_FOUNDATION) {
                // Undo by moving the card from foundation back to wastepile
                if (foundation[lastCommand.destination - 10].isEmpty()) {
                    cout << "Foundation is empty, cannot undo waste to foundation!" << endl;
                    return;
                }
                Card cardMoved = foundation[lastCommand.destination - 10].pop();  // Remove card from foundation
                wastepile.push(cardMoved);  // Add card back to wastepile
            }
            else {
                cout << "Last command was not a waste to foundation move!" << endl;
            }
        }
        else {
            cout << "No moves to undo!" << endl;
        }
    }

    // Undo the move of a card from foundation to column
    void undoFoundationToTableau() {
        if (!undoStack.isEmpty()) {
            Command lastCommand = undoStack.pop();  // Get the last command
            if (lastCommand.type == Command::TABLEAU_TO_FOUNDATION) {
                // Undo by moving the card from foundation back to tableau
                if (foundation[lastCommand.source - 10].isEmpty()) {
                    cout << "Foundation is empty, cannot undo foundation to tableau!" << endl;
                    return;
                }
                Card cardMoved = foundation[lastCommand.source - 10].pop();  // Remove card from foundation
                tableau[lastCommand.destination].pushBack(cardMoved);  // Add card back to tableau
            }
            else {
                cout << "Last command was not a foundation to tableau move!" << endl;
            }
        }
        else {
            cout << "No moves to undo!" << endl;
        }
    }

    // Undo the move of a card from column to foundation
    void undoTableauToFoundation() {
        if (!undoStack.isEmpty()) {
            Command lastCommand = undoStack.pop();  // Get the last command
            if (lastCommand.type == Command::TABLEAU_TO_FOUNDATION) {
                // Undo by moving the card from tableau back to foundation
                if (tableau[lastCommand.source].isEmpty()) {
                    cout << "Tableau is empty, cannot undo tableau to foundation!" << endl;
                    return;
                }
                Card cardMoved = tableau[lastCommand.source].popBack();  // Remove card from tableau
                foundation[lastCommand.destination - 10].push(cardMoved);  // Add card back to foundation

                // Output feedback
                cout << "Moved card back from tableau " << (lastCommand.source + 1)
                    << " to foundation " << (lastCommand.destination - 10 + 1) << endl;
            }
            else {
                cout << "Last command was not a tableau to foundation move!" << endl;
            }
        }
        else {
            cout << "No moves to undo!" << endl;
        }
    }

    // Undo the move of a card from waste to column
    void undoWasteToTableau() {
        if (!undoStack.isEmpty()) {
            Command lastCommand = undoStack.pop();  // Get the last command
            if (lastCommand.type == Command::WASTE_TO_TABLEAU) {
                // Undo by moving the card from column back to wastepile
                if (tableau[lastCommand.destination].isEmpty()) {
                    cout << "Tableau is empty, cannot undo waste to tableau!" << endl;
                    return;
                }
                Card cardMoved = tableau[lastCommand.destination].popBack();  // Remove card from tableau
                wastepile.push(cardMoved);  // Add card back to wastepile
            }
            else {
                cout << "Last command was not a waste to tableau move!" << endl;
            }
        }
        else {
            cout << "No moves to undo!" << endl;
        }
    }

    // Undo the last move, determining the type 
    void undo() {
        if (!undoStack.isEmpty()) {
            Command lastCommand = undoStack.pop();  // Get the last command
            if (lastCommand.type == Command::DRAW) {
                wastepile.pop();  // Undo draw by removing the card from wastepile
                stockpile.push(wastepile.pop());  // Restore the card back to stockpile
            }
            else if (lastCommand.type == Command::MOVE) {
                tableau[lastCommand.destination].moveTo(tableau[lastCommand.source], lastCommand.numCards);  // Move cards back
                if (!tableau[lastCommand.source].isEmpty()) {
                    tableau[lastCommand.source].flipTopCard();  // Flip top card if necessary
                }
            }
            else {
                cout << "Unknown command to undo!" << endl;
            }
        }
        else {
            cout << "No moves to undo!" << endl;
        }
    }

    // Move multiple cards between tableau columns
    void moveMultipleCards(int source, int destination, int numCards) {
        if (source < 0 || source >= 7 || destination < 0 || destination >= 7) {
            cout << "Invalid column indices!" << endl;
            return;
        }

        // Ensure enough face-up cards are available to move
        if (numCards <= tableau[source].countFaceUp()) {
            Card topCard = tableau[source].back();

            // Validate if the top card can be moved to the destination
            if (canPlaceOnColumn(topCard, destination)) {
                tableau[source].moveTo(tableau[destination], numCards);  // Move cards between tableau columns

                // Record the move for undoing later
                Command cmd;
                cmd.type = Command::MOVE;
                cmd.source = source;
                cmd.destination = destination;
                cmd.numCards = numCards;
                undoStack.push(cmd);

                // Flip the new top card in the source column if necessary
                if (!tableau[source].isEmpty()) {
                    tableau[source].flipTopCard();
                }
            }
            else {
                cout << "Cannot move to destination column!" << endl;
            }
        }
        else {
            cout << "Not enough face-up cards to move!" << endl;
        }
    }

    // Check if the player has won the game by completing all foundation piles
    bool checkWinCondition() const {
        for (int i = 0; i < 4; ++i) {
            if (!foundation[i].isComplete()) {
                return false;  //foundation is incomplete so not won
            }
        }
        return true;  // foundation are complete
    }
};

int main() {
    Game game;
    Command command;
    char input[50];

    game.display();

    while (true) {
        cout << "Enter a command: ";
        cin.getline(input, 50);
        cout << endl << endl << endl;
        command.parseInput(input);
        command.display();
        game.executeCommand(command);
        game.display();
    }
    cout << endl;
    cout << endl;

    return 0;

}





