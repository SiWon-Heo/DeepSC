static void await_reference_mb_row ( H264Context * const h , Picture * ref , int mb_y ) {
 int ref_field = ref -> f . reference - 1 ;
 int ref_field_picture = ref -> field_picture ;
 int ref_height = 16 * h -> mb_height >> ref_field_picture ;
 if ( ! HAVE_THREADS || ! ( h -> avctx -> active_thread_type & FF_THREAD_FRAME ) ) return ;
 ff_thread_await_progress ( & ref -> f , FFMIN ( 16 * mb_y >> ref_field_picture , ref_height - 1 ) , ref_field_picture && ref_field ) ;
 }