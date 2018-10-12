typedef struct complex{
   float real_part;
   float imaginary_part;
} Complex;


Complex getComplex(float real_part,float imaginary_part);
Complex add(Complex num1,Complex num2);
Complex sub(Complex num1,Complex num2);
Complex multReal(Complex num1,float num2);
Complex multImg(Complex num1,float num2);
Complex multComp(Complex num1,Complex num2);
float comp_abs(Complex num);
