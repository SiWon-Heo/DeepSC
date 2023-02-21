MIMEField * mime_hdr_field_find ( MIMEHdrImpl * mh , const char * field_name_str , int field_name_len ) {
 HdrTokenHeapPrefix * token_info ;
 const bool is_wks = hdrtoken_is_wks ( field_name_str ) ;
 ink_assert ( field_name_len >= 0 ) ;
 # if TRACK_FIELD_FIND_CALLS Debug ( "http" , "mime_hdr_field_find(hdr 0x%X, field %.*s): is_wks = %d" , mh , field_name_len , field_name_str , is_wks ) ;
 # endif if ( is_wks ) {
 token_info = hdrtoken_wks_to_prefix ( field_name_str ) ;
 if ( ( token_info -> wks_info . mask ) && ( ( mh -> m_presence_bits & token_info -> wks_info . mask ) == 0 ) ) {
 # if TRACK_FIELD_FIND_CALLS Debug ( "http" , "mime_hdr_field_find(hdr 0x%X, field %.*s): MISS (due to presence bits)" , mh , field_name_len , field_name_str ) ;
 # endif return nullptr ;
 }
 int32_t slot_id = token_info -> wks_info . slotid ;
 if ( slot_id != MIME_SLOTID_NONE ) {
 uint32_t slotnum = mime_hdr_get_accelerator_slotnum ( mh , slot_id ) ;
 if ( slotnum != MIME_FIELD_SLOTNUM_UNKNOWN ) {
 MIMEField * f = _mime_hdr_field_list_search_by_slotnum ( mh , slotnum ) ;
 ink_assert ( ( f == nullptr ) || f -> is_live ( ) ) ;
 # if TRACK_FIELD_FIND_CALLS Debug ( "http" , "mime_hdr_field_find(hdr 0x%X, field %.*s): %s (due to slot accelerators)" , mh , field_name_len , field_name_str , ( f ? "HIT" : "MISS" ) ) ;
 # endif return f ;
 }
 else {
 # if TRACK_FIELD_FIND_CALLS Debug ( "http" , "mime_hdr_field_find(hdr 0x%X, field %.*s): UNKNOWN (slot too big)" , mh , field_name_len , field_name_str ) ;
 # endif }
 }
 MIMEField * f = _mime_hdr_field_list_search_by_wks ( mh , token_info -> wks_idx ) ;
 ink_assert ( ( f == nullptr ) || f -> is_live ( ) ) ;
 # if TRACK_FIELD_FIND_CALLS Debug ( "http" , "mime_hdr_field_find(hdr 0x%X, field %.*s): %s (due to WKS list walk)" , mh , field_name_len , field_name_str , ( f ? "HIT" : "MISS" ) ) ;
 # endif return f ;
 }
 else {
 MIMEField * f = _mime_hdr_field_list_search_by_string ( mh , field_name_str , field_name_len ) ;
 ink_assert ( ( f == nullptr ) || f -> is_live ( ) ) ;
 # if TRACK_FIELD_FIND_CALLS Debug ( "http" , "mime_hdr_field_find(hdr 0x%X, field %.*s): %s (due to strcmp list walk)" , mh , field_name_len , field_name_str , ( f ? "HIT" : "MISS" ) ) ;
 # endif return f ;
 }
 }