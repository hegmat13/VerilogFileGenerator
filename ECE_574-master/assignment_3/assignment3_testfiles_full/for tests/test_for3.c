input Int32 a, b, c, d, e, f, g, h, num, stride, zero, one
output Int32 avg

variable Int32 t1, t2, t3, t4, t5, t6, t7, atemp, i

t1 = a + b
t2 = t1 + c 
t3 = t2 + d 
t4 = t3 + e 
t5 = t4 + f 
t6 = t5 + g 
t7 = t6 + h 
atemp = a + zero

for ( i = num + zero; i > zero;  i = i - stride ) {
   atemp = atemp >> one
   avg = atemp >> one
}
