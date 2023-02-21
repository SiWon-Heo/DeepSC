static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 AnmContext * s = avctx -> priv_data ;
 const int buf_size = avpkt -> size ;
 uint8_t * dst , * dst_end ;
 int count , ret ;
 if ( ( ret = avctx -> reget_buffer ( avctx , & s -> frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 dst = s -> frame . data [ 0 ] ;
 dst_end = s -> frame . data [ 0 ] + s -> frame . linesize [ 0 ] * avctx -> height ;
 bytestream2_init ( & s -> gb , avpkt -> data , buf_size ) ;
 if ( bytestream2_get_byte ( & s -> gb ) != 0x42 ) {
 av_log_ask_for_sample ( avctx , "unknown record type\n" ) ;
 return buf_size ;
 }
 if ( bytestream2_get_byte ( & s -> gb ) ) {
 av_log_ask_for_sample ( avctx , "padding bytes not supported\n" ) ;
 return buf_size ;
 }
 bytestream2_skip ( & s -> gb , 2 ) ;
 s -> x = 0 ;
 do {
 # define OP ( gb , pixel , count ) op ( & dst , dst_end , ( gb ) , ( pixel ) , ( count ) , & s -> x , avctx -> width , s -> frame . linesize [ 0 ] ) int type = bytestream2_get_byte ( & s -> gb ) ;
 count = type & 0x7F ;
 type >>= 7 ;
 if ( count ) {
 if ( OP ( type ? NULL : & s -> gb , - 1 , count ) ) break ;
 }
 else if ( ! type ) {
 int pixel ;
 count = bytestream2_get_byte ( & s -> gb ) ;
 pixel = bytestream2_get_byte ( & s -> gb ) ;
 if ( OP ( NULL , pixel , count ) ) break ;
 }
 else {
 int pixel ;
 type = bytestream2_get_le16 ( & s -> gb ) ;
 count = type & 0x3FFF ;
 type >>= 14 ;
 if ( ! count ) {
 if ( type == 0 ) break ;
 if ( type == 2 ) {
 av_log_ask_for_sample ( avctx , "unknown opcode" ) ;
 return AVERROR_PATCHWELCOME ;
 }
 continue ;
 }
 pixel = type == 3 ? bytestream2_get_byte ( & s -> gb ) : - 1 ;
 if ( type == 1 ) count += 0x4000 ;
 if ( OP ( type == 2 ? & s -> gb : NULL , pixel , count ) ) break ;
 }
 }
 while ( bytestream2_get_bytes_left ( & s -> gb ) > 0 ) ;
 memcpy ( s -> frame . data [ 1 ] , s -> palette , AVPALETTE_SIZE ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> frame ;
 return buf_size ;
 }