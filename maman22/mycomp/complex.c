#include <math.h>
#include "complex.h"

/*initialize a complex number*/
Complex getComplex(float real_part, float imaginary_part) {
    Complex c;
    c.real_part = real_part;
    c.imaginary_part = imaginary_part;
    return c;
}

/*Given 2 complex numbers num1, num2, return the value of num1+num2 */
Complex add(Complex num1, Complex num2) {
    return getComplex(num1.real_part + num2.real_part, num1.imaginary_part + num2.imaginary_part);
}

/*Given 2 complex numbers num1, num2, return the value of num1-num2 */
Complex sub(Complex num1, Complex num2) {
    return getComplex(num1.real_part - num2.real_part, num1.imaginary_part - num2.imaginary_part);
}


/*Given  complex number cmp, and real number "real" return the value of cmp * real*/
Complex multReal(Complex cmp, float real) {
    return getComplex(cmp.real_part * real, cmp.imaginary_part * real);
}


/*Given  complex number cmp, and real number "img"that represent
 * the number img*i return the value of cmp * img * i */
Complex multImg(Complex cmp, float img) {
    return getComplex(-img * cmp.imaginary_part, img * cmp.real_part);
}

/*Given 2 complex numbers num1, num2, return the value of num1*num2 */
Complex multComp(Complex num1, Complex num2) {
    return getComplex(num1.real_part * num2.real_part - num1.imaginary_part * num2.imaginary_part,
                      num1.real_part * num2.imaginary_part + num1.imaginary_part * num2.real_part);
}

/*Given a complex number num return the value of |num| */
float comp_abs(Complex num) {
    return sqrt(num.real_part * num.real_part + num.imaginary_part * num.imaginary_part);
}

