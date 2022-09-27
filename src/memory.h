/** @file
  Interfejs funkcji zarządzających pamięcią

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/
#ifndef POLYNOMIALS_MEMORY_H
#define POLYNOMIALS_MEMORY_H

#include <stdlib.h>

/**
 * Alokuje pamięć poprzez wywołanie funkcji calloc. Sprawdza przy tym, czy
 * alokacja się powiodła. Jeśli tak, to zwraca wskaźnik do zaalokowanej pamięci.
 * W przeciwnym wypadku program kończy się kodem 1.
 * @param[in] num : liczba elementów w tablicy
 * @param[in] size : rozmiar pojedynczego elementu tablicy
 * @return wskaźnik do zaalokowanej pamięci
 */
void *SafeCalloc(size_t num, size_t size);

/**
 * Alokuje pamięć poprzez wywołanie funkcji malloc. Sprawdza przy tym, czy
 * alokacja się powiodła. Jeśli tak, to zwraca wskaźnik do zaalokowanej pamięci.
 * W przeciwnym wypadku program kończy się kodem 1.
 * @param[in] size : rozmiar pamięci
 * @return wskaźnik do zaalokowanej pamięci
 */
void *SafeMalloc(size_t size);

/**
 * Realokuje pamięć poprzez wywołanie funkcji realloc. Sprawdza przy tym, czy
 * realokacja się powiodła. Jeśli tak, to zwraca wskaźnik do zaalokowanej
 * pamięci. W przeciwnym wypadku program kończy się kodem 1.
 * @param[in] ptr : wskaźnik do tablicy
 * @param[in] size : nowy rozmiar tablicy
 * @return wskaźnik do zaalokowanej pamięci
 */
void *SafeRealloc(void *ptr, size_t size);

#endif //POLYNOMIALS_MEMORY_H
