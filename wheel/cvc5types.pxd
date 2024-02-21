cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5":
    cdef enum class UnknownExplanation "cvc5::UnknownExplanation":
       REQUIRES_FULL_CHECK,
       INCOMPLETE,
       TIMEOUT,
       RESOURCEOUT,
       MEMOUT,
       INTERRUPTED,
       UNSUPPORTED,
       OTHER,
       REQUIRES_CHECK_AGAIN,
       UNKNOWN_REASON,
cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5":
    cdef enum class RoundingMode "cvc5::RoundingMode":
       ROUND_NEAREST_TIES_TO_EVEN,
       ROUND_TOWARD_POSITIVE,
       ROUND_TOWARD_NEGATIVE,
       ROUND_TOWARD_ZERO,
       ROUND_NEAREST_TIES_TO_AWAY,
cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5::modes":
    cdef enum class BlockModelsMode "cvc5::modes::BlockModelsMode":
       LITERALS,
       VALUES,
cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5::modes":
    cdef enum class LearnedLitType "cvc5::modes::LearnedLitType":
       PREPROCESS_SOLVED,
       PREPROCESS,
       INPUT,
       SOLVABLE,
       CONSTANT_PROP,
       INTERNAL,
       UNKNOWN,
cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5::modes":
    cdef enum class ProofComponent "cvc5::modes::ProofComponent":
       RAW_PREPROCESS,
       PREPROCESS,
       SAT,
       THEORY_LEMMAS,
       FULL,
cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5::modes":
    cdef enum class ProofFormat "cvc5::modes::ProofFormat":
       NONE,
       DOT,
       LFSC,
       ALETHE,
       DEFAULT,
cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5::modes":
    cdef enum class FindSynthTarget "cvc5::modes::FindSynthTarget":
       ENUM,
       REWRITE,
       REWRITE_UNSOUND,
       REWRITE_INPUT,
       QUERY,
cdef extern from "<cvc5/cvc5_types.h>" namespace "cvc5::modes":
    cdef enum class InputLanguage "cvc5::modes::InputLanguage":
       SMT_LIB_2_6,
       SYGUS_2_1,
       UNKNOWN,
