static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 VmncContext * const c = avctx -> priv_data ;
 uint8_t * outptr ;
 const uint8_t * src = buf ;
 int dx , dy , w , h , depth , enc , chunks , res , size_left ;
 c -> pic . reference = 1 ;
 c -> pic . buffer_hints = FF_BUFFER_HINTS_VALID | FF_BUFFER_HINTS_PRESERVE | FF_BUFFER_HINTS_REUSABLE ;
 if ( avctx -> reget_buffer ( avctx , & c -> pic ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "reget_buffer() failed\n" ) ;
 return - 1 ;
 }
 c -> pic . key_frame = 0 ;
 c -> pic . pict_type = AV_PICTURE_TYPE_P ;
 if ( c -> screendta ) {
 int i ;
 w = c -> cur_w ;
 if ( c -> width < c -> cur_x + w ) w = c -> width - c -> cur_x ;
 h = c -> cur_h ;
 if ( c -> height < c -> cur_y + h ) h = c -> height - c -> cur_y ;
 dx = c -> cur_x ;
 if ( dx < 0 ) {
 w += dx ;
 dx = 0 ;
 }
 dy = c -> cur_y ;
 if ( dy < 0 ) {
 h += dy ;
 dy = 0 ;
 }
 if ( ( w > 0 ) && ( h > 0 ) ) {
 outptr = c -> pic . data [ 0 ] + dx * c -> bpp2 + dy * c -> pic . linesize [ 0 ] ;
 for ( i = 0 ;
 i < h ;
 i ++ ) {
 memcpy ( outptr , c -> screendta + i * c -> cur_w * c -> bpp2 , w * c -> bpp2 ) ;
 outptr += c -> pic . linesize [ 0 ] ;
 }
 }
 }
 src += 2 ;
 chunks = AV_RB16 ( src ) ;
 src += 2 ;
 while ( chunks -- ) {
 dx = AV_RB16 ( src ) ;
 src += 2 ;
 dy = AV_RB16 ( src ) ;
 src += 2 ;
 w = AV_RB16 ( src ) ;
 src += 2 ;
 h = AV_RB16 ( src ) ;
 src += 2 ;
 enc = AV_RB32 ( src ) ;
 src += 4 ;
 outptr = c -> pic . data [ 0 ] + dx * c -> bpp2 + dy * c -> pic . linesize [ 0 ] ;
 size_left = buf_size - ( src - buf ) ;
 switch ( enc ) {
 case MAGIC_WMVd : if ( size_left < 2 + w * h * c -> bpp2 * 2 ) {
 av_log ( avctx , AV_LOG_ERROR , "Premature end of data! (need %i got %i)\n" , 2 + w * h * c -> bpp2 * 2 , size_left ) ;
 return - 1 ;
 }
 src += 2 ;
 c -> cur_w = w ;
 c -> cur_h = h ;
 c -> cur_hx = dx ;
 c -> cur_hy = dy ;
 if ( ( c -> cur_hx > c -> cur_w ) || ( c -> cur_hy > c -> cur_h ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Cursor hot spot is not in image: %ix%i of %ix%i cursor size\n" , c -> cur_hx , c -> cur_hy , c -> cur_w , c -> cur_h ) ;
 c -> cur_hx = c -> cur_hy = 0 ;
 }
 c -> curbits = av_realloc ( c -> curbits , c -> cur_w * c -> cur_h * c -> bpp2 ) ;
 c -> curmask = av_realloc ( c -> curmask , c -> cur_w * c -> cur_h * c -> bpp2 ) ;
 c -> screendta = av_realloc ( c -> screendta , c -> cur_w * c -> cur_h * c -> bpp2 ) ;
 load_cursor ( c , src ) ;
 src += w * h * c -> bpp2 * 2 ;
 break ;
 case MAGIC_WMVe : src += 2 ;
 break ;
 case MAGIC_WMVf : c -> cur_x = dx - c -> cur_hx ;
 c -> cur_y = dy - c -> cur_hy ;
 break ;
 case MAGIC_WMVg : src += 10 ;
 break ;
 case MAGIC_WMVh : src += 4 ;
 break ;
 case MAGIC_WMVi : c -> pic . key_frame = 1 ;
 c -> pic . pict_type = AV_PICTURE_TYPE_I ;
 depth = * src ++ ;
 if ( depth != c -> bpp ) {
 av_log ( avctx , AV_LOG_INFO , "Depth mismatch. Container %i bpp, Frame data: %i bpp\n" , c -> bpp , depth ) ;
 }
 src ++ ;
 c -> bigendian = * src ++ ;
 if ( c -> bigendian & ( ~ 1 ) ) {
 av_log ( avctx , AV_LOG_INFO , "Invalid header: bigendian flag = %i\n" , c -> bigendian ) ;
 return - 1 ;
 }
 src += 13 ;
 break ;
 case MAGIC_WMVj : src += 2 ;
 break ;
 case 0x00000000 : if ( ( dx + w > c -> width ) || ( dy + h > c -> height ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Incorrect frame size: %ix%i+%ix%i of %ix%i\n" , w , h , dx , dy , c -> width , c -> height ) ;
 return - 1 ;
 }
 if ( size_left < w * h * c -> bpp2 ) {
 av_log ( avctx , AV_LOG_ERROR , "Premature end of data! (need %i got %i)\n" , w * h * c -> bpp2 , size_left ) ;
 return - 1 ;
 }
 paint_raw ( outptr , w , h , src , c -> bpp2 , c -> bigendian , c -> pic . linesize [ 0 ] ) ;
 src += w * h * c -> bpp2 ;
 break ;
 case 0x00000005 : if ( ( dx + w > c -> width ) || ( dy + h > c -> height ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Incorrect frame size: %ix%i+%ix%i of %ix%i\n" , w , h , dx , dy , c -> width , c -> height ) ;
 return - 1 ;
 }
 res = decode_hextile ( c , outptr , src , size_left , w , h , c -> pic . linesize [ 0 ] ) ;
 if ( res < 0 ) return - 1 ;
 src += res ;
 break ;
 default : av_log ( avctx , AV_LOG_ERROR , "Unsupported block type 0x%08X\n" , enc ) ;
 chunks = 0 ;
 }
 }
 if ( c -> screendta ) {
 int i ;
 w = c -> cur_w ;
 if ( c -> width < c -> cur_x + w ) w = c -> width - c -> cur_x ;
 h = c -> cur_h ;
 if ( c -> height < c -> cur_y + h ) h = c -> height - c -> cur_y ;
 dx = c -> cur_x ;
 if ( dx < 0 ) {
 w += dx ;
 dx = 0 ;
 }
 dy = c -> cur_y ;
 if ( dy < 0 ) {
 h += dy ;
 dy = 0 ;
 }
 if ( ( w > 0 ) && ( h > 0 ) ) {
 outptr = c -> pic . data [ 0 ] + dx * c -> bpp2 + dy * c -> pic . linesize [ 0 ] ;
 for ( i = 0 ;
 i < h ;
 i ++ ) {
 memcpy ( c -> screendta + i * c -> cur_w * c -> bpp2 , outptr , w * c -> bpp2 ) ;
 outptr += c -> pic . linesize [ 0 ] ;
 }
 outptr = c -> pic . data [ 0 ] ;
 put_cursor ( outptr , c -> pic . linesize [ 0 ] , c , c -> cur_x , c -> cur_y ) ;
 }
 }
 * got_frame = 1 ;
 * ( AVFrame * ) data = c -> pic ;
 return buf_size ;
 }