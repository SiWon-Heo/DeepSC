static int jbig2_decode_mmr_line ( Jbig2MmrCtx * mmr , const byte * ref , byte * dst ) {
 uint32_t a0 = MINUS1 ;
 uint32_t a1 , a2 , b1 , b2 ;
 int c = 0 ;
 while ( 1 ) {
 uint32_t word = mmr -> word ;
 if ( a0 != MINUS1 && a0 >= mmr -> width ) break ;
 if ( ( word >> ( 32 - 3 ) ) == 1 ) {
 int white_run , black_run ;
 jbig2_decode_mmr_consume ( mmr , 3 ) ;
 if ( a0 == MINUS1 ) a0 = 0 ;
 if ( c == 0 ) {
 white_run = jbig2_decode_get_run ( mmr , jbig2_mmr_white_decode , 8 ) ;
 black_run = jbig2_decode_get_run ( mmr , jbig2_mmr_black_decode , 7 ) ;
 a1 = a0 + white_run ;
 a2 = a1 + black_run ;
 if ( a1 > mmr -> width ) a1 = mmr -> width ;
 if ( a2 > mmr -> width ) a2 = mmr -> width ;
 if ( a1 == MINUS1 || a2 < a1 ) return - 1 ;
 jbig2_set_bits ( dst , a1 , a2 ) ;
 a0 = a2 ;
 }
 else {
 black_run = jbig2_decode_get_run ( mmr , jbig2_mmr_black_decode , 7 ) ;
 white_run = jbig2_decode_get_run ( mmr , jbig2_mmr_white_decode , 8 ) ;
 a1 = a0 + black_run ;
 a2 = a1 + white_run ;
 if ( a1 > mmr -> width ) a1 = mmr -> width ;
 if ( a2 > mmr -> width ) a2 = mmr -> width ;
 if ( a0 == MINUS1 || a1 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , a1 ) ;
 a0 = a2 ;
 }
 }
 else if ( ( word >> ( 32 - 4 ) ) == 1 ) {
 jbig2_decode_mmr_consume ( mmr , 4 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 b2 = jbig2_find_changing_element ( ref , b1 , mmr -> width ) ;
 if ( c ) {
 if ( a0 == MINUS1 || b2 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b2 ) ;
 }
 a0 = b2 ;
 }
 else if ( ( word >> ( 32 - 1 ) ) == 1 ) {
 jbig2_decode_mmr_consume ( mmr , 1 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 if ( c ) {
 if ( a0 == MINUS1 || b1 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b1 ) ;
 }
 a0 = b1 ;
 c = ! c ;
 }
 else if ( ( word >> ( 32 - 3 ) ) == 3 ) {
 jbig2_decode_mmr_consume ( mmr , 3 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 if ( b1 + 1 > mmr -> width ) break ;
 if ( c ) {
 if ( a0 == MINUS1 || b1 + 1 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b1 + 1 ) ;
 }
 a0 = b1 + 1 ;
 c = ! c ;
 }
 else if ( ( word >> ( 32 - 6 ) ) == 3 ) {
 jbig2_decode_mmr_consume ( mmr , 6 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 if ( b1 + 2 > mmr -> width ) break ;
 if ( c ) {
 if ( a0 == MINUS1 || b1 + 2 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b1 + 2 ) ;
 }
 a0 = b1 + 2 ;
 c = ! c ;
 }
 else if ( ( word >> ( 32 - 7 ) ) == 3 ) {
 jbig2_decode_mmr_consume ( mmr , 7 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 if ( b1 + 3 > ( int ) mmr -> width ) break ;
 if ( c ) {
 if ( a0 == MINUS1 || b1 + 3 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b1 + 3 ) ;
 }
 a0 = b1 + 3 ;
 c = ! c ;
 }
 else if ( ( word >> ( 32 - 3 ) ) == 2 ) {
 jbig2_decode_mmr_consume ( mmr , 3 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 if ( b1 < 1 ) break ;
 if ( c ) {
 if ( a0 == MINUS1 || b1 - 1 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b1 - 1 ) ;
 }
 a0 = b1 - 1 ;
 c = ! c ;
 }
 else if ( ( word >> ( 32 - 6 ) ) == 2 ) {
 jbig2_decode_mmr_consume ( mmr , 6 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 if ( b1 < 2 ) break ;
 if ( c ) {
 if ( b1 - 2 < a0 || a0 < 0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b1 - 2 ) ;
 }
 a0 = b1 - 2 ;
 c = ! c ;
 }
 else if ( ( word >> ( 32 - 7 ) ) == 2 ) {
 jbig2_decode_mmr_consume ( mmr , 7 ) ;
 b1 = jbig2_find_changing_element_of_color ( ref , a0 , mmr -> width , ! c ) ;
 if ( b1 < 3 ) break ;
 if ( c ) {
 if ( a0 == MINUS1 || b1 - 3 < a0 ) return - 1 ;
 jbig2_set_bits ( dst , a0 , b1 - 3 ) ;
 }
 a0 = b1 - 3 ;
 c = ! c ;
 }
 else break ;
 }
 return 0 ;
 }