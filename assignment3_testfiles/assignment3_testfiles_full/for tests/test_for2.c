input Int32 a, b, c, start, end
output Int32 z, x
variable Int32 d, e, f, g ,h
variable Int1 dLTe, dEQe
variable Int8 i

d = a + b
e = a + c
f = a - b  
dEQe = d == e
dLTe = d < e

for ( i = start; i < end; i = i + 1 ) {

   g = dLTe ? d : e
   h = dEQe ? g : f
   x = h << dLTe
   z = h >> dEQe
   dLTe = d < e
}
