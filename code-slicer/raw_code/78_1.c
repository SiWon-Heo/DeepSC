void name ## _free ( type * a ) ;
 # define DECLARE_ASN1_PRINT_FUNCTION ( stname ) DECLARE_ASN1_PRINT_FUNCTION_fname ( stname , stname ) # define DECLARE_ASN1_PRINT_FUNCTION_fname ( stname , fname ) int fname ## _print_ctx ( BIO * out , stname * x , int indent , const ASN1_PCTX * pctx ) ;
 # define D2I_OF ( type ) type * ( * ) ( type * * , const unsigned char * * , long ) # define I2D_OF ( type ) int ( * ) ( type * , unsigned char * * ) # define I2D_OF_const ( type ) int ( * ) ( const type * , unsigned char * * ) # define CHECKED_D2I_OF ( type , d2i ) ( ( d2i_of_void * ) ( 1 ? d2i : ( ( D2I_OF ( type ) ) 0 ) ) ) # define CHECKED_I2D_OF ( type , i2d ) ( ( i2d_of_void * ) ( 1 ? i2d : ( ( I2D_OF ( type ) ) 0 ) ) ) # define CHECKED_NEW_OF ( type , xnew ) ( ( void * ( * ) ( void ) ) ( 1 ? xnew : ( ( type * ( * ) ( void ) ) 0 ) ) ) # define CHECKED_PTR_OF ( type , p ) ( ( void * ) ( 1 ? p : ( type * ) 0 ) ) # define CHECKED_PPTR_OF ( type , p ) ( ( void * * ) ( 1 ? p : ( type * * ) 0 ) ) # define TYPEDEF_D2I_OF ( type ) typedef type * d2i_of_ ## type ( type * * , const unsigned char * * , long ) # define TYPEDEF_I2D_OF ( type ) typedef int i2d_of_ ## type ( type * , unsigned char * * ) # define TYPEDEF_D2I2D_OF ( type ) TYPEDEF_D2I_OF ( type ) ;
 TYPEDEF_I2D_OF ( type ) TYPEDEF_D2I2D_OF ( void ) ;
 # ifndef OPENSSL_EXPORT_VAR_AS_FUNCTION typedef const ASN1_ITEM ASN1_ITEM_EXP ;
 # define ASN1_ITEM_ptr ( iptr ) ( iptr ) # define ASN1_ITEM_ref ( iptr ) ( & ( iptr ## _it ) ) # define ASN1_ITEM_rptr ( ref ) ( & ( ref ## _it ) ) # define DECLARE_ASN1_ITEM ( name ) OPENSSL_EXTERN const ASN1_ITEM name ## _it ;
 # else typedef const ASN1_ITEM * ASN1_ITEM_EXP ( void ) ;
 # define ASN1_ITEM_ptr ( iptr ) ( iptr ( ) ) # define ASN1_ITEM_ref ( iptr ) ( iptr ## _it ) # define ASN1_ITEM_rptr ( ref ) ( ref ## _it ( ) ) # define DECLARE_ASN1_ITEM ( name ) const ASN1_ITEM * name ## _it ( void ) ;
 # endif # define ASN1_STRFLGS_ESC_2253 1 # define ASN1_STRFLGS_ESC_CTRL 2 # define ASN1_STRFLGS_ESC_MSB 4 # define ASN1_STRFLGS_ESC_QUOTE 8 # define CHARTYPE_PRINTABLESTRING 0x10 # define CHARTYPE_FIRST_ESC_2253 0x20 # define CHARTYPE_LAST_ESC_2253 0x40 # define ASN1_STRFLGS_UTF8_CONVERT 0x10 # define ASN1_STRFLGS_IGNORE_TYPE 0x20 # define ASN1_STRFLGS_SHOW_TYPE 0x40 # define ASN1_STRFLGS_DUMP_ALL 0x80 # define ASN1_STRFLGS_DUMP_UNKNOWN 0x100 # define ASN1_STRFLGS_DUMP_DER 0x200 # define ASN1_STRFLGS_ESC_2254 0x400 # define ASN1_STRFLGS_RFC2253 ( ASN1_STRFLGS_ESC_2253 | ASN1_STRFLGS_ESC_CTRL | ASN1_STRFLGS_ESC_MSB | ASN1_STRFLGS_UTF8_CONVERT | ASN1_STRFLGS_DUMP_UNKNOWN | ASN1_STRFLGS_DUMP_DER ) DEFINE_STACK_OF ( ASN1_INTEGER ) DEFINE_STACK_OF ( ASN1_GENERALSTRING ) DEFINE_STACK_OF ( ASN1_UTF8STRING ) typedef struct asn1_type_st {
 int type ;
 union {
 char * ptr ;
 ASN1_BOOLEAN boolean ;
 ASN1_STRING * asn1_string ;
 ASN1_OBJECT * object ;
 ASN1_INTEGER * integer ;
 ASN1_ENUMERATED * enumerated ;
 ASN1_BIT_STRING * bit_string ;
 ASN1_OCTET_STRING * octet_string ;
 ASN1_PRINTABLESTRING * printablestring ;
 ASN1_T61STRING * t61string ;
 ASN1_IA5STRING * ia5string ;
 ASN1_GENERALSTRING * generalstring ;
 ASN1_BMPSTRING * bmpstring ;
 ASN1_UNIVERSALSTRING * universalstring ;
 ASN1_UTCTIME * utctime ;
 ASN1_GENERALIZEDTIME * generalizedtime ;
 ASN1_VISIBLESTRING * visiblestring ;
 ASN1_UTF8STRING * utf8string ;
 ASN1_STRING * set ;
 ASN1_STRING * sequence ;
 ASN1_VALUE * asn1_value ;
 }
 value ;
 }
 ASN1_TYPE ;
 DEFINE_STACK_OF ( ASN1_TYPE ) typedef STACK_OF ( ASN1_TYPE ) ASN1_SEQUENCE_ANY ;
 DECLARE_ASN1_ENCODE_FUNCTIONS_const ( ASN1_SEQUENCE_ANY , ASN1_SEQUENCE_ANY ) DECLARE_ASN1_ENCODE_FUNCTIONS_const ( ASN1_SEQUENCE_ANY , ASN1_SET_ANY ) typedef struct BIT_STRING_BITNAME_st {
 int bitnum ;
 const char * lname ;
 const char * sname ;
 }
 BIT_STRING_BITNAME ;
 # define B_ASN1_TIME B_ASN1_UTCTIME | B_ASN1_GENERALIZEDTIME # define B_ASN1_PRINTABLE B_ASN1_NUMERICSTRING | B_ASN1_PRINTABLESTRING | B_ASN1_T61STRING | B_ASN1_IA5STRING | B_ASN1_BIT_STRING | B_ASN1_UNIVERSALSTRING | B_ASN1_BMPSTRING | B_ASN1_UTF8STRING | B_ASN1_SEQUENCE | B_ASN1_UNKNOWN # define B_ASN1_DIRECTORYSTRING B_ASN1_PRINTABLESTRING | B_ASN1_TELETEXSTRING | B_ASN1_BMPSTRING | B_ASN1_UNIVERSALSTRING | B_ASN1_UTF8STRING # define B_ASN1_DISPLAYTEXT B_ASN1_IA5STRING | B_ASN1_VISIBLESTRING | B_ASN1_BMPSTRING | B_ASN1_UTF8STRING DECLARE_ASN1_FUNCTIONS_fname ( ASN1_TYPE , ASN1_ANY , ASN1_TYPE )