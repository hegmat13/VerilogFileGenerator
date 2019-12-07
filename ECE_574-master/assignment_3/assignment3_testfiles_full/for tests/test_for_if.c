input UInt32 a, b, zero, one

output UInt32 gcd

variable UInt32 areg, breg, gcdIters
variable UInt1 aGTb, aLTb, aNEQb

areg = a + zero
breg = b + zero

for ( gcdIters = one; aNEQb == one; gcdIters = gcdIters + 1 ) {

   aGTb = a > b
   aLTb = a < b
   aNEQb = aGTb + aLTb

   if ( aGTb ) {
      areg = areg - breg
   }

   if ( aLTb ) {
      breg = breg - areg
   }

   aGTb = areg > breg
   aLTb = areg < breg
   aNEQb = aGTb + aLTb
}
   
gcd = areg + zero

