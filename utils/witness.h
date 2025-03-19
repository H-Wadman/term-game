/*!
 * \brief Labyrinth puzzle game
 *
 * This game contains a grid with squares and edges. The player is tasked with
 * creating a path on the edges. The marker is situated at the corner of some
 * square. It is the players goal to walk from start to end while separating
 * tiles of different types into separate groups.
 *
 * Note: Grid and board are used to refer to more or less the same thing.
 */
#ifndef WITNESS_H
#define WITNESS_H

#include "color.h"
#include "menu.h"
#include "utf8.h"
#include "vec.h"

/*!
 * \brief A set of color + symbol representing tiles that need to be grouped
 * together
 *
 */
typedef struct Group
{
    enum color color;
    char symbol[ASCII_BUF_SZ];
} Group;

/*!
 * \brief Different states for the edges of a tile to be in.
 */
enum Witness_enum
{
    //! The players path has to pass by this edge
    we_dot,
    //! The player has passed by this edge
    we_filled,
    //! Normal edge, the player may pass here if he wants to
    we_empty
};

/*!
 * \brief A square in the board
 *
 * Only color dictates what tiles need to be grouped together, although for
 * accessibility reasons it is recommended to always map 1 color to 1 symbol.
 * \ref col_default of \ref color.h are the normal tiles that can be apart of
 * any grouping.
 */
typedef struct Square
{
    //! The group the square belongs to
    Group group;
    //! The walls around the square
    enum Witness_enum walls[4];
} Sq;

//Todo transform into Command and rework the main loop to use pointers
/*!
 * \brief Option for specifying the witness game
 */
typedef struct Witness_command
{
    Sq* board;
    int height;
    int width;
    Vec_coord pos;
    coord end;
} Witness_command;

//! Play a witness game specified by this
Command* play_witness(void* this);

#endif
