input UInt32 a, b, c, five
output UInt32 z, x

variable UInt32 d, e, f, g

for ( d = a + b; d > five; d = d - 1 ) {
   e = a + c
   g = d > e
   z = g ? d : e
   f = a * c
   x = f - d
}
