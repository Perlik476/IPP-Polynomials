/** @file
  Interfejs parsowania wiersza

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/
#ifndef POLYNOMIALS_PARSER_H
#define POLYNOMIALS_PARSER_H

#include "stack.h"
#include "command.h"

/**
 * Jeśli w buforze jest zapisany poprawny wielomian, to go wczytuje i wrzuca na
 * stos. Zwraca wówczas kod poprawnego wykonania SUCCESS. W przeciwnym wypadku
 * zwraca kod błędu błędnego wielomianu WRONG_POLY.
 * @param[in, out] buffer : bufor
 * @param[in, out] stack : stos
 * @return kod błędu
 */
int ParsePoly(String buffer, Stack *stack);

/**
 * Jeśli w buforze jest zapisana poprawna komenda, to ją wczytuje i wywołuje
 * odpowiadającą jej funkcję i zwraca kod, który tamta zwróciła.
 * @param[in, out] buffer : bufor
 * @param[in] commandsArray : tablica komend
 * @param[in, out] stack : stos
 * @return kod błędu
 */
int ParseCommand(String buffer, Command *commandsArray, Stack *stack);

#endif //POLYNOMIALS_PARSER_H
