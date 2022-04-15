static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 FrapsContext * const s = avctx -> priv_data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 AVFrame * frame = data ;
 AVFrame * const f = & s -> frame ;
 uint32_t header ;
 unsigned int version , header_size ;
 unsigned int x , y ;
 const uint32_t * buf32 ;
 uint32_t * luma1 , * luma2 , * cb , * cr ;
 uint32_t offs [ 4 ] ;
 int i , j , ret , is_chroma , planes ;
 enum AVPixelFormat pix_fmt ;
 header = AV_RL32 ( buf ) ;
 version = header & 0xff ;
 header_size = ( header & ( 1 << 30 ) ) ? 8 : 4 ;
 if ( version > 5 ) {
 av_log ( avctx , AV_LOG_ERROR , "This file is encoded with Fraps version %d. " \ "This codec can only decode versions <= 5.\n" , version ) ;
 return AVERROR_PATCHWELCOME ;
 }
 buf += 4 ;
 if ( header_size == 8 ) buf += 4 ;
 pix_fmt = version & 1 ? AV_PIX_FMT_BGR24 : AV_PIX_FMT_YUVJ420P ;
 if ( avctx -> pix_fmt != pix_fmt && f -> data [ 0 ] ) {
 avctx -> release_buffer ( avctx , f ) ;
 }
 avctx -> pix_fmt = pix_fmt ;
 switch ( version ) {
 case 0 : default : if ( ( buf_size != avctx -> width * avctx -> height * 3 / 2 + header_size ) && ( buf_size != header_size ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid frame length %d (should be %d)\n" , buf_size , avctx -> width * avctx -> height * 3 / 2 + header_size ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ( ( avctx -> width % 8 ) != 0 ) || ( ( avctx -> height % 2 ) != 0 ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid frame size %dx%d\n" , avctx -> width , avctx -> height ) ;
 return AVERROR_INVALIDDATA ;
 }
 f -> reference = 1 ;
 f -> buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( ( ret = avctx -> reget_buffer ( avctx , f ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "reget_buffer() failed\n" ) ;
 return ret ;
 }
 f -> pict_type = ( header & ( 1U << 31 ) ) ? AV_PICTURE_TYPE_P : AV_PICTURE_TYPE_I ;
 f -> key_frame = f -> pict_type == AV_PICTURE_TYPE_I ;
 if ( f -> pict_type == AV_PICTURE_TYPE_I ) {
 buf32 = ( const uint32_t * ) buf ;
 for ( y = 0 ;
 y < avctx -> height / 2 ;
 y ++ ) {
 luma1 = ( uint32_t * ) & f -> data [ 0 ] [ y * 2 * f -> linesize [ 0 ] ] ;
 luma2 = ( uint32_t * ) & f -> data [ 0 ] [ ( y * 2 + 1 ) * f -> linesize [ 0 ] ] ;
 cr = ( uint32_t * ) & f -> data [ 1 ] [ y * f -> linesize [ 1 ] ] ;
 cb = ( uint32_t * ) & f -> data [ 2 ] [ y * f -> linesize [ 2 ] ] ;
 for ( x = 0 ;
 x < avctx -> width ;
 x += 8 ) {
 * ( luma1 ++ ) = * ( buf32 ++ ) ;
 * ( luma1 ++ ) = * ( buf32 ++ ) ;
 * ( luma2 ++ ) = * ( buf32 ++ ) ;
 * ( luma2 ++ ) = * ( buf32 ++ ) ;
 * ( cr ++ ) = * ( buf32 ++ ) ;
 * ( cb ++ ) = * ( buf32 ++ ) ;
 }
 }
 }
 break ;
 case 1 : if ( ( buf_size != avctx -> width * avctx -> height * 3 + header_size ) && ( buf_size != header_size ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid frame length %d (should be %d)\n" , buf_size , avctx -> width * avctx -> height * 3 + header_size ) ;
 return AVERROR_INVALIDDATA ;
 }
 f -> reference = 1 ;
 f -> buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( ( ret = avctx -> reget_buffer ( avctx , f ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "reget_buffer() failed\n" ) ;
 return ret ;
 }
 f -> pict_type = ( header & ( 1U << 31 ) ) ? AV_PICTURE_TYPE_P : AV_PICTURE_TYPE_I ;
 f -> key_frame = f -> pict_type == AV_PICTURE_TYPE_I ;
 if ( f -> pict_type == AV_PICTURE_TYPE_I ) {
 for ( y = 0 ;
 y < avctx -> height ;
 y ++ ) memcpy ( & f -> data [ 0 ] [ ( avctx -> height - y - 1 ) * f -> linesize [ 0 ] ] , & buf [ y * avctx -> width * 3 ] , 3 * avctx -> width ) ;
 }
 break ;
 case 2 : case 4 : planes = 3 ;
 f -> reference = 1 ;
 f -> buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( ( ret = avctx -> reget_buffer ( avctx , f ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "reget_buffer() failed\n" ) ;
 return ret ;
 }
 if ( buf_size == 8 ) {
 f -> pict_type = AV_PICTURE_TYPE_P ;
 f -> key_frame = 0 ;
 break ;
 }
 f -> pict_type = AV_PICTURE_TYPE_I ;
 f -> key_frame = 1 ;
 if ( ( AV_RL32 ( buf ) != FPS_TAG ) || ( buf_size < ( planes * 1024 + 24 ) ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Fraps: error in data stream\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 for ( i = 0 ;
 i < planes ;
 i ++ ) {
 offs [ i ] = AV_RL32 ( buf + 4 + i * 4 ) ;
 if ( offs [ i ] >= buf_size || ( i && offs [ i ] <= offs [ i - 1 ] + 1024 ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Fraps: plane %i offset is out of bounds\n" , i ) ;
 return AVERROR_INVALIDDATA ;
 }
 }
 offs [ planes ] = buf_size ;
 for ( i = 0 ;
 i < planes ;
 i ++ ) {
 is_chroma = ! ! i ;
 av_fast_padded_malloc ( & s -> tmpbuf , & s -> tmpbuf_size , offs [ i + 1 ] - offs [ i ] - 1024 ) ;
 if ( ! s -> tmpbuf ) return AVERROR ( ENOMEM ) ;
 if ( ( ret = fraps2_decode_plane ( s , f -> data [ i ] , f -> linesize [ i ] , avctx -> width >> is_chroma , avctx -> height >> is_chroma , buf + offs [ i ] , offs [ i + 1 ] - offs [ i ] , is_chroma , 1 ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "Error decoding plane %i\n" , i ) ;
 return ret ;
 }
 }
 break ;
 case 3 : case 5 : planes = 3 ;
 f -> reference = 1 ;
 f -> buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( ( ret = avctx -> reget_buffer ( avctx , f ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "reget_buffer() failed\n" ) ;
 return ret ;
 }
 if ( buf_size == 8 ) {
 f -> pict_type = AV_PICTURE_TYPE_P ;
 f -> key_frame = 0 ;
 break ;
 }
 f -> pict_type = AV_PICTURE_TYPE_I ;
 f -> key_frame = 1 ;
 if ( ( AV_RL32 ( buf ) != FPS_TAG ) || ( buf_size < ( planes * 1024 + 24 ) ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Fraps: error in data stream\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 for ( i = 0 ;
 i < planes ;
 i ++ ) {
 offs [ i ] = AV_RL32 ( buf + 4 + i * 4 ) ;
 if ( offs [ i ] >= buf_size || ( i && offs [ i ] <= offs [ i - 1 ] + 1024 ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Fraps: plane %i offset is out of bounds\n" , i ) ;
 return AVERROR_INVALIDDATA ;
 }
 }
 offs [ planes ] = buf_size ;
 for ( i = 0 ;
 i < planes ;
 i ++ ) {
 av_fast_padded_malloc ( & s -> tmpbuf , & s -> tmpbuf_size , offs [ i + 1 ] - offs [ i ] - 1024 ) ;
 if ( ! s -> tmpbuf ) return AVERROR ( ENOMEM ) ;
 if ( ( ret = fraps2_decode_plane ( s , f -> data [ 0 ] + i + ( f -> linesize [ 0 ] * ( avctx -> height - 1 ) ) , - f -> linesize [ 0 ] , avctx -> width , avctx -> height , buf + offs [ i ] , offs [ i + 1 ] - offs [ i ] , 0 , 3 ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "Error decoding plane %i\n" , i ) ;
 return ret ;
 }
 }
 for ( j = 0 ;
 j < avctx -> height ;
 j ++ ) {
 for ( i = 0 ;
 i < avctx -> width ;
 i ++ ) {
 f -> data [ 0 ] [ 0 + i * 3 + j * f -> linesize [ 0 ] ] += f -> data [ 0 ] [ 1 + i * 3 + j * f -> linesize [ 0 ] ] ;
 f -> data [ 0 ] [ 2 + i * 3 + j * f -> linesize [ 0 ] ] += f -> data [ 0 ] [ 1 + i * 3 + j * f -> linesize [ 0 ] ] ;
 }
 }
 break ;
 }
 * frame = * f ;
 * got_frame = 1 ;
 return buf_size ;
 }