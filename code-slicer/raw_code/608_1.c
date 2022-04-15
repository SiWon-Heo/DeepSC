static int tgq_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 TgqContext * s = avctx -> priv_data ;
 int x , y , ret ;
 int big_endian = AV_RL32 ( & buf [ 4 ] ) > 0x000FFFFF ;
 if ( buf_size < 16 ) {
 av_log ( avctx , AV_LOG_WARNING , "truncated header\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 bytestream2_init ( & s -> gb , buf + 8 , buf_size - 8 ) ;
 if ( big_endian ) {
 s -> width = bytestream2_get_be16u ( & s -> gb ) ;
 s -> height = bytestream2_get_be16u ( & s -> gb ) ;
 }
 else {
 s -> width = bytestream2_get_le16u ( & s -> gb ) ;
 s -> height = bytestream2_get_le16u ( & s -> gb ) ;
 }
 if ( s -> avctx -> width != s -> width || s -> avctx -> height != s -> height ) {
 avcodec_set_dimensions ( s -> avctx , s -> width , s -> height ) ;
 if ( s -> frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> frame ) ;
 }
 tgq_calculate_qtable ( s , bytestream2_get_byteu ( & s -> gb ) ) ;
 bytestream2_skip ( & s -> gb , 3 ) ;
 if ( ! s -> frame . data [ 0 ] ) {
 s -> frame . key_frame = 1 ;
 s -> frame . pict_type = AV_PICTURE_TYPE_I ;
 s -> frame . buffer_hints = FF_BUFFER_HINTS_VALID ;
 if ( ( ret = ff_get_buffer ( avctx , & s -> frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 }
 for ( y = 0 ;
 y < FFALIGN ( avctx -> height , 16 ) >> 4 ;
 y ++ ) for ( x = 0 ;
 x < FFALIGN ( avctx -> width , 16 ) >> 4 ;
 x ++ ) tgq_decode_mb ( s , y , x ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> frame ;
 return avpkt -> size ;
 }