int main ( int argc , char * argv [ ] ) {
 const char * globfile [ ] = {
 "one" , "two" , "three" , NULL }
 ;
 char tmpdir [ 32 ] ;
 struct passwd * pw ;
 const char * cwd ;
 int test ;
 int fail = 0 ;
 int i ;
 struct test_case_struct ts ;
 if ( argc > 1 ) {
 command_line_test ( argv [ 1 ] ) ;
 return 0 ;
 }
 cwd = getcwd ( NULL , 0 ) ;
 tmpnam ( tmpdir ) ;
 if ( mkdir ( tmpdir , S_IRWXU ) || chdir ( tmpdir ) ) return - 1 ;
 else {
 int fd ;
 for ( i = 0 ;
 globfile [ i ] ;
 ++ i ) if ( ( fd = creat ( globfile [ i ] , S_IRUSR | S_IWUSR ) ) == - 1 || close ( fd ) ) return - 1 ;
 }
 for ( test = 0 ;
 test_case [ test ] . retval != - 1 ;
 test ++ ) if ( testit ( & test_case [ test ] ) ) ++ fail ;
 pw = getpwnam ( "root" ) ;
 if ( pw != NULL ) {
 ts . retval = 0 ;
 ts . env = NULL ;
 ts . words = "~root " ;
 ts . flags = 0 ;
 ts . wordc = 1 ;
 ts . wordv [ 0 ] = pw -> pw_dir ;
 ts . ifs = IFS ;
 if ( testit ( & ts ) ) ++ fail ;
 ts . retval = 0 ;
 ts . env = pw -> pw_dir ;
 ts . words = "${
var#~root}
x" ;
 ts . flags = 0 ;
 ts . wordc = 1 ;
 ts . wordv [ 0 ] = "x" ;
 ts . ifs = IFS ;
 if ( testit ( & ts ) ) ++ fail ;
 }
 setenv ( "HOME" , "/dummy/home" , 1 ) ;
 ts . retval = 0 ;
 ts . env = NULL ;
 ts . words = "~ ~/foo" ;
 ts . flags = 0 ;
 ts . wordc = 2 ;
 ts . wordv [ 0 ] = "/dummy/home" ;
 ts . wordv [ 1 ] = "/dummy/home/foo" ;
 ts . ifs = IFS ;
 if ( testit ( & ts ) ) ++ fail ;
 pw = getpwuid ( getuid ( ) ) ;
 if ( pw != NULL ) {
 unsetenv ( "HOME" ) ;
 ts . retval = 0 ;
 ts . env = NULL ;
 ts . words = "~" ;
 ts . flags = 0 ;
 ts . wordc = 1 ;
 ts . wordv [ 0 ] = pw -> pw_dir ;
 ts . ifs = IFS ;
 if ( testit ( & ts ) ) ++ fail ;
 }
 puts ( "tests completed, now cleaning up" ) ;
 for ( i = 0 ;
 globfile [ i ] ;
 ++ i ) remove ( globfile [ i ] ) ;
 if ( cwd == NULL ) cwd = ".." ;
 chdir ( cwd ) ;
 rmdir ( tmpdir ) ;
 printf ( "tests failed: %d\n" , fail ) ;
 return fail != 0 ;
 }