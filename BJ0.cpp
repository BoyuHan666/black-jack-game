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
        Card c = this->deck.at(this->deck.size()-1);
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

    void display(){
        for(Card c:this->deck){
            c.displayCard();
        }
        cout << endl;
        cout << this->deck.size() << endl;
    }

    vector<Card> deck;

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

    int announce(Hand com_h){
        int best_value_human = 0;
        int best_value_opponent = 0;
        int human_option1 = this->h.getTotal().at(0);
        int human_option2 = this->h.getTotal().at(1);
        int opponent_option1 = com_h.getTotal().at(0);
        int opponent_option2 = com_h.getTotal().at(1);
        if(human_option2<=21){
            best_value_human = human_option2;
        }
        else{
            best_value_human = human_option1;
        }

        if(opponent_option2<=21){
            best_value_opponent = opponent_option2;
        }
        else{
            best_value_opponent = opponent_option1;
        }
        if(best_value_human<=21 and best_value_opponent<=21){
            if(best_value_human > best_value_opponent){
                cout << "Player wins." << endl;
                return 1;
            }
            else if(best_value_human < best_value_opponent){
                cout << "Casino wins." << endl;
                return 2;
            }
            else{
                cout << "Push: No one wins." << endl;
                return 0;
            }
        }else{
            if(best_value_human <= 21){
                cout << "Player wins." << endl;
                return 1;
            }
            else if(best_value_opponent <= 21){
                cout << "Casino wins." << endl;
                return 2;
            }
            else{
                cout << "Push: No one wins." << endl;
                return 0;
            }
        }
    }
    Hand h;
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
//             check if next card can increase the total value and not exceed 21
//             check if the cards is good enough that is sum up to 20 or 21
//            Card next = mk.deck.back();
//            if(hx.getTotal().at(1) == 20 or hx.getTotal().at(1) == 21){
//                return false;
//            }
//            else if(hx.getTotal().at(1) == 20 or hx.getTotal().at(0) == 21){
//                return false;
//            }
//            else if(hx.getTotal().at(0) == 20 or hx.getTotal().at(1) == 21){
//                return false;
//            }
//            else{
//                if(hx.getTotal().at(0)+next.getValue().at(0)<=21){
//                    return true;
//                }
//                else{
//                    return false;
//                }
//            }
        return false;
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
    double rate = 0.0;
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

        // initial the first turn, casino computer has 1 card and human player has 2 cards
        // For the casino
        Card casino_card = this->m_deck.deal();
        this->m_casino.h.add(casino_card);
        cout << "Casino: ";
        casino_card.displayCard();
        int optimal1 = this->find_opt(m_casino.h);
        cout << " [" << optimal1 << "]" << endl;
        // For the human player
        Card player_card1 = this->m_deck.deal();
        Card player_card2 = this->m_deck.deal();
        this->human.h.add(player_card1);
        this->human.h.add(player_card2);
        cout << "Player: ";
        player_card1.displayCard();
        cout << " ";
        player_card2.displayCard();
        int optimal2 = this->find_opt(human.h);
        cout << " [" << optimal2 << "]" << endl;

        // simulate the turn of human
        while(true){
            if(this->human.isDrawing()==0){
                break;
            }else{
                // give the player the worst card
//                Card player_card(FIVE,HEARTS);
//                if(this->casino_winning_rate<0.55){
//                    if(this-> human.h.getTotal().at(0) >= 12){
//                        player_card = this->m_deck.trick_max();
//                    }
//                    else if(this-> human.h.getTotal().at(1) < 12){
//                        player_card = this->m_deck.trick_min();
//                    }
//                    else{
//                        player_card = this->m_deck.deal();
//                    }
//                }else{
//                    player_card = this->m_deck.deal();
//                }
                Card player_card = this->m_deck.deal();
                this->human.h.add(player_card);
                cout << "Player: ";
                this->human.h.display();
                int human_optimal = this->find_opt(human.h);
                cout << "[" << human_optimal << "]" << endl;
                // if player busts, then end this turn and report casino is the winner
                if(this->human.h.getTotal().at(0)>21){
                    cout << "Player busts." << endl;
                    // record the result of this turn
                    int winner = this->human.announce(this->m_casino.h);
                    if(winner==1){
                        this->num_human_win += 1;
                    }
                    if(winner==2){
                        this->num_casino_win += 1;
                    }

                    // compute current winning rate of casino
                    double total_count = this->num_casino_win + this->num_human_win;
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
                    this->human.h.clear();
                    return;
                }
            }
        }

        // simulate the turn of casino
        while(true){
            this->m_casino.update_mdk(m_deck);
            int flag = this->m_casino.isDrawing();
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
//                cout << "the casino_c is:";
//                casino_c.displayCard();
//                cout << endl;
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
        int winner = this->human.announce(this->m_casino.h);
        if(winner==1){
            this->num_human_win += 1;
        }
        if(winner==2){
            this->num_casino_win += 1;
        }

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
        this->human.h.clear();
    }

    Deck m_deck;
    ComputerPlayer m_casino;
    HumanPlayer human;
    double num_human_win = 0;
    double num_casino_win = 0;
    double casino_winning_rate = 0;

};

int main() {
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
//    Deck d;
//    d.Populate();
//    d.shuffle();
//    d.display();
//    d.trick_max();
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
