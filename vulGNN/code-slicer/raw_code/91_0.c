void jbig2_free_segment ( Jbig2Ctx * ctx , Jbig2Segment * segment ) {
 if ( segment -> referred_to_segments != NULL ) {
 jbig2_free ( ctx -> allocator , segment -> referred_to_segments ) ;
 }
 switch ( segment -> flags & 63 ) {
 case 0 : if ( segment -> result != NULL ) jbig2_sd_release ( ctx , ( Jbig2SymbolDict * ) segment -> result ) ;
 break ;
 case 4 : case 40 : if ( segment -> result != NULL ) jbig2_image_release ( ctx , ( Jbig2Image * ) segment -> result ) ;
 break ;
 case 16 : if ( segment -> result != NULL ) jbig2_hd_release ( ctx , ( Jbig2PatternDict * ) segment -> result ) ;
 break ;
 case 53 : if ( segment -> result != NULL ) jbig2_table_free ( ctx , ( Jbig2HuffmanParams * ) segment -> result ) ;
 break ;
 case 62 : if ( segment -> result != NULL ) jbig2_metadata_free ( ctx , ( Jbig2Metadata * ) segment -> result ) ;
 break ;
 default : break ;
 }
 jbig2_free ( ctx -> allocator , segment ) ;
 }