static int execute_code ( AVCodecContext * avctx , int c ) {
 AnsiContext * s = avctx -> priv_data ;
 int ret , i , width , height ;
 switch ( c ) {
 case 'A' : s -> y = FFMAX ( s -> y - ( s -> nb_args > 0 ? s -> args [ 0 ] * s -> font_height : s -> font_height ) , 0 ) ;
 break ;
 case 'B' : s -> y = FFMIN ( s -> y + ( s -> nb_args > 0 ? s -> args [ 0 ] * s -> font_height : s -> font_height ) , avctx -> height - s -> font_height ) ;
 break ;
 case 'C' : s -> x = FFMIN ( s -> x + ( s -> nb_args > 0 ? s -> args [ 0 ] * FONT_WIDTH : FONT_WIDTH ) , avctx -> width - FONT_WIDTH ) ;
 break ;
 case 'D' : s -> x = FFMAX ( s -> x - ( s -> nb_args > 0 ? s -> args [ 0 ] * FONT_WIDTH : FONT_WIDTH ) , 0 ) ;
 break ;
 case 'H' : case 'f' : s -> y = s -> nb_args > 0 ? av_clip ( ( s -> args [ 0 ] - 1 ) * s -> font_height , 0 , avctx -> height - s -> font_height ) : 0 ;
 s -> x = s -> nb_args > 1 ? av_clip ( ( s -> args [ 1 ] - 1 ) * FONT_WIDTH , 0 , avctx -> width - FONT_WIDTH ) : 0 ;
 break ;
 case 'h' : case 'l' : if ( s -> nb_args < 2 ) s -> args [ 0 ] = DEFAULT_SCREEN_MODE ;
 switch ( s -> args [ 0 ] ) {
 case 0 : case 1 : case 4 : case 5 : case 13 : case 19 : s -> font = ff_cga_font ;
 s -> font_height = 8 ;
 width = 40 << 3 ;
 height = 25 << 3 ;
 break ;
 case 2 : case 3 : s -> font = ff_vga16_font ;
 s -> font_height = 16 ;
 width = 80 << 3 ;
 height = 25 << 4 ;
 break ;
 case 6 : case 14 : s -> font = ff_cga_font ;
 s -> font_height = 8 ;
 width = 80 << 3 ;
 height = 25 << 3 ;
 break ;
 case 7 : break ;
 case 15 : case 16 : s -> font = ff_cga_font ;
 s -> font_height = 8 ;
 width = 80 << 3 ;
 height = 43 << 3 ;
 break ;
 case 17 : case 18 : s -> font = ff_cga_font ;
 s -> font_height = 8 ;
 width = 80 << 3 ;
 height = 60 << 4 ;
 break ;
 default : av_log_ask_for_sample ( avctx , "unsupported screen mode\n" ) ;
 }
 if ( width != avctx -> width || height != avctx -> height ) {
 if ( s -> frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & s -> frame ) ;
 avcodec_set_dimensions ( avctx , width , height ) ;
 ret = ff_get_buffer ( avctx , & s -> frame ) ;
 if ( ret < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 s -> frame . pict_type = AV_PICTURE_TYPE_I ;
 s -> frame . palette_has_changed = 1 ;
 memcpy ( s -> frame . data [ 1 ] , ff_cga_palette , 16 * 4 ) ;
 erase_screen ( avctx ) ;
 }
 else if ( c == 'l' ) {
 erase_screen ( avctx ) ;
 }
 break ;
 case 'J' : switch ( s -> args [ 0 ] ) {
 case 0 : erase_line ( avctx , s -> x , avctx -> width - s -> x ) ;
 if ( s -> y < avctx -> height - s -> font_height ) memset ( s -> frame . data [ 0 ] + ( s -> y + s -> font_height ) * s -> frame . linesize [ 0 ] , DEFAULT_BG_COLOR , ( avctx -> height - s -> y - s -> font_height ) * s -> frame . linesize [ 0 ] ) ;
 break ;
 case 1 : erase_line ( avctx , 0 , s -> x ) ;
 if ( s -> y > 0 ) memset ( s -> frame . data [ 0 ] , DEFAULT_BG_COLOR , s -> y * s -> frame . linesize [ 0 ] ) ;
 break ;
 case 2 : erase_screen ( avctx ) ;
 }
 break ;
 case 'K' : switch ( s -> args [ 0 ] ) {
 case 0 : erase_line ( avctx , s -> x , avctx -> width - s -> x ) ;
 break ;
 case 1 : erase_line ( avctx , 0 , s -> x ) ;
 break ;
 case 2 : erase_line ( avctx , 0 , avctx -> width ) ;
 }
 break ;
 case 'm' : if ( s -> nb_args == 0 ) {
 s -> nb_args = 1 ;
 s -> args [ 0 ] = 0 ;
 }
 for ( i = 0 ;
 i < FFMIN ( s -> nb_args , MAX_NB_ARGS ) ;
 i ++ ) {
 int m = s -> args [ i ] ;
 if ( m == 0 ) {
 s -> attributes = 0 ;
 s -> fg = DEFAULT_FG_COLOR ;
 s -> bg = DEFAULT_BG_COLOR ;
 }
 else if ( m == 1 || m == 2 || m == 4 || m == 5 || m == 7 || m == 8 ) {
 s -> attributes |= 1 << ( m - 1 ) ;
 }
 else if ( m >= 30 && m <= 38 ) {
 s -> fg = ansi_to_cga [ m - 30 ] ;
 }
 else if ( m == 39 ) {
 s -> fg = ansi_to_cga [ DEFAULT_FG_COLOR ] ;
 }
 else if ( m >= 40 && m <= 47 ) {
 s -> bg = ansi_to_cga [ m - 40 ] ;
 }
 else if ( m == 49 ) {
 s -> fg = ansi_to_cga [ DEFAULT_BG_COLOR ] ;
 }
 else {
 av_log_ask_for_sample ( avctx , "unsupported rendition parameter\n" ) ;
 }
 }
 break ;
 case 'n' : case 'R' : break ;
 case 's' : s -> sx = s -> x ;
 s -> sy = s -> y ;
 break ;
 case 'u' : s -> x = av_clip ( s -> sx , 0 , avctx -> width - FONT_WIDTH ) ;
 s -> y = av_clip ( s -> sy , 0 , avctx -> height - s -> font_height ) ;
 break ;
 default : av_log_ask_for_sample ( avctx , "unsupported escape code\n" ) ;
 break ;
 }
 return 0 ;
 }