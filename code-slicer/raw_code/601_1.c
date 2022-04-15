void ff_draw_horiz_band ( AVCodecContext * avctx , DSPContext * dsp , Picture * cur , Picture * last , int y , int h , int picture_structure , int first_field , int draw_edges , int low_delay , int v_edge_pos , int h_edge_pos ) {
 const AVPixFmtDescriptor * desc = av_pix_fmt_desc_get ( avctx -> pix_fmt ) ;
 int hshift = desc -> log2_chroma_w ;
 int vshift = desc -> log2_chroma_h ;
 const int field_pic = picture_structure != PICT_FRAME ;
 if ( field_pic ) {
 h <<= 1 ;
 y <<= 1 ;
 }
 if ( ! avctx -> hwaccel && ! ( avctx -> codec -> capabilities & CODEC_CAP_HWACCEL_VDPAU ) && draw_edges && cur -> f . reference && ! ( avctx -> flags & CODEC_FLAG_EMU_EDGE ) ) {
 int * linesize = cur -> f . linesize ;
 int sides = 0 , edge_h ;
 if ( y == 0 ) sides |= EDGE_TOP ;
 if ( y + h >= v_edge_pos ) sides |= EDGE_BOTTOM ;
 edge_h = FFMIN ( h , v_edge_pos - y ) ;
 dsp -> draw_edges ( cur -> f . data [ 0 ] + y * linesize [ 0 ] , linesize [ 0 ] , h_edge_pos , edge_h , EDGE_WIDTH , EDGE_WIDTH , sides ) ;
 dsp -> draw_edges ( cur -> f . data [ 1 ] + ( y >> vshift ) * linesize [ 1 ] , linesize [ 1 ] , h_edge_pos >> hshift , edge_h >> vshift , EDGE_WIDTH >> hshift , EDGE_WIDTH >> vshift , sides ) ;
 dsp -> draw_edges ( cur -> f . data [ 2 ] + ( y >> vshift ) * linesize [ 2 ] , linesize [ 2 ] , h_edge_pos >> hshift , edge_h >> vshift , EDGE_WIDTH >> hshift , EDGE_WIDTH >> vshift , sides ) ;
 }
 h = FFMIN ( h , avctx -> height - y ) ;
 if ( field_pic && first_field && ! ( avctx -> slice_flags & SLICE_FLAG_ALLOW_FIELD ) ) return ;
 if ( avctx -> draw_horiz_band ) {
 AVFrame * src ;
 int offset [ AV_NUM_DATA_POINTERS ] ;
 int i ;
 if ( cur -> f . pict_type == AV_PICTURE_TYPE_B || low_delay || ( avctx -> slice_flags & SLICE_FLAG_CODED_ORDER ) ) src = & cur -> f ;
 else if ( last ) src = & last -> f ;
 else return ;
 if ( cur -> f . pict_type == AV_PICTURE_TYPE_B && picture_structure == PICT_FRAME && avctx -> codec_id != AV_CODEC_ID_H264 && avctx -> codec_id != AV_CODEC_ID_SVQ3 ) {
 for ( i = 0 ;
 i < AV_NUM_DATA_POINTERS ;
 i ++ ) offset [ i ] = 0 ;
 }
 else {
 offset [ 0 ] = y * src -> linesize [ 0 ] ;
 offset [ 1 ] = offset [ 2 ] = ( y >> vshift ) * src -> linesize [ 1 ] ;
 for ( i = 3 ;
 i < AV_NUM_DATA_POINTERS ;
 i ++ ) offset [ i ] = 0 ;
 }
 emms_c ( ) ;
 avctx -> draw_horiz_band ( avctx , src , offset , y , picture_structure , h ) ;
 }
 }