static int xan_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int ret , buf_size = avpkt -> size ;
 XanContext * s = avctx -> priv_data ;
 GetByteContext ctx ;
 int tag = 0 ;
 bytestream2_init ( & ctx , buf , buf_size ) ;
 while ( bytestream2_get_bytes_left ( & ctx ) > 8 && tag != VGA__TAG ) {
 unsigned * tmpptr ;
 uint32_t new_pal ;
 int size ;
 int i ;
 tag = bytestream2_get_le32 ( & ctx ) ;
 size = bytestream2_get_be32 ( & ctx ) ;
 size = FFMIN ( size , bytestream2_get_bytes_left ( & ctx ) ) ;
 switch ( tag ) {
 case PALT_TAG : if ( size < PALETTE_SIZE ) return AVERROR_INVALIDDATA ;
 if ( s -> palettes_count >= PALETTES_MAX ) return AVERROR_INVALIDDATA ;
 tmpptr = av_realloc ( s -> palettes , ( s -> palettes_count + 1 ) * AVPALETTE_SIZE ) ;
 if ( ! tmpptr ) return AVERROR ( ENOMEM ) ;
 s -> palettes = tmpptr ;
 tmpptr += s -> palettes_count * AVPALETTE_COUNT ;
 for ( i = 0 ;
 i < PALETTE_COUNT ;
 i ++ ) {
 # if RUNTIME_GAMMA int r = gamma_corr ( bytestream2_get_byteu ( & ctx ) ) ;
 int g = gamma_corr ( bytestream2_get_byteu ( & ctx ) ) ;
 int b = gamma_corr ( bytestream2_get_byteu ( & ctx ) ) ;
 # else int r = gamma_lookup [ bytestream2_get_byteu ( & ctx ) ] ;
 int g = gamma_lookup [ bytestream2_get_byteu ( & ctx ) ] ;
 int b = gamma_lookup [ bytestream2_get_byteu ( & ctx ) ] ;
 # endif * tmpptr ++ = ( r << 16 ) | ( g << 8 ) | b ;
 }
 s -> palettes_count ++ ;
 break ;
 case SHOT_TAG : if ( size < 4 ) return AVERROR_INVALIDDATA ;
 new_pal = bytestream2_get_le32 ( & ctx ) ;
 if ( new_pal < s -> palettes_count ) {
 s -> cur_palette = new_pal ;
 }
 else av_log ( avctx , AV_LOG_ERROR , "Invalid palette selected\n" ) ;
 break ;
 case VGA__TAG : break ;
 default : bytestream2_skip ( & ctx , size ) ;
 break ;
 }
 }
 buf_size = bytestream2_get_bytes_left ( & ctx ) ;
 if ( s -> palettes_count <= 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "No palette found\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ( ret = ff_get_buffer ( avctx , & s -> current_frame ) ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 s -> current_frame . reference = 3 ;
 if ( ! s -> frame_size ) s -> frame_size = s -> current_frame . linesize [ 0 ] * s -> avctx -> height ;
 memcpy ( s -> current_frame . data [ 1 ] , s -> palettes + s -> cur_palette * AVPALETTE_COUNT , AVPALETTE_SIZE ) ;
 s -> buf = ctx . buffer ;
 s -> size = buf_size ;
 if ( xan_wc3_decode_frame ( s ) < 0 ) return AVERROR_INVALIDDATA ;
 if ( s -> last_frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> last_frame ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> current_frame ;
 FFSWAP ( AVFrame , s -> current_frame , s -> last_frame ) ;
 return buf_size ;
 }