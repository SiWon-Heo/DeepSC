void ff_MPV_common_end ( MpegEncContext * s ) {
 int i ;
 if ( s -> slice_context_count > 1 ) {
 for ( i = 0 ;
 i < s -> slice_context_count ;
 i ++ ) {
 free_duplicate_context ( s -> thread_context [ i ] ) ;
 }
 for ( i = 1 ;
 i < s -> slice_context_count ;
 i ++ ) {
 av_freep ( & s -> thread_context [ i ] ) ;
 }
 s -> slice_context_count = 1 ;
 }
 else free_duplicate_context ( s ) ;
 av_freep ( & s -> parse_context . buffer ) ;
 s -> parse_context . buffer_size = 0 ;
 av_freep ( & s -> bitstream_buffer ) ;
 s -> allocated_bitstream_buffer_size = 0 ;
 av_freep ( & s -> avctx -> stats_out ) ;
 av_freep ( & s -> ac_stats ) ;
 av_freep ( & s -> q_intra_matrix ) ;
 av_freep ( & s -> q_inter_matrix ) ;
 av_freep ( & s -> q_intra_matrix16 ) ;
 av_freep ( & s -> q_inter_matrix16 ) ;
 av_freep ( & s -> input_picture ) ;
 av_freep ( & s -> reordered_input_picture ) ;
 av_freep ( & s -> dct_offset ) ;
 if ( s -> picture && ! s -> avctx -> internal -> is_copy ) {
 for ( i = 0 ;
 i < s -> picture_count ;
 i ++ ) {
 free_picture ( s , & s -> picture [ i ] ) ;
 }
 }
 av_freep ( & s -> picture ) ;
 free_context_frame ( s ) ;
 if ( ! ( s -> avctx -> active_thread_type & FF_THREAD_FRAME ) ) avcodec_default_free_buffers ( s -> avctx ) ;
 s -> context_initialized = 0 ;
 s -> last_picture_ptr = s -> next_picture_ptr = s -> current_picture_ptr = NULL ;
 s -> linesize = s -> uvlinesize = 0 ;
 }