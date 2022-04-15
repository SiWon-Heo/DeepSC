static int mimic_decode_update_thread_context ( AVCodecContext * avctx , const AVCodecContext * avctx_from ) {
 MimicContext * dst = avctx -> priv_data , * src = avctx_from -> priv_data ;
 if ( avctx == avctx_from ) return 0 ;
 dst -> cur_index = src -> next_cur_index ;
 dst -> prev_index = src -> next_prev_index ;
 memcpy ( dst -> buf_ptrs , src -> buf_ptrs , sizeof ( src -> buf_ptrs ) ) ;
 memcpy ( dst -> flipped_ptrs , src -> flipped_ptrs , sizeof ( src -> flipped_ptrs ) ) ;
 memset ( & dst -> buf_ptrs [ dst -> cur_index ] , 0 , sizeof ( AVFrame ) ) ;
 return 0 ;
 }