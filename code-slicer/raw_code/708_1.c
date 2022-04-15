int gs_nulldevice ( gs_gstate * pgs ) {
 int code = 0 ;
 if ( pgs -> device == 0 || ! gx_device_is_null ( pgs -> device ) ) {
 gx_device * ndev ;
 code = gs_copydevice ( & ndev , ( const gx_device * ) & gs_null_device , pgs -> memory ) ;
 if ( code < 0 ) return code ;
 rc_init ( ndev , pgs -> memory , 0 ) ;
 if ( pgs -> device != NULL ) {
 if ( ( code = dev_proc ( pgs -> device , get_profile ) ( pgs -> device , & ( ndev -> icc_struct ) ) ) < 0 ) return code ;
 rc_increment ( ndev -> icc_struct ) ;
 set_dev_proc ( ndev , get_profile , gx_default_get_profile ) ;
 }
 if ( ( code = gs_setdevice_no_erase ( pgs , ndev ) ) < 0 ) gs_free_object ( pgs -> memory , ndev , "gs_copydevice(device)" ) ;
 }
 return code ;
 }