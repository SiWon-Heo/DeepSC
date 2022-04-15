static int svq3_decode_end ( AVCodecContext * avctx ) {
 SVQ3Context * s = avctx -> priv_data ;
 H264Context * h = & s -> h ;
 free_picture ( avctx , s -> cur_pic ) ;
 free_picture ( avctx , s -> next_pic ) ;
 free_picture ( avctx , s -> last_pic ) ;
 ff_h264_free_context ( h ) ;
 return 0 ;
 }