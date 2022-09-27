/** @file
  Implementacja parsowania wiersza

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/
#include "parser.h"
#include "poly.h"
#include "memory.h"
#include "errno.h"
#include "definitions.h"

/**
 * To jest struktura przechowująca tablicę jednomianów
 */
typedef struct MonosArray {
    Mono *arr; ///< tablica jednomianów
    size_t size; ///< liczba jednomianów
    /** liczba jednomianów, na które zarezerwowana jest pamięć */
    size_t sizeReserved;
} MonosArray;

/**
 * Sprawdza, czy drugi ciąg znaków jest początkiem pierwszego
 * @param string1 : pierwszy ciąg znaków
 * @param string2 : drugi ciąg znaków
 * @result czy drugi ciąg znaków jest początkiem pierwszego?
 */
static bool CheckIfSecondStringIsTheBeginningOfTheFirst(String string1,
                                                 String string2) {
  if (string1.length < string2.length) {
    return false;
  }

  for (long i = 0; i < string2.length; ++i) {
    if (string1.string[i] != string2.string[i]) {
      return false;
    }
  }

  return true;
}

static bool TryPoly(char **buffer, Poly *poly);

/**
 * Próbuje sparsować bufor na wielomian stały. Wynik zapisuje w wielomianie
 * @p poly. Zwraca, czy operacja się powiodła.
 * @param[in, out] buffer : bufor
 * @param[in, out] poly : wielomian
 * @result czy operacja się powiodła?
 */
static bool TryPolyFromCoeff(char **buffer, Poly *poly) {
  if (!CanBeNumber(*buffer[0])) {
    return false;
  }

  char *ptr = NULL;
  errno = 0;
  poly_coeff_t x = strtoll(*buffer, &ptr, 10);

  if (errno != 0) {
    return false;
  }

  *poly = PolyFromCoeff(x);
  *buffer = ptr;

  return true;
}

/**
 * Próbuje sparsować bufor na wykładnik jednomianu. Wynik zapisuje w zmiennej
 * @p exp. Zwraca, czy operacja się powiodła.
 * @param[in, out] buffer : bufor
 * @param[in, out] exp : wykładnik
 * @result czy operacja się powiodła?
 */
static bool TryExp(char **buffer, poly_exp_t *exp) {
  if (!CanBeNumber(*buffer[0])) {
    return false;
  }

  char *ptr = NULL;
  errno = 0;
  *exp = (poly_exp_t) strtol(*buffer, &ptr, 10);

  if (ptr == NULL || errno != 0 || *exp < 0) {
    return false;
  }
  else {
    *buffer = ptr;
    return true;
  }
}


/**
 * Dodaje jednomian do tablicy jednomianów.
 * @param[in, out] monos : tablica jednomianów
 * @param[in] mono : jednomian
 */
static void MonosArrayAdd(MonosArray *monos, Mono mono) {
  if (monos->size == 0) {
    monos->arr = SafeMalloc(sizeof(Mono));
    monos->size = 0;
    monos->sizeReserved = 1;
  }

  if (monos->size + 1 > monos->sizeReserved) {
    monos->sizeReserved *= 2;
    monos->arr = SafeRealloc(monos->arr, sizeof(Mono) *
                                                          monos->sizeReserved);
  }

  monos->arr[monos->size] = mono;
  monos->size++;
}

/**
 * Próbuje sparsować bufor na jednomian. Wynik dodaje na koniec tablicy
 * jednomianów @p monos. Zwraca, czy operacja się powiodła.
 * @param[in, out] buffer : bufor
 * @param[in, out] monos : tablica jednomianów
 * @result czy operacja się powiodła?
 */
static bool TryMono(char **buffer, MonosArray *monos) {
  Poly poly;
  poly_exp_t exp;
  char *ptr = &(*buffer[0]);

  if (*ptr != '(') {
    return false;
  }
  ptr++;

  if (!TryPoly(&ptr, &poly)) {
    return false;
  }

  if (*ptr != ',') {
    PolyDestroy(&poly);
    return false;
  }
  ptr++;

  if (!TryExp(&ptr, &exp)) {
    PolyDestroy(&poly);
    return false;
  }

  if (*ptr != ')') {
    PolyDestroy(&poly);
    return false;
  }
  ptr++;

  Mono mono = (Mono) {.p = poly, .exp = exp};
  MonosArrayAdd(monos, mono);
  *buffer = ptr;

  return true;
}

/**
 * Tworzy tablicę jednomianów.
 * @result tablica jednomianów.
 */
static MonosArray CreateNewMonosArray() {
  return (MonosArray) {.arr = NULL, .size = 0, .sizeReserved = 0};
}

/**
 * Usuwa z pamięci tablicę jednomianów
 * @param[in] monos : tablica jednomianów
 */
static void RemoveMonosArray(MonosArray *monos) {
  if (monos->size > 0) {
    free(monos->arr);
  }
}

/**
 * Próbuje sparsować bufor na wielomian złożony z jednomianów. Wynik zapisuje
 * w wielomianie @p poly. Zwraca, czy operacja się powiodła.
 * @param[in, out] buffer : bufor
 * @param[in, out] poly : wielomian
 * @result czy operacja się powiodła?
 */
static bool TryPolyAddMonos(char **buffer, Poly *poly) {
  MonosArray monos = CreateNewMonosArray();
  bool areProperMonos = true;
  bool isFinished = false;

  while (!isFinished) {
    areProperMonos = TryMono(buffer, &monos);
    if (!areProperMonos) {
      isFinished = true;
    }
    if (*buffer[0] == '+') {
      (*buffer)++;
    }
    else {
      isFinished = true;
    }
  }

  if (areProperMonos) {
    *poly = PolyAddMonosGetProperty(monos.size, monos.arr, false);
    return true;
  }
  else {
    RemoveMonosArray(&monos);
    return false;
  }
}

/**
 * Próbuje sparsować bufor na wielomian. Wynik zapisuje w wielomianie
 * @p poly. Zwraca, czy operacja się powiodła.
 * @param[in, out] buffer : bufor
 * @param[in, out] poly : wielomian
 * @result czy operacja się powiodła?
 */
static bool TryPoly(char **buffer, Poly *poly) {
  if (TryPolyFromCoeff(buffer, poly)) {
    return true;
  }

  if (TryPolyAddMonos(buffer, poly)) {
    return true;
  }

  return false;
}


int ParseCommand(String buffer, Command *commandsArray, Stack *stack) {
  for (size_t i = 0; i < NUMBER_OF_COMMANDS; ++i) {
    Command command = commandsArray[i];
    if (CheckIfSecondStringIsTheBeginningOfTheFirst(buffer, command.name)) {
      if (!command.hasArgument && !(buffer.string[command.name.length] == '\n'
                              || buffer.string[command.name.length] == '\0')) {
        return WRONG_COMMAND;
      }

      buffer.string = buffer.string + command.name.length;
      buffer.length -= command.name.length;

      return (command.function)(&buffer, stack);
    }
  }
  return WRONG_COMMAND;
}

/**
 * Sprawdza warunki konieczne tego, że bufor zawiera poprawny wielomian.
 * @param[in] buffer : bufor
 * @return czy bufor zawiera poprawny wielomian?
 */
static bool CanBePoly(String buffer) {
  long long numberOfBrackets = 0;

  for (long long i = 0; i < buffer.length; ++i) {
    if (buffer.string[i] == '(') {
      numberOfBrackets++;
    }
    else if (buffer.string[i] == ')') {
      numberOfBrackets--;
    }
    else if (!(buffer.string[i] == ',' || CanBeNumber(buffer.string[i]) ||
              buffer.string[i] == '+' ||
              buffer.string[i] == '\0' || buffer.string[i] == '\n')) {
      return false;
    }
    if (numberOfBrackets < 0) {
      return false;
    }
  }

  if (numberOfBrackets != 0) {
    return false;
  }

  return true;
}

int ParsePoly(String buffer, Stack *stack) {
  Poly poly;

  if (CanBePoly(buffer) && TryPoly(&buffer.string, &poly)) {
    if (buffer.string[0] == '\n' || buffer.string[0] == '\0') {
      AddPolyToStack(poly, stack);
      return SUCCESS;
    }
    else {
      PolyDestroy(&poly);
      return WRONG_POLY;
    }
  }

  return WRONG_POLY;
}