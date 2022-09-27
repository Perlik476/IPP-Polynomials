/** @file
  Implementacja kalkulatora wielomianów opartego na odwrotnej notacji polskiej.

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "poly.h"
#include "stack.h"
#include "definitions.h"
#include "parser.h"

/**
 * Tworzy obiekt typu String na podstawie ciągu znaków @p string i zwraca go.
 * @param[in] string : ciąg znaków
 * @return obiekt typu String
 */
static String CreateString(char *string) {
  return (String){.length = (long long) strlen(string), .string = string};
}

/**
 * Tworzy komendę, która ma odpowiadać nazwie @p name. Odpowiada jej ponadto
 * funkcja @p function, a to, czy przyjmowany jest dodatkowy argument do komendy
 * opisuje @p hasArgument.
 * @param[in] name : nazwa komendy
 * @param[in] function : funkcja
 * @param[in] hasArgument : czy potrzebuje dodatkowe argumentu?
 * @return komenda
 */
static Command CreateCommand(char *name,
                             int (*function)(String *buffer, Stack *stack),
                             bool hasArgument) {
  return (Command) {.name = CreateString(name), .hasArgument = hasArgument,
                    .function = function};
}

/**
 * Wczytuje kolejny wiersz ze standardowego wejścia. Wiersz ten zapisuje w
 * zmiennej @p buffer.
 * @param[in, out] lineNumber : numer wiersza
 * @param[in, out] buffer : bufor
 * @param[in, out] bufferSize : rozmiar buforu
 */
void NextLine(size_t *lineNumber, String *buffer, size_t *bufferSize) {
  (*lineNumber)++;
  errno = 0;
  buffer->length = getline(&(buffer->string), bufferSize, stdin);

  if (errno != 0) {
    exit(1);
  }

  for (long long i = 0; i < buffer->length; i++) {
    if (buffer->string[i] == '\0') {
      buffer->string[i] = '@';
    }
  }
}

/**
 * Sprawdza, czy wiersz jest rozpoznawany jako komenda.
 * @param[in] buffer : bufor
 * @return czy wiersz jest rozpoznawany jako komenda?
 */
bool CanBeCommand(String *buffer) {
  return (buffer->string[0] >= 'a' && buffer->string[0] <= 'z') ||
          (buffer->string[0] >= 'A' && buffer->string[0] <= 'Z');
}

/**
 * Ustawia wartości w tablicy komend.
 * @param[in, out] commandsArray : tablica komend
 */
void SetCommands(Command *commandsArray) {
  Command commandsTempArray[] = {
    CreateCommand("ZERO", &PolyZeroOnStack, false),
    CreateCommand("IS_COEFF", &PolyIsCoeffOnStack, false),
    CreateCommand("IS_ZERO", &PolyIsZeroOnStack, false),
    CreateCommand("CLONE", &PolyCloneOnStack, false),
    CreateCommand("ADD", &PolyAddOnStack, false),
    CreateCommand("MUL", &PolyMulOnStack, false),
    CreateCommand("NEG", &PolyNegOnStack, false),
    CreateCommand("SUB", &PolySubOnStack, false),
    CreateCommand("IS_EQ", &PolyIsEqOnStack, false),
    CreateCommand("DEG_BY", &PolyDegByOnStack, true),
    CreateCommand("DEG", &PolyDegOnStack, false),
    CreateCommand("AT", &PolyAtOnStack, true),
    CreateCommand("POP", &RemoveTopFromStack, false),
    CreateCommand("PRINT", &PrintTopOfStack, false),
    CreateCommand("COMPOSE", &PolyComposeOnStack, true)};

  for (int i = 0; i < NUMBER_OF_COMMANDS; ++i) {
    commandsArray[i] = commandsTempArray[i];
  }
}

/**
 * Ustawia wartości w tablicy zawierającej wiadomości o błędach.
 * @param[in, out] errorMessages : tablica wiadomości o błędach
 */
void SetErrorMessages(char **errorMessages) {
  char *errorMessagesTemp[NUMBER_OF_ERROR_TYPES];

  errorMessagesTemp[WRONG_COMMAND] = "ERROR %zu WRONG COMMAND\n";
  errorMessagesTemp[STACK_UNDERFLOW] ="ERROR %zu STACK UNDERFLOW\n";
  errorMessagesTemp[AT_WRONG_VALUE] = "ERROR %zu AT WRONG VALUE\n";
  errorMessagesTemp[DEG_BY_WRONG_VARIABLE] = "ERROR %zu DEG BY WRONG VARIABLE\n";
  errorMessagesTemp[WRONG_POLY] = "ERROR %zu WRONG POLY\n";
  errorMessagesTemp[COMPOSE_WRONG_PARAMETER] = "ERROR %zu COMPOSE WRONG PARAMETER\n";

  for (int i = 0; i < NUMBER_OF_ERROR_TYPES; ++i) {
    errorMessages[i] = errorMessagesTemp[i];
  }
}

/**
 * Główna funkcja programu. Wczytuje wiersze z wejścia i wykonuje odpowiednie
 * komendy bądź dodaje wielomian na stos.
 */
int main() {
  Stack stack = EmptyStack();
  String buffer = CreateString("");
  size_t bufferSize = 0;
  Command commandsArray[NUMBER_OF_COMMANDS];
  SetCommands(commandsArray);
  char *errorMessages[NUMBER_OF_ERROR_TYPES];
  SetErrorMessages(errorMessages);

  size_t lineNumber = 0;
  NextLine(&lineNumber, &buffer, &bufferSize);

  while (buffer.length >= 0) {
    if (buffer.string[0] == '#' || buffer.string[0] == '\n') {
      NextLine(&lineNumber, &buffer, &bufferSize);
      continue;
    }

    int result;
    if (CanBeCommand(&buffer)) {
      result = ParseCommand(buffer, commandsArray, &stack);
    }
    else {
      result = ParsePoly(buffer, &stack);
    }

    if (result != SUCCESS) {
      fprintf(stderr, errorMessages[result], lineNumber);
    }

    NextLine(&lineNumber, &buffer, &bufferSize);
  }

  free(buffer.string);
  DestroyStack(&stack);
}