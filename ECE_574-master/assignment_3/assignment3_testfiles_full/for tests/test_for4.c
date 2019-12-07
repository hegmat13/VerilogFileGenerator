input Int1 go
input Int32 a, b, c, start, inc
input UInt1 status, notdone

output Int32 z, x

variable Int32 e, g, d, f, count

for ( count = start; status == notdone;  count = count * inc ) {
   d = a + b
   e = a + c
   g = d > e
   z = g ? d : e
   f = a * c
   x = f - d
}
