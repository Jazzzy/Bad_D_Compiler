#include "lexicalHelper.h"
#include "../DLang/D_DEFINE_NON_RESERVED_WORDS.h"
#include "../DLang/D_DEFINE_RESERVED_WORDS.h"
#include "string.h"

void fillComponentStr(char *str, int component) {

    switch (component) {
        case (IDENTIFIER):
            strcpy(str, "IDENTIFIER");
            break;
        case (BINARY_LITERAL):
            strcpy(str, "BINARY_LITERAL");
            break;
        case (FLOAT_LITERAL):
            strcpy(str, "FLOAT_LITERAL");
            break;
        case (INTEGER_LITERAL):
            strcpy(str, "INTEGER_LITERAL");
            break;
        case (STRING_LITERAL):
            strcpy(str, "STRING_LITERAL");
            break;
        case (CHARACTER_LITERAL):
            strcpy(str, "CHARACTER_LITERAL");
            break;
        case (END_OF_FILE):
            strcpy(str, "END_OF_FILE");
            break;
        case (NESTED_COMMENT):
            strcpy(str, "NESTED_COMMENT");
            break;
            break;
        case (DOCUMENTATION_COMMENT):
            strcpy(str, "DOCUMENTATION_COMMENT");
            break;
        case (OPE_SLASH_EQ):
            strcpy(str, "OPE_SLASH_EQ");
            break;
        case (OPE_TWO_POINTS):
            strcpy(str, "OPE_TWO_POINTS");
            break;
        case (OPE_THREE_POINTS):
            strcpy(str, "OPE_THREE_POINTS");
            break;
        case (OPE_AND_EQ):
            strcpy(str, "OPE_AND_EQ");
            break;
        case (OPE_AND_AND):
            strcpy(str, "OPE_AND_AND");
            break;
        case (OPE_VERT_EQ):
            strcpy(str, "OPE_VERT_EQ");
            break;
        case (OPE_VERT_VERT):
            strcpy(str, "OPE_VERT_VERT");
            break;
        case (OPE_MINUS_EQ):
            strcpy(str, "OPE_MINUS_EQ");
            break;
        case (OPE_MINUS_MINUS):
            strcpy(str, "OPE_MINUS_MINUS");
            break;
        case (OPE_PLUS_EQ):
            strcpy(str, "OPE_PLUS_EQ");
            break;
        case (OPE_PLUS_PLUS):
            strcpy(str, "OPE_PLUS_PLUS");
            break;
        case (OPE_LESSTHAN_EQ):
            strcpy(str, "OPE_LESSTHAN_EQ");
            break;
        case (OPE_LESSTHAN_LESSTHAN):
            strcpy(str, "OPE_LESSTHAN_LESSTHAN");
            break;
        case (OPE_LESSTHAN_LESSTHAN_EQ):
            strcpy(str, "OPE_LESSTHAN_LESSTHAN_EQ");
            break;
        case (OPE_LESSTHAN_MORETHAN):
            strcpy(str, "OPE_LESSTHAN_MORETHAN");
            break;
        case (OPE_LESSTHAN_MORETHAN_EQ):
            strcpy(str, "OPE_LESSTHAN_MORETHAN_EQ");
            break;
        case (OPE_MORETHAN_EQ):
            strcpy(str, "OPE_MORETHAN_EQ");
            break;
        case (OPE_MORETHAN_MORETHAN_EQ):
            strcpy(str, "OPE_MORETHAN_MORETHAN_EQ");
            break;
        case (OPE_MORETHAN_MORETHAN_MORETHAN_EQ):
            strcpy(str, "OPE_MORETHAN_MORETHAN_MORETHAN_EQ");
            break;
        case (OPE_MORETHAN_MORETHAN):
            strcpy(str, "OPE_MORETHAN_MORETHAN");
            break;
        case (OPE_MORETHAN_MORETHAN_MORETHAN):
            strcpy(str, "OPE_MORETHAN_MORETHAN_MORETHAN");
            break;
        case (OPE_EXCL_EQ):
            strcpy(str, "OPE_EXCL_EQ");
            break;
        case (OPE_EXCL_LESSTHAN_MORETHAN):
            strcpy(str, "OPE_EXCL_LESSTHAN_MORETHAN");
            break;
        case (OPE_EXCL_LESSTHAN_MORETHAN_EQ):
            strcpy(str, "OPE_EXCL_LESSTHAN_MORETHAN_EQ");
            break;
        case (OPE_EXCL_LESSTHAN):
            strcpy(str, "OPE_EXCL_LESSTHAN");
            break;
        case (OPE_EXCL_LESSTHAN_EQ):
            strcpy(str, "OPE_EXCL_LESSTHAN_EQ");
            break;
        case (OPE_EXCL_MORETHAN):
            strcpy(str, "OPE_EXCL_MORETHAN");
            break;
        case (OPE_EXCL_MORETHAN_EQ):
            strcpy(str, "OPE_EXCL_MORETHAN_EQ");
            break;
        case (OPE_EQ_EQ):
            strcpy(str, "OPE_EQ_EQ");
            break;
        case (OPE_TIMES_EQ):
            strcpy(str, "OPE_TIMES_EQ");
            break;
        case (OPE_PERC_EQ):
            strcpy(str, "OPE_PERC_EQ");
            break;
        case (OPE_HAT_EQ):
            strcpy(str, "OPE_HAT_EQ");
            break;
        case (OPE_HAT_HAT):
            strcpy(str, "OPE_HAT_HAT");
            break;
        case (OPE_HAT_HAT_EQ):
            strcpy(str, "OPE_HAT_HAT_EQ");
            break;
        case (OPE_VIRG_EQ):
            strcpy(str, "OPE_VIRG_EQ");
            break;
        case (OPE_EQ_MORE):
            strcpy(str, "OPE_EQ_MORE");
            break;
        case ('/'):
            strcpy(str, "OPE: /");
            break;
        case ('.'):
            strcpy(str, "OPE: .");
            break;
        case ('&'):
            strcpy(str, "OPE: &");
            break;
        case ('|'):
            strcpy(str, "OPE: |");
            break;
        case ('-'):
            strcpy(str, "OPE: -");
            break;
        case ('+'):
            strcpy(str, "OPE: +");
            break;
        case ('<'):
            strcpy(str, "OPE: <");
            break;
        case ('>'):
            strcpy(str, "OPE: >");
            break;
        case ('!'):
            strcpy(str, "OPE: !");
            break;
        case ('('):
            strcpy(str, "OPE: (");
            break;
        case (')'):
            strcpy(str, "OPE: )");
            break;
        case ('['):
            strcpy(str, "OPE: [");
            break;
        case (']'):
            strcpy(str, "OPE: ]");
            break;
        case ('{'):
            strcpy(str, "OPE: {");
            break;
        case ('}'):
            strcpy(str, "OPE: }");
            break;
        case ('?'):
            strcpy(str, "OPE: ?");
            break;
        case (','):
            strcpy(str, "OPE: ,");
            break;
        case (';'):
            strcpy(str, "OPE: ;");
            break;
        case (':'):
            strcpy(str, "OPE: :");
            break;
        case ('$'):
            strcpy(str, "OPE: $");
            break;
        case ('='):
            strcpy(str, "OPE: =");
            break;
        case ('*'):
            strcpy(str, "OPE: *");
            break;
        case ('%'):
            strcpy(str, "OPE: %");
            break;
        case ('^'):
            strcpy(str, "OPE: ^");
            break;
        case ('~'):
            strcpy(str, "OPE: ~");
            break;
        case ('@'):
            strcpy(str, "OPE: @");
            break;
        case ('#'):
            strcpy(str, "OPE: #");
            break;
        case (ABSTRACT):
            strcpy(str, "RESERVED WORD: ABSTRACT");
            break;
        case (ALIAS):
            strcpy(str, "RESERVED WORD: ALIAS");
            break;
        case (ALIGN):
            strcpy(str, "RESERVED WORD: ALIGN");
            break;
        case (ASM):
            strcpy(str, "RESERVED WORD: ASM");
            break;
        case (AUTO):
            strcpy(str, "RESERVED WORD: AUTO");
            break;
        case (BODY):
            strcpy(str, "RESERVED WORD: BODY");
            break;
        case (BOOL):
            strcpy(str, "RESERVED WORD: BOOL");
            break;
        case (BREAK):
            strcpy(str, "RESERVED WORD: BREAK");
            break;
        case (BYTE):
            strcpy(str, "RESERVED WORD: BYTE");
            break;
        case (CASE):
            strcpy(str, "RESERVED WORD: CASE");
            break;
        case (CAST):
            strcpy(str, "RESERVED WORD: CAST");
            break;
        case (CATCH):
            strcpy(str, "RESERVED WORD: CATCH");
            break;
        case (CDOUBLE):
            strcpy(str, "RESERVED WORD: CDOUBLE");
            break;
        case (CENT):
            strcpy(str, "RESERVED WORD: CENT");
            break;
        case (CFLOAT):
            strcpy(str, "RESERVED WORD: CFLOAT");
            break;
        case (CHAR):
            strcpy(str, "RESERVED WORD: CHAR");
            break;
        case (CLASS):
            strcpy(str, "RESERVED WORD: CLASS");
            break;
        case (CONST):
            strcpy(str, "RESERVED WORD: CONST");
            break;
        case (CONTINUE):
            strcpy(str, "RESERVED WORD: CONTINUE");
            break;
        case (CREAL):
            strcpy(str, "RESERVED WORD: CREAL");
            break;
        case (DCHAR):
            strcpy(str, "RESERVED WORD: DCHAR");
            break;
        case (DEBUG):
            strcpy(str, "RESERVED WORD: DEBUG");
            break;
        case (DEFAULT):
            strcpy(str, "RESERVED WORD: DEFAULT");
            break;
        case (DELEGATE):
            strcpy(str, "RESERVED WORD: DELEGATE");
            break;
        case (DELETE):
            strcpy(str, "RESERVED WORD: DELETE");
            break;
        case (DEPRECATED):
            strcpy(str, "RESERVED WORD: DEPRECATED");
            break;
        case (DO):
            strcpy(str, "RESERVED WORD: DO");
            break;
        case (DOUBLE):
            strcpy(str, "RESERVED WORD: DOUBLE");
            break;
        case (ELSE):
            strcpy(str, "RESERVED WORD: ELSE");
            break;
        case (ENUM):
            strcpy(str, "RESERVED WORD: ENUM");
            break;
        case (EXPORT):
            strcpy(str, "RESERVED WORD: EXPORT");
            break;
        case (EXTERN):
            strcpy(str, "RESERVED WORD: EXTERN");
            break;
        case (FALSE):
            strcpy(str, "RESERVED WORD: FALSE");
            break;
        case (FINAL):
            strcpy(str, "RESERVED WORD: FINAL");
            break;
        case (FINALLY):
            strcpy(str, "RESERVED WORD: FINALLY");
            break;
        case (FLOAT):
            strcpy(str, "RESERVED WORD: FLOAT");
            break;
        case (FOR):
            strcpy(str, "RESERVED WORD: FOR");
            break;
        case (FOREACH):
            strcpy(str, "RESERVED WORD: FOREACH");
            break;
        case (FOREACH_REVERSE):
            strcpy(str, "RESERVED WORD: FOREACH_REVERSE");
            break;
        case (FUNCTION):
            strcpy(str, "RESERVED WORD: FUNCTION");
            break;
        case (GOTO):
            strcpy(str, "RESERVED WORD: GOTO");
            break;
        case (IDOUBLE):
            strcpy(str, "RESERVED WORD: IDOUBLE");
            break;
        case (IF):
            strcpy(str, "RESERVED WORD: IF");
            break;
        case (IFLOAT):
            strcpy(str, "RESERVED WORD: IFLOAT");
            break;
        case (IMMUTABLE):
            strcpy(str, "RESERVED WORD: IMMUTABLE");
            break;
        case (IMPORT):
            strcpy(str, "RESERVED WORD: IMPORT");
            break;
        case (IN):
            strcpy(str, "RESERVED WORD: IN");
            break;
        case (INOUT):
            strcpy(str, "RESERVED WORD: INOUT");
            break;
        case (INT):
            strcpy(str, "RESERVED WORD: INT");
            break;
        case (INTERFACE):
            strcpy(str, "RESERVED WORD: INTERFACE");
            break;
        case (INVARIANT):
            strcpy(str, "RESERVED WORD: INVARIANT");
            break;
        case (IREAL):
            strcpy(str, "RESERVED WORD: IREAL");
            break;
        case (IS):
            strcpy(str, "RESERVED WORD: IS");
            break;
        case (LAZY):
            strcpy(str, "RESERVED WORD: LAZY");
            break;
        case (LONG):
            strcpy(str, "RESERVED WORD: LONG");
            break;
        case (MACRO):
            strcpy(str, "RESERVED WORD: MACRO");
            break;
        case (MIXIN):
            strcpy(str, "RESERVED WORD: MIXIN");
            break;
        case (MODULE):
            strcpy(str, "RESERVED WORD: MODULE");
            break;
        case (NEW):
            strcpy(str, "RESERVED WORD: NEW");
            break;
        case (NOTHROW):
            strcpy(str, "RESERVED WORD: NOTHROW");
            break;
        case (DEF_NULL):
            strcpy(str, "RESERVED WORD: DEF_NULL");
            break;
        case (OUT):
            strcpy(str, "RESERVED WORD: OUT");
            break;
        case (OVERRIDE):
            strcpy(str, "RESERVED WORD: OVERRIDE");
            break;
        case (PACKAGE):
            strcpy(str, "RESERVED WORD: PACKAGE");
            break;
        case (PRAGMA):
            strcpy(str, "RESERVED WORD: PRAGMA");
            break;
        case (PRIVATE):
            strcpy(str, "RESERVED WORD: PRIVATE");
            break;
        case (PROTECTED):
            strcpy(str, "RESERVED WORD: PROTECTED");
            break;
        case (PUBLIC):
            strcpy(str, "RESERVED WORD: PUBLIC");
            break;
        case (PURE):
            strcpy(str, "RESERVED WORD: PURE");
            break;
        case (REAL):
            strcpy(str, "RESERVED WORD: REAL");
            break;
        case (REF):
            strcpy(str, "RESERVED WORD: REF");
            break;
        case (RETURN):
            strcpy(str, "RESERVED WORD: RETURN");
            break;
        case (SCOPE):
            strcpy(str, "RESERVED WORD: SCOPE");
            break;
        case (SHARED):
            strcpy(str, "RESERVED WORD: SHARED");
            break;
        case (SHORT):
            strcpy(str, "RESERVED WORD: SHORT");
            break;
        case (STATIC):
            strcpy(str, "RESERVED WORD: STATIC");
            break;
        case (STRUCT):
            strcpy(str, "RESERVED WORD: STRUCT");
            break;
        case (SUPER):
            strcpy(str, "RESERVED WORD: SUPER");
            break;
        case (SWITCH):
            strcpy(str, "RESERVED WORD: SWITCH");
            break;
        case (SYNCHRONIZED):
            strcpy(str, "RESERVED WORD: SYNCHRONIZED");
            break;
        case (TEMPLATE):
            strcpy(str, "RESERVED WORD: TEMPLATE");
            break;
        case (THIS):
            strcpy(str, "RESERVED WORD: THIS");
            break;
        case (THROW):
            strcpy(str, "RESERVED WORD: THROW");
            break;
        case (TRUE):
            strcpy(str, "RESERVED WORD: TRUE");
            break;
        case (TRY):
            strcpy(str, "RESERVED WORD: TRY");
            break;
        case (TYPEDEF):
            strcpy(str, "RESERVED WORD: TYPEDEF");
            break;
        case (TYPEID):
            strcpy(str, "RESERVED WORD: TYPEID");
            break;
        case (TYPEOF):
            strcpy(str, "RESERVED WORD: TYPEOF");
            break;
        case (UBYTE):
            strcpy(str, "RESERVED WORD: UBYTE");
            break;
        case (UCENT):
            strcpy(str, "RESERVED WORD: UCENT");
            break;
        case (UINT):
            strcpy(str, "RESERVED WORD: UINT");
            break;
        case (ULONG):
            strcpy(str, "RESERVED WORD: ULONG");
            break;
        case (UNION):
            strcpy(str, "RESERVED WORD: UNION");
            break;
        case (UNITTEST):
            strcpy(str, "RESERVED WORD: UNITTEST");
            break;
        case (USHORT):
            strcpy(str, "RESERVED WORD: USHORT");
            break;
        case (VERSION):
            strcpy(str, "RESERVED WORD: VERSION");
            break;
        case (VOID):
            strcpy(str, "RESERVED WORD: VOID");
            break;
        case (VOLATILE):
            strcpy(str, "RESERVED WORD: VOLATILE");
            break;
        case (WCHAR):
            strcpy(str, "RESERVED WORD: WCHAR");
            break;
        case (WHILE):
            strcpy(str, "RESERVED WORD: WHILE");
            break;
        case (WITH):
            strcpy(str, "RESERVED WORD: WITH");
            break;
        case (DEF___FILE__):
            strcpy(str, "RESERVED WORD: DEF___FILE__");
            break;
        case (__FILE_FULL_PATH__):
            strcpy(str, "RESERVED WORD: __FILE_FULL_PATH__");
            break;
        case (__MODULE__):
            strcpy(str, "RESERVED WORD: __MODULE__");
            break;
        case (DEF___LINE__):
            strcpy(str, "RESERVED WORD: DEF___LINE__");
            break;
        case (__FUNCTION__):
            strcpy(str, "RESERVED WORD: __FUNCTION__");
            break;
        case (__PRETTY_FUNCTION__):
            strcpy(str, "RESERVED WORD: __PRETTY_FUNCTION__");
            break;
        case (__GSHARED):
            strcpy(str, "RESERVED WORD: __GSHARED");
            break;
        case (__TRAITS):
            strcpy(str, "RESERVED WORD: __TRAITS");
            break;
        case (__VECTOR):
            strcpy(str, "RESERVED WORD: __VECTOR");
            break;
        case (__PARAMETERS):
            strcpy(str, "RESERVED WORD: __PARAMETERS");
            break;
        default:
            strcpy(str, "NOT_KNOWN");
            break;
    }

    return;

}
