static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 EightBpsContext * const c = avctx -> priv_data ;
 const unsigned char * encoded = buf ;
 unsigned char * pixptr , * pixptr_end ;
 unsigned int height = avctx -> height ;
 unsigned int dlen , p , row ;
 const unsigned char * lp , * dp ;
 unsigned char count ;
 unsigned int px_inc ;
 unsigned int planes = c -> planes ;
 unsigned char * planemap = c -> planemap ;
 int ret ;
 if ( c -> pic . data [ 0 ] ) avctx -> release_buffer ( avctx , & c -> pic ) ;
 c -> pic . reference = 0 ;
 c -> pic . buffer_hints = FF_BUFFER_HINTS_VALID ;
 if ( ( ret = ff_get_buffer ( avctx , & c -> pic ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 dp = encoded + planes * ( height << 1 ) ;
 if ( planes == 4 ) planes -- ;
 px_inc = planes + ( avctx -> pix_fmt == AV_PIX_FMT_RGB32 ) ;
 for ( p = 0 ;
 p < planes ;
 p ++ ) {
 lp = encoded + p * ( height << 1 ) ;
 for ( row = 0 ;
 row < height ;
 row ++ ) {
 pixptr = c -> pic . data [ 0 ] + row * c -> pic . linesize [ 0 ] + planemap [ p ] ;
 pixptr_end = pixptr + c -> pic . linesize [ 0 ] ;
 dlen = av_be2ne16 ( * ( const unsigned short * ) ( lp + row * 2 ) ) ;
 while ( dlen > 0 ) {
 if ( dp + 1 >= buf + buf_size ) return AVERROR_INVALIDDATA ;
 if ( ( count = * dp ++ ) <= 127 ) {
 count ++ ;
 dlen -= count + 1 ;
 if ( pixptr + count * px_inc > pixptr_end ) break ;
 if ( dp + count > buf + buf_size ) return AVERROR_INVALIDDATA ;
 while ( count -- ) {
 * pixptr = * dp ++ ;
 pixptr += px_inc ;
 }
 }
 else {
 count = 257 - count ;
 if ( pixptr + count * px_inc > pixptr_end ) break ;
 while ( count -- ) {
 * pixptr = * dp ;
 pixptr += px_inc ;
 }
 dp ++ ;
 dlen -= 2 ;
 }
 }
 }
 }
 if ( avctx -> bits_per_coded_sample <= 8 ) {
 const uint8_t * pal = av_packet_get_side_data ( avpkt , AV_PKT_DATA_PALETTE , NULL ) ;
 if ( pal ) {
 c -> pic . palette_has_changed = 1 ;
 memcpy ( c -> pal , pal , AVPALETTE_SIZE ) ;
 }
 memcpy ( c -> pic . data [ 1 ] , c -> pal , AVPALETTE_SIZE ) ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = c -> pic ;
 return buf_size ;
 }