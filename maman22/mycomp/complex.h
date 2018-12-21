/* a structure of a complex number*/
typedef struct complex {
    double real_part;
    double imaginary_part;
} Complex;

/*initialize a complex number*/
Complex getComplex(double real_part, double imaginary_part);

/*Given 2 complex numbers num1, num2, return the value of num1+num2 */
Complex add(Complex num1, Complex num2);

/*Given 2 complex numbers num1, num2, return the value of num1-num2 */
Complex sub(Complex num1, Complex num2);


/*Given  complex number cmp, and real number "real" return the value of cmp * real*/
Complex multReal(Complex cmp, double real);

/*Given  complex number cmp, and real number "img"that represent
 * the number img*i return the value of cmp * img * i */
Complex multImg(Complex cmp, double img);

/*Given 2 complex numbers num1, num2, return the value of num1*num2 */
Complex multComp(Complex num1, Complex num2);

/*Given a complex number num return the value of |num| */
double comp_abs(Complex num);
