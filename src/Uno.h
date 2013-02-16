/* 
 * File:   Uno.h
 * Author: Lucas
 *
 * Created on May 17, 2009, 7:12 PM
 */

#ifndef _UNO_H
#define	_UNO_H

#include "Deck.h"
#include "Player.h"

#include <iostream>
using std::cin;

const unsigned int WINNING_SCORE = 500;
const int INIT_CARDS_PER_PLAYER = 7;
const bool CHEAT_DISABLE_VALID_MOVES = false;
const bool PRINT_DETAILS = true;

class Uno
{
public:
    Uno( int numHumans = 1, int numPlayers = 3 );

    void beginGame();

    bool isValidMove( const Card c ) const;
    void changeDirection();
    void nextPlayer();
    void playCard( const Card c );

    void dealCards();
    void initiateRound();
    void beginRound();
    void scoreRound();
    void mergePlayPileToDeck();

    Card getInputCardFromPlayer() const;

    void humanMove();
    void computerMove();

    void printCurrPlayerHand() const;
    void printPlayerInfo() const;

    bool hasWinner() const;
    bool roundOver() const;
private:
    vector<Player> players;
    vector<Card> playPile;
    Deck deck;  // inits itself

    bool currPlayerHasCard( const Card c ) const;
    void nextPlayerDraw( const int numCards );
    Player& getNextPlayerRef();

    int currentPlayer;
    int currentColor;
    bool clockwiseDirection;
};

Uno::Uno( int numHumans, int numPlayers )
{
    // DECK is a member and its default ctor builds it. The deck is initialized.

    players.resize(numPlayers);
    for ( int i = numHumans; i < numPlayers; ++i )
    {
        // players from [numHumans,numPlayers) are computers. 'true' in ctor represents this.
        players[i] = Player(true);
    }
    clockwiseDirection = true;
    currentPlayer = 0; //TODO: do we always want to start with this player?
}

void Uno::beginRound()
{
    while (!roundOver())
    {
        if ( players[currentPlayer].isHuman() )
        {
            humanMove();
            nextPlayer();
        }
        else
        {
            computerMove();
            nextPlayer();
        }
    } // end while (loops round)
}

void Uno::computerMove()
{
    printPlayerInfo();
    cout << endl;

    Player &currPlayerRef = players[currentPlayer];
    vector<Card>& handRef = players[currentPlayer].getHandRef();

    // Uno doesn't have much of a "winning" strategy.
    // We'll have the AI randomly choose to play a Matching color or a matching rank using 1:1 odds

    if  ( PRINT_DETAILS ) { cout << "MOVE DECIDED BY: "; }
    bool playingCard = true;
    vector<Card>::iterator itr;
    switch (rand() % 2)
    {
        case 0: // play a matching color
            if ( currPlayerRef.hasColor(currentColor) )
            {
                for ( itr = handRef.begin(); itr != handRef.end(); ++itr )
                {
                    if ( itr->getColor() == currentColor )
                    {
                        playPile.push_back(*itr);
                        handRef.erase(itr);
                        if  ( PRINT_DETAILS ) { cout << "COLOR\n"; }

                        // NOTE: MUST BREAK!!! .end() returns different values when we REMOVE an element from a vector
                        //    We could include this in the iteration condition instead.
                        break;
                    }
                }
                break;
            }
            // continue to next case if we didn't play




        case 1: // play a matching rank

            if ( currPlayerRef.hasFace(playPile.back().getRank()) )
            {
                for ( itr = handRef.begin(); itr != handRef.end(); ++itr )
                {
                    if ( itr->getRank() == playPile.back().getRank() )
                    {
                        playPile.push_back(*itr);
                        handRef.erase(itr);

                        if  ( PRINT_DETAILS ) { cout << "RANK\n"; }
                        break;
                    }
                }
                
                break;
            }
            // attempt to play something of the same color if we don't have the same Rank. ( repeats this stmt if 'case 0' fires )
            if ( currPlayerRef.hasColor(currentColor) )
            {
                for ( itr = handRef.begin(); itr != handRef.end(); ++itr )
                {
                    if ( itr->getColor() == currentColor )
                    {
                        playPile.push_back(*itr);
                        handRef.erase(itr);

                        if  ( PRINT_DETAILS ) { cout << "ATTEMPTED RANK, USED COLOR\n"; }
                        break;
                    }
                }
                
                break;
            }
            // We don't have same color or rank. Last chance is to play some wild card.
            if ( currPlayerRef.hasWild() )
            {
                // i give priority to wild4.

                bool foundWild4 = false;

                // Find any wild4/wild and play it
                for ( itr = handRef.begin(); itr != handRef.end(); ++itr )
                {
                    if ( itr->isWildDraw4() )
                    {
                        playPile.push_back(*itr);
                        handRef.erase(itr);
                        foundWild4 = true;

                        if  ( PRINT_DETAILS ) { cout << "MUST PLAY WILD4\n"; }
                        break; // stop for
                    }
                    
                } // end for
                

                if ( !foundWild4 ) // =TRUE if the above did not find a wild4 (a fctn exists, but this is a little faster)
                {
                    // wild4 not found but some wild exists.
                    for ( itr = handRef.begin(); itr != handRef.end(); ++itr )
                    {
                        if ( itr->isAnyWild() )
                        {
                            playPile.push_back(*itr);
                            handRef.erase(itr);

                            if ( PRINT_DETAILS ) { cout << "MUST PLAY WILD\n"; }
                            break; // stop for
                        }
                    }
                    
                }
                playingCard = true;
                
                break;
                
            } // end if...


            // if we don't have matching rank/color OR no wilds, fall through



        default: // Nothing to play! AI must draw.
            if  ( PRINT_DETAILS ) { cout << "LACK OF MOVES. MUST DRAW\n"; }
            playingCard = false; // AI is drawing
            break;
    } // end switch


    if ( playingCard )
    {
        // AI is playing a card. the card had already been removed from their hand and put onto play pile
        playCard( playPile.back() );
        cout << "PLAYING CARD: " << playPile.back() << endl;
    }
    else // AI is drawing a card
    {
        if ( deck.isEmpty() )
        {
            mergePlayPileToDeck();

            if ( deck.isEmpty() ) // if it's still empty after merge, BAD thing.
            {
                throw logic_error("ERROR: Uno::computerMove() -- AI does not have any cards to draw OR play. They're all in play.");
            }
            else
            {
                cout << "\n**Deck was replenished using the play pile. " << endl;
            }
        }
        players[currentPlayer].insertCard(deck.drawCard());
        cout << "DRAWING A CARD\n";
    }

    cout << endl << endl << endl;
}

void Uno::humanMove()
{
    bool drawExecuted = false;
    printPlayerInfo();
    
    cout << endl << "Which card would you like to play (or 'd' to draw)? ";
    Card cardToPlay;
    do
    {
        // get a card that the player has in their hand
        cardToPlay = Card();
        while (!drawExecuted && cardToPlay.isInvalid())
        {
            string inputLine;
            getline(cin, inputLine);
            if ( inputLine.length() == 2 ) // a card is only 2chars.
            {
                cardToPlay = Card(inputLine[1], inputLine[0], 0);
                if ( cardToPlay.isInvalid() )
                {
                    cout << "'" << inputLine << "' is not a valid card. ";
                }
                else if ( !this->currPlayerHasCard(cardToPlay) )
                {
                    cout << "You do not have that card. ";

                    // scrub the valid card and make it invalid so loop continues
                    cardToPlay = Card();
                }

                // ********
                // CARD IS VALID AND THE PLAYER HAS IT.
                // THE WHILE LOOP WILL EXIT
                // ********
            }
            else if ( inputLine[0] == 'd' ) // player drew a card
            {

                if ( deck.isEmpty() )
                {
                    cout << "The deck is empty. ";
                    mergePlayPileToDeck();
                    
                    if ( deck.isEmpty() ) // if it's still empty, BAD things would happen if we drew.
                    {
                        cout << "There are no cards to draw, they are all in play. ";
                        drawExecuted = false;
                    }
                    else
                    {
                        cout << "Deck was replenished using the play pile. ";
                        drawExecuted = true;
                    }
                }
                else
                {
                    // deck isn't empty and they want to draw.
                    drawExecuted = true;
                }
            }
            else
            {
                cout << "Bad input. ";
            }
        } // end while

        // the card input is Valid and the currPlayer has it.
        if ( !drawExecuted && !isValidMove(cardToPlay) )
        {
            cout << "You cannot play that card. ";
        }

    } while (!drawExecuted && !isValidMove(cardToPlay)); // end great big DO...while

    if ( !drawExecuted )
    {
        // We finally have a legal play. Remove from currPlayer's hand and place onto play pile.
        playPile.push_back(cardToPlay);
        players[currentPlayer].removeCard(cardToPlay);

        if ( players[currentPlayer].hasOneCard() )
        {
            cout << endl << "**You shout \"UNO!!\"" << endl;
        }
        playCard(cardToPlay);
    }
    else
    {
        players[currentPlayer].insertCard(deck.drawCard());
    }

    cout << endl;
}



// beginGame() -- start the uno game.
// PRE:
// POST: when this ends, the game is over
void Uno::beginGame()
{
    // while someone hasn't won, deal cards (b/c start of round).
    while ( ! hasWinner() )
    {
        // start of a round. initiate
        initiateRound();
        beginRound();

        scoreRound();
        cout << "\n\nPlayer " << currentPlayer+1 << " won the round!\n";
        cout << "Scores are now: \n";
        for ( size_t i = 0; i < players.size(); ++i )
        {
            cout << "\tPlayer " << i+1 << ": " << players[i].getScore() << endl;
        }
        cout << endl << endl;
    }
    cout << "\nPlayer " << currentPlayer+1 << " won the game!" << endl << "Thanks for playing." << endl;
}
void Uno::initiateRound()
{
    for ( size_t i = 0; i < playPile.size(); ++i )
    {
        deck.insertCard(playPile[i]);
    }
    playPile.clear();
    
    // for each player, copy all their cards
    for( size_t i  = 0; i < players.size(); ++i )
    {
        while ( !players[i].hasNoCards() )
        {
            vector<Card>::iterator itr;
            vector<Card>& handRef = players[i].getHandRef();

            // for all cards that the player has, put them into the deck.
            for ( itr = handRef.begin(); itr != handRef.end(); ++itr )
            {
                deck.insertCard( *itr );
            }
            // cards were inserted. clear.
            handRef.clear();
        }
    } // end for

    deck.shuffle();
    dealCards();
}
void Uno::dealCards()
{
    // for each player, give them 7 cards
    for ( size_t i = 0; i < players.size(); i++ )
    {
        for ( int card = 0; card < INIT_CARDS_PER_PLAYER; card++ )
        {
            players[i].insertCard( deck.drawCard() );
        }
    }

    // draw card and put into play
    playPile.push_back( deck.drawCard() );

    // if the first card put into play is wild4,
    // keep putting the card back into the deck and shuffling. Stop when it's not Wild4
    while (playPile.back().isWildDraw4())
    {
        deck.insertCardAndShuffle(playPile.back());
        playPile.pop_back();

        // draw new card.
        playPile.push_back(deck.drawCard());
    }

    // Initialization stuff is done.
    // play the first card of the game!
    this->playCard( playPile.back() );
}
Card Uno::getInputCardFromPlayer() const
{
    string inputLine;
    Card c;
    
    do
    {
        c = Card();
        getline(cin, inputLine);

        if ( inputLine.length() == 2 ) // a card is only 2chars.
        {
            c = Card(inputLine[1], inputLine[0], 0);
            if ( c.isInvalid() )
            {
                cout << "'" << inputLine << "' is not a valid card. ";
                inputLine = ""; // set to something !=2 so the loop does not break.
            }
            else if ( !this->currPlayerHasCard(c) )
            {
                cout << "You do not have that card. ";
                inputLine = "";
            }
        }
        else
        {
            cout << "Bad input. ";
        }
    } while ( inputLine.length() != 2 );

    return c;
}
void Uno::changeDirection()
{
    clockwiseDirection = !clockwiseDirection;
}
bool Uno::roundOver() const
{
    for ( size_t i = 0; i < players.size(); ++i )
    {
        if ( players[i].hasNoCards() )
        {
            return true;
        }
    }
    return false;
}
bool Uno::hasWinner() const
{
    for ( size_t i = 0; i < players.size(); ++i )
    {
        if ( players[i].getScore() >= WINNING_SCORE )
        {
            return true;
        }
    }
    return false;
}

bool Uno::currPlayerHasCard( const Card c ) const
{
    return players[currentPlayer].hasCard(c);
}

void Uno::scoreRound()
{
    // find the winner and store to currPlayer
    for ( size_t i = 0; i < players.size(); ++i )
    {
        if ( players[i].hasNoCards() )
        {
            currentPlayer = i;
            break;
        }
    }

    for ( int i = 0; i < (int)players.size(); ++i )
    {
        if ( i != currentPlayer )
        {
            players[currentPlayer].addToScore( players[i].scoreHand() );
        }
    }
}

// nextPlayer() -- switches the current player to the nextplayer based on direction.
// PRE: should probably have more than one player, but at least one. (controlled in Uno ctor)
// POST: a player CCW is LEFT in the vector and CW is RIGHT.
void Uno::nextPlayer()
{
    if ( clockwiseDirection )
    {
        currentPlayer = (currentPlayer+1)%players.size(); // move right one position
    }
    else
    {
        // moves from player X to X-1. I avoid using MOD on negative by using p.size()-1
        currentPlayer = (currentPlayer+ (players.size()-1) )  % players.size();
    }
}
void Uno::mergePlayPileToDeck()
{
    size_t i;
    for ( i = 0; i < playPile.size() - 1; ++i ) // deck gets all but the top card
    {
        deck.insertCard(playPile[i]);
    }
    Card temp = playPile[i]; // save the top
    playPile.clear();
    playPile.push_back(temp);

    deck.shuffle();
}

// nextPlayerDraw(int) -- the next player (based on CCW or CW) will draw X cards
// PRE: 
// POST:
void Uno::nextPlayerDraw( const int numCards )
{

    Player& nextPlayer = getNextPlayerRef();
    for ( int i = 0; i < numCards; ++i )
    {
        if ( deck.isEmpty() )
        {
            mergePlayPileToDeck();
            if ( deck.isEmpty() )
            {
                throw logic_error( "ERROR: Uno::nextPlayerDraw() does not have enough cards in play for nextPlayer to draw");
            }
        }

        nextPlayer.insertCard(deck.drawCard());
    }
}

// getNextPlayerRef() -- returns a REF to the next player. Used for draw2
// PRE: none
// POST: ref is returned. CAN BE MODIFIED.
Player& Uno::getNextPlayerRef()
{
    int index;
    if ( clockwiseDirection ) // if CW
        index = (currentPlayer+1)%players.size();
    else // CCW
        index = (currentPlayer+ (players.size()-1) )  % players.size();

    return players[index];
}

// playCard() -- perform the action of the card.
void Uno::playCard( const Card c )
{
    if ( !CHEAT_DISABLE_VALID_MOVES )
    {
        // ensure valid card play
        this->isValidMove( c );
    }

    char newColor = ' ';

    
    switch( c.getRank() )
    {
        case WILD_4:
            this->nextPlayerDraw(4);
            if ( players[currentPlayer].isHuman() )
            {
                do
                {
                    cout << endl << "Here are the color keys:\n\t";
                    int i;
                    for ( i = 0; i < TOTAL_COLORS - 1; i++ ) // print all but last. followed shortly
                    {
                        cout << COLOR[i] << ", ";
                    }
                    cout << COLOR[i] << endl << "What is the new color? ";

                    cin >> newColor;
                    cin.ignore();
                    if ( !cin || !COLOR.find(newColor) == string::npos )
                    {
                        cout << "\nThat color does not exist. Try again.";
                        if ( !cin )
                        {
                            cin.clear();
                            cin.ignore(9999, '\n');
                        }
                    }
                } while (!COLOR.find(newColor) == string::npos || !cin);
            }
            else
            {
                newColor = players[currentPlayer].findMostFrequentColor();
            }
            this->nextPlayer();
            break;
        case WILD:
            if ( players[currentPlayer].isHuman() )
            {
                do
                {
                    cout << endl << "Here are the color keys:\n\t";
                    int i;
                    for ( i = 0; i < TOTAL_COLORS - 1; i++ ) // print all but last. followed shortly
                    {
                        cout << COLOR[i] << ", ";
                    }
                    cout << COLOR[i] << endl << "What is the new color? ";

                    cin >> newColor;
                    cin.ignore();
                    if ( !cin || !COLOR.find(newColor) == string::npos )
                    {
                        cout << "\nThat color does not exist. Try again.";
                        if ( !cin )
                        {
                            cin.clear();
                            cin.ignore(9999, '\n');
                        }
                    }
                } while (!COLOR.find(newColor) == string::npos || !cin);
            }
            else
            {
                newColor = players[currentPlayer].findMostFrequentColor();
            }
            break;

        case SKIP:
            // this will change the currentPlayer variable.
            //     The variable shouldn't be used until Real currPlayer is done
            this->nextPlayer();
            break;

        case DRAW_2:
            this->nextPlayerDraw(2);
            this->nextPlayer();
            break;

        case REVERSE:
            this->changeDirection();
            break;

        // non action card. Possibly changes the color. This case is covered below, outside the switch
        default:
            break;
    } // end switch

    // if it's a wild, the input was gathered above.
    if ( c.isAnyWild() )
    {
        currentColor = COLOR.find(newColor);
    }
    else // not a wild, so either it's a new color with a same "rank" or same color. We don't care.
    {
        currentColor = c.getColor();
    }
}

// isValidMove() -- given a card, is it a valid Uno move?
// PRE: play pile MUST have something on it.
// POST:
bool Uno::isValidMove(const Card c) const
{
    if ( playPile.empty() )
    {
        throw logic_error("ERROR: Uno::isValidMove cannot check for valid move. There are no cards in play.");
    }
    else if ( c.isAnyWild() )
    {
        // If the player has a color that is equal to the top discard card,
        //     he/she may NOT play the wild4.
        if ( c.isWildDraw4() )
        {
            return !players[currentPlayer].hasColor(currentColor);
        }
        else
        {
            // if it's a wild (not the draw4) they can play
            return true;
        }
    }
    else // not a wild. valid If: same color OR same rank
    {  
        return ( playPile.back().getRank() == c.getRank()
                           || currentColor == c.getColor() );
    }
}

void Uno::printCurrPlayerHand() const
{
    players[currentPlayer].printHand();
}

void Uno::printPlayerInfo() const
{
    
    cout << "****     Player " << currentPlayer + 1 << "'s turn   ****" << endl;
    if ( PRINT_DETAILS ) { cout << "Size of deck: " << deck.getSize() << endl; }
    cout << "Current color: " << COLOR[currentColor] << endl << "Play pile: " << playPile.back();

    cout << endl << "Player " << currentPlayer+1 << "'s hand: ";
    this->printCurrPlayerHand();
}

#endif	/* _UNO_H */
