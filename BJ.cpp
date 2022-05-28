#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

enum RANK{ACE=1, TWO, THREE, FOUR, FIVE, SIX,SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum TYPE{CLUBS, DIAMONDS, HEARTS, SPADES};

class Card{
public:
    Card(enum RANK r, enum TYPE c){
        this->crk = r;
        this->ctp = c;
    }
    vector<int> getValue(){
        vector<int> value;
        value.push_back(0);
        value.push_back(0);
        if(this->crk == JACK or this->crk == QUEEN or this->crk == KING){
            value.at(0) = 10;
            value.at(1) = 10;
        }
        else if(this->crk == ACE){
            value.at(0) = 1;
            value.at(1) = 11;
        }
        else{
            value.at(0) = this->crk;
            value.at(1) = this->crk;
        }
        return value;
    }
    void displayCard(){
        // print the type of the card
        char print_ctp;
        if(this->ctp == CLUBS){
            print_ctp = 'C';
        }
        if(this->ctp == DIAMONDS){
            print_ctp = 'D';
        }
        if(this->ctp == HEARTS){
            print_ctp = 'H';
        }
        if(this->ctp == SPADES){
            print_ctp = 'S';
        }
        // print the rank of the card
        char print_crk;
        if(this->crk == JACK){
            print_crk = 'J';
            cout << print_crk << print_ctp;
        }
        else if(this->crk == QUEEN){
            print_crk = 'Q';
            cout << print_crk << print_ctp;
        }
        else if(this->crk == KING){
            print_crk = 'K';
            cout << print_crk << print_ctp;
        }
        else{
            cout << this->crk << print_ctp;
        }
    }

private:
    enum RANK crk;
    enum TYPE ctp;
};

class Hand{
public:
    void add(Card c){
        this->cards.push_back(c);
    }
    void clear(){
        this->cards.clear();
    }
    vector<int> getTotal(){
        vector<int> total;
        total.push_back(0);
        total.push_back(0);
        total.push_back(0);
        int opt = 0;
        for(auto & card : this->cards){
            total.at(0) += card.getValue().at(0);
            total.at(1) += card.getValue().at(1);
            int card_value = card.getValue().at(0);
            if(card_value==1){
                if(opt+11<=21){
                    opt += 11;
                }
                else{
                    opt += 1;
                }
            }else{
                opt += card_value;
            }
        }
        total.at(2) += opt;
        return total;
    }
    void display(){
        for(Card c:this->cards){
            c.displayCard();
            cout << " ";
        }
    }
    vector<Card> cards;
};

class Deck{
public:
    void Populate(){
        vector<Card> d;
        // create the array represent the rank and type which is convenient to iterate
        RANK r[] = {ACE, TWO, THREE, FOUR, FIVE, SIX,SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
        TYPE t[] = {CLUBS, DIAMONDS, HEARTS, SPADES};
        // create a deck of cards
        for(RANK i : r){
            for(TYPE j : t){
                Card c(i,j);
                d.push_back(c);
            }
        }
        this->deck = d;
    }
    void shuffle(){
        std::shuffle(this->deck.begin(), this->deck.end(), std::mt19937(std::random_device()()));
    }

    Card deal(){
        Card c = this->deck.at(deck.size()-1);
        this->deck.pop_back();
        return c;
    }

    // give the player the max value card to let the player bust
    Card trick_max(){
        int max=0;
        int index=0;
        for(int i=0;i<this->deck.size();i++){
            if(this->deck.at(i).getValue().at(0)>max){
                max = this->deck.at(i).getValue().at(0);
                index = i;
            }
        }
        Card c = this->deck.at(index);
        this->deck.erase(this->deck.begin()+index);
        return c;
    }

    // give the player the min value card
    Card trick_min(){
        int min=0;
        int index=0;
        for(int i=0;i<this->deck.size();i++){
            if(this->deck.at(i).getValue().at(1)<min){
                min = this->deck.at(i).getValue().at(1);
                index = i;
            }
        }
        Card c = this->deck.at(index);
        this->deck.erase(this->deck.begin()+index);
        return c;
    }

    // return the best card for casino
    Card trick_best(Hand h){
        int total1 = h.getTotal().at(0);
        int total2 = h.getTotal().at(1);
        int dif = 21;
        // check the best value the casino can sum up to with a specific card in deck
        while(dif > 0){
            int dif1 = dif - total1;
            int dif2 = dif - total2;
            // find the best card to sum up to 21
            for(int i=0;i<this->deck.size();i++) {
                if (this->deck.at(i).getValue().at(0) == dif1) {
                    Card c = this->deck.at(i);
                    this->deck.erase(this->deck.begin() + i);
                    return c;
                }
                if (this->deck.at(i).getValue().at(0) == dif2) {
                    Card c = this->deck.at(i);
                    this->deck.erase(this->deck.begin() + i);
                    return c;
                }
            }
            dif--;
        }
        return this->deal();
    }

    vector<Card> deck;
//    void display(){
//        for(Card c:this->deck){
//            c.displayCard();
//        }
//        cout << this->deck.size() << endl;
//    }

};

class AbstractPlayer{
public:
    virtual bool isDrawing() const = 0;
    bool isBusted(){
        if(this->h.getTotal().at(0)>21){
            return true;
        }
        else{
            return false;
        }
    }

    Hand h;
};

class HumanPlayer: public AbstractPlayer{
public:
    bool isDrawing() const override{
        cout << "Do you want to draw? (y/n):";
        char want_to_draw = 'y';
        cin >> want_to_draw;
        return want_to_draw == 'y';
    }

    void announce(Hand com_h, int n) {
        int opt1 = this->find_opt(this->h1);
        int opt2 = this->find_opt(this->h2);
        int opt3 = this->find_opt(this->h3);
        int com_opt = this->find_opt(com_h);
        if(n==1){
            ann(this->h1, com_h, opt1, com_opt, 1);
        }
        if(n==2){
            ann(this->h1, com_h, opt1, com_opt, 1);
            ann(this->h2, com_h, opt2, com_opt, 2);
        }
        if(n==3){
            ann(this->h1, com_h, opt1, com_opt, 1);
            ann(this->h2, com_h, opt2, com_opt, 2);
            ann(this->h3, com_h, opt3, com_opt, 3);
        }
    }

    void ann(Hand human, Hand computer, int human_opt, int computer_opt, int index){
        cout << "Result for Hand" << index << ":" << endl;
        if(human.getTotal().at(0)<=21 and computer.getTotal().at(0)<=21){
            if(human_opt>computer_opt){
                cout << "Player wins." << endl;
                this->human_wins++;
            }
            else if(human_opt<computer_opt){
                cout << "Casino wins." << endl;
                this->casino_wins++;
            }else{
                cout << "Push: No one wins." << endl;
            }
        }else{
            if(human.getTotal().at(0)<=21){
                cout << "Player wins." << endl;
                this->human_wins++;
            }
            else if(computer.getTotal().at(0)<=21){
                cout << "Casino wins." << endl;
                this->casino_wins++;
            }else{
                cout << "Push: No one wins." << endl;
            }
        }
    }
    // find optimal value of cards on hand
    int find_opt(Hand h){
        int opt = 0;
        int count_ace = 0;
        for(auto & card : h.cards){
            int i = card.getValue().at(0);
            if(i==1){
                count_ace += 1;
            }else{
                opt += i;
            }
        }
        while(count_ace>0){
            if(opt+11<=21){
                opt += 11;
            }else{
                opt += 1;
            }
            count_ace--;
        }
        return opt;
    }
    Hand h1;
    Hand h2;
    Hand h3;
    double human_wins = 0;
    double casino_wins = 0;
};

class ComputerPlayer: public AbstractPlayer{
public:
    bool isDrawing() const override{
        Hand hx = this->h;
        double win_rate = this->rate;
        Deck mk = this->m_dk;
        if(win_rate<0.55) {
            if (ComputerPlayer::find_opt(hx) >= 20 or hx.getTotal().at(1) == 21) {
                return false;
            } else {
                return true;
            }
        }
        if(win_rate>=0.55){
            if(hx.getTotal().at(0)<=16){
                return true;
            }else{
                return false;
            }
        }
    }

    void update_rate(double r){
        this->rate = r;
    }

    void update_mdk(Deck mdk){
        this->m_dk = mdk;
    }

    static int find_opt(Hand h){
        int opt = 0;
        int count_ace = 0;
        for(auto & card : h.cards){
            int i = card.getValue().at(0);
            if(i==1){
                count_ace += 1;
            }else{
                opt += i;
            }
        }
        while(count_ace>0){
            if(opt+11<=21){
                opt += 11;
            }else{
                opt += 1;
            }
            count_ace--;
        }
        return opt;
    }
    Hand h;
    double rate = 1.0;
    Deck m_dk;
};

class BlackJackGame{
public:
    // find optimal value of cards on hand
    int find_opt(Hand h){
        int opt = 0;
        int count_ace = 0;
        for(auto & card : h.cards){
            int i = card.getValue().at(0);
            if(i==1){
                count_ace += 1;
            }else{
                opt += i;
            }
        }
        while(count_ace>0){
            if(opt+11<=21){
                opt += 11;
            }else{
                opt += 1;
            }
            count_ace--;
        }
        return opt;
    }

    void play(){
        // initial the deck
        this->m_deck.Populate();
        this->m_deck.shuffle();

        cout << "How many hands do you want to have? (1/2/3):";
        int num_of_hands = 0;
        cin >> num_of_hands;
        while(num_of_hands > 3 or num_of_hands < 1){
            cout << "please enter a number of 1 2 and 3:";
            cin >> num_of_hands;
        }
        // initial the first turn, casino computer has 1 card and human player has 2 cards
        // For the casino
        Card casino_card = this->m_deck.deal();
        this->m_casino.h.add(casino_card);
        cout << "Casino: ";
        casino_card.displayCard();
        int optimal1 = this->find_opt(m_casino.h);
        cout << " [" << optimal1 << "]" << endl;
        // determine how many hands the player would like to have
        if(num_of_hands == 1){
            // For the human player
            // hand1
            Card player_hand1_card1 = this->m_deck.deal();
            Card player_hand1_card2 = this->m_deck.deal();
            this->human.h1.add(player_hand1_card1);
            this->human.h1.add(player_hand1_card2);
            cout << "Player Hand1: ";
            player_hand1_card1.displayCard();
            cout << " ";
            player_hand1_card2.displayCard();
            int hand1_optimal = this->find_opt(human.h1);
            cout << " [" << hand1_optimal << "]" << endl;

            // simulate the turn of human
            // hand1
            while(true){
                cout << "Process on Player Hand1..." << endl;
                if(this->human.isDrawing()==0){
                    break;
                }else{
                    Card player_card = this->m_deck.deal();
                    this->human.h1.add(player_card);
                    cout << "Player Hand1: ";
                    this->human.h1.display();
                    int human_optimal = this->find_opt(human.h1);
                    cout << "[" << human_optimal << "]" << endl;
                    if(this->human.h1.getTotal().at(0)>21){
                        break;
                    }
                }
            }

            // simulate the turn of casino
            while(true){
                this->m_casino.update_mdk(m_deck);
                bool flag = m_casino.isDrawing();
                if(flag==0){
                    break;
                }else{
                    Card casino_c(FIVE,HEARTS);
                    if(this->casino_winning_rate<0.55){
                        // give the casino the best card
                        if(find_opt(this->m_casino.h)<=11){
                            casino_c = this->m_deck.trick_max();
                        }else{
                            casino_c = this->m_deck.trick_best(this->m_casino.h);
                        }
                    }else{
                        casino_c = this->m_deck.deal();
                    }
                    this->m_casino.h.add(casino_c);
                    cout << "Casino: ";
                    this->m_casino.h.display();
                    int casino_optimal = this->find_opt(m_casino.h);
                    cout << "[" << casino_optimal << "]" << endl;
                    if(this->m_casino.h.getTotal().at(0)>21){
                        break;
                    }
                }
            }

            // announce the result
            this->human.announce(this->m_casino.h, 1);
            this->num_casino_win = human.casino_wins;
            this->num_human_win = human.human_wins;

            // compute current winning rate of casino
            double total_count = this->num_casino_win+this->num_human_win;
            if(total_count==0){
                this->casino_winning_rate = 0;
            }else{
                this->casino_winning_rate = this->num_casino_win/total_count;
            }
            cout << "num of human wins: " << this->num_human_win << endl;
            cout << "num of casino wins: " << this->num_casino_win << endl;
            cout << "the winning rate of casino is: " << this->casino_winning_rate << endl;

            // update the winning rate of casino
            this->m_casino.update_rate(this->casino_winning_rate);

            // clear the hand for the next play
            this->m_casino.h.clear();
            this->human.h1.clear();

        }
        if(num_of_hands == 2){
            // For the human player
            // hand1
            Card player_hand1_card1 = this->m_deck.deal();
            Card player_hand1_card2 = this->m_deck.deal();
            this->human.h1.add(player_hand1_card1);
            this->human.h1.add(player_hand1_card2);
            cout << "Player Hand1: ";
            player_hand1_card1.displayCard();
            cout << " ";
            player_hand1_card2.displayCard();
            int hand1_optimal = this->find_opt(human.h1);
            cout << " [" << hand1_optimal << "]" << endl;

            // hand2
            Card player_hand2_card1 = this->m_deck.deal();
            Card player_hand2_card2 = this->m_deck.deal();
            this->human.h2.add(player_hand2_card1);
            this->human.h2.add(player_hand2_card2);
            cout << "Player Hand2: ";
            player_hand2_card1.displayCard();
            cout << " ";
            player_hand2_card2.displayCard();
            int hand2_optimal = this->find_opt(human.h2);
            cout << " [" << hand2_optimal << "]" << endl;

            // simulate the turn of human
            // hand1
            while(true){
                cout << "Process on Player Hand1..." << endl;
                if(this->human.isDrawing()==0){
                    break;
                }else{
                    Card player_card = this->m_deck.deal();
                    this->human.h1.add(player_card);
                    cout << "Player Hand1: ";
                    this->human.h1.display();
                    int human_optimal = this->find_opt(human.h1);
                    cout << "[" << human_optimal << "]" << endl;
                    if(this->human.h1.getTotal().at(0)>21){
                        break;
                    }
                }
            }
            // hand2
            while(true){
                cout << "Process on Player Hand2..." << endl;
                if(this->human.isDrawing()==0){
                    break;
                }else{
                    Card player_card = this->m_deck.deal();
                    this->human.h2.add(player_card);
                    cout << "Player Hand2: ";
                    this->human.h2.display();
                    int human_optimal = this->find_opt(human.h2);
                    cout << "[" << human_optimal << "]" << endl;
                    if(this->human.h2.getTotal().at(0)>21){
                        break;
                    }
                }
            }
            // simulate the turn of casino
            while(true){
                this->m_casino.update_mdk(m_deck);
                bool flag = m_casino.isDrawing();
                if(flag==0){
                    break;
                }else{
                    Card casino_c(FIVE,HEARTS);
                    if(this->casino_winning_rate<0.55){
                        // give the casino the best card
                        if(find_opt(this->m_casino.h)<=11){
                            casino_c = this->m_deck.trick_max();
                        }else{
                            casino_c = this->m_deck.trick_best(this->m_casino.h);
                        }
                    }else{
                        casino_c = this->m_deck.deal();
                    }
                    this->m_casino.h.add(casino_c);
                    cout << "Casino: ";
                    this->m_casino.h.display();
                    int casino_optimal = this->find_opt(m_casino.h);
                    cout << "[" << casino_optimal << "]" << endl;
                    if(this->m_casino.h.getTotal().at(0)>21){
                        break;
                    }
                }
            }

            // announce the result
            this->human.announce(this->m_casino.h, 2);
            this->num_casino_win = human.casino_wins;
            this->num_human_win = human.human_wins;

            // compute current winning rate of casino
            double total_count = this->num_casino_win+this->num_human_win;
            if(total_count==0){
                this->casino_winning_rate = 0;
            }else{
                this->casino_winning_rate = this->num_casino_win/total_count;
            }
            cout << "num of human wins: " << this->num_human_win << endl;
            cout << "num of casino wins: " << this->num_casino_win << endl;
            cout << "the winning rate of casino is: " << this->casino_winning_rate << endl;

            // update the winning rate of casino
            this->m_casino.update_rate(this->casino_winning_rate);

            // clear the hand for the next play
            this->m_casino.h.clear();
            this->human.h1.clear();
            this->human.h2.clear();

        }
        if(num_of_hands == 3){
            // For the human player
            // hand1
            Card player_hand1_card1 = this->m_deck.deal();
            Card player_hand1_card2 = this->m_deck.deal();
            this->human.h1.add(player_hand1_card1);
            this->human.h1.add(player_hand1_card2);
            cout << "Player Hand1: ";
            player_hand1_card1.displayCard();
            cout << " ";
            player_hand1_card2.displayCard();
            int hand1_optimal = this->find_opt(human.h1);
            cout << " [" << hand1_optimal << "]" << endl;

            // hand2
            Card player_hand2_card1 = this->m_deck.deal();
            Card player_hand2_card2 = this->m_deck.deal();
            this->human.h2.add(player_hand2_card1);
            this->human.h2.add(player_hand2_card2);
            cout << "Player Hand2: ";
            player_hand2_card1.displayCard();
            cout << " ";
            player_hand2_card2.displayCard();
            int hand2_optimal = this->find_opt(human.h2);
            cout << " [" << hand2_optimal << "]" << endl;

            // hand3
            Card player_hand3_card1 = this->m_deck.deal();
            Card player_hand3_card2 = this->m_deck.deal();
            this->human.h3.add(player_hand3_card1);
            this->human.h3.add(player_hand3_card2);
            cout << "Player Hand3: ";
            player_hand3_card1.displayCard();
            cout << " ";
            player_hand3_card2.displayCard();
            int hand3_optimal = this->find_opt(human.h3);
            cout << " [" << hand3_optimal << "]" << endl;

            // simulate the turn of human
            // hand1
            while(true){
                cout << "Process on Player Hand1..." << endl;
                if(this->human.isDrawing()==0){
                    break;
                }else{
                    Card player_card = this->m_deck.deal();
                    this->human.h1.add(player_card);
                    cout << "Player Hand1: ";
                    this->human.h1.display();
                    int human_optimal = this->find_opt(human.h1);
                    cout << "[" << human_optimal << "]" << endl;
                    if(this->human.h1.getTotal().at(0)>21){
                        break;
                    }
                }
            }
            // hand2
            while(true){
                cout << "Process on Player Hand2..." << endl;
                if(this->human.isDrawing()==0){
                    break;
                }else{
                    Card player_card = this->m_deck.deal();
                    this->human.h2.add(player_card);
                    cout << "Player Hand2: ";
                    this->human.h2.display();
                    int human_optimal = this->find_opt(human.h2);
                    cout << "[" << human_optimal << "]" << endl;
                    if(this->human.h2.getTotal().at(0)>21){
                        break;
                    }
                }
            }
            // hand3
            while(true){
                cout << "Process on Player Hand3..." << endl;
                if(this->human.isDrawing()==0){
                    break;
                }else{
                    Card player_card = this->m_deck.deal();
                    this->human.h3.add(player_card);
                    cout << "Player Hand3: ";
                    this->human.h3.display();
                    int human_optimal = this->find_opt(human.h3);
                    cout << "[" << human_optimal << "]" << endl;
                    if(this->human.h3.getTotal().at(0)>21){
                        break;
                    }
                }
            }


            // simulate the turn of casino
            while(true){
                this->m_casino.update_mdk(m_deck);
                bool flag = m_casino.isDrawing();
                if(flag==0){
                    break;
                }else{
                    Card casino_c(FIVE,HEARTS);
                    if(this->casino_winning_rate<0.55){
                        // give the casino the best card
                        if(find_opt(this->m_casino.h)<=11){
                            casino_c = this->m_deck.trick_max();
                        }else{
                            casino_c = this->m_deck.trick_best(this->m_casino.h);
                        }
                    }else{
                        casino_c = this->m_deck.deal();
                    }
                    this->m_casino.h.add(casino_c);
                    cout << "Casino: ";
                    this->m_casino.h.display();
                    int casino_optimal = this->find_opt(m_casino.h);
                    cout << "[" << casino_optimal << "]" << endl;
                    if(this->m_casino.h.getTotal().at(0)>21){
                        break;
                    }
                }
            }

            // announce the result
            this->human.announce(this->m_casino.h, 3);
            this->num_casino_win = human.casino_wins;
            this->num_human_win = human.human_wins;

            // compute current winning rate of casino
            double total_count = this->num_casino_win+this->num_human_win;
            if(total_count==0){
                this->casino_winning_rate = 0;
            }else{
                this->casino_winning_rate = this->num_casino_win/total_count;
            }
            cout << "num of human wins: " << this->num_human_win << endl;
            cout << "num of casino wins: " << this->num_casino_win << endl;
            cout << "the winning rate of casino is: " << this->casino_winning_rate << endl;

            // update the winning rate of casino
            this->m_casino.update_rate(this->casino_winning_rate);

            // clear the hand for the next play
            this->m_casino.h.clear();
            this->human.h1.clear();
            this->human.h2.clear();
            this->human.h3.clear();
        }
    }

    Deck m_deck;
    ComputerPlayer m_casino;
    HumanPlayer human;
    double num_human_win = 0;
    double num_casino_win = 0;
    double casino_winning_rate = 0;

};

int main() {
    cout << "\tWelcome to the Comp322 Blackjack game2!" << endl << endl;
//    Card c1(ACE, HEARTS);
//    c1.displayCard();
//    cout << c1.getValue().at(0) << endl;
//
//    Card c2(TEN, SPADES);
//    Card c3(THREE, SPADES);
//    Card c4(NINE, SPADES);
//
//    Hand h;
//    h.add(c1);
//    h.add(c2);
//    cout << h.getTotal().at(0) << endl;
//    h.add(c3);
//    cout << h.getTotal().at(0) << endl;
//    h.clear();
//    h.add(c4);
//    cout << h.getTotal().at(0) << endl;

//    Deck d;
//    d.Populate();
//    d.shuffle();
//    d.display();
//    d.deal().displayCard();
//    d.display();

    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
        game.play();
        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl << endl;
        playAgain = (answer == 'y' ? true : false);
    }
    cout <<"Gave over!";
    return 0;
}
