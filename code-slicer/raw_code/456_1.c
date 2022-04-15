static int wma_decode_superframe ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 WMACodecContext * s = avctx -> priv_data ;
 int nb_frames , bit_offset , i , pos , len , ret ;
 uint8_t * q ;
 float * * samples ;
 int samples_offset ;
 tprintf ( avctx , "***decode_superframe:\n" ) ;
 if ( buf_size == 0 ) {
 s -> last_superframe_len = 0 ;
 return 0 ;
 }
 if ( buf_size < avctx -> block_align ) {
 av_log ( avctx , AV_LOG_ERROR , "Input packet size too small (%d < %d)\n" , buf_size , avctx -> block_align ) ;
 return AVERROR_INVALIDDATA ;
 }
 buf_size = avctx -> block_align ;
 init_get_bits ( & s -> gb , buf , buf_size * 8 ) ;
 if ( s -> use_bit_reservoir ) {
 skip_bits ( & s -> gb , 4 ) ;
 nb_frames = get_bits ( & s -> gb , 4 ) - ( s -> last_superframe_len <= 0 ) ;
 }
 else {
 nb_frames = 1 ;
 }
 frame -> nb_samples = nb_frames * s -> frame_len ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 samples = ( float * * ) frame -> extended_data ;
 samples_offset = 0 ;
 if ( s -> use_bit_reservoir ) {
 bit_offset = get_bits ( & s -> gb , s -> byte_offset_bits + 3 ) ;
 if ( bit_offset > get_bits_left ( & s -> gb ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid last frame bit offset %d > buf size %d (%d)\n" , bit_offset , get_bits_left ( & s -> gb ) , buf_size ) ;
 goto fail ;
 }
 if ( s -> last_superframe_len > 0 ) {
 if ( ( s -> last_superframe_len + ( ( bit_offset + 7 ) >> 3 ) ) > MAX_CODED_SUPERFRAME_SIZE ) goto fail ;
 q = s -> last_superframe + s -> last_superframe_len ;
 len = bit_offset ;
 while ( len > 7 ) {
 * q ++ = ( get_bits ) ( & s -> gb , 8 ) ;
 len -= 8 ;
 }
 if ( len > 0 ) {
 * q ++ = ( get_bits ) ( & s -> gb , len ) << ( 8 - len ) ;
 }
 memset ( q , 0 , FF_INPUT_BUFFER_PADDING_SIZE ) ;
 init_get_bits ( & s -> gb , s -> last_superframe , s -> last_superframe_len * 8 + bit_offset ) ;
 if ( s -> last_bitoffset > 0 ) skip_bits ( & s -> gb , s -> last_bitoffset ) ;
 if ( wma_decode_frame ( s , samples , samples_offset ) < 0 ) goto fail ;
 samples_offset += s -> frame_len ;
 nb_frames -- ;
 }
 pos = bit_offset + 4 + 4 + s -> byte_offset_bits + 3 ;
 if ( pos >= MAX_CODED_SUPERFRAME_SIZE * 8 || pos > buf_size * 8 ) return AVERROR_INVALIDDATA ;
 init_get_bits ( & s -> gb , buf + ( pos >> 3 ) , ( buf_size - ( pos >> 3 ) ) * 8 ) ;
 len = pos & 7 ;
 if ( len > 0 ) skip_bits ( & s -> gb , len ) ;
 s -> reset_block_lengths = 1 ;
 for ( i = 0 ;
 i < nb_frames ;
 i ++ ) {
 if ( wma_decode_frame ( s , samples , samples_offset ) < 0 ) goto fail ;
 samples_offset += s -> frame_len ;
 }
 pos = get_bits_count ( & s -> gb ) + ( ( bit_offset + 4 + 4 + s -> byte_offset_bits + 3 ) & ~ 7 ) ;
 s -> last_bitoffset = pos & 7 ;
 pos >>= 3 ;
 len = buf_size - pos ;
 if ( len > MAX_CODED_SUPERFRAME_SIZE || len < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "len %d invalid\n" , len ) ;
 goto fail ;
 }
 s -> last_superframe_len = len ;
 memcpy ( s -> last_superframe , buf + pos , len ) ;
 }
 else {
 if ( wma_decode_frame ( s , samples , samples_offset ) < 0 ) goto fail ;
 samples_offset += s -> frame_len ;
 }
 av_dlog ( s -> avctx , "%d %d %d %d outbytes:%td eaten:%d\n" , s -> frame_len_bits , s -> block_len_bits , s -> frame_len , s -> block_len , ( int8_t * ) samples - ( int8_t * ) data , avctx -> block_align ) ;
 * got_frame_ptr = 1 ;
 return avctx -> block_align ;
 fail : s -> last_superframe_len = 0 ;
 return - 1 ;
 }