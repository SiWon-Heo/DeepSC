extern int name ( int ) __THROW __exctype ( isalnum ) ;
 __exctype ( isalpha ) ;
 __exctype ( iscntrl ) ;
 __exctype ( isdigit ) ;
 __exctype ( islower ) ;
 __exctype ( isgraph ) ;
 __exctype ( isprint ) ;
 __exctype ( ispunct ) ;
 __exctype ( isspace ) ;
 __exctype ( isupper ) ;
 __exctype ( isxdigit ) ;
 extern int tolower ( int __c ) __THROW ;
 extern int toupper ( int __c ) __THROW ;
 # ifdef __USE_ISOC99 __exctype ( isblank ) ;
 # endif # ifdef __USE_GNU extern int isctype ( int __c , int __mask ) __THROW ;
 # endif # if defined __USE_MISC || defined __USE_XOPEN extern int isascii ( int __c ) __THROW ;
 extern int toascii ( int __c ) __THROW ;
 __exctype ( _toupper ) ;
 __exctype ( _tolower )