static int pdf_dsc_process ( gx_device_pdf * pdev , const gs_param_string_array * pma ) {
 int code = 0 ;
 uint i ;
 if ( ! pdev -> ParseDSCComments ) return 0 ;
 for ( i = 0 ;
 i + 1 < pma -> size && code >= 0 ;
 i += 2 ) {
 const gs_param_string * pkey = & pma -> data [ i ] ;
 gs_param_string * pvalue = ( gs_param_string * ) & pma -> data [ i + 1 ] ;
 const char * key ;
 int newsize ;
 if ( pdf_key_eq ( pkey , "Creator" ) && pdev -> CompatibilityLevel <= 1.7 ) {
 key = "/Creator" ;
 newsize = unescape_octals ( pdev , ( char * ) pvalue -> data , pvalue -> size ) ;
 code = cos_dict_put_c_key_string ( pdev -> Info , key , pvalue -> data , newsize ) ;
 continue ;
 }
 else if ( pdf_key_eq ( pkey , "Title" ) && pdev -> CompatibilityLevel <= 1.7 ) {
 key = "/Title" ;
 newsize = unescape_octals ( pdev , ( char * ) pvalue -> data , pvalue -> size ) ;
 code = cos_dict_put_c_key_string ( pdev -> Info , key , pvalue -> data , newsize ) ;
 continue ;
 }
 else if ( pdf_key_eq ( pkey , "For" ) && pdev -> CompatibilityLevel <= 1.7 ) {
 key = "/Author" ;
 newsize = unescape_octals ( pdev , ( char * ) pvalue -> data , pvalue -> size ) ;
 code = cos_dict_put_c_key_string ( pdev -> Info , key , pvalue -> data , newsize ) ;
 continue ;
 }
 else {
 pdf_page_dsc_info_t * ppdi ;
 char scan_buf [ 200 ] ;
 if ( ( ppdi = & pdev -> doc_dsc_info , pdf_key_eq ( pkey , "Orientation" ) ) || ( ppdi = & pdev -> page_dsc_info , pdf_key_eq ( pkey , "PageOrientation" ) ) ) {
 if ( pvalue -> size == 1 && pvalue -> data [ 0 ] >= '0' && pvalue -> data [ 0 ] <= '3' ) ppdi -> orientation = pvalue -> data [ 0 ] - '0' ;
 else ppdi -> orientation = - 1 ;
 }
 else if ( ( ppdi = & pdev -> doc_dsc_info , pdf_key_eq ( pkey , "ViewingOrientation" ) ) || ( ppdi = & pdev -> page_dsc_info , pdf_key_eq ( pkey , "PageViewingOrientation" ) ) ) {
 gs_matrix mat ;
 int orient ;
 if ( pvalue -> size >= sizeof ( scan_buf ) - 1 ) continue ;
 memcpy ( scan_buf , pvalue -> data , pvalue -> size ) ;
 scan_buf [ pvalue -> size ] = 0 ;
 if ( sscanf ( scan_buf , "[%g %g %g %g]" , & mat . xx , & mat . xy , & mat . yx , & mat . yy ) != 4 ) continue ;
 for ( orient = 0 ;
 orient < 4 ;
 ++ orient ) {
 if ( mat . xx == 1 && mat . xy == 0 && mat . yx == 0 && mat . yy == 1 ) break ;
 gs_matrix_rotate ( & mat , - 90.0 , & mat ) ;
 }
 if ( orient == 4 ) orient = - 1 ;
 ppdi -> viewing_orientation = orient ;
 }
 else {
 gs_rect box ;
 if ( pdf_key_eq ( pkey , "EPSF" ) ) {
 pdev -> is_EPS = ( pvalue -> size >= 1 && pvalue -> data [ 0 ] != '0' ) ;
 continue ;
 }
 if ( pdf_key_eq ( pkey , "BoundingBox" ) ) ppdi = & pdev -> doc_dsc_info ;
 else if ( pdf_key_eq ( pkey , "PageBoundingBox" ) ) ppdi = & pdev -> page_dsc_info ;
 else continue ;
 if ( pvalue -> size >= sizeof ( scan_buf ) - 1 ) continue ;
 memcpy ( scan_buf , pvalue -> data , pvalue -> size ) ;
 scan_buf [ pvalue -> size ] = 0 ;
 if ( sscanf ( scan_buf , "[%lg %lg %lg %lg]" , & box . p . x , & box . p . y , & box . q . x , & box . q . y ) != 4 ) continue ;
 ppdi -> bounding_box = box ;
 }
 continue ;
 }
 }
 return code ;
 }