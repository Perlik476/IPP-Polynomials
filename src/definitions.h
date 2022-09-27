/** @file
  Interfejs definicji globalnych

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/

#ifndef POLYNOMIALS_DEFINITIONS_H
#define POLYNOMIALS_DEFINITIONS_H


#define SUCCESS -1 ///< kod błędu - operacja zakończyła się powodzeniem
#define WRONG_COMMAND 0 ///< kod błędu - błędna komenda
#define STACK_UNDERFLOW 1 ///< kod błędu - za mało elementów na stosie
/** kod błędu - błędna zmienna podana do komendy DEG_BY */
#define DEG_BY_WRONG_VARIABLE 2
#define AT_WRONG_VALUE 3 ///< kod błędu - błędna wartość podana do komendy AT
#define WRONG_POLY 4 ///< kod błędu - błędny wielomian
/** kod błędu - błędny parametr do komendy COMPOSE */
#define COMPOSE_WRONG_PARAMETER 5
#define NUMBER_OF_ERROR_TYPES 6 ///< liczba kodów błędu
#define NUMBER_OF_COMMANDS 15 ///< liczba komend
#define SIZE(x) (sizeof(x) / sizeof((x)[0])) ///< daje rozmiar tablicy

/**
 * To jest struktura przechowująca ciąg znaków zapamiętując przy tym jego
 * długość.
 */
typedef struct String {
    long long length; ///< długość ciągu znaków
    char *string; ///< ciąg znaków
} String;

/**
 * Sprawdza, czy dany znak może być początkiem liczby.
 * @param[in] c : znak
 * @return czy @f$c@f$ może być początkiem liczby?
 */
static inline bool CanBeNumber(char c) {
  return ((c >= '0' && c <= '9') || c == '-');
}

#endif //POLYNOMIALS_DEFINITIONS_H
