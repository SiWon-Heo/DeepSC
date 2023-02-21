int checksum_block ( const char * s , int len ) {
 int sum = 0 ;
 while ( len -- ) {
 sum ^= * s ++ ;
 }
 return sum ;
 }