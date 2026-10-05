#!/usr/bin/env python3
"""Owner-boundary CLI checks. Usage: python3 test_cli.py NUMBER /absolute/binary."""
import re
import subprocess
import sys


def invoke(binary, data, status=0):
    result = subprocess.run([binary], input=data, text=True, encoding="utf-8", capture_output=True, timeout=5)
    assert result.returncode == status, (data, result.returncode, result.stdout[-1000:], result.stderr)
    return result.stdout


def contains(binary, cases):
    for case in cases:
        data, expected, *status = case
        output = invoke(binary, data, status[0] if status else 0)
        for fragment in expected:
            assert fragment in output, (data, fragment, output)


CASES = {
2: [("-7\n12\n", ["La suma de -7 y 12 es 5"]), ("0\n0\n", ["La suma de 0 y 0 es 0"])],
3: [("-40\n", ["-40.0 grados Celsius equivalen a -40.0 grados Fahrenheit"]), ("100\n", ["100.0 grados Celsius equivalen a 212.0 grados Fahrenheit"])],
4: [("2.5\n", ["El área del círculo es 19.64"]), ("0\n", ["El área del círculo es 0.00"])],
5: [("3661\n", ["Equivalente: 01:01:01"]), ("60\n", ["Equivalente: 00:01:00"]), ("0\n", ["Equivalente: 00:00:00"])],
6: [("A\n", ["El código ASCII de 'A' es: 65"]), (" \n", ["El código ASCII de ' ' es: 32"])],
7: [("-1 -2\n2 2\n", ["La distancia entre los dos puntos es: 5.00"]), ("1 1\n1 1\n", ["La distancia entre los dos puntos es: 0.00"])],
8: [("1 2 2\n", ["El promedio es: 1.67"]), ("-3 0 3\n", ["El promedio es: 0.00"])],
9: [("-3 7\n", ["Antes del intercambio: a = -3, b = 7", "Después del intercambio: a = 7, b = -3"])],
10: [("1\n", ["El volumen de la esfera con radio 1.00 es: 4.188790 unidades cúbicas"]), ("0\n", ["es: 0.000000 unidades cúbicas"]), ("-1\n", ["Error: El radio no puede ser negativo"], 1)],
11: [("-3\n", ["El número -3 es impar."]), ("-4\n", ["El número -4 es par."]), ("0\n", ["El número 0 es par."])],
12: [("3 9\n", ["El mayor es: 9"]), ("9 3\n", ["El mayor es: 9"]), ("-7 -7\n", ["Ambos números son iguales."])],
13: [("1\n", ["El número es positivo."]), ("-1\n", ["El número es negativo."]), ("0\n", ["El número es cero."])],
14: [("9 3 4\n", ["El mayor es: 9"]), ("3 9 4\n", ["El mayor es: 9"]), ("3 4 9\n", ["El mayor es: 9"]), ("-2 -2 -5\n", ["El mayor es: -2"])],
15: [("7 + 3\n", ["Resultado: 10.00"]), ("7 - 3\n", ["Resultado: 4.00"]), ("4.5 * 2\n", ["Resultado: 9.00"]), ("7 / 2\n", ["Resultado: 3.50"]), ("7 / 0\n", ["Error: división por cero."]), ("7 ? 2\n", ["Operador no válido."])],
16: [("0\n", ["El factorial de 0 es 1"]), ("5\n", ["El factorial de 5 es 120"]), ("20\n", ["El factorial de 20 es 2432902008176640000"]), ("-1\n", ["El factorial no está definido para números negativos."])],
17: [("10029\n", ["La suma de los dígitos es: 12"]), ("0\n", ["La suma de los dígitos es: 0"]), ("-10\n", ["El número debe ser positivo."])],
21: [("-3 -7\n", ["El número mayor es: -3"]), ("4 4\n", ["El número mayor es: 4"]), ("2 9\n", ["El número mayor es: 9"])],
26: [("-3\n", ["-3 es impar.", "Números pares: 11", "Números impares: 10"]), ("0\n", ["0 es par."])],
27: [("0\n", ["Número de bits a 1: 0", "Método básico: 0", "Método Kernighan: 0", "Método tabla: 0", "Método paralelo: 0"]), ("2147483649\n", ["Número de bits a 1: 2", "Método básico: 2", "Método Kernighan: 2", "Método tabla: 2", "Método paralelo: 2"])],
28: [("64\n", ["64 es una potencia de 2.", "Específicamente: 2^6 = 64"]), ("65\n", ["65 NO es una potencia de 2.", "Potencia anterior: 64", "Potencia siguiente: 128"]), ("0\n", ["0 NO es una potencia de 2.", "Potencia anterior: 0", "Potencia siguiente: 1"])],
29: [("-5 9\n", ["Después del intercambio XOR: a = 9, b = -5"])],
30: [("18\n52\n", ["Decimal:      13330", "Hexadecimal:  0x3412", "Verificación: Byte1=18, Byte2=52"]), ("255\n255\n", ["Decimal:      65535", "Hexadecimal:  0xFFFF"]), ("256\n", ["Error: El valor debe estar entre 0 y 255"], 1), ("abc\n", ["Error: Entrada inválida"], 1)],
32: [("4\n-5 0 8 2\n", ["La suma es: 5", "El promedio es: 1.25"]), ("0\n", ["Cantidad inválida."], 1)],
33: [("4\n-8 -3 -12 -3\n", ["Valor mínimo: -12", "Valor máximo: -3"]), ("1\n9\n", ["Valor mínimo: 9", "Valor máximo: 9"])],
34: [("5\n7 2 7 -1 0\n7\n", ["Valor encontrado en la posición 1 (índice 0)."]), ("3\n2 4 6\n8\n", ["Valor no encontrado en el array."])],
38: [("niño á\n", ["La longitud de la cadena es: 6", "Longitud total (incluyendo '\\n' si existe): 7"]), ("\n", ["La longitud de la cadena es: 0", "Longitud total (incluyendo '\\n' si existe): 1"])],
39: [("1\nÁrbol y pingüino\n2\nAaÉíÓüu\n4\n", ["La cadena \"Árbol y pingüino\" tiene 6 vocal(es)", "  a: 2", "  e: 1", "  i: 1", "  o: 1", "  u: 2", "Total: 7 vocales"])],
40: [("ab cd\n", ["Cadena invertida (método 1): \"dc ba\"", "Cadena invertida (método 2): \"dc ba\"", "Después de invertir in-place: \"dc ba\""]), ("\n", ["Cadena vacía. No hay nada que invertir."])],
41: [("Hola\nhola\n", ["1. Comparación exacta: Las cadenas son DIFERENTES", '2. Comparación ordenada: "Hola" es MENOR que "hola"', "3. Comparación sin caso: Las cadenas son IGUALES (ignorando mayús/minús)", "Primera diferencia en posición 0:"]), ("ab\nabc\n", ['2. Comparación ordenada: "ab" es MENOR que "abc"', "Primera diferencia en posición 2:"]), ("\n\n", ["1. Comparación exacta: Las cadenas son IGUALES"])],
42: [("aaaa\naa\n", ["Primera ocurrencia encontrada en posición 0", "Última ocurrencia encontrada en posición 2", "Total de ocurrencias: 3", "Posiciones encontradas: 0, 1, 2"]), ("ABC\nb\n", ["Subcadena no encontrada", "Total de ocurrencias: 0", "Encontrada ignorando mayús/minús en posición 1"]), ("abc\n\n", ["La subcadena está vacía. No hay nada que buscar."])],
}


def check(number, binary):
    if number in CASES:
        contains(binary, CASES[number])
    elif number == 1:
        assert invoke(binary, "") == "Hola Mundo\n"
    elif number == 18:
        for guesses, hint in [(range(1, 101), "mayor"), (range(100, 0, -1), "menor")]:
            output = invoke(binary, "".join(f"{guess}\n" for guess in guesses))
            attempts = output.count("Tu intento: ")
            assert 1 <= attempts <= 100, output
            assert output.count("¡Correcto! Has adivinado el número.") == 1, output
            assert output.count(f"El número es {hint}.") == attempts - 1, output
            opposite = "menor" if hint == "mayor" else "mayor"
            assert f"El número es {opposite}." not in output, output
    elif number == 19:
        output = invoke(binary, "4\n")
        assert re.findall(r"\*+", output) == ["*", "**", "***", "****"], output
    elif number in (20, 23):
        for limit, expected in [(1, []), (20, [2, 3, 5, 7, 11, 13, 17, 19]), (49, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47])]:
            output = invoke(binary, f"{limit}\n")
            values = output.rsplit(":", 1)[1]
            assert [int(x) for x in values.split()] == expected, output
    elif number == 24:
        output = invoke(binary, "")
        rows = re.findall(r"^\s*(-?\d+)\s+(-?\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(Sí|No)\s*$", output, re.M)
        expected = [('48','18','6','6','144','No'),('56','42','14','14','168','No'),('17','13','1','1','221','Sí'),('100','75','25','25','300','No'),('24','-18','6','6','72','No'),('0','5','5','5','0','No'),('13','13','13','13','13','No')]
        assert rows == expected, output
        match = re.search(r"Coeficientes: x = (-?\d+), y = (-?\d+)", output)
        assert match and 56 * int(match[1]) + 42 * int(match[2]) == 14, output
    elif number in (31, 35):
        data = "5\n3 -2 3 0 9\n"
        output = invoke(binary, data)
        marker = "Los números en orden inverso son:\n" if number == 31 else "Array ordenado:\n"
        values = [int(x) for x in output.split(marker, 1)[1].split()]
        assert values == ([9, 0, 3, -2, 3] if number == 31 else [-2, 0, 3, 3, 9]), output
        contains(binary, [("101\n", ["Cantidad inválida."], 1)])
    elif number == 36:
        data = "1 -2 3 4 0 -6 7 8 9\n9 2 -3 -4 5 6 -7 -8 1\n"
        output = invoke(binary, data)
        values = [int(x) for x in output.split("Matriz resultante de la suma:\n", 1)[1].split()]
        assert values == [10, 0, 0, 0, 5, 0, 0, 0, 10], output
    elif number == 37:
        output = invoke(binary, "")
        assert output.splitlines()[0] == "Resultado de la multiplicación:", output
        assert [int(x) for x in " ".join(output.splitlines()[1:]).split()] == [58, 64, 139, 154], output
    else:
        raise ValueError(f"No CLI owner configured for {number:03}")


if __name__ == "__main__":
    check(int(sys.argv[1]), sys.argv[2])
    print(f"{int(sys.argv[1]):03}: observable CLI contract passed")
