static int vp8_decode ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 VP8Context * ctx = avctx -> priv_data ;
 AVFrame * picture = data ;
 const void * iter = NULL ;
 struct vpx_image * img ;
 if ( vpx_codec_decode ( & ctx -> decoder , avpkt -> data , avpkt -> size , NULL , 0 ) != VPX_CODEC_OK ) {
 const char * error = vpx_codec_error ( & ctx -> decoder ) ;
 const char * detail = vpx_codec_error_detail ( & ctx -> decoder ) ;
 av_log ( avctx , AV_LOG_ERROR , "Failed to decode frame: %s\n" , error ) ;
 if ( detail ) av_log ( avctx , AV_LOG_ERROR , " Additional information: %s\n" , detail ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ( img = vpx_codec_get_frame ( & ctx -> decoder , & iter ) ) ) {
 if ( img -> fmt != VPX_IMG_FMT_I420 ) {
 av_log ( avctx , AV_LOG_ERROR , "Unsupported output colorspace (%d)\n" , img -> fmt ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( ( int ) img -> d_w != avctx -> width || ( int ) img -> d_h != avctx -> height ) {
 av_log ( avctx , AV_LOG_INFO , "dimension change! %dx%d -> %dx%d\n" , avctx -> width , avctx -> height , img -> d_w , img -> d_h ) ;
 if ( av_image_check_size ( img -> d_w , img -> d_h , 0 , avctx ) ) return AVERROR_INVALIDDATA ;
 avcodec_set_dimensions ( avctx , img -> d_w , img -> d_h ) ;
 }
 picture -> data [ 0 ] = img -> planes [ 0 ] ;
 picture -> data [ 1 ] = img -> planes [ 1 ] ;
 picture -> data [ 2 ] = img -> planes [ 2 ] ;
 picture -> data [ 3 ] = NULL ;
 picture -> linesize [ 0 ] = img -> stride [ 0 ] ;
 picture -> linesize [ 1 ] = img -> stride [ 1 ] ;
 picture -> linesize [ 2 ] = img -> stride [ 2 ] ;
 picture -> linesize [ 3 ] = 0 ;
 * got_frame = 1 ;
 }
 return avpkt -> size ;
 }