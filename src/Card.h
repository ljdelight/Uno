/* 
 * File:   Card.h
 * Author: Lucas
 *
 * Created on May 17, 2009, 7:07 PM
 */

#ifndef _CARD_H
#define	_CARD_H

#include <iostream>
using std::cout;
using std::ostream;

#include <string>
using std::string;

#include <cctype>
using std::toupper;
using std::tolower;
using std::isdigit;

#include <stdexcept>
using std::invalid_argument;

// Colors are on: [0,9]. 10 total possible colors.
// A std game has 4 colors (red, yellow, green, blue), but we can have up to 10.
const string COLOR = "rygbpxwcmv";
const string ACTION = "drs";
const int BAD_VALUE = -1;

// Any action card will have a Card rank that will be on (Action_Start, ActionEnd).
//        If rank is not on this range, it's not an action.
// Since a wild card does not have a color, I give it the color of WILD_ACTION_CARD.
//        There cannot be another color that corresponds to this value -- Hence colors are on [0,9].
enum { WILD_ACTION_CARD = 10, ACTION_START = 10, DRAW_2, REVERSE, SKIP, WILD, WILD_4, ACTION_END };

class Card
{
public:
    Card( const int rank = BAD_VALUE, const int color = BAD_VALUE );
    Card( const char rank, const char color, const bool throwError = 1 );
    bool operator==( const Card & c ) const
    {
        return c.color == color && c.rank == rank;
    }
    friend ostream& operator<<( ostream& out, const Card& c );

    bool isInvalid() const { return rank == BAD_VALUE || color == BAD_VALUE; }
    bool isAction() const;
    bool isAnyWild() const;
    bool isWildDraw4() const;

    int getRank() const;
    int getColor() const;
    int getPointValue() const;

    void print() const;
    void print( ostream& cout ) const;
private:
    int rank;
    int color;
};

ostream& operator<<( ostream& out, const Card &c )
{
    c.print(out);
    return out;
}

Card::Card( const int r, const int c)
{
    rank = r;
    color = c;
}
// Card(char,char) -- constructs a card given the char vals.
// NOTE: this is NOT a case sensitive ctor. ALSO input can be EITHER W4 OR 4W
//       One more thing, throwError determines whether to throw. (default is True, see definition if you don't believe)
//
// PRE: rank must be an int on [0,9] or 'd','r','s'
//      color must be somewhere in the COLOR string.
// POST:
Card::Card(const char r, const char c, const bool throwError )
{
    //wild cards
    if ( tolower(r) == 'w' && ( c == '-' || c == '4') ) // -> w4 w-
    {
        // valid card!
        if ( c == '-' )
        {
            rank = WILD;
        }
        else
        {
            rank = WILD_4;
        }
        color = WILD_ACTION_CARD;
        return;
    }
    else if ( ( r=='-' || r=='4' )&& tolower(c) == 'w' ) // -> -w 4w
    {
        // valid!
        if ( r == '-' )
        {
            rank = WILD;
        }
        else
        {
            rank = WILD_4;
        }
        color = WILD_ACTION_CARD;
        return;
    }
    // rank is a digit or somewhere in ACTION
    if ( isdigit(r) || ACTION.find( tolower(r) ) != string::npos )
    {
        // colors are only found in the COLOR str
        if ( COLOR.find( tolower(c) ) != string::npos )
        {
            // valid card! Now assign correct values

            if ( isdigit( tolower(r) ) )
            {
                rank = tolower(r)-'0';
            }
            else // valid but not digit
            {
                rank = ACTION_START+ (ACTION.find( tolower(r) )+1);
            }

            color = COLOR.find( tolower(c) );
            return;
        }
    }

    if ( throwError )
    {
        throw invalid_argument( "ERROR: Card::Card(char,char) given bad value");
    }
    else
    {
        rank = BAD_VALUE;
        color = BAD_VALUE;
    }

}
int Card::getColor() const { return color; }
int Card::getRank() const { return rank; }
int Card::getPointValue() const
{
    switch (rank)
    {
        case DRAW_2:
        case REVERSE:
        case SKIP:
            return 20;
            break; // I know, i don't need break after a return. I like them.

        case WILD:
        case WILD_4:
            return 50;
            break;
            
        default:
            return rank;
    }
}
bool Card::isAction() const { return rank > ACTION_START && rank < ACTION_END; }
bool Card::isAnyWild() const { return color == WILD_ACTION_CARD; }
bool Card::isWildDraw4() const { return rank == WILD_4; }
void Card::print() const
{
    if ( isAnyWild() )
    {
        if ( rank == WILD ) // TODO: is there a better textual representation for wilds?
        {
            cout << "W-";
        }
        else if ( rank == WILD_4 )
        {
            cout << "W4";
        }
    }
    else
    {
        cout << (char)COLOR[color];
        if ( isAction() )
        {
            cout << (char)toupper( ACTION[ rank-ACTION_START-1] );
        }
        else
        {
            cout << rank;
        }
    }
}
void Card::print( ostream& cout ) const
{
    if ( isAnyWild() )
    {
        if ( rank == WILD ) // TODO: is there a better textual representation for wilds?
        {
            cout << "W-";
        }
        else if ( rank == WILD_4 )
        {
            cout << "W4";
        }
    }
    else
    {
        cout << (char)COLOR[color];
        if ( isAction() )
        {
            cout << (char)toupper( ACTION[ rank-ACTION_START-1] );
        }
        else
        {
            cout << rank;
        }
    }
}
#endif	/* _CARD_H */
