static int cyuv_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 CyuvDecodeContext * s = avctx -> priv_data ;
 unsigned char * y_plane ;
 unsigned char * u_plane ;
 unsigned char * v_plane ;
 int y_ptr ;
 int u_ptr ;
 int v_ptr ;
 const signed char * y_table = ( const signed char * ) buf + 0 ;
 const signed char * u_table = ( const signed char * ) buf + 16 ;
 const signed char * v_table = ( const signed char * ) buf + 32 ;
 unsigned char y_pred , u_pred , v_pred ;
 int stream_ptr ;
 unsigned char cur_byte ;
 int pixel_groups ;
 int ret ;
 if ( avctx -> codec_id == AV_CODEC_ID_AURA ) {
 y_table = u_table ;
 u_table = v_table ;
 }
 if ( buf_size != 48 + s -> height * ( s -> width * 3 / 4 ) ) {
 av_log ( avctx , AV_LOG_ERROR , "got a buffer with %d bytes when %d were expected\n" , buf_size , 48 + s -> height * ( s -> width * 3 / 4 ) ) ;
 return AVERROR_INVALIDDATA ;
 }
 stream_ptr = 48 ;
 if ( s -> frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> frame ) ;
 s -> frame . buffer_hints = FF_BUFFER_HINTS_VALID ;
 s -> frame . reference = 0 ;
 if ( ( ret = ff_get_buffer ( avctx , & s -> frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 y_plane = s -> frame . data [ 0 ] ;
 u_plane = s -> frame . data [ 1 ] ;
 v_plane = s -> frame . data [ 2 ] ;
 for ( y_ptr = 0 , u_ptr = 0 , v_ptr = 0 ;
 y_ptr < ( s -> height * s -> frame . linesize [ 0 ] ) ;
 y_ptr += s -> frame . linesize [ 0 ] - s -> width , u_ptr += s -> frame . linesize [ 1 ] - s -> width / 4 , v_ptr += s -> frame . linesize [ 2 ] - s -> width / 4 ) {
 cur_byte = buf [ stream_ptr ++ ] ;
 u_plane [ u_ptr ++ ] = u_pred = cur_byte & 0xF0 ;
 y_plane [ y_ptr ++ ] = y_pred = ( cur_byte & 0x0F ) << 4 ;
 cur_byte = buf [ stream_ptr ++ ] ;
 v_plane [ v_ptr ++ ] = v_pred = cur_byte & 0xF0 ;
 y_pred += y_table [ cur_byte & 0x0F ] ;
 y_plane [ y_ptr ++ ] = y_pred ;
 cur_byte = buf [ stream_ptr ++ ] ;
 y_pred += y_table [ cur_byte & 0x0F ] ;
 y_plane [ y_ptr ++ ] = y_pred ;
 y_pred += y_table [ ( cur_byte & 0xF0 ) >> 4 ] ;
 y_plane [ y_ptr ++ ] = y_pred ;
 pixel_groups = s -> width / 4 - 1 ;
 while ( pixel_groups -- ) {
 cur_byte = buf [ stream_ptr ++ ] ;
 u_pred += u_table [ ( cur_byte & 0xF0 ) >> 4 ] ;
 u_plane [ u_ptr ++ ] = u_pred ;
 y_pred += y_table [ cur_byte & 0x0F ] ;
 y_plane [ y_ptr ++ ] = y_pred ;
 cur_byte = buf [ stream_ptr ++ ] ;
 v_pred += v_table [ ( cur_byte & 0xF0 ) >> 4 ] ;
 v_plane [ v_ptr ++ ] = v_pred ;
 y_pred += y_table [ cur_byte & 0x0F ] ;
 y_plane [ y_ptr ++ ] = y_pred ;
 cur_byte = buf [ stream_ptr ++ ] ;
 y_pred += y_table [ cur_byte & 0x0F ] ;
 y_plane [ y_ptr ++ ] = y_pred ;
 y_pred += y_table [ ( cur_byte & 0xF0 ) >> 4 ] ;
 y_plane [ y_ptr ++ ] = y_pred ;
 }
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> frame ;
 return buf_size ;
 }