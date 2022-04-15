void mime_init_cache_control_cooking_masks ( ) {
 static struct {
 const char * name ;
 uint32_t mask ;
 }
 cc_mask_table [ ] = {
 {
 "max-age" , MIME_COOKED_MASK_CC_MAX_AGE }
 , {
 "no-cache" , MIME_COOKED_MASK_CC_NO_CACHE }
 , {
 "no-store" , MIME_COOKED_MASK_CC_NO_STORE }
 , {
 "no-transform" , MIME_COOKED_MASK_CC_NO_TRANSFORM }
 , {
 "max-stale" , MIME_COOKED_MASK_CC_MAX_STALE }
 , {
 "min-fresh" , MIME_COOKED_MASK_CC_MIN_FRESH }
 , {
 "only-if-cached" , MIME_COOKED_MASK_CC_ONLY_IF_CACHED }
 , {
 "public" , MIME_COOKED_MASK_CC_PUBLIC }
 , {
 "private" , MIME_COOKED_MASK_CC_PRIVATE }
 , {
 "must-revalidate" , MIME_COOKED_MASK_CC_MUST_REVALIDATE }
 , {
 "proxy-revalidate" , MIME_COOKED_MASK_CC_PROXY_REVALIDATE }
 , {
 "s-maxage" , MIME_COOKED_MASK_CC_S_MAXAGE }
 , {
 "need-revalidate-once" , MIME_COOKED_MASK_CC_NEED_REVALIDATE_ONCE }
 , {
 nullptr , 0 }
 }
 ;
 for ( int i = 0 ;
 cc_mask_table [ i ] . name != nullptr ;
 i ++ ) {
 const char * wks = hdrtoken_string_to_wks ( cc_mask_table [ i ] . name ) ;
 HdrTokenHeapPrefix * p = hdrtoken_wks_to_prefix ( wks ) ;
 p -> wks_type_specific . u . cache_control . cc_mask = cc_mask_table [ i ] . mask ;
 }
 }