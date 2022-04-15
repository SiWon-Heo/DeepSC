struct config_filter_parser * const * config_filter_find_subset ( struct config_filter_context * ctx , const struct config_filter * filter ) {
 ARRAY_TYPE ( config_filter_parsers ) matches ;
 struct config_filter tmp_mask ;
 unsigned int i ;
 t_array_init ( & matches , 8 ) ;
 for ( i = 0 ;
 ctx -> parsers [ i ] != NULL ;
 i ++ ) {
 const struct config_filter * mask = & ctx -> parsers [ i ] -> filter ;
 if ( filter -> service != NULL ) {
 if ( ! config_filter_match_service ( mask , filter ) ) continue ;
 }
 tmp_mask = * mask ;
 if ( filter -> local_name == NULL ) tmp_mask . local_name = NULL ;
 if ( filter -> local_bits == 0 ) tmp_mask . local_bits = 0 ;
 if ( filter -> remote_bits == 0 ) tmp_mask . remote_bits = 0 ;
 if ( config_filter_match_rest ( & tmp_mask , filter ) ) array_append ( & matches , & ctx -> parsers [ i ] , 1 ) ;
 }
 array_sort ( & matches , config_filter_parser_cmp_rev ) ;
 array_append_zero ( & matches ) ;
 return array_idx ( & matches , 0 ) ;
 }