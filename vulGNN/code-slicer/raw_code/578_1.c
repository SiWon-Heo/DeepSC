static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 ProresContext * ctx = avctx -> priv_data ;
 AVFrame * picture = avctx -> coded_frame ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 int frame_hdr_size , pic_num , pic_data_size ;
 if ( buf_size < 28 || buf_size < AV_RB32 ( buf ) || AV_RB32 ( buf + 4 ) != FRAME_ID ) {
 av_log ( avctx , AV_LOG_ERROR , "invalid frame\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 MOVE_DATA_PTR ( 8 ) ;
 frame_hdr_size = decode_frame_header ( ctx , buf , buf_size , avctx ) ;
 if ( frame_hdr_size < 0 ) return AVERROR_INVALIDDATA ;
 MOVE_DATA_PTR ( frame_hdr_size ) ;
 if ( picture -> data [ 0 ] ) avctx -> release_buffer ( avctx , picture ) ;
 picture -> reference = 0 ;
 if ( ff_get_buffer ( avctx , picture ) < 0 ) return - 1 ;
 for ( pic_num = 0 ;
 ctx -> picture . interlaced_frame - pic_num + 1 ;
 pic_num ++ ) {
 pic_data_size = decode_picture_header ( ctx , buf , buf_size , avctx ) ;
 if ( pic_data_size < 0 ) return AVERROR_INVALIDDATA ;
 if ( decode_picture ( ctx , pic_num , avctx ) ) return - 1 ;
 MOVE_DATA_PTR ( pic_data_size ) ;
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = * avctx -> coded_frame ;
 return avpkt -> size ;
 }