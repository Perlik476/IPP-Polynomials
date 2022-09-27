/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/

#include "poly.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"


static Mono AddTwoMonos(Mono *m, Mono *n);

/**
 * Wypisuje jednomian na standardowe wyjście.
 * @param[in] mono : jednomian
 */
static void PrintMonoCalc(const Mono *mono);

void PrintPolyCalc(const Poly *poly) {
  if (PolyIsCoeff(poly)) {
    printf("%lld", poly->coeff);
  }
  else {
    for (size_t i = 0; i < poly->size; ++i) {
      PrintMonoCalc(&poly->arr[i]);
      if (i < poly->size - 1)
        printf("+");
    }
  }
}

static void PrintMonoCalc(const Mono *mono) {
  printf("(");
  PrintPolyCalc(&mono->p);
  printf(",%d)", mono->exp);
}

/**
 * Komparator porównujący wykładniki dwóch jednomianów.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$\begin{cases} 1 &\text{ dla } e_m > e_n \\
 * 0 &\text{ dla } e_m = e_n \\
 * -1 &\text{ dla } e_m < e_n \end{cases}@f$, gdzie @f$e_k@f$
 * jest wykładnikiem jednomianu @f$k@f$.
 */
static int MonosCompareExponents(const Mono *m, const Mono *n) {
  if (m->exp > n->exp)
    return 1;
  if (m->exp < n->exp)
    return -1;
  return 0;
}

/**
 * Komparator porównujący wykładniki dwóch jednomianów zgodny z funkcją qsort.
 * Wykorzystuje funkcję MonosCompareExponents.
 * @see MonosCompareExponents
 */
static int MonosCompareExponentsQsort(const void *arg1, const void *arg2) {
  Mono m = *(Mono *)arg1;
  Mono n = *(Mono *)arg2;
  return MonosCompareExponents(&m, &n);
}


/**
 * Sprawdza, czy tablica jednomianów jest posortowana.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return Czy talibca jednomianów jest posortowana?
 */
static bool MonosAreSorted(size_t count, Mono monos[]) {
  for (size_t i = 1; i < count; ++i) {
    if (MonosCompareExponents(&monos[i], &monos[i - 1]) > 1)
      return false;
  }
  return true;
}

/**
 * Sprawdza, czy tablica jednomianów nie ma jednomianów postaci
 * @f$C \cdot x_0^0@f$ dla pewnej stałej @f$C@f$.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return Czy talibca jednomianów nie ma jednomianów postaci
 * @f$C \cdot x_0^0@f$?
 */
static bool MonosCanShorten(size_t count, Mono monos[]) {
  for (size_t i = 0; i < count; ++i) {
    if (monos[i].exp == 0 && PolyIsCoeff(&monos[i].p) &&
        !PolyIsZero(&monos[i].p)) {
      return true;
    }
  }
  return false;
}

/**
 * Sprawdza, czy tablica jednomianów jest uproszczona, czyli czy nie jest
 * to jeden jednomian postaci @f$C \cdot x_0^0@f$ oraz czy nie zawiera
 * jednomianów zerowych.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return Czy tablica jednomianów jest uproszczona?
 */
static bool MonosAreSimplified(size_t count, Mono monos[]) {
  if (count == 0) {
    return true;
  }
  else if (count == 1) {
    return !MonosCanShorten(1, monos);
  }
  else {
    for (size_t i = 0; i < count; ++i) {
      if (PolyIsZero(&monos[i].p)) {
        return false;
      }
    }
    return true;
  }
}

/**
 * Sprawdza, czy tablica jednomianów jest zapisana w przyjętej jednoznacznej
 * postaci.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @return Czy tablica jednomianów jest w jednoznacznej postaci?
 */
static bool MonosAreInProperForm(size_t count, Mono monos[]) {
  return MonosAreSorted(count, monos) && MonosAreSimplified(count, monos);
}

/**
 * Sprawdza, czy wielomian jest zapisany w przyjętej jednoznacznej postaci.
 * @param[in] p : wielomian
 * @return Czy wielomian jest w jednoznacznej postaci?
 */
static inline bool PolyIsInProperForm(const Poly *p) {
  return PolyIsCoeff(p) || MonosAreInProperForm(p->size, p->arr);
}


/**
 * Sprawdza równość dwóch jednomianów.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$m = n@f$
 */
static bool MonoIsEq(const Mono *m, const Mono *n) {
  return (m->exp == n->exp && PolyIsEq(&m->p, &n->p));
}

bool PolyIsEq(const Poly *p, const Poly *q) {
  if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
    return (p->coeff == q->coeff);
  }

  if ((PolyIsCoeff(p) && !PolyIsCoeff(q)) ||
      (!PolyIsCoeff(p) && PolyIsCoeff(q))) {
    return false;
  }

  if (p->size != q->size) {
    return false;
  }

  bool result = true;
  for (size_t i = 0; i <p->size; ++i) {
    result &= MonoIsEq(&p->arr[i], &q->arr[i]);
  }

  return result;
}


Poly PolyAddGetProperty(Poly *p, Poly *q) {
  assert(PolyIsInProperForm(p) && PolyIsInProperForm(q));

  if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
    return PolyFromCoeff(p->coeff + q->coeff);
  }
  if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
    return PolyAddGetProperty(q, p);
  }

  Poly polyResult;
  Mono *monos;
  if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
    monos = SafeMalloc((p->size + 1) * sizeof(Mono));
    monos[0] = MonoFromPoly(q, 0);
    for (size_t i = 0; i < p->size; ++i) {
      monos[i + 1] = p->arr[i];
    }
    polyResult = PolyAddMonosGetProperty(p->size + 1, monos, true);
  }

  if (!PolyIsCoeff(p) && !PolyIsCoeff(q)) {
    monos = SafeMalloc((p->size + q->size) * sizeof(Mono));
    size_t i = 0, j = 0;
    while (i + j < p->size + q->size) {
      if (j < q->size && (i == p->size || p->arr[i].exp >= q->arr[j].exp)) {
        monos[i + j] = q->arr[j];
        j++;
      }
      else {
        monos[i + j] = p->arr[i];
        i++;
      }
    }
    polyResult = PolyAddMonosGetProperty(p->size + q->size, monos, true);
  }

  free(p->arr);
  free(q->arr);
  return polyResult;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
  Poly p2 = PolyClone(p);
  Poly q2 = PolyClone(q);

  Poly polyResult = PolyAddGetProperty(&p2, &q2);

  return polyResult;
}


/**
 * Dodaje do siebie dwa niezerowe jednomiany o tym samym wykładniku poprzez
 * dodanie współczynników przy tych jednomianach i zwraca wynik. A jeśli jeden
 * z jednomianów jest zerowy, to zwraca drugi jednomian.
 * @param[in] m : jednomian @f$m@f$
 * @param[in] n : jednomian @f$n@f$
 * @return @f$m + n@f$
 */
static Mono AddTwoMonos(Mono *m, Mono *n) {
  assert(m->exp == n->exp || PolyIsZero(&m->p) || PolyIsZero(&n->p));

  if (m->exp == n->exp) {
    Poly p = PolyAddGetProperty(&m->p, &n->p);
    poly_exp_t exp = m->exp;
    if (PolyIsZero(&p)) {
      exp = 0;
    }
    return (Mono) {.exp = exp, .p = p};
  }
  else if (PolyIsZero(&m->p)) {
    return *n;
  }
  else { // PolyIsZero(&n->p)
    return *m;
  }
}


/**
 * Upraszcza posortowaną tablicę jednomianów.
 * Upraszcza posortowaną tablicę jednomianów poprzez sumowanie jednomianów o
 * tych samych wykładnikach i pomijanie zerowych jednomianów. Modyfikuje
 * tablicę daną na wejściu. Zwraca liczbę jednomianów po uproszczeniu.
 * @param[in] count : liczba jednomianów
 * @param[in, out] monos : tablica jednomianów
 * @return liczba jednomianów po uproszczeniu
 */
static size_t MonosAddAndSimplify(size_t count, Mono *monos) {
  assert(MonosAreSorted(count, monos));

  size_t j = 0;
  // Zmienna j wskazuje na miejsce w tablicy monos, do którego wszystkie
  // poprzednie jednomiany są już uproszczone.

  if (count >= 1 && PolyIsZero(&monos[0].p)) {
    j = -1; // Intencjonalny overflow
  }

  for (size_t i = 1; i < count; ++i) {
    if (PolyIsZero(&monos[i].p)) { // Pomijamy jednomiany zerowe.
      continue;
    }
    if (j + 1 != 0 && (monos[i].exp == monos[j].exp ||
                       PolyIsZero(&monos[j].p))) {
      // Dodajemy i-ty jednomian do j-tego jednomianu, gdy mają te same
      // wykładniki bądź j-ty jest jednomianem zerowym.
      Mono monoTemp = AddTwoMonos(&monos[i], &monos[j]);
      monos[j] = monoTemp;
    }
    else {
      // Jednomianów o różnych wykładnikach nie da się uprościć, przeto
      // przepisujemy i-ty jednomian w następnym miejscu zwiększając wartość
      // zmiennej j.
      ++j;
      if (j != i) {
        monos[j] = monos[i];
        monos[i] = (Mono) {.p = PolyZero(), .exp = 0};
      }
    }
  }

  // Poprawka, żeby zmienna j przechowywała liczbę jednomianów po uproszczeniu.
  if (j + 1 == 0 || !PolyIsZero(&monos[j].p)) {
    ++j;
  }

  return j;
}


Poly PolyAddMonosGetProperty(size_t count, Mono *monos, bool isSorted) {
  if (count == 0) {
    free(monos);
    return PolyZero();
  }

  if (!isSorted) {
    qsort(monos, count, sizeof(Mono), MonosCompareExponentsQsort);
  }

  size_t newCount = MonosAddAndSimplify(count, monos);

  if (newCount == 0) {
    free(monos);
    return PolyZero();
  }
  if (newCount >= 1 && newCount < count) {
    monos = SafeRealloc(monos, newCount * sizeof(Mono));
  }
  Poly polyResult = (Poly) {.size = newCount, .arr = monos};

  // Uprszaczenie w przypadku, gdy mamy jeden jednomian postaci
  // C \cdot x_0^0 dla pewnej stałej C.
  if (newCount == 1 && monos[0].exp == 0 &&
      PolyIsCoeff(&monos[0].p)) {
    polyResult = monos[0].p;
    free(monos);
  }
  return polyResult;
}


/**
 * Robi płytką/głęboką kopię tablicy jednomianów i wywołuje z nią funkcję
 * PolyAddMonosGetProperty. Przejmuje na własność zawartość tablicy @p monos.
 * @param[in] count : liczba jednomianów
 * @param[in] monos : tablica jednomianów
 * @param[in] isSorted : czy tablica jednomianów jest posortowana po
 * wykładnikach?
 * @param[in] isDeep : czy kopia ma być głęboka?
 * @return wielomian będący sumą jednomianów
 */
static Poly PolyAddMonosCopy(size_t count, const Mono monos[], bool isSorted,
                             bool isDeep) {
  Mono *monosCopy = SafeMalloc(count * sizeof(Mono));

  if (isDeep) {
    for (size_t i = 0; i < count; ++i) {
      monosCopy[i] = MonoClone(&monos[i]);
    }
  }
  else {
    memcpy(monosCopy, monos, count * sizeof(Mono));
  }

  Poly polyResult = PolyAddMonosGetProperty(count, monosCopy, isSorted);

  return polyResult;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
  return PolyAddMonosCopy(count, monos, false, false);
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
  return PolyAddMonosGetProperty(count, monos, false);
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
  return PolyAddMonosCopy(count, monos, false, true);
}


void PolyDestroy(Poly *p) {
  if (p == NULL) {
    return;
  }

  if (!PolyIsCoeff(p)) {
    for (size_t k = 0; k < p->size; ++k)
      MonoDestroy(&p->arr[k]);
    free(p->arr);
    p->arr = NULL;
  }
}


Poly PolySub(const Poly *p, const Poly *q) {
  assert(PolyIsInProperForm(p) && PolyIsInProperForm(q));

  Poly p2 = PolyClone(p);
  Poly q2 = PolyClone(q);

  Poly polyResult = PolySubGetProperty(&p2, &q2);

  return polyResult;
}

Poly PolyClone(const Poly *p) {
  if (p == NULL)
    return PolyZero();
  if (p->arr == NULL)
    return PolyFromCoeff(p->coeff);

  Mono *monos = SafeMalloc(p->size * sizeof(Mono));
  for (size_t i = 0; i < p->size; ++i) {
    monos[i] = MonoClone(&p->arr[i]);
  }

  Poly polyResult = (Poly) {.arr = monos, .size = p->size};

  return polyResult;
}

/**
 * Zwraca maksymalną z dwóch wartości całkowitych.
 * @param[in] x : liczba całkowita
 * @param[in] y : liczba całkowita
 * @return @f$\max \{x, y\}@f$.
 */
static poly_exp_t Max(poly_exp_t x, poly_exp_t y) {
  return x > y ? x : y;
}

poly_exp_t PolyDegBy(const Poly *p, size_t varIdx) {
  assert(PolyIsInProperForm(p));

  if (PolyIsZero(p))
    return -1;

  if (p->arr == NULL)
    return 0;

  if (varIdx == 0) {
    return p->arr[p->size - 1].exp;
  }

  poly_exp_t maxExp = 0;
  for (size_t i = 0; i < p->size; ++i) {
    maxExp = Max(maxExp, PolyDegBy(&p->arr[i].p, varIdx - 1));
  }

  return maxExp;
}

poly_exp_t PolyDeg(const Poly *p) {
  assert(PolyIsInProperForm(p));

  if (PolyIsZero(p))
    return -1;

  if (PolyIsCoeff(p))
    return 0;

  poly_exp_t maxExp = 0;
  for (size_t i = 0; i < p->size; ++i) {
    maxExp = Max(maxExp, p->arr[i].exp + PolyDeg(&p->arr[i].p));
  }

  return maxExp;
}

/**
 * Zwraca liczbę całkowitą będącą @f$n@f$ - tą potęgą liczby @f$x@f$, gdzie
 * @f$x \in \mathbb{Z}, n \in \mathbb{Z_+} \cup \{ 0 \}@f$. Wynik jest wyliczany
 * algorytmem szybkiego potęgowania.
 * @param[in] x : podstawa potęgi
 * @param[in] n : wykładnik
 * @return @f$ x ^ n @f$
 */
static poly_coeff_t Power(poly_coeff_t x, poly_exp_t n) {
  assert(n >= 0);

  poly_coeff_t result = 1;

  while (n > 0) {
    if (n % 2 == 1) {
      result *= x;
    }
    x *= x;
    n /= 2;
  }

  return result;
}


/**
 * Sprawdza, czy wielomian jest postaci @f$C \cdot x_0^0@f$.
 * @param p
 * @return czy wielomian jest postaci @f$C \cdot x_0^0@f$?
 */
static bool IsPolyASingleCoeff(Poly *p) {
  return !PolyIsCoeff(p) && p->size == 1 && p->arr[0].exp == 0 &&
                                                  PolyIsCoeff(&p->arr[0].p);
}

/**
 * Mnoży wielomian przez liczbę całkowitą. Przejmuje wielomian na własność.
 * @param[in, out] p : wielomian @f$p@f$
 * @param[in] x : liczba całkowita @f$x@f$
 */
static void PolyMulByNum(Poly *p, poly_coeff_t x) {
  assert(PolyIsInProperForm(p));

  if (p == NULL || x == 0) {
    PolyDestroy(p);
    *p = PolyZero();
    return;
  }

  if (p->arr == NULL) {
    PolyDestroy(p);
    *p = PolyFromCoeff(x * (p->coeff));
    return;
  }

  size_t numberOfZeros = 0;
  for (size_t i = 0; i < p->size; ++i) {
    PolyMulByNum(&p->arr[i].p, x);
    if (PolyIsZero(&p->arr[i].p))
      numberOfZeros++;
  }

  if (numberOfZeros == p->size) {
    PolyDestroy(p);
    *p = PolyZero();
  }

  if (!PolyIsCoeff(p)) {
    size_t newCount = MonosAddAndSimplify(p->size, p->arr);
    if (newCount != p->size) {
      p->arr = SafeRealloc(p->arr, newCount * sizeof(Mono));
      p->size = newCount;
    }
  }

  if (IsPolyASingleCoeff(p)) {
    Poly polyResult = PolyFromCoeff(p->arr[0].p.coeff);
    PolyDestroy(p);
    *p = polyResult;
  }
}

Poly PolyNeg(const Poly *p) {
  assert(PolyIsInProperForm(p));

  Poly q = PolyClone(p);
  PolyMulByNum(&q, -1);

  return q;
}

Poly PolyNegGetProperty(Poly *p) {
  assert(PolyIsInProperForm(p));

  PolyMulByNum(p, -1);

  return *p;
}

Poly PolySubGetProperty(Poly *p, Poly *q) {
  assert(PolyIsInProperForm(p) && PolyIsInProperForm(q));

  PolyNegGetProperty(q);
  Poly polyResult = PolyAddGetProperty(p, q);

  return polyResult;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
  assert(PolyIsInProperForm(p));

  if (p->arr == NULL) {
    return PolyClone(p);
  }

  Mono *monos = SafeMalloc(p->size * sizeof(Mono));
  for (size_t i = 0; i < p->size; ++i) {
    Poly poly = PolyClone(&p->arr[i].p);
    PolyMulByNum(&poly, Power(x, p->arr[i].exp));
    monos[i] = MonoFromPoly(&poly, 0);
  }

  Poly polyTemp = PolyAddMonosCopy(p->size, monos, true, false);
  Poly polyResult = polyTemp;

  if (!PolyIsCoeff(&polyTemp)) {
    polyResult = polyTemp.arr[0].p;
    free(polyTemp.arr);
  }

  free(monos);
  return polyResult;
}

Poly PolyMul(const Poly *p, const Poly *q) {
  assert(PolyIsInProperForm(p) && PolyIsInProperForm(q));

  if (PolyIsZero(p) || PolyIsZero(q)) {
    return PolyZero();
  }

  if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
    return PolyFromCoeff(p->coeff * q->coeff);
  }

  if (PolyIsCoeff(p)) {
    Poly polyResult = PolyClone(q);
    PolyMulByNum(&polyResult, p->coeff);
    return polyResult;
  }

  if (PolyIsCoeff(q)) {
    return PolyMul(q, p);
  }

  Mono *monos = SafeMalloc((p->size * q->size) * sizeof(Mono));
  for (size_t i = 0; i < p->size; ++i) {
    for (size_t j = 0; j < q->size; ++j) {
      Poly poly = PolyMul(&p->arr[i].p, &q->arr[j].p);
      monos[q->size * i + j] = MonoFromPoly(&poly, PolyIsZero(&poly) ? 0 :
                                            (p->arr[i].exp + q->arr[j].exp));
    }
  }

  Poly polyResult = PolyAddMonos(p->size * q->size, monos);
  free(monos);

  return polyResult;
}

/**
 * Podnosi wielomian będący elementem o indeksie 1 w tablicy @p polyPowersOfTwo
 * do potęgi @p n. Wykorzystuje przy tym potęgi tego wielomianu o wykładnikach
 * będących kolejnymi potęgami dwójki.
 * @param[in] polyPowersOfTwo : tablica wielomianu podniesionego do kolejnych potęg
 * liczby 2
 * @param[in] n : wykładnik
 * @return Wielomian podniesiony do @p n - tej potęgi
 */
Poly PolyPower(const Poly *polyPowersOfTwo, poly_exp_t n) {
  Poly polyResult = PolyFromCoeff(1);

  size_t i = 1;
  while (n != 0) {
    if (n % 2 == 1) {
      Poly polyResultTemp = PolyMul(&polyResult, &polyPowersOfTwo[i]);
      PolyDestroy(&polyResult);
      polyResult = polyResultTemp;
    }

    i++;
    n /= 2;
  }

  return polyResult;
}

/**
 * Generuje tablicę @p numberOfPowersOfTwo wielomianów, w którym @f$i@f$-ty
 * wielomian wnosi @f$p^i@f$.
 * @param[in] p : wielomian
 * @param[in] numberOfPowersOfTwo : rozmiar docelowej tablicy
 * @return tablica wielomianów
*/
Poly *GeneratePolyPowersOfTwo(const Poly *p, size_t numberOfPowersOfTwo) {
  Poly *polyPowersOfTwo = SafeMalloc(sizeof(Poly) * numberOfPowersOfTwo);
  Poly polyMultiplier = PolyClone(p);

  polyPowersOfTwo[0] = PolyFromCoeff(1);
  polyPowersOfTwo[1] = polyMultiplier;
  size_t i = 2;
  numberOfPowersOfTwo -= 2;

  while (numberOfPowersOfTwo != 0) {
    polyMultiplier = PolyMul(&polyMultiplier, &polyMultiplier);
    polyPowersOfTwo[i] = polyMultiplier;
    i++;
    numberOfPowersOfTwo--;
  }

  return polyPowersOfTwo;
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
  if (PolyIsCoeff(p)) {
    return *p;
  }

  size_t count = p->size;

  poly_exp_t maxExp = 0;
  for (size_t i = 0; i < count; ++i) {
    maxExp = p->arr[i].exp > maxExp ? p->arr[i].exp : maxExp;
  }

  size_t numberOfQPowersOfTwo = 2;
  maxExp /= 2;
  while (maxExp != 0) {
    numberOfQPowersOfTwo++;
    maxExp /= 2;
  }

  Poly *qPowersOfTwo = GeneratePolyPowersOfTwo(&q[0], numberOfQPowersOfTwo);

  Mono *monos = SafeMalloc(sizeof(Mono) * count);
  for (size_t i = 0; i < count; ++i) {
    monos[i] = MonoClone(&p->arr[i]);
  }

  Poly polyResult = PolyZero();

  for (size_t i = 0; i < count; ++i) {
    Poly polyComposed = PolyCompose(&monos[i].p, k > 1 ? k - 1 : 0,
                                    k > 1 ? q + 1 : NULL);
    Poly polyPowered = PolyPower(qPowersOfTwo, monos[i].exp);

    PolyDestroy(&monos[i].p);
    monos[i].p = PolyMul(&polyComposed, &polyPowered);

    PolyDestroy(&polyComposed);
    PolyDestroy(&polyPowered);

    polyResult = PolyAddGetProperty(&polyResult, &monos[i].p);
  }

  free(monos);
  for (size_t i = 0; i < numberOfQPowersOfTwo; ++i) {
    PolyDestroy(&qPowersOfTwo[i]);
  }
  free(qPowersOfTwo);

  return polyResult;
}