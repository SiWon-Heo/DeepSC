int gdev_pdf_get_params ( gx_device * dev , gs_param_list * plist ) {
 gx_device_pdf * pdev = ( gx_device_pdf * ) dev ;
 float cl = ( float ) pdev -> CompatibilityLevel ;
 int code ;
 int cdv = CoreDistVersion ;
 pdev -> ParamCompatibilityLevel = cl ;
 code = gdev_psdf_get_params ( dev , plist ) ;
 if ( code < 0 || ( code = param_write_int ( plist , "CoreDistVersion" , & cdv ) ) < 0 || ( code = param_write_float ( plist , "CompatibilityLevel" , & cl ) ) < 0 || ( ! pdev -> is_ps2write && ( code = param_write_bool ( plist , "ForOPDFRead" , & pdev -> ForOPDFRead ) ) < 0 ) || ( param_requested ( plist , "pdfmark" ) > 0 && ( code = param_write_null ( plist , "pdfmark" ) ) < 0 ) || ( param_requested ( plist , "DSC" ) > 0 && ( code = param_write_null ( plist , "DSC" ) ) < 0 ) || ( code = gs_param_write_items ( plist , pdev , NULL , pdf_param_items ) ) < 0 ) {
 }
 return code ;
 }