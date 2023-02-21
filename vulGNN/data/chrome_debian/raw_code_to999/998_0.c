static void inject_file ( const char * path , char * file ) {
 char * filename ;
 int fd ;
 void * buf ;
 size_t size , ret ;
 DEBUG_MSG ( "inject_file %s/%s" , path , file ) ;
 SAFE_CALLOC ( filename , strlen ( path ) + strlen ( file ) + 2 , sizeof ( char ) ) ;
 snprintf ( filename , strlen ( path ) + strlen ( file ) + 2 , "%s/%s" , path , file ) ;
 if ( ( fd = open ( filename , O_RDONLY | O_BINARY ) ) == - 1 ) {
 ui_error ( "Can't load the file" ) ;
 return ;
 }
 SAFE_FREE ( filename ) ;
 size = lseek ( fd , 0 , SEEK_END ) ;
 SAFE_CALLOC ( buf , size , sizeof ( char ) ) ;
 lseek ( fd , 0 , SEEK_SET ) ;
 ret = read ( fd , buf , size ) ;
 close ( fd ) ;
 if ( ret != size ) {
 ui_error ( "Cannot read the file into memory" ) ;
 return ;
 }
 if ( wdg_c1 -> flags & WDG_OBJ_FOCUSED ) {
 user_inject ( buf , size , curr_conn , 1 ) ;
 }
 else if ( wdg_c2 -> flags & WDG_OBJ_FOCUSED ) {
 user_inject ( buf , size , curr_conn , 2 ) ;
 }
 SAFE_FREE ( buf ) ;
 }