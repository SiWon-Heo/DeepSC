static void free_picture ( AVCodecContext * avctx , Picture * pic ) {
 int i ;
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 av_freep ( & pic -> motion_val_base [ i ] ) ;
 av_freep ( & pic -> f . ref_index [ i ] ) ;
 }
 av_freep ( & pic -> mb_type_base ) ;
 if ( pic -> f . data [ 0 ] ) avctx -> release_buffer ( avctx , & pic -> f ) ;
 av_freep ( & pic ) ;
 }