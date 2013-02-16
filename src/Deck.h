/* 
 * File:   Deck.h
 * Author: Lucas
 *
 * Created on May 17, 2009, 7:10 PM
 */

#ifndef _DECK_H
#define	_DECK_H

#include "Card.h"

#include <algorithm>
using std::random_shuffle;

#include <iostream>
using std::cout;
using std::endl;

#include <stdexcept>
using std::logic_error;

#include <vector>
using std::vector;

// Colors are on: [0,9]. 10 total possible colors.
const int HIGHEST_FACE_CARD = 9; // the highest int card, does NOT include action cards. must be on [1,9]
const int TOTAL_COLORS = 4; // TOTAL COLORS MUST BE LESS THAN 11 (default: 4)
const int STANDARD_CARDS_PER_COLOR = 2; // number of face/action cards per color (default: 2)
const int NUMBER_OF_WILD_CARDS = 4; // num wild and wild4 per deck. (default: 4 of each)


class Deck
{
public:
    Deck();

    Card drawCard();
    void shuffle();
    void insertCard( const Card c );
    void insertCardAndShuffle( const Card c );
    void print() const;

    int getSize() const { return deck.size(); }
    bool isEmpty() const { return deck.empty(); }
private:
    vector<Card> deck;
};

// deck ctor
// PRE:
// POST: deck has all cards and is shuffled.
Deck::Deck()
{
    // for each color: add a 0 card, add the face cards, then add the action cards that have colors.
    for ( int currColor = 0; currColor < TOTAL_COLORS; currColor++ )
    {
        // each color gets a 0
        deck.push_back( Card(0,currColor) );

        // add normal face cards (the 1-9 cards) to the deck
        for ( int face = 1; face <= HIGHEST_FACE_CARD; face++ )
        {
            // each face card gets TWO for every color
            for ( int count = 0; count < STANDARD_CARDS_PER_COLOR; count++ )
            {
                deck.push_back( Card(face, currColor) );
            }
        }

        // add the action cards that have colors
        for ( int count = 0; count < STANDARD_CARDS_PER_COLOR; count++ )
        {
            deck.push_back( Card(DRAW_2, currColor) );
            deck.push_back( Card(REVERSE, currColor) );
            deck.push_back( Card(SKIP, currColor) );
        }

    } // end outer for ( color placement of cards )

    // add 4 of each WILD and WILD4 to the deck
    for ( int i = 0; i < NUMBER_OF_WILD_CARDS; i++ )
    {
        deck.push_back( Card( WILD, WILD_ACTION_CARD ) );
        deck.push_back( Card( WILD_4, WILD_ACTION_CARD ) );
    }

    // The deck is initialized. Now shuffle a couple times, for good measure
    this->shuffle();
    this->shuffle();
} // end Deck::Deck()

// insertCardAndShuffle() -- inserts the card then shuffles the deck using member fctn
void Deck::insertCardAndShuffle(const Card c)
{
    deck.push_back(c);
    this->shuffle();
}
void Deck::insertCard(const Card c)
{
    deck.push_back(c);
}

// popTop() -- draws the top card
// PRE: deck cannot be empty
// POST: removes the top card from the deck
Card Deck::drawCard()
{
    if ( deck.empty() )
    {
        throw logic_error("ERROR: Deck::drawCard() is drawing from empty deck.");
    }
    else
    {
        // save the top card
        Card drawn = deck.back();

        // pop the top card
        deck.pop_back();

        return drawn;
    }
}

// print() -- print the deck. (debugging only)
void Deck::print() const
{
    for ( size_t i = 0; i < deck.size(); i++ )
    {
        deck[i].print();
        cout << " ";

        if( (i+1)%5 == 0 )
        {
            cout << endl;
        }
    }
}

// shuffle() -- shuffles the deck
// PRE: Must have a nonempty deck.
void Deck::shuffle()
{
    if ( deck.empty() )
    {
        throw logic_error("ERROR: Deck::shuffle() -- Deck is EMPTY. Nothing to shuffle.");
    }
    random_shuffle( deck.begin(), deck.end() );
}
#endif	/* _DECK_H */
