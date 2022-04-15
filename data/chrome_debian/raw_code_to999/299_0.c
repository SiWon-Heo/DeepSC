int gdev_pdf_get_param ( gx_device * dev , char * Param , void * list ) {
 gx_device_pdf * pdev = ( gx_device_pdf * ) dev ;
 const gs_param_item_t * pi ;
 gs_param_list * plist = ( gs_param_list * ) list ;
 int code = 0 ;
 for ( pi = pdf_param_items ;
 pi -> key != 0 ;
 ++ pi ) {
 if ( strcmp ( pi -> key , Param ) == 0 ) {
 const char * key = pi -> key ;
 const void * pvalue = ( const void * ) ( ( const char * ) pdev + pi -> offset ) ;
 int size = xfer_item_sizes [ pi -> type ] ;
 gs_param_typed_value typed ;
 memcpy ( & typed . value , pvalue , size ) ;
 typed . type = pi -> type ;
 code = ( * plist -> procs -> xmit_typed ) ( plist , key , & typed ) ;
 return code ;
 }
 }
 if ( strcmp ( Param , "CoreDistVersion" ) == 0 ) {
 return ( param_write_int ( plist , "CoreDistVersion" , & CoreDistVersion ) ) ;
 }
 if ( strcmp ( Param , "CompatibilityLevel" ) == 0 ) {
 float f = pdev -> CompatibilityLevel ;
 return ( param_write_float ( plist , "CompatibilityLevel" , & f ) ) ;
 }
 if ( strcmp ( Param , "ForOPDFRead" ) == 0 ) {
 return ( param_write_bool ( plist , "ForOPDFRead" , & pdev -> ForOPDFRead ) ) ;
 }
 if ( ! pdev -> is_ps2write ) {
 if ( strcmp ( Param , "pdfmark" ) == 0 ) {
 return ( param_write_null ( plist , "pdfmark" ) ) ;
 }
 if ( strcmp ( Param , "DSC" ) == 0 ) {
 return ( param_write_null ( plist , "DSC" ) ) ;
 }
 }
 return gdev_psdf_get_param ( dev , Param , list ) ;
 }