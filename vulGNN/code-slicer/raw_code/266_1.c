static int do_uncompress ( compress_filter_context_t * zfx , z_stream * zs , IOBUF a , size_t * ret_len ) {
 int zrc ;
 int rc = 0 ;
 size_t n ;
 int nread , count ;
 int refill = ! zs -> avail_in ;
 if ( DBG_FILTER ) log_debug ( "begin inflate: avail_in=%u, avail_out=%u, inbuf=%u\n" , ( unsigned ) zs -> avail_in , ( unsigned ) zs -> avail_out , ( unsigned ) zfx -> inbufsize ) ;
 do {
 if ( zs -> avail_in < zfx -> inbufsize && refill ) {
 n = zs -> avail_in ;
 if ( ! n ) # ifndef __riscos__ zs -> next_in = zfx -> inbuf ;
 # else zs -> next_in = ( Bytef * ) zfx -> inbuf ;
 # endif count = zfx -> inbufsize - n ;
 nread = iobuf_read ( a , zfx -> inbuf + n , count ) ;
 if ( nread == - 1 ) nread = 0 ;
 n += nread ;
 if ( nread < count && zfx -> algo == 1 ) {
 * ( zfx -> inbuf + n ) = 0xFF ;
 zfx -> algo1hack = 1 ;
 n ++ ;
 }
 zs -> avail_in = n ;
 }
 refill = 1 ;
 if ( DBG_FILTER ) log_debug ( "enter inflate: avail_in=%u, avail_out=%u\n" , ( unsigned ) zs -> avail_in , ( unsigned ) zs -> avail_out ) ;
 # ifdef Z_SYNC_FLUSH zrc = inflate ( zs , Z_SYNC_FLUSH ) ;
 # else zrc = inflate ( zs , Z_PARTIAL_FLUSH ) ;
 # endif if ( DBG_FILTER ) log_debug ( "leave inflate: avail_in=%u, avail_out=%u, zrc=%d\n" , ( unsigned ) zs -> avail_in , ( unsigned ) zs -> avail_out , zrc ) ;
 if ( zrc == Z_STREAM_END ) rc = - 1 ;
 else if ( zrc != Z_OK && zrc != Z_BUF_ERROR ) {
 if ( zs -> msg ) log_fatal ( "zlib inflate problem: %s\n" , zs -> msg ) ;
 else log_fatal ( "zlib inflate problem: rc=%d\n" , zrc ) ;
 }
 }
 while ( zs -> avail_out && zrc != Z_STREAM_END && zrc != Z_BUF_ERROR ) ;
 * ret_len = zfx -> outbufsize - zs -> avail_out ;
 if ( DBG_FILTER ) log_debug ( "do_uncompress: returning %u bytes\n" , ( unsigned ) * ret_len ) ;
 return rc ;
 }