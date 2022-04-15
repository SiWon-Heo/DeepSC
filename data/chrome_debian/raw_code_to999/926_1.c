int main ( int argc , char * * argv ) {
 BlockBackend * blk ;
 BlockDriverState * bs ;
 off_t dev_offset = 0 ;
 uint16_t nbdflags = 0 ;
 bool disconnect = false ;
 const char * bindto = NULL ;
 const char * port = NULL ;
 char * sockpath = NULL ;
 char * device = NULL ;
 off_t fd_size ;
 QemuOpts * sn_opts = NULL ;
 const char * sn_id_or_name = NULL ;
 const char * sopt = "hVb:o:p:rsnP:c:dvk:e:f:tl:x:T:D:" ;
 struct option lopt [ ] = {
 {
 "help" , no_argument , NULL , 'h' }
 , {
 "version" , no_argument , NULL , 'V' }
 , {
 "bind" , required_argument , NULL , 'b' }
 , {
 "port" , required_argument , NULL , 'p' }
 , {
 "socket" , required_argument , NULL , 'k' }
 , {
 "offset" , required_argument , NULL , 'o' }
 , {
 "read-only" , no_argument , NULL , 'r' }
 , {
 "partition" , required_argument , NULL , 'P' }
 , {
 "connect" , required_argument , NULL , 'c' }
 , {
 "disconnect" , no_argument , NULL , 'd' }
 , {
 "snapshot" , no_argument , NULL , 's' }
 , {
 "load-snapshot" , required_argument , NULL , 'l' }
 , {
 "nocache" , no_argument , NULL , 'n' }
 , {
 "cache" , required_argument , NULL , QEMU_NBD_OPT_CACHE }
 , {
 "aio" , required_argument , NULL , QEMU_NBD_OPT_AIO }
 , {
 "discard" , required_argument , NULL , QEMU_NBD_OPT_DISCARD }
 , {
 "detect-zeroes" , required_argument , NULL , QEMU_NBD_OPT_DETECT_ZEROES }
 , {
 "shared" , required_argument , NULL , 'e' }
 , {
 "format" , required_argument , NULL , 'f' }
 , {
 "persistent" , no_argument , NULL , 't' }
 , {
 "verbose" , no_argument , NULL , 'v' }
 , {
 "object" , required_argument , NULL , QEMU_NBD_OPT_OBJECT }
 , {
 "export-name" , required_argument , NULL , 'x' }
 , {
 "description" , required_argument , NULL , 'D' }
 , {
 "tls-creds" , required_argument , NULL , QEMU_NBD_OPT_TLSCREDS }
 , {
 "image-opts" , no_argument , NULL , QEMU_NBD_OPT_IMAGE_OPTS }
 , {
 "trace" , required_argument , NULL , 'T' }
 , {
 "fork" , no_argument , NULL , QEMU_NBD_OPT_FORK }
 , {
 NULL , 0 , NULL , 0 }
 }
 ;
 int ch ;
 int opt_ind = 0 ;
 char * end ;
 int flags = BDRV_O_RDWR ;
 int partition = - 1 ;
 int ret = 0 ;
 bool seen_cache = false ;
 bool seen_discard = false ;
 bool seen_aio = false ;
 pthread_t client_thread ;
 const char * fmt = NULL ;
 Error * local_err = NULL ;
 BlockdevDetectZeroesOptions detect_zeroes = BLOCKDEV_DETECT_ZEROES_OPTIONS_OFF ;
 QDict * options = NULL ;
 const char * export_name = NULL ;
 const char * export_description = NULL ;
 const char * tlscredsid = NULL ;
 bool imageOpts = false ;
 bool writethrough = true ;
 char * trace_file = NULL ;
 bool fork_process = false ;
 int old_stderr = - 1 ;
 unsigned socket_activation ;
 struct sigaction sa_sigterm ;
 memset ( & sa_sigterm , 0 , sizeof ( sa_sigterm ) ) ;
 sa_sigterm . sa_handler = termsig_handler ;
 sigaction ( SIGTERM , & sa_sigterm , NULL ) ;
 module_call_init ( MODULE_INIT_TRACE ) ;
 qcrypto_init ( & error_fatal ) ;
 module_call_init ( MODULE_INIT_QOM ) ;
 qemu_add_opts ( & qemu_object_opts ) ;
 qemu_add_opts ( & qemu_trace_opts ) ;
 qemu_init_exec_dir ( argv [ 0 ] ) ;
 while ( ( ch = getopt_long ( argc , argv , sopt , lopt , & opt_ind ) ) != - 1 ) {
 switch ( ch ) {
 case 's' : flags |= BDRV_O_SNAPSHOT ;
 break ;
 case 'n' : optarg = ( char * ) "none" ;
 case QEMU_NBD_OPT_CACHE : if ( seen_cache ) {
 error_report ( "-n and --cache can only be specified once" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 seen_cache = true ;
 if ( bdrv_parse_cache_mode ( optarg , & flags , & writethrough ) == - 1 ) {
 error_report ( "Invalid cache mode `%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case QEMU_NBD_OPT_AIO : if ( seen_aio ) {
 error_report ( "--aio can only be specified once" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 seen_aio = true ;
 if ( ! strcmp ( optarg , "native" ) ) {
 flags |= BDRV_O_NATIVE_AIO ;
 }
 else if ( ! strcmp ( optarg , "threads" ) ) {
 }
 else {
 error_report ( "invalid aio mode `%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case QEMU_NBD_OPT_DISCARD : if ( seen_discard ) {
 error_report ( "--discard can only be specified once" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 seen_discard = true ;
 if ( bdrv_parse_discard_flags ( optarg , & flags ) == - 1 ) {
 error_report ( "Invalid discard mode `%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case QEMU_NBD_OPT_DETECT_ZEROES : detect_zeroes = qapi_enum_parse ( BlockdevDetectZeroesOptions_lookup , optarg , BLOCKDEV_DETECT_ZEROES_OPTIONS__MAX , BLOCKDEV_DETECT_ZEROES_OPTIONS_OFF , & local_err ) ;
 if ( local_err ) {
 error_reportf_err ( local_err , "Failed to parse detect_zeroes mode: " ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( detect_zeroes == BLOCKDEV_DETECT_ZEROES_OPTIONS_UNMAP && ! ( flags & BDRV_O_UNMAP ) ) {
 error_report ( "setting detect-zeroes to unmap is not allowed " "without setting discard operation to unmap" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case 'b' : bindto = optarg ;
 break ;
 case 'p' : port = optarg ;
 break ;
 case 'o' : dev_offset = strtoll ( optarg , & end , 0 ) ;
 if ( * end ) {
 error_report ( "Invalid offset `%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( dev_offset < 0 ) {
 error_report ( "Offset must be positive `%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case 'l' : if ( strstart ( optarg , SNAPSHOT_OPT_BASE , NULL ) ) {
 sn_opts = qemu_opts_parse_noisily ( & internal_snapshot_opts , optarg , false ) ;
 if ( ! sn_opts ) {
 error_report ( "Failed in parsing snapshot param `%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 }
 else {
 sn_id_or_name = optarg ;
 }
 case 'r' : nbdflags |= NBD_FLAG_READ_ONLY ;
 flags &= ~ BDRV_O_RDWR ;
 break ;
 case 'P' : partition = strtol ( optarg , & end , 0 ) ;
 if ( * end ) {
 error_report ( "Invalid partition `%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( partition < 1 || partition > 8 ) {
 error_report ( "Invalid partition %d" , partition ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case 'k' : sockpath = optarg ;
 if ( sockpath [ 0 ] != '/' ) {
 error_report ( "socket path must be absolute" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case 'd' : disconnect = true ;
 break ;
 case 'c' : device = optarg ;
 break ;
 case 'e' : shared = strtol ( optarg , & end , 0 ) ;
 if ( * end ) {
 error_report ( "Invalid shared device number '%s'" , optarg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( shared < 1 ) {
 error_report ( "Shared device number must be greater than 0" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 break ;
 case 'f' : fmt = optarg ;
 break ;
 case 't' : persistent = 1 ;
 break ;
 case 'x' : export_name = optarg ;
 break ;
 case 'D' : export_description = optarg ;
 break ;
 case 'v' : verbose = 1 ;
 break ;
 case 'V' : version ( argv [ 0 ] ) ;
 exit ( 0 ) ;
 break ;
 case 'h' : usage ( argv [ 0 ] ) ;
 exit ( 0 ) ;
 break ;
 case '?' : error_report ( "Try `%s --help' for more information." , argv [ 0 ] ) ;
 exit ( EXIT_FAILURE ) ;
 case QEMU_NBD_OPT_OBJECT : {
 QemuOpts * opts ;
 opts = qemu_opts_parse_noisily ( & qemu_object_opts , optarg , true ) ;
 if ( ! opts ) {
 exit ( EXIT_FAILURE ) ;
 }
 }
 break ;
 case QEMU_NBD_OPT_TLSCREDS : tlscredsid = optarg ;
 break ;
 case QEMU_NBD_OPT_IMAGE_OPTS : imageOpts = true ;
 break ;
 case 'T' : g_free ( trace_file ) ;
 trace_file = trace_opt_parse ( optarg ) ;
 break ;
 case QEMU_NBD_OPT_FORK : fork_process = true ;
 break ;
 }
 }
 if ( ( argc - optind ) != 1 ) {
 error_report ( "Invalid number of arguments" ) ;
 error_printf ( "Try `%s --help' for more information.\n" , argv [ 0 ] ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( qemu_opts_foreach ( & qemu_object_opts , user_creatable_add_opts_foreach , NULL , NULL ) ) {
 exit ( EXIT_FAILURE ) ;
 }
 if ( ! trace_init_backends ( ) ) {
 exit ( 1 ) ;
 }
 trace_init_file ( trace_file ) ;
 qemu_set_log ( LOG_TRACE ) ;
 socket_activation = check_socket_activation ( ) ;
 if ( socket_activation == 0 ) {
 setup_address_and_port ( & bindto , & port ) ;
 }
 else {
 const char * err_msg = socket_activation_validate_opts ( device , sockpath , bindto , port ) ;
 if ( err_msg != NULL ) {
 error_report ( "%s" , err_msg ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( socket_activation > 1 ) {
 error_report ( "qemu-nbd does not support socket activation with %s > 1" , "LISTEN_FDS" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 }
 if ( tlscredsid ) {
 if ( sockpath ) {
 error_report ( "TLS is only supported with IPv4/IPv6" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( device ) {
 error_report ( "TLS is not supported with a host device" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( ! export_name ) {
 export_name = "" ;
 }
 tlscreds = nbd_get_tls_creds ( tlscredsid , & local_err ) ;
 if ( local_err ) {
 error_report ( "Failed to get TLS creds %s" , error_get_pretty ( local_err ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 }
 if ( disconnect ) {
 int nbdfd = open ( argv [ optind ] , O_RDWR ) ;
 if ( nbdfd < 0 ) {
 error_report ( "Cannot open %s: %s" , argv [ optind ] , strerror ( errno ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 nbd_disconnect ( nbdfd ) ;
 close ( nbdfd ) ;
 printf ( "%s disconnected\n" , argv [ optind ] ) ;
 return 0 ;
 }
 if ( ( device && ! verbose ) || fork_process ) {
 int stderr_fd [ 2 ] ;
 pid_t pid ;
 int ret ;
 if ( qemu_pipe ( stderr_fd ) < 0 ) {
 error_report ( "Error setting up communication pipe: %s" , strerror ( errno ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 pid = fork ( ) ;
 if ( pid < 0 ) {
 error_report ( "Failed to fork: %s" , strerror ( errno ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 else if ( pid == 0 ) {
 close ( stderr_fd [ 0 ] ) ;
 ret = qemu_daemon ( 1 , 0 ) ;
 old_stderr = dup ( STDERR_FILENO ) ;
 dup2 ( stderr_fd [ 1 ] , STDERR_FILENO ) ;
 if ( ret < 0 ) {
 error_report ( "Failed to daemonize: %s" , strerror ( errno ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 close ( stderr_fd [ 1 ] ) ;
 }
 else {
 bool errors = false ;
 char * buf ;
 close ( stderr_fd [ 1 ] ) ;
 buf = g_malloc ( 1024 ) ;
 while ( ( ret = read ( stderr_fd [ 0 ] , buf , 1024 ) ) > 0 ) {
 errors = true ;
 ret = qemu_write_full ( STDERR_FILENO , buf , ret ) ;
 if ( ret < 0 ) {
 exit ( EXIT_FAILURE ) ;
 }
 }
 if ( ret < 0 ) {
 error_report ( "Cannot read from daemon: %s" , strerror ( errno ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 exit ( errors ) ;
 }
 }
 if ( device != NULL && sockpath == NULL ) {
 sockpath = g_malloc ( 128 ) ;
 snprintf ( sockpath , 128 , SOCKET_PATH , basename ( device ) ) ;
 }
 if ( socket_activation == 0 ) {
 server_ioc = qio_channel_socket_new ( ) ;
 saddr = nbd_build_socket_address ( sockpath , bindto , port ) ;
 if ( qio_channel_socket_listen_sync ( server_ioc , saddr , & local_err ) < 0 ) {
 object_unref ( OBJECT ( server_ioc ) ) ;
 error_report_err ( local_err ) ;
 return 1 ;
 }
 }
 else {
 assert ( socket_activation == 1 ) ;
 server_ioc = qio_channel_socket_new_fd ( FIRST_SOCKET_ACTIVATION_FD , & local_err ) ;
 if ( server_ioc == NULL ) {
 error_report ( "Failed to use socket activation: %s" , error_get_pretty ( local_err ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 }
 if ( qemu_init_main_loop ( & local_err ) ) {
 error_report_err ( local_err ) ;
 exit ( EXIT_FAILURE ) ;
 }
 bdrv_init ( ) ;
 atexit ( bdrv_close_all ) ;
 srcpath = argv [ optind ] ;
 if ( imageOpts ) {
 QemuOpts * opts ;
 if ( fmt ) {
 error_report ( "--image-opts and -f are mutually exclusive" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 opts = qemu_opts_parse_noisily ( & file_opts , srcpath , true ) ;
 if ( ! opts ) {
 qemu_opts_reset ( & file_opts ) ;
 exit ( EXIT_FAILURE ) ;
 }
 options = qemu_opts_to_qdict ( opts , NULL ) ;
 qemu_opts_reset ( & file_opts ) ;
 blk = blk_new_open ( NULL , NULL , options , flags , & local_err ) ;
 }
 else {
 if ( fmt ) {
 options = qdict_new ( ) ;
 qdict_put_str ( options , "driver" , fmt ) ;
 }
 blk = blk_new_open ( srcpath , NULL , options , flags , & local_err ) ;
 }
 if ( ! blk ) {
 error_reportf_err ( local_err , "Failed to blk_new_open '%s': " , argv [ optind ] ) ;
 exit ( EXIT_FAILURE ) ;
 }
 bs = blk_bs ( blk ) ;
 blk_set_enable_write_cache ( blk , ! writethrough ) ;
 if ( sn_opts ) {
 ret = bdrv_snapshot_load_tmp ( bs , qemu_opt_get ( sn_opts , SNAPSHOT_OPT_ID ) , qemu_opt_get ( sn_opts , SNAPSHOT_OPT_NAME ) , & local_err ) ;
 }
 else if ( sn_id_or_name ) {
 ret = bdrv_snapshot_load_tmp_by_id_or_name ( bs , sn_id_or_name , & local_err ) ;
 }
 if ( ret < 0 ) {
 error_reportf_err ( local_err , "Failed to load snapshot: " ) ;
 exit ( EXIT_FAILURE ) ;
 }
 bs -> detect_zeroes = detect_zeroes ;
 fd_size = blk_getlength ( blk ) ;
 if ( fd_size < 0 ) {
 error_report ( "Failed to determine the image length: %s" , strerror ( - fd_size ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( dev_offset >= fd_size ) {
 error_report ( "Offset (%lld) has to be smaller than the image size " "(%lld)" , ( long long int ) dev_offset , ( long long int ) fd_size ) ;
 exit ( EXIT_FAILURE ) ;
 }
 fd_size -= dev_offset ;
 if ( partition != - 1 ) {
 ret = find_partition ( blk , partition , & dev_offset , & fd_size ) ;
 if ( ret < 0 ) {
 error_report ( "Could not find partition %d: %s" , partition , strerror ( - ret ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 }
 exp = nbd_export_new ( bs , dev_offset , fd_size , nbdflags , nbd_export_closed , writethrough , NULL , & local_err ) ;
 if ( ! exp ) {
 error_report_err ( local_err ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( export_name ) {
 nbd_export_set_name ( exp , export_name ) ;
 nbd_export_set_description ( exp , export_description ) ;
 newproto = true ;
 }
 else if ( export_description ) {
 error_report ( "Export description requires an export name" ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( device ) {
 int ret ;
 ret = pthread_create ( & client_thread , NULL , nbd_client_thread , device ) ;
 if ( ret != 0 ) {
 error_report ( "Failed to create client thread: %s" , strerror ( ret ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 }
 else {
 memset ( & client_thread , 0 , sizeof ( client_thread ) ) ;
 }
 nbd_update_server_watch ( ) ;
 if ( chdir ( "/" ) < 0 ) {
 error_report ( "Could not chdir to root directory: %s" , strerror ( errno ) ) ;
 exit ( EXIT_FAILURE ) ;
 }
 if ( fork_process ) {
 dup2 ( old_stderr , STDERR_FILENO ) ;
 close ( old_stderr ) ;
 }
 state = RUNNING ;
 do {
 main_loop_wait ( false ) ;
 if ( state == TERMINATE ) {
 state = TERMINATING ;
 nbd_export_close ( exp ) ;
 nbd_export_put ( exp ) ;
 exp = NULL ;
 }
 }
 while ( state != TERMINATED ) ;
 blk_unref ( blk ) ;
 if ( sockpath ) {
 unlink ( sockpath ) ;
 }
 qemu_opts_del ( sn_opts ) ;
 if ( device ) {
 void * ret ;
 pthread_join ( client_thread , & ret ) ;
 exit ( ret != NULL ) ;
 }
 else {
 exit ( EXIT_SUCCESS ) ;
 }
 }