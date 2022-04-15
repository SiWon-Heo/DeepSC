static void gtkui_inject_file ( const char * filename , int side ) {
 int fd ;
 void * buf ;
 size_t size , ret ;
 DEBUG_MSG ( "inject_file %s" , filename ) ;
 if ( ( fd = open ( filename , O_RDONLY | O_BINARY ) ) == - 1 ) {
 ui_error ( "Can't load the file" ) ;
 return ;
 }
 size = lseek ( fd , 0 , SEEK_END ) ;
 SAFE_CALLOC ( buf , size , sizeof ( char ) ) ;
 lseek ( fd , 0 , SEEK_SET ) ;
 ret = read ( fd , buf , size ) ;
 close ( fd ) ;
 if ( ret != size ) {
 ui_error ( "Cannot read the file into memory" ) ;
 return ;
 }
 if ( side == 1 || side == 2 ) {
 user_inject ( buf , size , curr_conn , side ) ;
 }
 SAFE_FREE ( buf ) ;
 }