int xmlrpc_about ( void * userdata , int ac , char * * av ) {
 char buf [ XMLRPC_BUFSIZE ] ;
 char buf2 [ XMLRPC_BUFSIZE ] ;
 char buf3 [ XMLRPC_BUFSIZE ] ;
 char buf4 [ XMLRPC_BUFSIZE ] ;
 char * arraydata ;
 ( void ) userdata ;
 xmlrpc_integer ( buf3 , ac ) ;
 xmlrpc_string ( buf4 , av [ 0 ] ) ;
 xmlrpc_string ( buf , ( char * ) XMLLIB_VERSION ) ;
 xmlrpc_string ( buf2 , ( char * ) XMLLIB_AUTHOR ) ;
 arraydata = xmlrpc_array ( 4 , buf , buf2 , buf3 , buf4 ) ;
 xmlrpc_send ( 1 , arraydata ) ;
 free ( arraydata ) ;
 return XMLRPC_CONT ;
 }