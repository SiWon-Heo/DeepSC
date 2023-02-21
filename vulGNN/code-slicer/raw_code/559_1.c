static int vcr1_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 VCR1Context * const a = avctx -> priv_data ;
 AVFrame * picture = data ;
 AVFrame * const p = & a -> picture ;
 const uint8_t * bytestream = buf ;
 int i , x , y , ret ;
 if ( p -> data [ 0 ] ) avctx -> release_buffer ( avctx , p ) ;
 p -> reference = 0 ;
 if ( ( ret = ff_get_buffer ( avctx , p ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 p -> pict_type = AV_PICTURE_TYPE_I ;
 p -> key_frame = 1 ;
 for ( i = 0 ;
 i < 16 ;
 i ++ ) {
 a -> delta [ i ] = * bytestream ++ ;
 bytestream ++ ;
 }
 for ( y = 0 ;
 y < avctx -> height ;
 y ++ ) {
 int offset ;
 uint8_t * luma = & a -> picture . data [ 0 ] [ y * a -> picture . linesize [ 0 ] ] ;
 if ( ( y & 3 ) == 0 ) {
 uint8_t * cb = & a -> picture . data [ 1 ] [ ( y >> 2 ) * a -> picture . linesize [ 1 ] ] ;
 uint8_t * cr = & a -> picture . data [ 2 ] [ ( y >> 2 ) * a -> picture . linesize [ 2 ] ] ;
 for ( i = 0 ;
 i < 4 ;
 i ++ ) a -> offset [ i ] = * bytestream ++ ;
 offset = a -> offset [ 0 ] - a -> delta [ bytestream [ 2 ] & 0xF ] ;
 for ( x = 0 ;
 x < avctx -> width ;
 x += 4 ) {
 luma [ 0 ] = offset += a -> delta [ bytestream [ 2 ] & 0xF ] ;
 luma [ 1 ] = offset += a -> delta [ bytestream [ 2 ] >> 4 ] ;
 luma [ 2 ] = offset += a -> delta [ bytestream [ 0 ] & 0xF ] ;
 luma [ 3 ] = offset += a -> delta [ bytestream [ 0 ] >> 4 ] ;
 luma += 4 ;
 * cb ++ = bytestream [ 3 ] ;
 * cr ++ = bytestream [ 1 ] ;
 bytestream += 4 ;
 }
 }
 else {
 offset = a -> offset [ y & 3 ] - a -> delta [ bytestream [ 2 ] & 0xF ] ;
 for ( x = 0 ;
 x < avctx -> width ;
 x += 8 ) {
 luma [ 0 ] = offset += a -> delta [ bytestream [ 2 ] & 0xF ] ;
 luma [ 1 ] = offset += a -> delta [ bytestream [ 2 ] >> 4 ] ;
 luma [ 2 ] = offset += a -> delta [ bytestream [ 3 ] & 0xF ] ;
 luma [ 3 ] = offset += a -> delta [ bytestream [ 3 ] >> 4 ] ;
 luma [ 4 ] = offset += a -> delta [ bytestream [ 0 ] & 0xF ] ;
 luma [ 5 ] = offset += a -> delta [ bytestream [ 0 ] >> 4 ] ;
 luma [ 6 ] = offset += a -> delta [ bytestream [ 1 ] & 0xF ] ;
 luma [ 7 ] = offset += a -> delta [ bytestream [ 1 ] >> 4 ] ;
 luma += 8 ;
 bytestream += 4 ;
 }
 }
 }
 * picture = a -> picture ;
 * got_frame = 1 ;
 return buf_size ;
 }