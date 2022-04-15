static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 RALFContext * ctx = avctx -> priv_data ;
 AVFrame * frame = data ;
 int16_t * samples0 ;
 int16_t * samples1 ;
 int ret ;
 GetBitContext gb ;
 int table_size , table_bytes , i ;
 const uint8_t * src , * block_pointer ;
 int src_size ;
 int bytes_left ;
 if ( ctx -> has_pkt ) {
 ctx -> has_pkt = 0 ;
 table_bytes = ( AV_RB16 ( avpkt -> data ) + 7 ) >> 3 ;
 if ( table_bytes + 3 > avpkt -> size || avpkt -> size > RALF_MAX_PKT_SIZE ) {
 av_log ( avctx , AV_LOG_ERROR , "Wrong packet's breath smells of wrong data!\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( memcmp ( ctx -> pkt , avpkt -> data , 2 + table_bytes ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Wrong packet tails are wrong!\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 src = ctx -> pkt ;
 src_size = RALF_MAX_PKT_SIZE + avpkt -> size ;
 memcpy ( ctx -> pkt + RALF_MAX_PKT_SIZE , avpkt -> data + 2 + table_bytes , avpkt -> size - 2 - table_bytes ) ;
 }
 else {
 if ( avpkt -> size == RALF_MAX_PKT_SIZE ) {
 memcpy ( ctx -> pkt , avpkt -> data , avpkt -> size ) ;
 ctx -> has_pkt = 1 ;
 * got_frame_ptr = 0 ;
 return avpkt -> size ;
 }
 src = avpkt -> data ;
 src_size = avpkt -> size ;
 }
 frame -> nb_samples = ctx -> max_frame_size ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "Me fail get_buffer()? That's unpossible!\n" ) ;
 return ret ;
 }
 samples0 = ( int16_t * ) frame -> data [ 0 ] ;
 samples1 = ( int16_t * ) frame -> data [ 1 ] ;
 if ( src_size < 5 ) {
 av_log ( avctx , AV_LOG_ERROR , "too short packets are too short!\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 table_size = AV_RB16 ( src ) ;
 table_bytes = ( table_size + 7 ) >> 3 ;
 if ( src_size < table_bytes + 3 ) {
 av_log ( avctx , AV_LOG_ERROR , "short packets are short!\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 init_get_bits ( & gb , src + 2 , table_size ) ;
 ctx -> num_blocks = 0 ;
 while ( get_bits_left ( & gb ) > 0 ) {
 ctx -> block_size [ ctx -> num_blocks ] = get_bits ( & gb , 15 ) ;
 if ( get_bits1 ( & gb ) ) {
 ctx -> block_pts [ ctx -> num_blocks ] = get_bits ( & gb , 9 ) ;
 }
 else {
 ctx -> block_pts [ ctx -> num_blocks ] = 0 ;
 }
 ctx -> num_blocks ++ ;
 }
 block_pointer = src + table_bytes + 2 ;
 bytes_left = src_size - table_bytes - 2 ;
 ctx -> sample_offset = 0 ;
 for ( i = 0 ;
 i < ctx -> num_blocks ;
 i ++ ) {
 if ( bytes_left < ctx -> block_size [ i ] ) {
 av_log ( avctx , AV_LOG_ERROR , "I'm pedaling backwards\n" ) ;
 break ;
 }
 init_get_bits ( & gb , block_pointer , ctx -> block_size [ i ] * 8 ) ;
 if ( decode_block ( avctx , & gb , samples0 + ctx -> sample_offset , samples1 + ctx -> sample_offset ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "Sir, I got carsick in your office. Not decoding the rest of packet.\n" ) ;
 break ;
 }
 block_pointer += ctx -> block_size [ i ] ;
 bytes_left -= ctx -> block_size [ i ] ;
 }
 frame -> nb_samples = ctx -> sample_offset ;
 * got_frame_ptr = ctx -> sample_offset > 0 ;
 return avpkt -> size ;
 }