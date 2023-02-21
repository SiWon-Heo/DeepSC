static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 TM2Context * const l = avctx -> priv_data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size & ~ 3 ;
 AVFrame * const p = & l -> pic ;
 int offset = TM2_HEADER_SIZE ;
 int i , t , ret ;
 uint8_t * swbuf ;
 swbuf = av_malloc ( buf_size + FF_INPUT_BUFFER_PADDING_SIZE ) ;
 if ( ! swbuf ) {
 av_log ( avctx , AV_LOG_ERROR , "Cannot allocate temporary buffer\n" ) ;
 return AVERROR ( ENOMEM ) ;
 }
 p -> reference = 1 ;
 p -> buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( ( ret = avctx -> reget_buffer ( avctx , p ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 av_free ( swbuf ) ;
 return ret ;
 }
 l -> dsp . bswap_buf ( ( uint32_t * ) swbuf , ( const uint32_t * ) buf , buf_size >> 2 ) ;
 if ( ( ret = tm2_read_header ( l , swbuf ) ) < 0 ) {
 av_free ( swbuf ) ;
 return ret ;
 }
 for ( i = 0 ;
 i < TM2_NUM_STREAMS ;
 i ++ ) {
 if ( offset >= buf_size ) {
 av_free ( swbuf ) ;
 return AVERROR_INVALIDDATA ;
 }
 t = tm2_read_stream ( l , swbuf + offset , tm2_stream_order [ i ] , buf_size - offset ) ;
 if ( t < 0 ) {
 av_free ( swbuf ) ;
 return t ;
 }
 offset += t ;
 }
 p -> key_frame = tm2_decode_blocks ( l , p ) ;
 if ( p -> key_frame ) p -> pict_type = AV_PICTURE_TYPE_I ;
 else p -> pict_type = AV_PICTURE_TYPE_P ;
 l -> cur = ! l -> cur ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = l -> pic ;
 av_free ( swbuf ) ;
 return buf_size ;
 }