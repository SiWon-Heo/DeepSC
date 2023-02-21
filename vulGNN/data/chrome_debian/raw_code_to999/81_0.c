static int jbig2_decode_get_code ( Jbig2MmrCtx * mmr , const mmr_table_node * table , int initial_bits ) {
 uint32_t word = mmr -> word ;
 int table_ix = word >> ( 32 - initial_bits ) ;
 int val = table [ table_ix ] . val ;
 int n_bits = table [ table_ix ] . n_bits ;
 if ( n_bits > initial_bits ) {
 int mask = ( 1 << ( 32 - initial_bits ) ) - 1 ;
 table_ix = val + ( ( word & mask ) >> ( 32 - n_bits ) ) ;
 val = table [ table_ix ] . val ;
 n_bits = initial_bits + table [ table_ix ] . n_bits ;
 }
 jbig2_decode_mmr_consume ( mmr , n_bits ) ;
 return val ;
 }