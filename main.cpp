 // Samuel Maldonado 801-23-8979
// Ricardo Lopez 801-23-
// Luis Caraballo
// CCOM3033-002     Diciembre 4, 2024
// Projecto Blackjack

// -------------------------------------Referencias----------------------------------------------//
// ascii text to art converter for menu ==
// https://patorjk.com/software/taag/#p=display&f=Big%20Money-nw&t=Welcome%20to%20blackjack
// https://patorjk.com/software/taag/#p=display&f=Cards&t=SME
// for random number generator ==
// https://www.bogotobogo.com/cplusplus/RandomNumbers.php#:~:text=The%20best%20solution%20is%20to,((int)%20time(0))%3B
//for color handling in the terminal using ANSI escape codes ==
//https://www.geeksforgeeks.org/how-to-change-console-color-in-cpp/

//------------------------Startup------------------------------//

#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <iostream>
using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"

int option_selected = 0;
bool playerwin = false, roundwon;
bool tableopen = true;
int deck[52][2];
int dealtcards = 0, player_count = 0, dealer_count = 0, player_money = 100,dealer_money = 100;
string play_again;

// ------------Prototipos------------//

int main_menu();
bool gameplay();
bool multiplayer_gameplay();
void endgame_message(bool playerwin);
void createDeck(int deck[52][2]);
void shuffleDeck(int deck[52][2]);
void printDeck(const int deck[52][2]);
int dealdealer(int deck[52][2], int &dealtcards);
int dealplayer(int deck[52][2], int &dealtcards);
int playeroptions(int deck[52][2], int dealtcards);
int dealerlogic(int deck[52][2], int &dealtcards, int &dealer_count);
int dealer_hit(int deck[52][2], int &dealtcards, int &dealer_count);
int player_hit(int deck[52][2], int &dealtcards, int &player_count);
void round_end(int player_count, int dealer_count, int &player_money);
void print_division();
int playerI_hit(int deck[52][2], int &dealtcards, int &player_count);
int dealplayerI(int deck[52][2], int &dealtcards);

// -------------Main------------//
int main() {
    
  // main menu
  option_selected =
      main_menu(); // Displays menu and verfifies input for menu selection

  // menu options selected
  switch (option_selected) {
  //--- Singleplayer option code ---//
  case 1:
    createDeck(deck);  // creates the array
    shuffleDeck(deck); // shuffles randomly the deck array

    do {
      dealtcards = 0;
      shuffleDeck(deck); // shuffles randomly the deck array
      print_division();
      dealer_count =dealdealer(deck, dealtcards); // dealer takes cards from deck
      player_count =dealplayer(deck, dealtcards); // player takes cards from deck
      player_count = playeroptions(deck, dealtcards); // player options affect players count
      dealer_count = dealerlogic(deck, dealtcards,dealer_count); // dealer hits if more than 17
      round_end(player_count, dealer_count,player_money); // displays the results of the round
      do{
    cout << "\n Play another round? (y/n): ";
    cin >> play_again;
    }while (play_again != "y" && play_again != "n");

    if (play_again == "f"){
      tableopen = false;
    }

      


      if (player_money <= 0) {
        playerwin = false;
        tableopen = false;
      }

      if (player_money >= 200) {
        playerwin = true;
        tableopen = false;
      }

    } while (tableopen == true);
    // printDeck(deck);   commented function to check if deck works

    endgame_message(playerwin); // gameplay end
    break;

    //--- Multiplayer option code ---//
  case 2:
    createDeck(deck);  // Creates the array
    shuffleDeck(deck); // Shuffles the deck

    int num_players;
    cout << "How many players are playing? (2-4): ";
    cin >> num_players; // Get number of players

    int player_money[num_players];
    int player_counts[num_players];
    bool player_busted[4] = {false}; // Fixed size for up to 4 players

    for (int i = 0; i < num_players; ++i) {
      player_money[i] = 100; // Each player starts with 100 money
      player_counts[i] = 0;  // Initialize player counts
    }

    dealer_money = 100; // Dealer's initial money

    while (true) {
      dealer_count = dealdealer(deck, dealtcards); // Dealer starts with 2 cards

      // Each player plays their turn
      for (int i = 0; i < num_players; ++i) {
        if (player_money[i] > 0) { // Skip players with no money
          cout << "\nPlayer " << i + 1 << "'s turn. Money: $" << player_money[i]
               << endl;
          player_counts[i] =
              dealplayer(deck, dealtcards); // Player starts with 2 cards

          // Player actions: Hit or Stand
          while (true) {
            cout << "\nPlayer " << i + 1
                 << "'s current count: " << player_counts[i] << endl;
            int choice;
            cout << "Hit (1) or Stand (2)? ";
            cin >> choice;

            if (choice == 1) {
              player_counts[i] = player_hit(deck, dealtcards, player_counts[i]);
              if (player_counts[i] > 21) {
                cout << "Player " << i + 1 << " busted!" << endl;
                player_busted[i] = true;
                break;
              }
            } else if (choice == 2) {
              break; // Player chooses to stand
            } else {
              cout << "Invalid choice. Please enter 1 or 2.\n";
            }
          }
        }
      }

      // Dealer's turn
      dealer_count = dealerlogic(deck, dealtcards, dealer_count);

      // Compare dealer's count with each player's count
      for (int i = 0; i < num_players; ++i) {
        if (!player_busted[i] && player_money[i] > 0) {
          cout << "\nPlayer " << i + 1 << " count: " << player_counts[i]
               << "\tDealer count: " << dealer_count << endl;

          if (dealer_count > 21 || player_counts[i] > dealer_count) {
            cout << "Player " << i + 1 << " wins this round!\n";
            player_money[i] += 10;
          } else if (player_counts[i] < dealer_count) {
            cout << "Dealer wins against Player " << i + 1 << ".\n";
            player_money[i] -= 10;
          } else {
            cout << "Player " << i + 1 << " ties with the dealer.\n";
          }

          if (player_money[i] <= 0) {
            cout << "Player " << i + 1 << " is out of money!" << endl;
          }
        }
      }

      // Check if all players are out of money or a player reaches the win
      // condition
      bool all_out_of_money = true;
      for (int i = 0; i < num_players; ++i) {
        if (player_money[i] > 0) {
          all_out_of_money = false;
          if (player_money[i] >= 200) {
            cout << "\nPlayer " << i + 1
                 << " wins the game with $200 or more!\n";
            endgame_message(true);
            return 0;
          }
        }
      }

      if (all_out_of_money) {
        cout << "All players are out of money! Dealer wins the game.\n";
        endgame_message(false);
        return 0;
      }
    }
  }
}


//-------------------Definiciones---------------------------//

// function for how the menu works and is displayed
int main_menu() {
  string option_selected_string;

  cout
      << "\033[34m  "
         "+-----------------------------------------------------------------+\n"
         "  |                                                                 "
         "|\n"
         "  |   \033[32m____   _       ____    __  __  _   ____   ____    __  __  _   "
         "\033[34m|\n"
         "  |  |    \\ | |     /    |  /  ]|  |/ ] |    | /    |  /  ]|  |/ ]  "
         "|\n"
         "  |  |  o  )| |    |  o  | /  / |  ' /  |__  ||  o  | /  / |  ' /   "
         "|\n"
         "  |  |     || |___ |     |/  /  |    \\  __|  ||     |/  /  |    \\  "
         " |\n"
         "  |  |  O  ||     ||  _  /   \\_ |     \\/  |  ||  _  /   \\_ |     "
         "\\  |\n"
         "  |  |     ||     ||  |  \\     ||  .  |\\  `  ||  |  \\     ||  .  "
         "|  |\n"
         "  |  |_____||_____||__|__|\\____||__|\\_| "
         "\\____||__|__|\\____||__|\\_|  |\n"
         "  |                                                                 "
         "|\n"
         "  |                                                                 "
         "|\n"
         "  |           \033[31mS                    \033[33mM                    \033[32mE           "
         "\033[34m|\n"
         "  |        [Solo]         [Multiplayer(2-4)]          [End]         "
         "|\n"
         "  |                                                                 "
         "|\n"
         "  |       .------.             .------.              .------.       "
         "|\n"
         "  |       |S.--. |             |M.--. |              |E.--. |       "
         "|\n"
         "  |       | :/\\: |             | (\\/) |              | (\\/) |     "
         "  |\n"
         "  |       | :\\/::|             | :\\/::|              | :\\/::|     "
         "  |\n"
         "  |       | '--'S|             | '--'M|              | '--'E|       "
         "|\n"
         "  |       `------'             `------'              `------'       "
         "|\n"
         "  |                                                                 "
         "|\n\033[0m";
  do {
    cout << "\033[33m  |  Enter an options S M or E: \033[0m";
    cin >> option_selected_string;
  } while (option_selected_string != "S" && option_selected_string != "M" && option_selected_string != "E");
  if (option_selected_string == "S")
    option_selected = 1;
  else if (option_selected_string == "M")
    option_selected = 2;
  else if (option_selected_string == "E")
    option_selected = 3;

  return option_selected;
}

bool multiplayer_gameplay() {
  bool won_game = false;

  return won_game;
}

// function for the logic and gameplay
bool gameplay() {

  bool won_game = false;

  return won_game;
}

// Function for the message after win condition
void endgame_message(bool playerwin) {
  if (playerwin) {
    cout << "\033[32m  "
            "+-----------------------------------------------------------------"
            "+\n"
            "  |                                                               "
            "  |\n"
            "  |              *** You won, thanks for playing! ***             "
            "  |\n"
            "  |                                                               "
            "  |\n"
            "  "
            "+-----------------------------------------------------------------"
            "+\n\033[0m";

  } else {
    cout << "\033[31m  "
            "+-----------------------------------------------------------------"
            "+\n"
            "  |                                                               "
            "  |\n"
            "  |         The house always wins... better luck next time.       "
            "  |\n"
            "  |                                                               "
            "  |\n"
            "  "
            "+-----------------------------------------------------------------"
            "+\n\033[0m";
  }
}

// Function to create a deck of cards using integers
void createDeck(int deck[52][2]) {
  int index = 0;

  // Loop through suits (1 = hearts, 2 = spades, 3 = diamonds, 4 = clubs)
  for (int suit = 1; suit <= 4; ++suit) {
    // Loop through ranks (1 = Ace, 2 = 2, ..., 11 = Jack, 12 = Queen, 13 =
    // King)
    for (int rank = 1; rank <= 13; ++rank) {
      deck[index][0] = rank; // Assign rank
      deck[index][1] = suit; // Assign suit
      ++index;
    }
  }
}

// Function to shuffle the deck
void shuffleDeck(int deck[52][2]) {
  srand(time(0)); // Seed the random generator

  for (int i = 0; i < 52; ++i) {
    int randomIndex = rand() % 52;

    // Manual swapping of rank and suit
    int tempRank = deck[i][0];
    int tempSuit = deck[i][1];

    deck[i][0] = deck[randomIndex][0];
    deck[i][1] = deck[randomIndex][1];

    deck[randomIndex][0] = tempRank;
    deck[randomIndex][1] = tempSuit;
  }
}

// Function to print the deck in a readable format
void printDeck(const int deck[52][2]) {
  for (int i = 0; i < 52; ++i) {
    string rank;
    string suit;

    // Convert rank to string
    if (deck[i][0] == 1)
      rank = "A";
    else if (deck[i][0] == 11)
      rank = "J";
    else if (deck[i][0] == 12)
      rank = "Q";
    else if (deck[i][0] == 13)
      rank = "K";
    else
      rank = to_string(deck[i][0]);

    // Convert suit to string
    switch (deck[i][1]) {
    case 1:
      suit = "hearts";
      break;
    case 2:
      suit = "spades";
      break;
    case 3:
      suit = "diamonds";
      break;
    case 4:
      suit = "clubs";
      break;
    }

    // Print the card
    cout << rank << "_" << suit << " ";
  }
  cout << endl;
}

// function to deal cards to dealer
int dealdealer(int deck[52][2], int &dealtcards) {
  dealer_count = 0;
  dealer_hit(deck, dealtcards, dealer_count);
  dealer_hit(deck, dealtcards, dealer_count); // Dealer starts with 2 cards
  return dealer_count;
}

// function to deal cards to user
int dealplayer(int deck[52][2], int &dealtcards) {
  player_count = 0;
  player_hit(deck, dealtcards, player_count);
  player_hit(deck, dealtcards, player_count); // Player starts with 2 cards
  return player_count;
}

// function to hit or stand
int playeroptions(int deck[52][2], int dealtcards) {
  int choice;
  do {
    cout << "\nYour current count: " << player_count
         << "\t\t Dealers count:" << dealer_count << endl;
    cout << "\nHit (1) or Stand (2)? ";
    cin >> choice;
  } while (choice != 1 && choice != 2);
  if (choice == 1) {
    player_count = player_hit(deck, dealtcards, player_count);
  }
  return player_count;
}

// dealer logic to hit again if below 17
int dealerlogic(int deck[52][2], int &dealtcards, int &dealer_count) {
  while (dealer_count < 17) {

    dealer_hit(deck, dealtcards, dealer_count);
  }
  return dealer_count;
}

int player_hit(int deck[52][2], int &dealtcards, int &player_count) {
    // Get the next card
    int rank = deck[dealtcards][0];
    int suit = deck[dealtcards][1];
    ++dealtcards;

    // Print the dealt card with suit emoji color
    string suitStr;
    string colorStr;

    switch (suit) {
    case 1: // Hearts
        suitStr = "\u2665"; // Heart emoji
        colorStr = RED; // Hearts are red
        break;
    case 2: // Spades
        suitStr = "\u2660"; // Spade emoji
        colorStr = BLUE; // Spades are blue
        break;
    case 3: // Diamonds
        suitStr = "\u2666"; // Diamond emoji
        colorStr = YELLOW; // Diamonds are red
        break;
    case 4: // Clubs
        suitStr = "\u2663"; // Club emoji
        colorStr = GREEN; // Clubs are green
        break;
    }

    string rankStr;
    if (rank == 1)
        rankStr = "A";
    else if (rank == 11)
        rankStr = "J";
    else if (rank == 12)
        rankStr = "Q";
    else if (rank == 13)
        rankStr = "K";
    else
        rankStr = to_string(rank);

    // Print the dealt card with color
    cout << "\nDealt card to player: " << colorStr << rankStr << suitStr << RESET << endl;

    // Add card value to count
    if (rank >= 10) // Face cards are worth 10
        player_count += 10;
    else if (rank == 1) { // Ace can be 1 or 11
        player_count += (player_count + 11 <= 21) ? 11 : 1;
    } else {
        player_count += rank;
    }

    return player_count;
}

int dealer_hit(int deck[52][2], int &dealtcards, int &dealer_count) {
    // Get the next card
    int rank = deck[dealtcards][0];
    int suit = deck[dealtcards][1];
    ++dealtcards;

    // Print the dealt card with suit emoji color
    string suitStr;
    string colorStr;

    switch (suit) {
    case 1: // Hearts
        suitStr = "\u2665"; // Heart emoji
        colorStr = RED; // Hearts are red
        break;
    case 2: // Spades
        suitStr = "\u2660"; // Spade emoji
        colorStr = BLUE; // Spades are blue
        break;
    case 3: // Diamonds
        suitStr = "\u2666"; // Diamond emoji
        colorStr = YELLOW; // Diamonds are red
        break;
    case 4: // Clubs
        suitStr = "\u2663"; // Club emoji
        colorStr = GREEN; // Clubs are green
        break;
    }

    string rankStr;
    if (rank == 1)
        rankStr = "A";
    else if (rank == 11)
        rankStr = "J";
    else if (rank == 12)
        rankStr = "Q";
    else if (rank == 13)
        rankStr = "K";
    else
        rankStr = to_string(rank);

    // Print the dealt card with color
    cout << "\nDealt card to dealer: " << colorStr << rankStr << suitStr << RESET << endl;

    // Add card value to count
    if (rank >= 10) // Face cards are worth 10
        dealer_count += 10;
    else if (rank == 1) { // Ace can be 1 or 11
        dealer_count += (dealer_count + 11 <= 21) ? 11 : 1;
    } else {
        dealer_count += rank;
    }

    return dealer_count;
}

void round_end(int player_count, int dealer_count, int &player_money) {

  cout << "\n\t\033[34mPlayer count: \033[32m" << player_count
       << "\033[34m\tDealer count: \033[31m" << dealer_count << "\033[0m\n";

  if (player_count > 21) {
    cout << "\033[31mYou busted.\033[0m\n";
    player_money -= 20;
  } else if (dealer_count > 21) {
    cout << "\033[32mDealer bust.\033[0m\n";
    player_money += 10;
  } else if (player_count > dealer_count) {
    cout << "\033[32mYou won.\033[0m\n";
    player_money += 10;
  } else if (player_count < dealer_count) {
    cout << "\033[31mDealer won.\033[0m\n";
    player_money -= 10;
  } else {
    cout << "\033[33mTied.\033[0m\n";
  }

  cout << "\033[34mPlayer money: $\033[33m" << player_money << "\033[0m\n";
}

void print_division() {
  cout << "  |                                                                 "
          "|\n";
  cout << "  "
          "+-----------------------------------------------------------------+"
          "\n";
  cout << "  |                                                                 "
          "|\n";
}


int dealplayerI(int deck[52][2], int &dealtcards) {
  player_count = 0;
  player_hit(deck, dealtcards, player_count);
  player_hit(deck, dealtcards, player_count); // Player starts with 2 cards
  return player_count;
}

bool tablecheck(bool &playerwin){
do{
    cout << "\n Play another round? (y/n): ";
    cin >> play_again;
    }while (play_again != "y" && play_again != "n");

    if (play_again == "f"){
      tableopen = false;
    }

      if (player_money <= 0) {
        playerwin = false;
        tableopen = false;
      }

      if (player_money >= 200) {
        playerwin = true;
        tableopen = false;

      }

return tableopen;


}

