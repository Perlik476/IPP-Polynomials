/** @file
  Interfejs komendy.

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/
#ifndef POLYNOMIALS_COMMAND_H
#define POLYNOMIALS_COMMAND_H

#include "stack.h"
#include "definitions.h"

/**
 * To jest struktura przechowująca komendę rozpoznawaną na wejściu.
 */
typedef struct Command {
    String name; ///< nazwa komendy
    bool hasArgument; ///< czy potrzebuje dodatkowego argumentu
    /** funkcja odpowiadająca komendzie */
    int (*function)(String *buffer, Stack *stack);
} Command;

#endif //POLYNOMIALS_COMMAND_H
