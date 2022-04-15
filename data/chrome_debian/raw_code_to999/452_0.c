static char * xmlrpc_write_header ( int length ) {
 char buf [ 512 ] ;
 time_t ts ;
 char timebuf [ 64 ] ;
 struct tm tm ;
 * buf = '\0' ;
 ts = time ( NULL ) ;
 tm = * localtime ( & ts ) ;
 strftime ( timebuf , sizeof timebuf , "%Y-%m-%d %H:%M:%S" , & tm ) ;
 snprintf ( buf , sizeof buf , "HTTP/1.1 200 OK\r\nConnection: close\r\n" "Content-Length: %d\r\n" "Content-Type: text/xml\r\n" "Date: %s\r\n" "Server: Atheme/%s\r\n\r\n" , length , timebuf , PACKAGE_VERSION ) ;
 return sstrdup ( buf ) ;
 }