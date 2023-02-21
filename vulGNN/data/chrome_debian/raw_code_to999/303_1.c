static int aura_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * pkt ) {
 AuraDecodeContext * s = avctx -> priv_data ;
 uint8_t * Y , * U , * V ;
 uint8_t val ;
 int x , y , ret ;
 const uint8_t * buf = pkt -> data ;
 const int8_t * delta_table = ( const int8_t * ) buf + 16 ;
 if ( pkt -> size != 48 + avctx -> height * avctx -> width ) {
 av_log ( avctx , AV_LOG_ERROR , "got a buffer with %d bytes when %d were expected\n" , pkt -> size , 48 + avctx -> height * avctx -> width ) ;
 return AVERROR_INVALIDDATA ;
 }
 buf += 48 ;
 if ( s -> frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> frame ) ;
 s -> frame . buffer_hints = FF_BUFFER_HINTS_VALID ;
 s -> frame . reference = 0 ;
 if ( ( ret = ff_get_buffer ( avctx , & s -> frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 Y = s -> frame . data [ 0 ] ;
 U = s -> frame . data [ 1 ] ;
 V = s -> frame . data [ 2 ] ;
 for ( y = 0 ;
 y < avctx -> height ;
 y ++ ) {
 val = * buf ++ ;
 U [ 0 ] = val & 0xF0 ;
 Y [ 0 ] = val << 4 ;
 val = * buf ++ ;
 V [ 0 ] = val & 0xF0 ;
 Y [ 1 ] = Y [ 0 ] + delta_table [ val & 0xF ] ;
 Y += 2 ;
 U ++ ;
 V ++ ;
 for ( x = 1 ;
 x < ( avctx -> width >> 1 ) ;
 x ++ ) {
 val = * buf ++ ;
 U [ 0 ] = U [ - 1 ] + delta_table [ val >> 4 ] ;
 Y [ 0 ] = Y [ - 1 ] + delta_table [ val & 0xF ] ;
 val = * buf ++ ;
 V [ 0 ] = V [ - 1 ] + delta_table [ val >> 4 ] ;
 Y [ 1 ] = Y [ 0 ] + delta_table [ val & 0xF ] ;
 Y += 2 ;
 U ++ ;
 V ++ ;
 }
 Y += s -> frame . linesize [ 0 ] - avctx -> width ;
 U += s -> frame . linesize [ 1 ] - ( avctx -> width >> 1 ) ;
 V += s -> frame . linesize [ 2 ] - ( avctx -> width >> 1 ) ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> frame ;
 return pkt -> size ;
 }