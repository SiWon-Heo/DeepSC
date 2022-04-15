uint8_t * * jbig2_decode_gray_scale_image ( Jbig2Ctx * ctx , Jbig2Segment * segment , const byte * data , const size_t size , bool GSMMR , uint32_t GSW , uint32_t GSH , uint32_t GSBPP , bool GSUSESKIP , Jbig2Image * GSKIP , int GSTEMPLATE , Jbig2ArithCx * GB_stats ) {
 uint8_t * * GSVALS = NULL ;
 size_t consumed_bytes = 0 ;
 int i , j , code , stride ;
 int x , y ;
 Jbig2Image * * GSPLANES ;
 Jbig2GenericRegionParams rparams ;
 Jbig2WordStream * ws = NULL ;
 Jbig2ArithState * as = NULL ;
 GSPLANES = jbig2_new ( ctx , Jbig2Image * , GSBPP ) ;
 if ( GSPLANES == NULL ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "failed to allocate %d bytes for GSPLANES" , GSBPP ) ;
 return NULL ;
 }
 for ( i = 0 ;
 i < GSBPP ;
 ++ i ) {
 GSPLANES [ i ] = jbig2_image_new ( ctx , GSW , GSH ) ;
 if ( GSPLANES [ i ] == NULL ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "failed to allocate %dx%d image for GSPLANES" , GSW , GSH ) ;
 for ( j = i - 1 ;
 j >= 0 ;
 -- j ) {
 jbig2_image_release ( ctx , GSPLANES [ j ] ) ;
 }
 jbig2_free ( ctx -> allocator , GSPLANES ) ;
 return NULL ;
 }
 }
 rparams . MMR = GSMMR ;
 rparams . GBTEMPLATE = GSTEMPLATE ;
 rparams . TPGDON = 0 ;
 rparams . USESKIP = GSUSESKIP ;
 rparams . gbat [ 0 ] = ( GSTEMPLATE <= 1 ? 3 : 2 ) ;
 rparams . gbat [ 1 ] = - 1 ;
 rparams . gbat [ 2 ] = - 3 ;
 rparams . gbat [ 3 ] = - 1 ;
 rparams . gbat [ 4 ] = 2 ;
 rparams . gbat [ 5 ] = - 2 ;
 rparams . gbat [ 6 ] = - 2 ;
 rparams . gbat [ 7 ] = - 2 ;
 if ( GSMMR ) {
 code = jbig2_decode_halftone_mmr ( ctx , & rparams , data , size , GSPLANES [ GSBPP - 1 ] , & consumed_bytes ) ;
 }
 else {
 ws = jbig2_word_stream_buf_new ( ctx , data , size ) ;
 if ( ws == NULL ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "failed to allocate ws in jbig2_decode_gray_scale_image" ) ;
 goto cleanup ;
 }
 as = jbig2_arith_new ( ctx , ws ) ;
 if ( as == NULL ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "failed to allocate as in jbig2_decode_gray_scale_image" ) ;
 goto cleanup ;
 }
 code = jbig2_decode_generic_region ( ctx , segment , & rparams , as , GSPLANES [ GSBPP - 1 ] , GB_stats ) ;
 }
 if ( code != 0 ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "error decoding GSPLANES for halftone image" ) ;
 goto cleanup ;
 }
 j = GSBPP - 2 ;
 while ( j >= 0 ) {
 if ( GSMMR ) {
 code = jbig2_decode_halftone_mmr ( ctx , & rparams , data + consumed_bytes , size - consumed_bytes , GSPLANES [ j ] , & consumed_bytes ) ;
 }
 else {
 code = jbig2_decode_generic_region ( ctx , segment , & rparams , as , GSPLANES [ j ] , GB_stats ) ;
 }
 if ( code != 0 ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "error decoding GSPLANES for halftone image" ) ;
 goto cleanup ;
 }
 stride = GSPLANES [ 0 ] -> stride ;
 for ( i = 0 ;
 i < stride * GSH ;
 ++ i ) GSPLANES [ j ] -> data [ i ] ^= GSPLANES [ j + 1 ] -> data [ i ] ;
 -- j ;
 }
 GSVALS = jbig2_new ( ctx , uint8_t * , GSW ) ;
 if ( GSVALS == NULL ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "failed to allocate GSVALS: %d bytes" , GSW ) ;
 goto cleanup ;
 }
 for ( i = 0 ;
 i < GSW ;
 ++ i ) {
 GSVALS [ i ] = jbig2_new ( ctx , uint8_t , GSH ) ;
 if ( GSVALS [ i ] == NULL ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_FATAL , segment -> number , "failed to allocate GSVALS: %d bytes" , GSH * GSW ) ;
 for ( j = i - 1 ;
 j >= 0 ;
 -- j ) {
 jbig2_free ( ctx -> allocator , GSVALS [ j ] ) ;
 }
 jbig2_free ( ctx -> allocator , GSVALS ) ;
 GSVALS = NULL ;
 goto cleanup ;
 }
 }
 for ( x = 0 ;
 x < GSW ;
 ++ x ) {
 for ( y = 0 ;
 y < GSH ;
 ++ y ) {
 GSVALS [ x ] [ y ] = 0 ;
 for ( j = 0 ;
 j < GSBPP ;
 ++ j ) GSVALS [ x ] [ y ] += jbig2_image_get_pixel ( GSPLANES [ j ] , x , y ) << j ;
 }
 }
 cleanup : if ( ! GSMMR ) {
 jbig2_free ( ctx -> allocator , as ) ;
 jbig2_word_stream_buf_free ( ctx , ws ) ;
 }
 for ( i = 0 ;
 i < GSBPP ;
 ++ i ) jbig2_image_release ( ctx , GSPLANES [ i ] ) ;
 jbig2_free ( ctx -> allocator , GSPLANES ) ;
 return GSVALS ;
 }