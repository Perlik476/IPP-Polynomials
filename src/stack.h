/** @file
  Interfejs stosu przechowującego wielomiany

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#include <stdlib.h>
#include "poly.h"
#include "definitions.h"


/**
 * To jest struktura przechowująca stos, którego elementy są wielomianami.
 */
typedef struct Stack {
    size_t size; ///< liczba elementów
    /** liczba elementów, na które zarezerwowana jest pamięć */
    size_t sizeReserved;
    Poly *array; ///< tablica wielomianów
} Stack;


/**
 * Tworzy pusty stos.
 * @return pusty stos
 */
Stack EmptyStack();

/**
 * Sprawdza, czy stos ma co najmniej @f$n@f$ elementów.
 * @param[in] stack : stos
 * @param[in] n : wymagana liczba elementów
 * @return czy stos ma co najmniej @f$n@f$ elementów?
 */
bool CheckStackSize(Stack *stack, size_t n);

/**
 * Dodaje wielomian na wierzch stosu.
 * @param[in] p : wielomian
 * @param[in, out] stack : stos
 */
void AddPolyToStack(Poly p, Stack *stack);

/**
 * Usuwa wierzchołek stosu i go zwraca.
 * @param[in, out] stack : stos
 * @return wierzchołek stosu
 */
Poly *PopFromStack(Stack *stack);

/**
 * Usuwa wierzchołek stosu
 * @param[in] buffer : bufor (nieużywany)
 * @param[in, out] stack : stos
 */
int RemoveTopFromStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Zwraca wierzchołek stosu bez usuwania go ze stosu.
 * @param[in, out] stack : stos
 * @return wierzchołek stosu
 */
Poly *TopOfStack(Stack *stack);

/**
 * Wypisuje wierzchołek stosu. Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in] stack : stos
 * @return wartość błędu wykonania
 */
int PrintTopOfStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Usuwa stos z pamięci.
 * @param[in] stack : stos
 */
void DestroyStack(Stack *stack);

/**
 * Usuwa dwa wielomiany z góry stosu, dodaje je, a wynik dodaje na górę stosu.
 * Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolyAddOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Usuwa dwa wielomiany z góry stosu, mnoży je, a wynik dodaje na górę stosu.
 * Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolyMulOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Wstawia wielomian zerowy na górę stosu. Zwraca wartość odpowiadającą błędowi
 * wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolyZeroOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Sprawdza, czy wierzchołek stosu jest wielomianem stałym. Jeśli tak, to
 * wypisuje 1 na standardowe wyjście, w przeciwnym wypadku wypisuje 0.
 * Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in] stack : stos
 * @return wartość błędu wykonania
 */
int PolyIsCoeffOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Sprawdza, czy wierzchołek stosu jest wielomianem zerowym. Jeśli tak, to
 * wypisuje 1 na standardowe wyjście, w przeciwnym wypadku wypisuje 0.
 * Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in] stack : stos
 * @return wartość błędu wykonania
 */
int PolyIsZeroOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Kopiuje wierzchołek stosu i dodaje go na górę stosu. Zwraca wartość
 * odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolyCloneOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Wstawia wielomian przeciwny do wierzchołka na górę stosu. Zwraca wartość
 * odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolyNegOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Usuwa dwa wielomiany z góry stosu, odejmuje je, a wynik dodaje na górę stosu.
 * Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolySubOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Sprawdza, czy dwa wielomiany z góry stosu są tożsame. Jeśli tak, to
 * wypisuje 1 na standardowe wyjście, w przeciwnym wypadku wypisuje 0.
 * Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in] stack : stos
 * @return wartość błędu wykonania
 */
int PolyIsEqOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Wypisuje stopień wierzchołka stosu. Zwraca wartość odpowiadającą błędowi
 * wykonania.
 * @param[in] buffer : bufor (nieużywany)
 * @param[in] stack : stos
 * @return wartość błędu wykonania
 */
int PolyDegOnStack(__attribute__((unused)) String *buffer, Stack *stack);

/**
 * Wypisuje stopień wierzchołka stosu ze względu na zadaną zmienną. Zwraca
 * wartość odpowiadającą błędowi wykonania.
 * @param[in] stack : stos
 * @param[in] varIdx : indeks zmiennej
 * @return wartość błędu wykonania
 */
int PolyDegByOnStackExec(Stack *stack, size_t varIdx);

/**
 * Wczytuje zmienną z buforu. Wypisuje stopień wierzchołka stosu ze względu na
 * zadaną zmienną. Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor
 * @param[in] stack : stos
 * @return wartość błędu wykonania
 */
int PolyDegByOnStack(String *buffer, Stack *stack);

/**
 * Usuwa wierzchołek stosu, wylicza wartość wielomianu z góry stosu w punkcie
 * @p x i wstawia nowo powstały wielomian na stos. Zwraca wartość odpowiadającą
 * błędowi wykonania.
 * @param[in, out] stack : stos
 * @param[in] x : wartość argumentu
 * @return wartość błędu wykonania
 */
int PolyAtOnStackExec(Stack *stack, poly_coeff_t x);

/**
 * Wczytuje wartość z buforu. Usuwa wierzchołek stosu, wylicza wartość
 * wielomianu z góry stosu w punkcie @p x i wstawia nowo powstały wielomian na
 * stos. Zwraca wartość odpowiadającą błędowi wykonania.
 * @param[in] buffer : bufor
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolyAtOnStack(String *buffer, Stack *stack);

/**
 * Wykonuje operację składania wielomianów na stosie.
 * Usuwa wierzchołek stosu, nazwijmy go @p p. Następnie usuwa @f$k@f$
 * elementów ze stosu i przypisuje je do @p k elementowej tablicy @f$q@f$ w
 * kolejności malejącej po indeksach. Potem zostaje wykonana operacja
 * złożenia wielomianów, tzn. w miejsce zmiennej @f$x_i@f$ w wielomianie @p p
 * wstawiany jest wielomian @f$q_i@f$. Jeśli @p k jest mniejsze niż liczba
 * zmiennych w wielomianie @p p, to pozostałe zmienne zamieniane są na zero.
 * @param[in, out] stack : stos
 * @param[in] k : liczba elementów ze stosu, które mają być wstawione w kolejne
 * zmienne wielomianu z wierzchołka.
 * @return wartość błędu wykonania
 */
int PolyComposeOnStackExec(Stack *stack, size_t k);

/**
 * Wczytuje wartość z buforu i następnie wywołuje funkcję
 * @p PolyComposeOnStackExec.
 * @param[in] buffer : bufor
 * @param[in, out] stack : stos
 * @return wartość błędu wykonania
 */
int PolyComposeOnStack(String *buffer, Stack *stack);

#endif //POLYNOMIALS_STACK_H
