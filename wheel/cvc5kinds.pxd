cdef extern from "<cvc5/cvc5_kind.h>" namespace "cvc5":
    cdef enum class Kind "cvc5::Kind":
       INTERNAL_KIND,
       UNDEFINED_KIND,
       NULL_TERM,
       UNINTERPRETED_SORT_VALUE,
       EQUAL,
       DISTINCT,
       CONSTANT,
       VARIABLE,
       SEXPR,
       LAMBDA,
       WITNESS,
       CONST_BOOLEAN,
       NOT,
       AND,
       IMPLIES,
       OR,
       XOR,
       ITE,
       APPLY_UF,
       CARDINALITY_CONSTRAINT,
       HO_APPLY,
       ADD,
       MULT,
       IAND,
       POW2,
       SUB,
       NEG,
       DIVISION,
       INTS_DIVISION,
       INTS_MODULUS,
       ABS,
       POW,
       EXPONENTIAL,
       SINE,
       COSINE,
       TANGENT,
       COSECANT,
       SECANT,
       COTANGENT,
       ARCSINE,
       ARCCOSINE,
       ARCTANGENT,
       ARCCOSECANT,
       ARCSECANT,
       ARCCOTANGENT,
       SQRT,
       DIVISIBLE,
       CONST_RATIONAL,
       CONST_INTEGER,
       LT,
       LEQ,
       GT,
       GEQ,
       IS_INTEGER,
       TO_INTEGER,
       TO_REAL,
       PI,
       CONST_BITVECTOR,
       BITVECTOR_CONCAT,
       BITVECTOR_AND,
       BITVECTOR_OR,
       BITVECTOR_XOR,
       BITVECTOR_NOT,
       BITVECTOR_NAND,
       BITVECTOR_NOR,
       BITVECTOR_XNOR,
       BITVECTOR_COMP,
       BITVECTOR_MULT,
       BITVECTOR_ADD,
       BITVECTOR_SUB,
       BITVECTOR_NEG,
       BITVECTOR_UDIV,
       BITVECTOR_UREM,
       BITVECTOR_SDIV,
       BITVECTOR_SREM,
       BITVECTOR_SMOD,
       BITVECTOR_SHL,
       BITVECTOR_LSHR,
       BITVECTOR_ASHR,
       BITVECTOR_ULT,
       BITVECTOR_ULE,
       BITVECTOR_UGT,
       BITVECTOR_UGE,
       BITVECTOR_SLT,
       BITVECTOR_SLE,
       BITVECTOR_SGT,
       BITVECTOR_SGE,
       BITVECTOR_ULTBV,
       BITVECTOR_SLTBV,
       BITVECTOR_ITE,
       BITVECTOR_REDOR,
       BITVECTOR_REDAND,
       BITVECTOR_NEGO,
       BITVECTOR_UADDO,
       BITVECTOR_SADDO,
       BITVECTOR_UMULO,
       BITVECTOR_SMULO,
       BITVECTOR_USUBO,
       BITVECTOR_SSUBO,
       BITVECTOR_SDIVO,
       BITVECTOR_EXTRACT,
       BITVECTOR_REPEAT,
       BITVECTOR_ZERO_EXTEND,
       BITVECTOR_SIGN_EXTEND,
       BITVECTOR_ROTATE_LEFT,
       BITVECTOR_ROTATE_RIGHT,
       INT_TO_BITVECTOR,
       BITVECTOR_TO_NAT,
       CONST_FINITE_FIELD,
       FINITE_FIELD_NEG,
       FINITE_FIELD_ADD,
       FINITE_FIELD_BITSUM,
       FINITE_FIELD_MULT,
       CONST_FLOATINGPOINT,
       CONST_ROUNDINGMODE,
       FLOATINGPOINT_FP,
       FLOATINGPOINT_EQ,
       FLOATINGPOINT_ABS,
       FLOATINGPOINT_NEG,
       FLOATINGPOINT_ADD,
       FLOATINGPOINT_SUB,
       FLOATINGPOINT_MULT,
       FLOATINGPOINT_DIV,
       FLOATINGPOINT_FMA,
       FLOATINGPOINT_SQRT,
       FLOATINGPOINT_REM,
       FLOATINGPOINT_RTI,
       FLOATINGPOINT_MIN,
       FLOATINGPOINT_MAX,
       FLOATINGPOINT_LEQ,
       FLOATINGPOINT_LT,
       FLOATINGPOINT_GEQ,
       FLOATINGPOINT_GT,
       FLOATINGPOINT_IS_NORMAL,
       FLOATINGPOINT_IS_SUBNORMAL,
       FLOATINGPOINT_IS_ZERO,
       FLOATINGPOINT_IS_INF,
       FLOATINGPOINT_IS_NAN,
       FLOATINGPOINT_IS_NEG,
       FLOATINGPOINT_IS_POS,
       FLOATINGPOINT_TO_FP_FROM_IEEE_BV,
       FLOATINGPOINT_TO_FP_FROM_FP,
       FLOATINGPOINT_TO_FP_FROM_REAL,
       FLOATINGPOINT_TO_FP_FROM_SBV,
       FLOATINGPOINT_TO_FP_FROM_UBV,
       FLOATINGPOINT_TO_UBV,
       FLOATINGPOINT_TO_SBV,
       FLOATINGPOINT_TO_REAL,
       SELECT,
       STORE,
       CONST_ARRAY,
       EQ_RANGE,
       APPLY_CONSTRUCTOR,
       APPLY_SELECTOR,
       APPLY_TESTER,
       APPLY_UPDATER,
       MATCH,
       MATCH_CASE,
       MATCH_BIND_CASE,
       TUPLE_PROJECT,
       NULLABLE_LIFT,
       SEP_NIL,
       SEP_EMP,
       SEP_PTO,
       SEP_STAR,
       SEP_WAND,
       SET_EMPTY,
       SET_UNION,
       SET_INTER,
       SET_MINUS,
       SET_SUBSET,
       SET_MEMBER,
       SET_SINGLETON,
       SET_INSERT,
       SET_CARD,
       SET_COMPLEMENT,
       SET_UNIVERSE,
       SET_COMPREHENSION,
       SET_CHOOSE,
       SET_IS_SINGLETON,
       SET_MAP,
       SET_FILTER,
       SET_FOLD,
       RELATION_JOIN,
       RELATION_PRODUCT,
       RELATION_TRANSPOSE,
       RELATION_TCLOSURE,
       RELATION_JOIN_IMAGE,
       RELATION_IDEN,
       RELATION_GROUP,
       RELATION_AGGREGATE,
       RELATION_PROJECT,
       BAG_EMPTY,
       BAG_UNION_MAX,
       BAG_UNION_DISJOINT,
       BAG_INTER_MIN,
       BAG_DIFFERENCE_SUBTRACT,
       BAG_DIFFERENCE_REMOVE,
       BAG_SUBBAG,
       BAG_COUNT,
       BAG_MEMBER,
       BAG_DUPLICATE_REMOVAL,
       BAG_MAKE,
       BAG_CARD,
       BAG_CHOOSE,
       BAG_IS_SINGLETON,
       BAG_FROM_SET,
       BAG_TO_SET,
       BAG_MAP,
       BAG_FILTER,
       BAG_FOLD,
       BAG_PARTITION,
       TABLE_PRODUCT,
       TABLE_PROJECT,
       TABLE_AGGREGATE,
       TABLE_JOIN,
       TABLE_GROUP,
       STRING_CONCAT,
       STRING_IN_REGEXP,
       STRING_LENGTH,
       STRING_SUBSTR,
       STRING_UPDATE,
       STRING_CHARAT,
       STRING_CONTAINS,
       STRING_INDEXOF,
       STRING_INDEXOF_RE,
       STRING_REPLACE,
       STRING_REPLACE_ALL,
       STRING_REPLACE_RE,
       STRING_REPLACE_RE_ALL,
       STRING_TO_LOWER,
       STRING_TO_UPPER,
       STRING_REV,
       STRING_TO_CODE,
       STRING_FROM_CODE,
       STRING_LT,
       STRING_LEQ,
       STRING_PREFIX,
       STRING_SUFFIX,
       STRING_IS_DIGIT,
       STRING_FROM_INT,
       STRING_TO_INT,
       CONST_STRING,
       STRING_TO_REGEXP,
       REGEXP_CONCAT,
       REGEXP_UNION,
       REGEXP_INTER,
       REGEXP_DIFF,
       REGEXP_STAR,
       REGEXP_PLUS,
       REGEXP_OPT,
       REGEXP_RANGE,
       REGEXP_REPEAT,
       REGEXP_LOOP,
       REGEXP_NONE,
       REGEXP_ALL,
       REGEXP_ALLCHAR,
       REGEXP_COMPLEMENT,
       SEQ_CONCAT,
       SEQ_LENGTH,
       SEQ_EXTRACT,
       SEQ_UPDATE,
       SEQ_AT,
       SEQ_CONTAINS,
       SEQ_INDEXOF,
       SEQ_REPLACE,
       SEQ_REPLACE_ALL,
       SEQ_REV,
       SEQ_PREFIX,
       SEQ_SUFFIX,
       CONST_SEQUENCE,
       SEQ_UNIT,
       SEQ_NTH,
       FORALL,
       EXISTS,
       VARIABLE_LIST,
       INST_PATTERN,
       INST_NO_PATTERN,
       INST_POOL,
       INST_ADD_TO_POOL,
       SKOLEM_ADD_TO_POOL,
       INST_ATTRIBUTE,
       INST_PATTERN_LIST,
       LAST_KIND,
cdef extern from "<cvc5/cvc5_kind.h>" namespace "cvc5":
    cdef enum class SortKind "cvc5::SortKind":
       INTERNAL_SORT_KIND,
       UNDEFINED_SORT_KIND,
       NULL_SORT,
       ABSTRACT_SORT,
       ARRAY_SORT,
       BAG_SORT,
       BOOLEAN_SORT,
       BITVECTOR_SORT,
       DATATYPE_SORT,
       FINITE_FIELD_SORT,
       FLOATINGPOINT_SORT,
       FUNCTION_SORT,
       INTEGER_SORT,
       REAL_SORT,
       REGLAN_SORT,
       ROUNDINGMODE_SORT,
       SEQUENCE_SORT,
       SET_SORT,
       STRING_SORT,
       TUPLE_SORT,
       NULLABLE_SORT,
       UNINTERPRETED_SORT,
       LAST_SORT_KIND,