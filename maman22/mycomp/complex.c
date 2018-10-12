#include <math.h>
#include "complex.h"

Complex getComplex(float real_part, float imaginary_part) {
    Complex c;
    c.real_part = real_part;
    c.imaginary_part = imaginary_part;
    return c;
}

Complex add(Complex num1, Complex num2) {
    return getComplex(num1.real_part + num2.real_part, num1.imaginary_part + num2.imaginary_part);
}

Complex sub(Complex num1, Complex num2) {
    return getComplex(num1.real_part - num2.real_part, num1.imaginary_part - num2.imaginary_part);
}

Complex multReal(Complex cmp, float real) {
    return getComplex(cmp.real_part * real, cmp.imaginary_part * real);
}

Complex multImg(Complex cmp, float img) {
    return getComplex(-img * cmp.imaginary_part, img * cmp.real_part);
}

Complex multComp(Complex num1, Complex num2) {
    return getComplex(num1.real_part * num2.real_part - num1.imaginary_part * num2.imaginary_part,
                      num1.real_part * num2.imaginary_part + num1.imaginary_part * num2.real_part);
}

float comp_abs(Complex num) {
    return sqrt(num.real_part * num.real_part + num.imaginary_part * num.imaginary_part);
}

