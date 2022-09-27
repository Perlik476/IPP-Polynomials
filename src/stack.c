/** @file
  Implementacja stosu przechowującego wielomiany

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/

#include "stack.h"
#include "memory.h"
#include <errno.h>
#include <stdio.h>

#define INITIAL_STACK_SIZE 4 ///< rozmiar nowego stosu

Stack EmptyStack() {
  return (Stack){.size = 0, .sizeReserved = INITIAL_STACK_SIZE,
                 .array = SafeMalloc(INITIAL_STACK_SIZE * sizeof(Poly))};
}

bool CheckStackSize(Stack *stack, size_t n) {
  if (stack->size < n) {
    return false;
  }
  return true;
}

void AddPolyToStack(Poly p, Stack *stack) {
  stack->size++;

  if (stack->size > stack->sizeReserved) {
    stack->sizeReserved *= 2;
    stack->array = SafeRealloc(stack->array,
                               stack->sizeReserved * sizeof(Poly));
  }

  stack->array[stack->size - 1] = p;
}

Poly *PopFromStack(Stack *stack) {
  if (stack->size == 0) {
    return NULL;
  }
  stack->size--;

  if (stack->sizeReserved >= 2 * INITIAL_STACK_SIZE &&
      stack->size < stack->sizeReserved / 4) {
    stack->sizeReserved /= 2;
    stack->array = SafeRealloc(stack->array,
                               stack->sizeReserved * sizeof(Poly));
  }

  return &(stack->array[stack->size]);
}

int RemoveTopFromStack(__attribute__((unused)) String *buffer, Stack *stack) {
  Poly *poly = PopFromStack(stack);

  if (poly != NULL) {
    PolyDestroy(poly);
    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

Poly *TopOfStack(Stack *stack) {
  if (stack->size ==0) {
    return NULL;
  }
  return &(stack->array[stack->size - 1]);
}

int PrintTopOfStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 1)) {
    Poly poly = *TopOfStack(stack);
    PrintPolyCalc(&poly);
    printf("\n");

    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

void DestroyStack(Stack *stack) {
  for (size_t i = 0; i < stack->size; i++) {
    PolyDestroy(&stack->array[i]);
  }
  free(stack->array);
}

int PolyAddOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 2)) {
    Poly p1 = *PopFromStack(stack);
    Poly p2 = *PopFromStack(stack);

    Poly polyResult = PolyAddGetProperty(&p1, &p2);
    AddPolyToStack(polyResult, stack);

    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyMulOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 2)) {
    Poly p1 = *PopFromStack(stack);
    Poly p2 = *PopFromStack(stack);

    Poly polyResult = PolyMul(&p1, &p2);
    AddPolyToStack(polyResult, stack);

    PolyDestroy(&p1);
    PolyDestroy(&p2);

    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyZeroOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  AddPolyToStack(PolyZero(), stack);
  return SUCCESS;
}

int PolyIsCoeffOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 1)) {
    printf("%d\n", PolyIsCoeff(TopOfStack(stack)));
    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyIsZeroOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 1)) {
    printf("%d\n", PolyIsZero(TopOfStack(stack)));
    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyCloneOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 1)) {
    AddPolyToStack(PolyClone(TopOfStack(stack)), stack);
    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyNegOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 1)) {
    Poly *p = TopOfStack(stack);
    PolyNegGetProperty(p);
    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolySubOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 2)) {
    Poly p1 = *PopFromStack(stack);
    Poly p2 = *PopFromStack(stack);

    Poly polyResult = PolySubGetProperty(&p1, &p2);
    AddPolyToStack(polyResult, stack);

    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyIsEqOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 2)) {
    Poly p1 = *PopFromStack(stack);
    Poly p2 = *PopFromStack(stack);

    bool answer = PolyIsEq(&p1, &p2);

    AddPolyToStack(p2, stack);
    AddPolyToStack(p1, stack);

    printf("%d\n", answer);

    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyDegOnStack(__attribute__((unused)) String *buffer, Stack *stack) {
  if (CheckStackSize(stack, 1)) {
    Poly p = *TopOfStack(stack);
    printf("%d\n", PolyDeg(&p));
    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

/**
 * Sprawdza, czy znak jest znakiem białym.
 * @param[in] character : znak
 * @return czy znak jest biały?
 */
static bool IsWhitespace(char character) {
  return (character == ' ' ||
          character == '\t' ||
          character == '\n' ||
          character == '\v' ||
          character == '\f' ||
          character == '\r');
}

/**
 * Sprawdza, czy część buforu po nazwie komendy ma być uznana za błędny
 * argument.
 * @param[in] buffer : bufor
 * @return czy jest błędny argument?
 */
static bool CheckWrongArgument(String *buffer) {
  return (IsWhitespace(buffer->string[0]) &&
          buffer->string[0] != ' ')
          ||
          (buffer->string[0] == ' ' &&
          !CanBeNumber(buffer->string[1]));
}


int PolyDegByOnStackExec(Stack *stack, size_t varIdx) {
  if (CheckStackSize(stack, 1)) {
    Poly p = *TopOfStack(stack);
    printf("%d\n", PolyDegBy(&p, varIdx));
    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyDegByOnStack(String *buffer, Stack *stack) {
  if (buffer->length >= 1) {
    if (CheckWrongArgument(buffer) || buffer->string[1] == '-') {
      return DEG_BY_WRONG_VARIABLE;
    }

    if (!IsWhitespace(buffer->string[0])) {
      return WRONG_COMMAND;
    }

    char *ptr = NULL;
    errno = 0;
    (buffer->string)++;
    size_t varIdx = strtoull(buffer->string, &ptr, 10);

    if (ptr == NULL || !(*ptr == '\n' || *ptr == '\0') || errno != 0) {
      return DEG_BY_WRONG_VARIABLE;
    }
    else {
      return PolyDegByOnStackExec(stack, varIdx);
    }
  }
  else {
    return DEG_BY_WRONG_VARIABLE;
  }
}

int PolyAtOnStackExec(Stack *stack, poly_coeff_t x) {
  if (CheckStackSize(stack, 1)) {
    Poly p = *PopFromStack(stack);

    AddPolyToStack(PolyAt(&p, x), stack);
    PolyDestroy(&p);

    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}



int PolyAtOnStack(String *buffer, Stack *stack) {
  if (buffer->length >= 1) {
    if (CheckWrongArgument(buffer)) {
      return AT_WRONG_VALUE;
    }

    if (!IsWhitespace(buffer->string[0])) {
      return WRONG_COMMAND;
    }

    char *ptr = NULL;
    errno = 0;
    (buffer->string)++;
    poly_coeff_t x = strtoll(buffer->string, &ptr, 10);

    if (ptr == NULL || !(*ptr == '\n' || *ptr == '\0') || errno != 0) {
      return AT_WRONG_VALUE;
    }
    else {
      return PolyAtOnStackExec(stack, x);
    }
  }
  else {
    return AT_WRONG_VALUE;
  }
}


int PolyComposeOnStackExec(Stack *stack, size_t k) {
  if (CheckStackSize(stack, k + 1) && k + 1 != 0) {
    Poly p = *PopFromStack(stack);
    Poly *q = SafeMalloc(k * sizeof(Poly));

    for (size_t i = k - 1; i + 1 >= 1; --i) {
      q[i] = *PopFromStack(stack);
    }

    AddPolyToStack(PolyCompose(&p, k, q), stack);

    PolyDestroy(&p);
    for (size_t i = 0; i < k; ++i) {
      PolyDestroy(&q[i]);
    }
    free(q);

    return SUCCESS;
  }
  else {
    return STACK_UNDERFLOW;
  }
}

int PolyComposeOnStack(String *buffer, Stack *stack) {
  if (buffer->length >= 1) {
    if (CheckWrongArgument(buffer) || buffer->string[1] == '-') {
      return COMPOSE_WRONG_PARAMETER;
    }

    if (!IsWhitespace(buffer->string[0])) {
      return WRONG_COMMAND;
    }

    char *ptr = NULL;
    errno = 0;
    (buffer->string)++;
    size_t k = strtoull(buffer->string, &ptr, 10);

    if (ptr == NULL || !(*ptr == '\n' || *ptr == '\0') || errno != 0) {
      return COMPOSE_WRONG_PARAMETER;
    }
    else {
      return PolyComposeOnStackExec(stack, k);
    }
  }
  else {
    return COMPOSE_WRONG_PARAMETER;
  }
}