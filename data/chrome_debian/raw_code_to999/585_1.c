static void free_picture ( MpegEncContext * s , Picture * pic ) {
 int i ;
 if ( pic -> f . data [ 0 ] && pic -> f . type != FF_BUFFER_TYPE_SHARED ) {
 free_frame_buffer ( s , pic ) ;
 }
 av_freep ( & pic -> mb_var ) ;
 av_freep ( & pic -> mc_mb_var ) ;
 av_freep ( & pic -> mb_mean ) ;
 av_freep ( & pic -> f . mbskip_table ) ;
 av_freep ( & pic -> qscale_table_base ) ;
 pic -> f . qscale_table = NULL ;
 av_freep ( & pic -> mb_type_base ) ;
 pic -> f . mb_type = NULL ;
 av_freep ( & pic -> f . dct_coeff ) ;
 av_freep ( & pic -> f . pan_scan ) ;
 pic -> f . mb_type = NULL ;
 for ( i = 0 ;
 i < 2 ;
 i ++ ) {
 av_freep ( & pic -> motion_val_base [ i ] ) ;
 av_freep ( & pic -> f . ref_index [ i ] ) ;
 pic -> f . motion_val [ i ] = NULL ;
 }
 if ( pic -> f . type == FF_BUFFER_TYPE_SHARED ) {
 for ( i = 0 ;
 i < 4 ;
 i ++ ) {
 pic -> f . base [ i ] = pic -> f . data [ i ] = NULL ;
 }
 pic -> f . type = 0 ;
 }
 }