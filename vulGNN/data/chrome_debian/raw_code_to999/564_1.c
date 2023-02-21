static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 CamStudioContext * c = avctx -> priv_data ;
 AVFrame * picture = data ;
 int ret ;
 if ( buf_size < 2 ) {
 av_log ( avctx , AV_LOG_ERROR , "coded frame too small\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( c -> pic . data [ 0 ] ) avctx -> release_buffer ( avctx , & c -> pic ) ;
 c -> pic . reference = 1 ;
 c -> pic . buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_READABLE | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( ( ret = ff_get_buffer ( avctx , & c -> pic ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 switch ( ( buf [ 0 ] >> 1 ) & 7 ) {
 case 0 : {
 int outlen = c -> decomp_size , inlen = buf_size - 2 ;
 if ( av_lzo1x_decode ( c -> decomp_buf , & outlen , & buf [ 2 ] , & inlen ) ) av_log ( avctx , AV_LOG_ERROR , "error during lzo decompression\n" ) ;
 break ;
 }
 case 1 : {
 # if CONFIG_ZLIB unsigned long dlen = c -> decomp_size ;
 if ( uncompress ( c -> decomp_buf , & dlen , & buf [ 2 ] , buf_size - 2 ) != Z_OK ) av_log ( avctx , AV_LOG_ERROR , "error during zlib decompression\n" ) ;
 break ;
 # else av_log ( avctx , AV_LOG_ERROR , "compiled without zlib support\n" ) ;
 return AVERROR ( ENOSYS ) ;
 # endif }
 default : av_log ( avctx , AV_LOG_ERROR , "unknown compression\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( buf [ 0 ] & 1 ) {
 c -> pic . pict_type = AV_PICTURE_TYPE_I ;
 c -> pic . key_frame = 1 ;
 switch ( c -> bpp ) {
 case 16 : copy_frame_16 ( & c -> pic , c -> decomp_buf , c -> linelen , c -> height ) ;
 break ;
 case 32 : copy_frame_32 ( & c -> pic , c -> decomp_buf , c -> linelen , c -> height ) ;
 break ;
 default : copy_frame_default ( & c -> pic , c -> decomp_buf , FFALIGN ( c -> linelen , 4 ) , c -> linelen , c -> height ) ;
 }
 }
 else {
 c -> pic . pict_type = AV_PICTURE_TYPE_P ;
 c -> pic . key_frame = 0 ;
 switch ( c -> bpp ) {
 case 16 : add_frame_16 ( & c -> pic , c -> decomp_buf , c -> linelen , c -> height ) ;
 break ;
 case 32 : add_frame_32 ( & c -> pic , c -> decomp_buf , c -> linelen , c -> height ) ;
 break ;
 default : add_frame_default ( & c -> pic , c -> decomp_buf , FFALIGN ( c -> linelen , 4 ) , c -> linelen , c -> height ) ;
 }
 }
 * picture = c -> pic ;
 * got_frame = 1 ;
 return buf_size ;
 }