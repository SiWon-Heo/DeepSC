static int cook_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 COOKContext * q = avctx -> priv_data ;
 float * * samples = NULL ;
 int i , ret ;
 int offset = 0 ;
 int chidx = 0 ;
 if ( buf_size < avctx -> block_align ) return buf_size ;
 if ( q -> discarded_packets >= 2 ) {
 frame -> nb_samples = q -> samples_per_channel ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 samples = ( float * * ) frame -> extended_data ;
 }
 q -> subpacket [ 0 ] . size = avctx -> block_align ;
 for ( i = 1 ;
 i < q -> num_subpackets ;
 i ++ ) {
 q -> subpacket [ i ] . size = 2 * buf [ avctx -> block_align - q -> num_subpackets + i ] ;
 q -> subpacket [ 0 ] . size -= q -> subpacket [ i ] . size + 1 ;
 if ( q -> subpacket [ 0 ] . size < 0 ) {
 av_log ( avctx , AV_LOG_DEBUG , "frame subpacket size total > avctx->block_align!\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 }
 for ( i = 0 ;
 i < q -> num_subpackets ;
 i ++ ) {
 q -> subpacket [ i ] . bits_per_subpacket = ( q -> subpacket [ i ] . size * 8 ) >> q -> subpacket [ i ] . bits_per_subpdiv ;
 q -> subpacket [ i ] . ch_idx = chidx ;
 av_log ( avctx , AV_LOG_DEBUG , "subpacket[%i] size %i js %i %i block_align %i\n" , i , q -> subpacket [ i ] . size , q -> subpacket [ i ] . joint_stereo , offset , avctx -> block_align ) ;
 if ( ( ret = decode_subpacket ( q , & q -> subpacket [ i ] , buf + offset , samples ) ) < 0 ) return ret ;
 offset += q -> subpacket [ i ] . size ;
 chidx += q -> subpacket [ i ] . num_channels ;
 av_log ( avctx , AV_LOG_DEBUG , "subpacket[%i] %i %i\n" , i , q -> subpacket [ i ] . size * 8 , get_bits_count ( & q -> gb ) ) ;
 }
 if ( q -> discarded_packets < 2 ) {
 q -> discarded_packets ++ ;
 * got_frame_ptr = 0 ;
 return avctx -> block_align ;
 }
 * got_frame_ptr = 1 ;
 return avctx -> block_align ;
 }