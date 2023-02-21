static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 int h , w , ret ;
 AVFrame * pic = avctx -> coded_frame ;
 const uint8_t * psrc = avpkt -> data ;
 uint16_t * y , * u , * v ;
 int aligned_width = ( ( avctx -> width + 47 ) / 48 ) * 48 ;
 int stride = aligned_width * 8 / 3 ;
 if ( pic -> data [ 0 ] ) avctx -> release_buffer ( avctx , pic ) ;
 if ( avpkt -> size < stride * avctx -> height ) {
 av_log ( avctx , AV_LOG_ERROR , "packet too small\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 pic -> reference = 0 ;
 if ( ( ret = ff_get_buffer ( avctx , pic ) ) < 0 ) return ret ;
 y = ( uint16_t * ) pic -> data [ 0 ] ;
 u = ( uint16_t * ) pic -> data [ 1 ] ;
 v = ( uint16_t * ) pic -> data [ 2 ] ;
 pic -> pict_type = AV_PICTURE_TYPE_I ;
 pic -> key_frame = 1 ;
 # define READ_PIXELS ( a , b , c ) do {
 val = av_le2ne32 ( * src ++ ) ;
 * a ++ = val & 0x3FF ;
 * b ++ = ( val >> 10 ) & 0x3FF ;
 * c ++ = ( val >> 20 ) & 0x3FF ;
 }
 while ( 0 ) for ( h = 0 ;
 h < avctx -> height ;
 h ++ ) {
 const uint32_t * src = ( const uint32_t * ) psrc ;
 uint32_t val ;
 for ( w = 0 ;
 w < avctx -> width - 5 ;
 w += 6 ) {
 READ_PIXELS ( u , y , v ) ;
 READ_PIXELS ( y , u , y ) ;
 READ_PIXELS ( v , y , u ) ;
 READ_PIXELS ( y , v , y ) ;
 }
 if ( w < avctx -> width - 1 ) {
 READ_PIXELS ( u , y , v ) ;
 val = av_le2ne32 ( * src ++ ) ;
 * y ++ = val & 0x3FF ;
 }
 if ( w < avctx -> width - 3 ) {
 * u ++ = ( val >> 10 ) & 0x3FF ;
 * y ++ = ( val >> 20 ) & 0x3FF ;
 val = av_le2ne32 ( * src ++ ) ;
 * v ++ = val & 0x3FF ;
 * y ++ = ( val >> 10 ) & 0x3FF ;
 }
 psrc += stride ;
 y += pic -> linesize [ 0 ] / 2 - avctx -> width ;
 u += pic -> linesize [ 1 ] / 2 - avctx -> width / 2 ;
 v += pic -> linesize [ 2 ] / 2 - avctx -> width / 2 ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = * avctx -> coded_frame ;
 return avpkt -> size ;
 }