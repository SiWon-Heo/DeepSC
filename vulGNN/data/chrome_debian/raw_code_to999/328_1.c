static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 FourXContext * const f = avctx -> priv_data ;
 AVFrame * picture = data ;
 AVFrame * p ;
 int i , frame_4cc , frame_size , ret ;
 frame_4cc = AV_RL32 ( buf ) ;
 if ( buf_size != AV_RL32 ( buf + 4 ) + 8 || buf_size < 20 ) av_log ( f -> avctx , AV_LOG_ERROR , "size mismatch %d %d\n" , buf_size , AV_RL32 ( buf + 4 ) ) ;
 if ( frame_4cc == AV_RL32 ( "cfrm" ) ) {
 int free_index = - 1 ;
 const int data_size = buf_size - 20 ;
 const int id = AV_RL32 ( buf + 12 ) ;
 const int whole_size = AV_RL32 ( buf + 16 ) ;
 CFrameBuffer * cfrm ;
 for ( i = 0 ;
 i < CFRAME_BUFFER_COUNT ;
 i ++ ) if ( f -> cfrm [ i ] . id && f -> cfrm [ i ] . id < avctx -> frame_number ) av_log ( f -> avctx , AV_LOG_ERROR , "lost c frame %d\n" , f -> cfrm [ i ] . id ) ;
 for ( i = 0 ;
 i < CFRAME_BUFFER_COUNT ;
 i ++ ) {
 if ( f -> cfrm [ i ] . id == id ) break ;
 if ( f -> cfrm [ i ] . size == 0 ) free_index = i ;
 }
 if ( i >= CFRAME_BUFFER_COUNT ) {
 i = free_index ;
 f -> cfrm [ i ] . id = id ;
 }
 cfrm = & f -> cfrm [ i ] ;
 cfrm -> data = av_fast_realloc ( cfrm -> data , & cfrm -> allocated_size , cfrm -> size + data_size + FF_INPUT_BUFFER_PADDING_SIZE ) ;
 if ( ! cfrm -> data ) {
 av_log ( f -> avctx , AV_LOG_ERROR , "realloc failure" ) ;
 return AVERROR ( ENOMEM ) ;
 }
 memcpy ( cfrm -> data + cfrm -> size , buf + 20 , data_size ) ;
 cfrm -> size += data_size ;
 if ( cfrm -> size >= whole_size ) {
 buf = cfrm -> data ;
 frame_size = cfrm -> size ;
 if ( id != avctx -> frame_number ) av_log ( f -> avctx , AV_LOG_ERROR , "cframe id mismatch %d %d\n" , id , avctx -> frame_number ) ;
 cfrm -> size = cfrm -> id = 0 ;
 frame_4cc = AV_RL32 ( "pfrm" ) ;
 }
 else return buf_size ;
 }
 else {
 buf = buf + 12 ;
 frame_size = buf_size - 12 ;
 }
 FFSWAP ( AVFrame * , f -> current_picture , f -> last_picture ) ;
 p = f -> current_picture ;
 avctx -> coded_frame = p ;
 avctx -> flags |= CODEC_FLAG_EMU_EDGE ;
 if ( p -> data [ 0 ] ) avctx -> release_buffer ( avctx , p ) ;
 p -> reference = 1 ;
 if ( ( ret = ff_get_buffer ( avctx , p ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 if ( frame_4cc == AV_RL32 ( "ifr2" ) ) {
 p -> pict_type = AV_PICTURE_TYPE_I ;
 if ( ( ret = decode_i2_frame ( f , buf - 4 , frame_size + 4 ) ) < 0 ) return ret ;
 }
 else if ( frame_4cc == AV_RL32 ( "ifrm" ) ) {
 p -> pict_type = AV_PICTURE_TYPE_I ;
 if ( ( ret = decode_i_frame ( f , buf , frame_size ) ) < 0 ) return ret ;
 }
 else if ( frame_4cc == AV_RL32 ( "pfrm" ) || frame_4cc == AV_RL32 ( "pfr2" ) ) {
 if ( ! f -> last_picture -> data [ 0 ] ) {
 f -> last_picture -> reference = 1 ;
 if ( ( ret = ff_get_buffer ( avctx , f -> last_picture ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 memset ( f -> last_picture -> data [ 0 ] , 0 , avctx -> height * FFABS ( f -> last_picture -> linesize [ 0 ] ) ) ;
 }
 p -> pict_type = AV_PICTURE_TYPE_P ;
 if ( ( ret = decode_p_frame ( f , buf , frame_size ) ) < 0 ) return ret ;
 }
 else if ( frame_4cc == AV_RL32 ( "snd_" ) ) {
 av_log ( avctx , AV_LOG_ERROR , "ignoring snd_ chunk length:%d\n" , buf_size ) ;
 }
 else {
 av_log ( avctx , AV_LOG_ERROR , "ignoring unknown chunk length:%d\n" , buf_size ) ;
 }
 p -> key_frame = p -> pict_type == AV_PICTURE_TYPE_I ;
 * picture = * p ;
 * got_frame = 1 ;
 emms_c ( ) ;
 return buf_size ;
 }