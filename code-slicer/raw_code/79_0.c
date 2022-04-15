static int jbig2_decode_get_run ( Jbig2MmrCtx * mmr , const mmr_table_node * table , int initial_bits ) {
 int result = 0 ;
 int val ;
 do {
 val = jbig2_decode_get_code ( mmr , table , initial_bits ) ;
 result += val ;
 }
 while ( val >= 64 ) ;
 return result ;
 }