static int s302m_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 int block_size , ret ;
 int frame_size = s302m_parse_frame_header ( avctx , buf , buf_size ) ;
 if ( frame_size < 0 ) return frame_size ;
 buf_size -= AES3_HEADER_LEN ;
 buf += AES3_HEADER_LEN ;
 block_size = ( avctx -> bits_per_coded_sample + 4 ) / 4 ;
 frame -> nb_samples = 2 * ( buf_size / block_size ) / avctx -> channels ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 buf_size = ( frame -> nb_samples * avctx -> channels / 2 ) * block_size ;
 if ( avctx -> bits_per_coded_sample == 24 ) {
 uint32_t * o = ( uint32_t * ) frame -> data [ 0 ] ;
 for ( ;
 buf_size > 6 ;
 buf_size -= 7 ) {
 * o ++ = ( ff_reverse [ buf [ 2 ] ] << 24 ) | ( ff_reverse [ buf [ 1 ] ] << 16 ) | ( ff_reverse [ buf [ 0 ] ] << 8 ) ;
 * o ++ = ( ff_reverse [ buf [ 6 ] & 0xf0 ] << 28 ) | ( ff_reverse [ buf [ 5 ] ] << 20 ) | ( ff_reverse [ buf [ 4 ] ] << 12 ) | ( ff_reverse [ buf [ 3 ] & 0x0f ] << 4 ) ;
 buf += 7 ;
 }
 }
 else if ( avctx -> bits_per_coded_sample == 20 ) {
 uint32_t * o = ( uint32_t * ) frame -> data [ 0 ] ;
 for ( ;
 buf_size > 5 ;
 buf_size -= 6 ) {
 * o ++ = ( ff_reverse [ buf [ 2 ] & 0xf0 ] << 28 ) | ( ff_reverse [ buf [ 1 ] ] << 20 ) | ( ff_reverse [ buf [ 0 ] ] << 12 ) ;
 * o ++ = ( ff_reverse [ buf [ 5 ] & 0xf0 ] << 28 ) | ( ff_reverse [ buf [ 4 ] ] << 20 ) | ( ff_reverse [ buf [ 3 ] ] << 12 ) ;
 buf += 6 ;
 }
 }
 else {
 uint16_t * o = ( uint16_t * ) frame -> data [ 0 ] ;
 for ( ;
 buf_size > 4 ;
 buf_size -= 5 ) {
 * o ++ = ( ff_reverse [ buf [ 1 ] ] << 8 ) | ff_reverse [ buf [ 0 ] ] ;
 * o ++ = ( ff_reverse [ buf [ 4 ] & 0xf0 ] << 12 ) | ( ff_reverse [ buf [ 3 ] ] << 4 ) | ( ff_reverse [ buf [ 2 ] ] >> 4 ) ;
 buf += 5 ;
 }
 }
 * got_frame_ptr = 1 ;
 return avpkt -> size ;
 }