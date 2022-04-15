static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 ASV1Context * const a = avctx -> priv_data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 AVFrame * picture = data ;
 AVFrame * const p = & a -> picture ;
 int mb_x , mb_y , ret ;
 if ( p -> data [ 0 ] ) avctx -> release_buffer ( avctx , p ) ;
 p -> reference = 0 ;
 if ( ( ret = ff_get_buffer ( avctx , p ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 p -> pict_type = AV_PICTURE_TYPE_I ;
 p -> key_frame = 1 ;
 av_fast_padded_malloc ( & a -> bitstream_buffer , & a -> bitstream_buffer_size , buf_size ) ;
 if ( ! a -> bitstream_buffer ) return AVERROR ( ENOMEM ) ;
 if ( avctx -> codec_id == AV_CODEC_ID_ASV1 ) a -> dsp . bswap_buf ( ( uint32_t * ) a -> bitstream_buffer , ( const uint32_t * ) buf , buf_size / 4 ) ;
 else {
 int i ;
 for ( i = 0 ;
 i < buf_size ;
 i ++ ) a -> bitstream_buffer [ i ] = ff_reverse [ buf [ i ] ] ;
 }
 init_get_bits ( & a -> gb , a -> bitstream_buffer , buf_size * 8 ) ;
 for ( mb_y = 0 ;
 mb_y < a -> mb_height2 ;
 mb_y ++ ) {
 for ( mb_x = 0 ;
 mb_x < a -> mb_width2 ;
 mb_x ++ ) {
 if ( ( ret = decode_mb ( a , a -> block ) ) < 0 ) return ret ;
 idct_put ( a , mb_x , mb_y ) ;
 }
 }
 if ( a -> mb_width2 != a -> mb_width ) {
 mb_x = a -> mb_width2 ;
 for ( mb_y = 0 ;
 mb_y < a -> mb_height2 ;
 mb_y ++ ) {
 if ( ( ret = decode_mb ( a , a -> block ) ) < 0 ) return ret ;
 idct_put ( a , mb_x , mb_y ) ;
 }
 }
 if ( a -> mb_height2 != a -> mb_height ) {
 mb_y = a -> mb_height2 ;
 for ( mb_x = 0 ;
 mb_x < a -> mb_width ;
 mb_x ++ ) {
 if ( ( ret = decode_mb ( a , a -> block ) ) < 0 ) return ret ;
 idct_put ( a , mb_x , mb_y ) ;
 }
 }
 * picture = a -> picture ;
 * got_frame = 1 ;
 emms_c ( ) ;
 return ( get_bits_count ( & a -> gb ) + 31 ) / 32 * 4 ;
 }