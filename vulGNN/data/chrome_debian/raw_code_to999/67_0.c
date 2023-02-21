Jbig2PatternDict * jbig2_decode_ht_region_get_hpats ( Jbig2Ctx * ctx , Jbig2Segment * segment ) {
 int index = 0 ;
 Jbig2PatternDict * pattern_dict = NULL ;
 Jbig2Segment * rsegment = NULL ;
 while ( ! pattern_dict && segment -> referred_to_segment_count > index ) {
 rsegment = jbig2_find_segment ( ctx , segment -> referred_to_segments [ index ] ) ;
 if ( rsegment ) {
 if ( ( rsegment -> flags & 0x3f ) == 16 && rsegment -> result ) {
 pattern_dict = ( Jbig2PatternDict * ) rsegment -> result ;
 return pattern_dict ;
 }
 }
 index ++ ;
 }
 return pattern_dict ;
 }