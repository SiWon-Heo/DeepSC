void ff_h264_fill_mbaff_ref_list ( H264Context * h ) {
 int list , i , j ;
 for ( list = 0 ;
 list < 2 ;
 list ++ ) {
 for ( i = 0 ;
 i < h -> ref_count [ list ] ;
 i ++ ) {
 Picture * frame = & h -> ref_list [ list ] [ i ] ;
 Picture * field = & h -> ref_list [ list ] [ 16 + 2 * i ] ;
 field [ 0 ] = * frame ;
 for ( j = 0 ;
 j < 3 ;
 j ++ ) field [ 0 ] . f . linesize [ j ] <<= 1 ;
 field [ 0 ] . f . reference = PICT_TOP_FIELD ;
 field [ 0 ] . poc = field [ 0 ] . field_poc [ 0 ] ;
 field [ 1 ] = field [ 0 ] ;
 for ( j = 0 ;
 j < 3 ;
 j ++ ) field [ 1 ] . f . data [ j ] += frame -> f . linesize [ j ] ;
 field [ 1 ] . f . reference = PICT_BOTTOM_FIELD ;
 field [ 1 ] . poc = field [ 1 ] . field_poc [ 1 ] ;
 h -> luma_weight [ 16 + 2 * i ] [ list ] [ 0 ] = h -> luma_weight [ 16 + 2 * i + 1 ] [ list ] [ 0 ] = h -> luma_weight [ i ] [ list ] [ 0 ] ;
 h -> luma_weight [ 16 + 2 * i ] [ list ] [ 1 ] = h -> luma_weight [ 16 + 2 * i + 1 ] [ list ] [ 1 ] = h -> luma_weight [ i ] [ list ] [ 1 ] ;
 for ( j = 0 ;
 j < 2 ;
 j ++ ) {
 h -> chroma_weight [ 16 + 2 * i ] [ list ] [ j ] [ 0 ] = h -> chroma_weight [ 16 + 2 * i + 1 ] [ list ] [ j ] [ 0 ] = h -> chroma_weight [ i ] [ list ] [ j ] [ 0 ] ;
 h -> chroma_weight [ 16 + 2 * i ] [ list ] [ j ] [ 1 ] = h -> chroma_weight [ 16 + 2 * i + 1 ] [ list ] [ j ] [ 1 ] = h -> chroma_weight [ i ] [ list ] [ j ] [ 1 ] ;
 }
 }
 }
 }