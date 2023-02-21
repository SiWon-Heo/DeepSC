void ff_h263_loop_filter ( MpegEncContext * s ) {
 int qp_c ;
 const int linesize = s -> linesize ;
 const int uvlinesize = s -> uvlinesize ;
 const int xy = s -> mb_y * s -> mb_stride + s -> mb_x ;
 uint8_t * dest_y = s -> dest [ 0 ] ;
 uint8_t * dest_cb = s -> dest [ 1 ] ;
 uint8_t * dest_cr = s -> dest [ 2 ] ;
 if ( ! IS_SKIP ( s -> current_picture . f . mb_type [ xy ] ) ) {
 qp_c = s -> qscale ;
 s -> dsp . h263_v_loop_filter ( dest_y + 8 * linesize , linesize , qp_c ) ;
 s -> dsp . h263_v_loop_filter ( dest_y + 8 * linesize + 8 , linesize , qp_c ) ;
 }
 else qp_c = 0 ;
 if ( s -> mb_y ) {
 int qp_dt , qp_tt , qp_tc ;
 if ( IS_SKIP ( s -> current_picture . f . mb_type [ xy - s -> mb_stride ] ) ) qp_tt = 0 ;
 else qp_tt = s -> current_picture . f . qscale_table [ xy - s -> mb_stride ] ;
 if ( qp_c ) qp_tc = qp_c ;
 else qp_tc = qp_tt ;
 if ( qp_tc ) {
 const int chroma_qp = s -> chroma_qscale_table [ qp_tc ] ;
 s -> dsp . h263_v_loop_filter ( dest_y , linesize , qp_tc ) ;
 s -> dsp . h263_v_loop_filter ( dest_y + 8 , linesize , qp_tc ) ;
 s -> dsp . h263_v_loop_filter ( dest_cb , uvlinesize , chroma_qp ) ;
 s -> dsp . h263_v_loop_filter ( dest_cr , uvlinesize , chroma_qp ) ;
 }
 if ( qp_tt ) s -> dsp . h263_h_loop_filter ( dest_y - 8 * linesize + 8 , linesize , qp_tt ) ;
 if ( s -> mb_x ) {
 if ( qp_tt || IS_SKIP ( s -> current_picture . f . mb_type [ xy - 1 - s -> mb_stride ] ) ) qp_dt = qp_tt ;
 else qp_dt = s -> current_picture . f . qscale_table [ xy - 1 - s -> mb_stride ] ;
 if ( qp_dt ) {
 const int chroma_qp = s -> chroma_qscale_table [ qp_dt ] ;
 s -> dsp . h263_h_loop_filter ( dest_y - 8 * linesize , linesize , qp_dt ) ;
 s -> dsp . h263_h_loop_filter ( dest_cb - 8 * uvlinesize , uvlinesize , chroma_qp ) ;
 s -> dsp . h263_h_loop_filter ( dest_cr - 8 * uvlinesize , uvlinesize , chroma_qp ) ;
 }
 }
 }
 if ( qp_c ) {
 s -> dsp . h263_h_loop_filter ( dest_y + 8 , linesize , qp_c ) ;
 if ( s -> mb_y + 1 == s -> mb_height ) s -> dsp . h263_h_loop_filter ( dest_y + 8 * linesize + 8 , linesize , qp_c ) ;
 }
 if ( s -> mb_x ) {
 int qp_lc ;
 if ( qp_c || IS_SKIP ( s -> current_picture . f . mb_type [ xy - 1 ] ) ) qp_lc = qp_c ;
 else qp_lc = s -> current_picture . f . qscale_table [ xy - 1 ] ;
 if ( qp_lc ) {
 s -> dsp . h263_h_loop_filter ( dest_y , linesize , qp_lc ) ;
 if ( s -> mb_y + 1 == s -> mb_height ) {
 const int chroma_qp = s -> chroma_qscale_table [ qp_lc ] ;
 s -> dsp . h263_h_loop_filter ( dest_y + 8 * linesize , linesize , qp_lc ) ;
 s -> dsp . h263_h_loop_filter ( dest_cb , uvlinesize , chroma_qp ) ;
 s -> dsp . h263_h_loop_filter ( dest_cr , uvlinesize , chroma_qp ) ;
 }
 }
 }
 }