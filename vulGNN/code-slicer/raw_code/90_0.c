int jbig2_parse_segment ( Jbig2Ctx * ctx , Jbig2Segment * segment , const uint8_t * segment_data ) {
 jbig2_error ( ctx , JBIG2_SEVERITY_INFO , segment -> number , "Segment %d, flags=%x, type=%d, data_length=%d" , segment -> number , segment -> flags , segment -> flags & 63 , segment -> data_length ) ;
 switch ( segment -> flags & 63 ) {
 case 0 : return jbig2_symbol_dictionary ( ctx , segment , segment_data ) ;
 case 4 : case 6 : case 7 : return jbig2_text_region ( ctx , segment , segment_data ) ;
 case 16 : return jbig2_pattern_dictionary ( ctx , segment , segment_data ) ;
 case 20 : case 22 : case 23 : return jbig2_halftone_region ( ctx , segment , segment_data ) ;
 case 36 : return jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "unhandled segment type 'intermediate generic region'" ) ;
 case 38 : case 39 : return jbig2_immediate_generic_region ( ctx , segment , segment_data ) ;
 case 40 : case 42 : case 43 : return jbig2_refinement_region ( ctx , segment , segment_data ) ;
 case 48 : return jbig2_page_info ( ctx , segment , segment_data ) ;
 case 49 : return jbig2_end_of_page ( ctx , segment , segment_data ) ;
 case 50 : return jbig2_end_of_stripe ( ctx , segment , segment_data ) ;
 case 51 : ctx -> state = JBIG2_FILE_EOF ;
 return jbig2_error ( ctx , JBIG2_SEVERITY_INFO , segment -> number , "end of file" ) ;
 case 52 : return jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "unhandled segment type 'profile'" ) ;
 case 53 : return jbig2_table ( ctx , segment , segment_data ) ;
 case 62 : return jbig2_parse_extension_segment ( ctx , segment , segment_data ) ;
 default : jbig2_error ( ctx , JBIG2_SEVERITY_WARNING , segment -> number , "unknown segment type %d" , segment -> flags & 63 ) ;
 }
 return 0 ;
 }