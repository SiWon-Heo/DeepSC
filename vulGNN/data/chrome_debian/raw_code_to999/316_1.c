static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 VideoXLContext * const a = avctx -> priv_data ;
 AVFrame * const p = & a -> pic ;
 uint8_t * Y , * U , * V ;
 int i , j , ret ;
 int stride ;
 uint32_t val ;
 int y0 , y1 , y2 , y3 = 0 , c0 = 0 , c1 = 0 ;
 if ( p -> data [ 0 ] ) avctx -> release_buffer ( avctx , p ) ;
 p -> reference = 0 ;
 if ( ( ret = ff_get_buffer ( avctx , p ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 p -> pict_type = AV_PICTURE_TYPE_I ;
 p -> key_frame = 1 ;
 Y = a -> pic . data [ 0 ] ;
 U = a -> pic . data [ 1 ] ;
 V = a -> pic . data [ 2 ] ;
 stride = avctx -> width - 4 ;
 if ( buf_size < avctx -> width * avctx -> height ) {
 av_log ( avctx , AV_LOG_ERROR , "Packet is too small\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 for ( i = 0 ;
 i < avctx -> height ;
 i ++ ) {
 buf += stride ;
 for ( j = 0 ;
 j < avctx -> width ;
 j += 4 ) {
 val = AV_RL32 ( buf ) ;
 buf -= 4 ;
 val = ( ( val >> 16 ) & 0xFFFF ) | ( ( val & 0xFFFF ) << 16 ) ;
 if ( ! j ) y0 = ( val & 0x1F ) << 2 ;
 else y0 = y3 + xl_table [ val & 0x1F ] ;
 val >>= 5 ;
 y1 = y0 + xl_table [ val & 0x1F ] ;
 val >>= 5 ;
 y2 = y1 + xl_table [ val & 0x1F ] ;
 val >>= 6 ;
 y3 = y2 + xl_table [ val & 0x1F ] ;
 val >>= 5 ;
 if ( ! j ) c0 = ( val & 0x1F ) << 2 ;
 else c0 += xl_table [ val & 0x1F ] ;
 val >>= 5 ;
 if ( ! j ) c1 = ( val & 0x1F ) << 2 ;
 else c1 += xl_table [ val & 0x1F ] ;
 Y [ j + 0 ] = y0 << 1 ;
 Y [ j + 1 ] = y1 << 1 ;
 Y [ j + 2 ] = y2 << 1 ;
 Y [ j + 3 ] = y3 << 1 ;
 U [ j >> 2 ] = c0 << 1 ;
 V [ j >> 2 ] = c1 << 1 ;
 }
 buf += avctx -> width + 4 ;
 Y += a -> pic . linesize [ 0 ] ;
 U += a -> pic . linesize [ 1 ] ;
 V += a -> pic . linesize [ 2 ] ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = a -> pic ;
 return buf_size ;
 }