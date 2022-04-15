int gdev_pdf_put_params ( gx_device * dev , gs_param_list * plist ) {
 int code ;
 gx_device_pdf * pdev = ( gx_device_pdf * ) dev ;
 gs_memory_t * mem = gs_memory_stable ( pdev -> memory ) ;
 gx_device_pdf * save_dev = gs_malloc ( mem , sizeof ( gx_device_pdf ) , 1 , "saved gx_device_pdf" ) ;
 if ( ! save_dev ) return_error ( gs_error_VMerror ) ;
 memcpy ( save_dev , pdev , sizeof ( gx_device_pdf ) ) ;
 code = gdev_pdf_put_params_impl ( dev , save_dev , plist ) ;
 gs_free ( mem , save_dev , sizeof ( gx_device_pdf ) , 1 , "saved gx_device_pdf" ) ;
 return code ;
 }