static int tmv_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 TMVContext * tmv = avctx -> priv_data ;
 const uint8_t * src = avpkt -> data ;
 uint8_t * dst ;
 unsigned char_cols = avctx -> width >> 3 ;
 unsigned char_rows = avctx -> height >> 3 ;
 unsigned x , y , fg , bg , c ;
 int ret ;
 if ( tmv -> pic . data [ 0 ] ) avctx -> release_buffer ( avctx , & tmv -> pic ) ;
 if ( ( ret = ff_get_buffer ( avctx , & tmv -> pic ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 if ( avpkt -> size < 2 * char_rows * char_cols ) {
 av_log ( avctx , AV_LOG_ERROR , "Input buffer too small, truncated sample?\n" ) ;
 * got_frame = 0 ;
 return AVERROR_INVALIDDATA ;
 }
 tmv -> pic . pict_type = AV_PICTURE_TYPE_I ;
 tmv -> pic . key_frame = 1 ;
 dst = tmv -> pic . data [ 0 ] ;
 tmv -> pic . palette_has_changed = 1 ;
 memcpy ( tmv -> pic . data [ 1 ] , ff_cga_palette , 16 * 4 ) ;
 for ( y = 0 ;
 y < char_rows ;
 y ++ ) {
 for ( x = 0 ;
 x < char_cols ;
 x ++ ) {
 c = * src ++ ;
 bg = * src >> 4 ;
 fg = * src ++ & 0xF ;
 ff_draw_pc_font ( dst + x * 8 , tmv -> pic . linesize [ 0 ] , ff_cga_font , 8 , c , fg , bg ) ;
 }
 dst += tmv -> pic . linesize [ 0 ] * 8 ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = tmv -> pic ;
 return avpkt -> size ;
 }