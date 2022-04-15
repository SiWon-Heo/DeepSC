static PyObject * get_value ( PyObject * self , const char * buffer , int * position , int type , int max , PyObject * as_class , unsigned char tz_aware , unsigned char uuid_subtype ) {
 struct module_state * state = GETSTATE ( self ) ;
 PyObject * value ;
 PyObject * error ;
 switch ( type ) {
 case 1 : {
 double d ;
 if ( max < 8 ) {
 goto invalid ;
 }
 memcpy ( & d , buffer + * position , 8 ) ;
 value = PyFloat_FromDouble ( d ) ;
 if ( ! value ) {
 return NULL ;
 }
 * position += 8 ;
 break ;
 }
 case 2 : case 14 : {
 int value_length = ( ( int * ) ( buffer + * position ) ) [ 0 ] - 1 ;
 if ( max < value_length ) {
 goto invalid ;
 }
 * position += 4 ;
 value = PyUnicode_DecodeUTF8 ( buffer + * position , value_length , "strict" ) ;
 if ( ! value ) {
 return NULL ;
 }
 * position += value_length + 1 ;
 break ;
 }
 case 3 : {
 int size ;
 memcpy ( & size , buffer + * position , 4 ) ;
 if ( max < size ) {
 goto invalid ;
 }
 value = elements_to_dict ( self , buffer + * position + 4 , size - 5 , as_class , tz_aware , uuid_subtype ) ;
 if ( ! value ) {
 return NULL ;
 }
 if ( strcmp ( buffer + * position + 5 , "$ref" ) == 0 ) {
 PyObject * dbref ;
 PyObject * collection = PyDict_GetItemString ( value , "$ref" ) ;
 PyObject * id = PyDict_GetItemString ( value , "$id" ) ;
 PyObject * database = PyDict_GetItemString ( value , "$db" ) ;
 Py_INCREF ( collection ) ;
 PyDict_DelItemString ( value , "$ref" ) ;
 Py_INCREF ( id ) ;
 PyDict_DelItemString ( value , "$id" ) ;
 if ( database == NULL ) {
 database = Py_None ;
 Py_INCREF ( database ) ;
 }
 else {
 Py_INCREF ( database ) ;
 PyDict_DelItemString ( value , "$db" ) ;
 }
 dbref = PyObject_CallFunctionObjArgs ( state -> DBRef , collection , id , database , value , NULL ) ;
 Py_DECREF ( value ) ;
 value = dbref ;
 Py_DECREF ( id ) ;
 Py_DECREF ( collection ) ;
 Py_DECREF ( database ) ;
 if ( ! value ) {
 return NULL ;
 }
 }
 * position += size ;
 break ;
 }
 case 4 : {
 int size , end ;
 memcpy ( & size , buffer + * position , 4 ) ;
 if ( max < size ) {
 goto invalid ;
 }
 end = * position + size - 1 ;
 * position += 4 ;
 value = PyList_New ( 0 ) ;
 if ( ! value ) {
 return NULL ;
 }
 while ( * position < end ) {
 PyObject * to_append ;
 int type = ( int ) buffer [ ( * position ) ++ ] ;
 int key_size = strlen ( buffer + * position ) ;
 * position += key_size + 1 ;
 to_append = get_value ( self , buffer , position , type , max - key_size , as_class , tz_aware , uuid_subtype ) ;
 if ( ! to_append ) {
 Py_DECREF ( value ) ;
 return NULL ;
 }
 PyList_Append ( value , to_append ) ;
 Py_DECREF ( to_append ) ;
 }
 ( * position ) ++ ;
 break ;
 }
 case 5 : {
 PyObject * data ;
 PyObject * st ;
 int length , subtype ;
 memcpy ( & length , buffer + * position , 4 ) ;
 if ( max < length ) {
 goto invalid ;
 }
 subtype = ( unsigned char ) buffer [ * position + 4 ] ;
 # if PY_MAJOR_VERSION >= 3 if ( subtype == 0 ) {
 value = PyBytes_FromStringAndSize ( buffer + * position + 5 , length ) ;
 * position += length + 5 ;
 break ;
 }
 if ( subtype == 2 ) {
 data = PyBytes_FromStringAndSize ( buffer + * position + 9 , length - 4 ) ;
 }
 else {
 data = PyBytes_FromStringAndSize ( buffer + * position + 5 , length ) ;
 }
 # else if ( subtype == 2 ) {
 data = PyString_FromStringAndSize ( buffer + * position + 9 , length - 4 ) ;
 }
 else {
 data = PyString_FromStringAndSize ( buffer + * position + 5 , length ) ;
 }
 # endif if ( ! data ) {
 return NULL ;
 }
 if ( ( subtype == 3 || subtype == 4 ) && state -> UUID ) {
 PyObject * kwargs ;
 PyObject * args = PyTuple_New ( 0 ) ;
 if ( ! args ) {
 Py_DECREF ( data ) ;
 return NULL ;
 }
 kwargs = PyDict_New ( ) ;
 if ( ! kwargs ) {
 Py_DECREF ( data ) ;
 Py_DECREF ( args ) ;
 return NULL ;
 }
 assert ( length == 16 ) ;
 if ( uuid_subtype == CSHARP_LEGACY ) {
 PyDict_SetItemString ( kwargs , "bytes_le" , data ) ;
 }
 else {
 if ( uuid_subtype == JAVA_LEGACY ) {
 char big_endian [ 16 ] ;
 _fix_java ( buffer + * position + 5 , big_endian ) ;
 Py_DECREF ( data ) ;
 # if PY_MAJOR_VERSION >= 3 data = PyBytes_FromStringAndSize ( big_endian , length ) ;
 # else data = PyString_FromStringAndSize ( big_endian , length ) ;
 # endif }
 PyDict_SetItemString ( kwargs , "bytes" , data ) ;
 }
 value = PyObject_Call ( state -> UUID , args , kwargs ) ;
 Py_DECREF ( args ) ;
 Py_DECREF ( kwargs ) ;
 Py_DECREF ( data ) ;
 if ( ! value ) {
 return NULL ;
 }
 * position += length + 5 ;
 break ;
 }
 # if PY_MAJOR_VERSION >= 3 st = PyLong_FromLong ( subtype ) ;
 # else st = PyInt_FromLong ( subtype ) ;
 # endif if ( ! st ) {
 Py_DECREF ( data ) ;
 return NULL ;
 }
 value = PyObject_CallFunctionObjArgs ( state -> Binary , data , st , NULL ) ;
 Py_DECREF ( st ) ;
 Py_DECREF ( data ) ;
 if ( ! value ) {
 return NULL ;
 }
 * position += length + 5 ;
 break ;
 }
 case 6 : case 10 : {
 value = Py_None ;
 Py_INCREF ( value ) ;
 break ;
 }
 case 7 : {
 if ( max < 12 ) {
 goto invalid ;
 }
 # if PY_MAJOR_VERSION >= 3 value = PyObject_CallFunction ( state -> ObjectId , "y#" , buffer + * position , 12 ) ;
 # else value = PyObject_CallFunction ( state -> ObjectId , "s#" , buffer + * position , 12 ) ;
 # endif if ( ! value ) {
 return NULL ;
 }
 * position += 12 ;
 break ;
 }
 case 8 : {
 value = buffer [ ( * position ) ++ ] ? Py_True : Py_False ;
 Py_INCREF ( value ) ;
 break ;
 }
 case 9 : {
 PyObject * naive ;
 PyObject * replace ;
 PyObject * args ;
 PyObject * kwargs ;
 if ( max < 8 ) {
 goto invalid ;
 }
 naive = datetime_from_millis ( * ( long long * ) ( buffer + * position ) ) ;
 * position += 8 ;
 if ( ! tz_aware ) {
 value = naive ;
 break ;
 }
 if ( ! naive ) {
 return NULL ;
 }
 replace = PyObject_GetAttrString ( naive , "replace" ) ;
 Py_DECREF ( naive ) ;
 if ( ! replace ) {
 return NULL ;
 }
 args = PyTuple_New ( 0 ) ;
 if ( ! args ) {
 Py_DECREF ( replace ) ;
 return NULL ;
 }
 kwargs = PyDict_New ( ) ;
 if ( ! kwargs ) {
 Py_DECREF ( replace ) ;
 Py_DECREF ( args ) ;
 return NULL ;
 }
 if ( PyDict_SetItemString ( kwargs , "tzinfo" , state -> UTC ) == - 1 ) {
 Py_DECREF ( replace ) ;
 Py_DECREF ( args ) ;
 Py_DECREF ( kwargs ) ;
 return NULL ;
 }
 value = PyObject_Call ( replace , args , kwargs ) ;
 Py_DECREF ( replace ) ;
 Py_DECREF ( args ) ;
 Py_DECREF ( kwargs ) ;
 break ;
 }
 case 11 : {
 PyObject * pattern ;
 int flags_length , flags , i ;
 int pattern_length = strlen ( buffer + * position ) ;
 if ( max < pattern_length ) {
 goto invalid ;
 }
 pattern = PyUnicode_DecodeUTF8 ( buffer + * position , pattern_length , "strict" ) ;
 if ( ! pattern ) {
 return NULL ;
 }
 * position += pattern_length + 1 ;
 flags_length = strlen ( buffer + * position ) ;
 if ( max < pattern_length + flags_length ) {
 Py_DECREF ( pattern ) ;
 goto invalid ;
 }
 flags = 0 ;
 for ( i = 0 ;
 i < flags_length ;
 i ++ ) {
 if ( buffer [ * position + i ] == 'i' ) {
 flags |= 2 ;
 }
 else if ( buffer [ * position + i ] == 'l' ) {
 flags |= 4 ;
 }
 else if ( buffer [ * position + i ] == 'm' ) {
 flags |= 8 ;
 }
 else if ( buffer [ * position + i ] == 's' ) {
 flags |= 16 ;
 }
 else if ( buffer [ * position + i ] == 'u' ) {
 flags |= 32 ;
 }
 else if ( buffer [ * position + i ] == 'x' ) {
 flags |= 64 ;
 }
 }
 * position += flags_length + 1 ;
 value = PyObject_CallFunction ( state -> RECompile , "Oi" , pattern , flags ) ;
 Py_DECREF ( pattern ) ;
 break ;
 }
 case 12 : {
 int collection_length ;
 PyObject * collection ;
 PyObject * id ;
 * position += 4 ;
 collection_length = strlen ( buffer + * position ) ;
 if ( max < collection_length ) {
 goto invalid ;
 }
 collection = PyUnicode_DecodeUTF8 ( buffer + * position , collection_length , "strict" ) ;
 if ( ! collection ) {
 return NULL ;
 }
 * position += collection_length + 1 ;
 if ( max < collection_length + 12 ) {
 Py_DECREF ( collection ) ;
 goto invalid ;
 }
 id = PyObject_CallFunction ( state -> ObjectId , "s#" , buffer + * position , 12 ) ;
 if ( ! id ) {
 Py_DECREF ( collection ) ;
 return NULL ;
 }
 * position += 12 ;
 value = PyObject_CallFunctionObjArgs ( state -> DBRef , collection , id , NULL ) ;
 Py_DECREF ( collection ) ;
 Py_DECREF ( id ) ;
 break ;
 }
 case 13 : {
 PyObject * code ;
 int value_length = ( ( int * ) ( buffer + * position ) ) [ 0 ] - 1 ;
 if ( max < value_length ) {
 goto invalid ;
 }
 * position += 4 ;
 code = PyUnicode_DecodeUTF8 ( buffer + * position , value_length , "strict" ) ;
 if ( ! code ) {
 return NULL ;
 }
 * position += value_length + 1 ;
 value = PyObject_CallFunctionObjArgs ( state -> Code , code , NULL , NULL ) ;
 Py_DECREF ( code ) ;
 break ;
 }
 case 15 : {
 int code_length , scope_size ;
 PyObject * code ;
 PyObject * scope ;
 * position += 8 ;
 code_length = strlen ( buffer + * position ) ;
 if ( max < 8 + code_length ) {
 goto invalid ;
 }
 code = PyUnicode_DecodeUTF8 ( buffer + * position , code_length , "strict" ) ;
 if ( ! code ) {
 return NULL ;
 }
 * position += code_length + 1 ;
 memcpy ( & scope_size , buffer + * position , 4 ) ;
 scope = elements_to_dict ( self , buffer + * position + 4 , scope_size - 5 , ( PyObject * ) & PyDict_Type , tz_aware , uuid_subtype ) ;
 if ( ! scope ) {
 Py_DECREF ( code ) ;
 return NULL ;
 }
 * position += scope_size ;
 value = PyObject_CallFunctionObjArgs ( state -> Code , code , scope , NULL ) ;
 Py_DECREF ( code ) ;
 Py_DECREF ( scope ) ;
 break ;
 }
 case 16 : {
 int i ;
 if ( max < 4 ) {
 goto invalid ;
 }
 memcpy ( & i , buffer + * position , 4 ) ;
 # if PY_MAJOR_VERSION >= 3 value = PyLong_FromLong ( i ) ;
 # else value = PyInt_FromLong ( i ) ;
 # endif if ( ! value ) {
 return NULL ;
 }
 * position += 4 ;
 break ;
 }
 case 17 : {
 unsigned int time , inc ;
 if ( max < 8 ) {
 goto invalid ;
 }
 memcpy ( & inc , buffer + * position , 4 ) ;
 memcpy ( & time , buffer + * position + 4 , 4 ) ;
 value = PyObject_CallFunction ( state -> Timestamp , "II" , time , inc ) ;
 if ( ! value ) {
 return NULL ;
 }
 * position += 8 ;
 break ;
 }
 case 18 : {
 long long ll ;
 if ( max < 8 ) {
 goto invalid ;
 }
 memcpy ( & ll , buffer + * position , 8 ) ;
 value = PyLong_FromLongLong ( ll ) ;
 if ( ! value ) {
 return NULL ;
 }
 * position += 8 ;
 break ;
 }
 case - 1 : {
 value = PyObject_CallFunctionObjArgs ( state -> MinKey , NULL ) ;
 break ;
 }
 case 127 : {
 value = PyObject_CallFunctionObjArgs ( state -> MaxKey , NULL ) ;
 break ;
 }
 default : {
 PyObject * InvalidDocument = _error ( "InvalidDocument" ) ;
 PyErr_SetString ( InvalidDocument , "no c decoder for this type yet" ) ;
 Py_DECREF ( InvalidDocument ) ;
 return NULL ;
 }
 }
 return value ;
 invalid : error = _error ( "InvalidBSON" ) ;
 PyErr_SetNone ( error ) ;
 Py_DECREF ( error ) ;
 return NULL ;
 }