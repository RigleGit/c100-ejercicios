#include "evaluador_rpn.h"
#include <assert.h>
#include <math.h>
int main(void) {
    const struct {
        const char *expression;
        double expected;
    } cases[] = {{"5 1 2 + 4 * + 3 -", 14}, {"10 3 -", 7},  {"20 4 /", 5},
                 {"2 3 2 ^ ^", 512},        {"-3 2 *", -6}, {"5 2 %", 1}};
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        resultado_evaluacion_t r = evaluar_rpn(cases[i].expression);
        assert(r.error == RPN_SUCCESS && fabs(r.resultado - cases[i].expected) < 1e-9);
    }
    assert(evaluar_rpn(NULL).error == RPN_ERROR_ENTRADA_INVALIDA);
    assert(evaluar_rpn("1 0 /").error == RPN_ERROR_DIVISION_CERO);
    assert(evaluar_rpn("1 +").error == RPN_ERROR_EXPRESION_MALFORMADA);
    assert(evaluar_rpn("1 2").error == RPN_ERROR_EXPRESION_MALFORMADA);
    assert(evaluar_rpn("oops").error != RPN_SUCCESS);
    char postfix[64];
    assert(infija_a_postfija("2 ^ 3 ^ 2", postfix, sizeof(postfix)) == RPN_SUCCESS);
    resultado_evaluacion_t r = evaluar_rpn(postfix);
    assert(r.error == RPN_SUCCESS && r.resultado == 512);
    pila_t *stack = pila_crear(2);
    double x;
    assert(stack && pila_push(stack, 4) == RPN_SUCCESS && pila_push(stack, 7) == RPN_SUCCESS);
    assert(pila_push(stack, 9) != RPN_SUCCESS);
    assert(pila_pop(stack, &x) == RPN_SUCCESS && x == 7);
    assert(pila_pop(stack, &x) == RPN_SUCCESS && x == 4);
    assert(pila_pop(stack, &x) == RPN_ERROR_PILA_VACIA);
    pila_destruir(stack);
    return 0;
}
