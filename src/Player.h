/* 
 * File:   Player.h
 * Author: Lucas
 *
 * Created on May 17, 2009, 7:12 PM
 */

#ifndef _PLAYER_H
#define	_PLAYER_H

#include "Card.h"
#include "Deck.h"
#include <stdexcept>
using std::invalid_argument;

#include <algorithm>
using std::find;
using std::max_element;

class Player
{
public:
    Player( bool isComputer = false );

    void insertCard( Card c );
    void removeCard( Card c );

    void printHand() const;
    bool isHuman() const { return human; }

    unsigned int getScore() const { return score; }
    bool hasCard( const Card c ) const;
    bool hasWild() const;
    bool hasFace( const int face ) const;
    bool hasColor( const int color ) const;
    bool hasNoCards() const;
    bool hasOneCard() const;

    char findMostFrequentColor() const;

    int scoreHand() const;
    
    void addToScore( const int s );
    vector<Card>& getHandRef() { return hand; }
private:
    vector<Card> hand;
    unsigned int score;

    bool human;
};

Player::Player( bool isComputer )
{
    score = 0;
    human = !isComputer; // header defaults a player as Human.
}

int Player::scoreHand() const
{
    int total = 0;
    for ( size_t i = 0; i < hand.size(); ++i )
    {
        total += hand[i].getPointValue();
    }

    return total;
}

// addToScore(int) -- adds a value to the player's score. Uno doesn't have neg additions.
// PRE: s>=0.
// POST:
void Player::addToScore(const int s)
{
    score += s;
}
bool Player::hasNoCards() const
{
    return hand.empty();
}
bool Player::hasOneCard() const
{
    return hand.size() == 1;
}
bool Player::hasCard(const Card c) const
{
    vector<Card>::const_iterator itr;
    itr = find(hand.begin(), hand.end(), c);

    return itr != hand.end();
}
void Player::printHand() const
{
    for ( size_t i = 0; i < hand.size(); ++i )
    {
        hand[i].print();
        cout << ' ';
    }
}

char Player::findMostFrequentColor() const
{
    vector<int> count;
    count.resize( TOTAL_COLORS );

    size_t i;

    // set count for everything to 0
    for ( i = 0; i < count.size(); ++i )
        count[i] = 0;

    // count the number of colors in their hand
    for ( i = 0; i < hand.size(); ++i )
    {
        // if the card in hand is Not an action, count the color.
        if ( ! hand[i].isAction() )
        {
            count[ hand[i].getColor() ] += 1;
        }
    }

    // find the max value location. Assume the first element is largest
    int maxLoc = 0;
    for ( i = 0; i < count.size(); ++i )
    {
        if ( count[maxLoc] < count[i] )
        {
            maxLoc = i;
        }
    }

    return COLOR[maxLoc];
    
}
// hasWild() -- does the player have a wild?
bool Player::hasWild() const
{
    // loop through the hand, if a card is a wild ret true.
    for ( size_t i = 0; i < hand.size(); i++ )
    {
        if ( hand[i].isAnyWild() )
        {
            return true;
        }
    }
    return false;
}
// hasFace(const int face) -- does the player have matching face? (For any matching card they can play;
//          a REVERSE can be played on a REVERSE)
// PRE:
// POST: ATTN: this will return true when comparing two WILD cards. This shouldn't be a problem,
//             the player could play a wild on a wild.
bool Player::hasFace(const int face) const // TODO: is this ever used?
{
    // for each card in hand, compare face value. if found ret true
    for ( size_t i = 0; i < hand.size(); i++ )
    {
        if ( hand[i].getRank() == face )
        {
            return true;
        }
    }
    return false;
}

// hasColor() -- does the player have the color?
// PRE: param must be in the given bounds.
bool Player::hasColor(const int color) const
{
    // colors have an int value on the range [0,Total_Colors)
    if ( color < 0 || color >= TOTAL_COLORS )
    {
        throw invalid_argument("Player::hasColor(const int) const given bad parameter.");
    }

    for ( size_t i = 0; i < hand.size(); i++ )
    {
        if ( hand[i].getColor() == color )
        {
            return true;
        }
    }
    return false;
}

// insertCard() -- adds a card to the player's hand
// PRE: Card should be a valid card...
// POST: card is added to hand.
void Player::insertCard( Card c )
{
    hand.push_back(c);
}

// removeCard() -- removes the card from the player's hand
// PRE: Card c must be in the hand
// POST: card is removed
void Player::removeCard( Card c )
{
    // create itr then find location of Card c
    vector<Card>::iterator itr;
    itr = find(hand.begin(), hand.end(), c);

    // if card is found, erase. else throw
    if ( itr != hand.end() )
    {
        hand.erase(itr);
    }
    else
    {
        throw logic_error( "Player::removeCard( Card ): attempt to remove nonexistant card");
    }
}
#endif	/* _PLAYER_H */
