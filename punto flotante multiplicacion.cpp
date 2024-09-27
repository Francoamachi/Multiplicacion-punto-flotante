#include <iostream>
#include <cmath>
#include <iomanip>
#include <bitset>
#include <cstdint>

union BitsFlotante {
    float f;
    uint32_t i;
};

void imprimirDetallesFlotante(float valor) {
    BitsFlotante bf;
    bf.f = valor;
    int signo = (bf.i >> 31) & 1;
    int exponente = (bf.i >> 23) & 0xFF;
    int mantisa = bf.i & 0x7FFFFF;

    std::cout << "Valor: " << valor << std::endl;
    std::cout << "Signo: " << signo << std::endl;
    std::cout << "Exponente: " << exponente << std::endl;
    std::cout << "Mantisa: " << mantisa << std::endl;
}

float multiplicacionPersonalizada(float a, float b) {
    if (a == 0.0f || b == 0.0f) return 0.0f;

    BitsFlotante fa{ a }, fb{ b }, resultado;
    int signo = ((fa.i ^ fb.i) >> 31) & 1;
    int exponente = ((fa.i >> 23) & 0xFF) + ((fb.i >> 23) & 0xFF) - 127;
    uint64_t mantisa = (uint64_t)((fa.i & 0x7FFFFF) | 0x800000) * ((fb.i & 0x7FFFFF) | 0x800000);

    if (mantisa & (1ULL << 47)) {
        mantisa >>= 24;
        exponente++;
    }
    else {
        mantisa >>= 23;
    }

    if (exponente >= 255) return signo ? -INFINITY : INFINITY;
    if (exponente <= 0) return 0.0f;

    resultado.i = (signo << 31) | (exponente << 23) | (mantisa & 0x7FFFFF);
    return resultado.f;
}
BitsFlotante convertirABitsFlotante(float num) {
    BitsFlotante resultado;
    resultado.f = num;
    return resultado;
}



int main() {
    float a, b;
    std::cout << "Ingrese el primer numero flotante: ";
    std::cin >> a;
    std::cout << "Ingrese el segundo numero flotante: ";
    std::cin >> b;

    std::cout << "\nDetalles del primer numero:" << std::endl;
    imprimirDetallesFlotante(a);
    std::cout << "\nDetalles del segundo numero:" << std::endl;
    imprimirDetallesFlotante(b);

    float resultadoPersonalizado = multiplicacionPersonalizada(a, b);
    float resultadoCPP = a * b;

    std::cout << std::fixed << std::setprecision(20);
    std::cout << "\nResultado de la multiplicacion " << resultadoPersonalizado << std::endl;
    std::cout << "Resultado de la multiplicacion de C++: " << resultadoCPP << std::endl;

    std::cout << "\nRepresentación en IEEE 754: " << std::bitset<32>(convertirABitsFlotante(resultadoPersonalizado).i) << std::endl;

    return 0;
}