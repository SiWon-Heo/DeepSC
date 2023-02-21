static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 NuvContext * c = avctx -> priv_data ;
 AVFrame * picture = data ;
 int orig_size = buf_size ;
 int keyframe ;
 int result , init_frame = ! avctx -> frame_number ;
 enum {
 NUV_UNCOMPRESSED = '0' , NUV_RTJPEG = '1' , NUV_RTJPEG_IN_LZO = '2' , NUV_LZO = '3' , NUV_BLACK = 'N' , NUV_COPY_LAST = 'L' }
 comptype ;
 if ( buf_size < 12 ) {
 av_log ( avctx , AV_LOG_ERROR , "coded frame too small\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( buf [ 0 ] == 'D' && buf [ 1 ] == 'R' ) {
 int ret ;
 buf = & buf [ 12 ] ;
 buf_size -= 12 ;
 ret = get_quant ( avctx , c , buf , buf_size ) ;
 if ( ret < 0 ) return ret ;
 ff_rtjpeg_decode_init ( & c -> rtj , & c -> dsp , c -> width , c -> height , c -> lq , c -> cq ) ;
 return orig_size ;
 }
 if ( buf [ 0 ] != 'V' || buf_size < 12 ) {
 av_log ( avctx , AV_LOG_ERROR , "not a nuv video frame\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 comptype = buf [ 1 ] ;
 switch ( comptype ) {
 case NUV_RTJPEG_IN_LZO : case NUV_RTJPEG : keyframe = ! buf [ 2 ] ;
 break ;
 case NUV_COPY_LAST : keyframe = 0 ;
 break ;
 default : keyframe = 1 ;
 break ;
 }
 buf = & buf [ 12 ] ;
 buf_size -= 12 ;
 if ( comptype == NUV_RTJPEG_IN_LZO || comptype == NUV_LZO ) {
 int outlen = c -> decomp_size , inlen = buf_size ;
 if ( av_lzo1x_decode ( c -> decomp_buf , & outlen , buf , & inlen ) ) av_log ( avctx , AV_LOG_ERROR , "error during lzo decompression\n" ) ;
 buf = c -> decomp_buf ;
 buf_size = c -> decomp_size ;
 }
 if ( c -> codec_frameheader ) {
 int w , h , q ;
 if ( buf_size < RTJPEG_HEADER_SIZE || buf [ 4 ] != RTJPEG_HEADER_SIZE || buf [ 5 ] != RTJPEG_FILE_VERSION ) {
 av_log ( avctx , AV_LOG_ERROR , "invalid nuv video frame\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 w = AV_RL16 ( & buf [ 6 ] ) ;
 h = AV_RL16 ( & buf [ 8 ] ) ;
 q = buf [ 10 ] ;
 if ( ( result = codec_reinit ( avctx , w , h , q ) ) < 0 ) return result ;
 buf = & buf [ RTJPEG_HEADER_SIZE ] ;
 buf_size -= RTJPEG_HEADER_SIZE ;
 }
 if ( keyframe && c -> pic . data [ 0 ] ) {
 avctx -> release_buffer ( avctx , & c -> pic ) ;
 init_frame = 1 ;
 }
 c -> pic . reference = 3 ;
 c -> pic . buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_READABLE | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 result = avctx -> reget_buffer ( avctx , & c -> pic ) ;
 if ( result < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return result ;
 }
 if ( init_frame ) {
 memset ( c -> pic . data [ 0 ] , 0 , avctx -> height * c -> pic . linesize [ 0 ] ) ;
 memset ( c -> pic . data [ 1 ] , 0x80 , avctx -> height * c -> pic . linesize [ 1 ] / 2 ) ;
 memset ( c -> pic . data [ 2 ] , 0x80 , avctx -> height * c -> pic . linesize [ 2 ] / 2 ) ;
 }
 c -> pic . pict_type = keyframe ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_P ;
 c -> pic . key_frame = keyframe ;
 switch ( comptype ) {
 case NUV_LZO : case NUV_UNCOMPRESSED : {
 int height = c -> height ;
 if ( buf_size < c -> width * height * 3 / 2 ) {
 av_log ( avctx , AV_LOG_ERROR , "uncompressed frame too short\n" ) ;
 height = buf_size / c -> width / 3 * 2 ;
 }
 copy_frame ( & c -> pic , buf , c -> width , height ) ;
 break ;
 }
 case NUV_RTJPEG_IN_LZO : case NUV_RTJPEG : ff_rtjpeg_decode_frame_yuv420 ( & c -> rtj , & c -> pic , buf , buf_size ) ;
 break ;
 case NUV_BLACK : memset ( c -> pic . data [ 0 ] , 0 , c -> width * c -> height ) ;
 memset ( c -> pic . data [ 1 ] , 128 , c -> width * c -> height / 4 ) ;
 memset ( c -> pic . data [ 2 ] , 128 , c -> width * c -> height / 4 ) ;
 break ;
 case NUV_COPY_LAST : break ;
 default : av_log ( avctx , AV_LOG_ERROR , "unknown compression\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 * picture = c -> pic ;
 * got_frame = 1 ;
 return orig_size ;
 }