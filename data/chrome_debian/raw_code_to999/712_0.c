SPL_METHOD ( SplFileInfo , func_name ) \ {
 \ spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 \ zend_error_handling error_handling ;
 \ if ( zend_parse_parameters_none ( ) == FAILURE ) {
 \ return ;
 \ }
 \ \ zend_replace_error_handling ( EH_THROW , spl_ce_RuntimeException , & error_handling TSRMLS_CC ) ;
 \ spl_filesystem_object_get_file_name ( intern TSRMLS_CC ) ;
 \ php_stat ( intern -> file_name , intern -> file_name_len , func_num , return_value TSRMLS_CC ) ;
 \ zend_restore_error_handling ( & error_handling TSRMLS_CC ) ;
 \ }
 FileInfoFunction ( getPerms , FS_PERMS ) FileInfoFunction ( getInode , FS_INODE ) FileInfoFunction ( getSize , FS_SIZE ) FileInfoFunction ( getOwner , FS_OWNER ) FileInfoFunction ( getGroup , FS_GROUP ) FileInfoFunction ( getATime , FS_ATIME ) FileInfoFunction ( getMTime , FS_MTIME ) FileInfoFunction ( getCTime , FS_CTIME ) FileInfoFunction ( getType , FS_TYPE ) FileInfoFunction ( isWritable , FS_IS_W ) FileInfoFunction ( isReadable , FS_IS_R ) FileInfoFunction ( isExecutable , FS_IS_X ) FileInfoFunction ( isFile , FS_IS_FILE ) FileInfoFunction ( isDir , FS_IS_DIR ) FileInfoFunction ( isLink , FS_IS_LINK )