
from cvc5types cimport UnknownExplanation as c_UnknownExplanation
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class UnknownExplanation(DocEnum):
    """The UnknownExplanation enum"""
    REQUIRES_FULL_CHECK=c_UnknownExplanation.REQUIRES_FULL_CHECK, """Full satisfiability check required (e.g., if only preprocessing was
performed)."""
    INCOMPLETE=c_UnknownExplanation.INCOMPLETE, """Incomplete theory solver."""
    TIMEOUT=c_UnknownExplanation.TIMEOUT, """Time limit reached."""
    RESOURCEOUT=c_UnknownExplanation.RESOURCEOUT, """Resource limit reached."""
    MEMOUT=c_UnknownExplanation.MEMOUT, """Memory limit reached."""
    INTERRUPTED=c_UnknownExplanation.INTERRUPTED, """Solver was interrupted."""
    UNSUPPORTED=c_UnknownExplanation.UNSUPPORTED, """Unsupported feature encountered."""
    OTHER=c_UnknownExplanation.OTHER, """Other reason."""
    REQUIRES_CHECK_AGAIN=c_UnknownExplanation.REQUIRES_CHECK_AGAIN, """Requires another satisfiability check"""
    UNKNOWN_REASON=c_UnknownExplanation.UNKNOWN_REASON, """No specific reason given."""

from cvc5types cimport RoundingMode as c_RoundingMode
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class RoundingMode(DocEnum):
    """The RoundingMode enum"""
    ROUND_NEAREST_TIES_TO_EVEN=c_RoundingMode.ROUND_NEAREST_TIES_TO_EVEN, """Round to the nearest even number.

If the two nearest floating-point numbers bracketing an unrepresentable
infinitely precise result are equally near, the one with an even least
significant digit will be delivered."""
    ROUND_TOWARD_POSITIVE=c_RoundingMode.ROUND_TOWARD_POSITIVE, """Round towards positive infinity (SMT-LIB: ``+oo``).

The result shall be the format's floating-point number (possibly ``+oo``)
closest to and no less than the infinitely precise result."""
    ROUND_TOWARD_NEGATIVE=c_RoundingMode.ROUND_TOWARD_NEGATIVE, """Round towards negative infinity (``-oo``).

The result shall be the format's floating-point number (possibly ``-oo``)
closest to and no less than the infinitely precise result."""
    ROUND_TOWARD_ZERO=c_RoundingMode.ROUND_TOWARD_ZERO, """Round towards zero.

The result shall be the format's floating-point number closest to and no
greater in magnitude than the infinitely precise result."""
    ROUND_NEAREST_TIES_TO_AWAY=c_RoundingMode.ROUND_NEAREST_TIES_TO_AWAY, """Round to the nearest number away from zero.

If the two nearest floating-point numbers bracketing an unrepresentable
infinitely precise result are equally near), the one with larger magnitude
will be selected."""

from cvc5types cimport BlockModelsMode as c_BlockModelsMode
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class BlockModelsMode(DocEnum):
    """The BlockModelsMode enum"""
    LITERALS=c_BlockModelsMode.LITERALS, """Block models based on the SAT skeleton."""
    VALUES=c_BlockModelsMode.VALUES, """Block models based on the concrete model values for the free variables."""

from cvc5types cimport LearnedLitType as c_LearnedLitType
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class LearnedLitType(DocEnum):
    """The LearnedLitType enum"""
    PREPROCESS_SOLVED=c_LearnedLitType.PREPROCESS_SOLVED, """An equality that was turned into a substitution during preprocessing.

In particular, literals in this category are of the form (= x t) where
x does not occur in t."""
    PREPROCESS=c_LearnedLitType.PREPROCESS, """A top-level literal (unit clause) from the preprocessed set of input
formulas."""
    INPUT=c_LearnedLitType.INPUT, """A literal from the preprocessed set of input formulas that does not
occur at top-level after preprocessing.

Typically), this is the most interesting category of literals to learn."""
    SOLVABLE=c_LearnedLitType.SOLVABLE, """An internal literal that is solvable for an input variable.

In particular, literals in this category are of the form (= x t) where
x does not occur in t, the preprocessed set of input formulas contains the
term x, but not the literal (= x t).

Note that solvable literals can be turned into substitutions during
preprocessing."""
    CONSTANT_PROP=c_LearnedLitType.CONSTANT_PROP, """An internal literal that can be made into a constant propagation for an
input term.

In particular, literals in this category are of the form (= t c) where
c is a constant, the preprocessed set of input formulas contains the
term t, but not the literal (= t c)."""
    INTERNAL=c_LearnedLitType.INTERNAL, """Any internal literal that does not fall into the above categories."""
    UNKNOWN=c_LearnedLitType.UNKNOWN, """Special case for when produce-learned-literals is not set."""

from cvc5types cimport ProofComponent as c_ProofComponent
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class ProofComponent(DocEnum):
    """The ProofComponent enum"""
    RAW_PREPROCESS=c_ProofComponent.RAW_PREPROCESS, """Proofs of G1 ... Gn whose free assumptions are a subset of
F1, ... Fm, where:
- G1, ... Gn are the preprocessed input formulas,
- F1, ... Fm are the input formulas.

Note that G1 ... Gn may be arbitrary formulas, not necessarily clauses."""
    PREPROCESS=c_ProofComponent.PREPROCESS, """Proofs of Gu1 ... Gun whose free assumptions are Fu1, ... Fum,
where:
- Gu1, ... Gun are clauses corresponding to input formulas used in the SAT
proof,
- Fu1, ... Fum is the subset of the input formulas that are used in the SAT
proof (i.e. the unsat core).

Note that Gu1 ... Gun are clauses that are added to the SAT solver before
its main search.

Only valid immediately after an unsat response."""
    SAT=c_ProofComponent.SAT, """A proof of false whose free assumptions are Gu1, ... Gun, L1 ... Lk,
where:
- Gu1, ... Gun, is a set of clauses corresponding to input formulas,
- L1, ..., Lk is a set of clauses corresponding to theory lemmas.

Only valid immediately after an unsat response."""
    THEORY_LEMMAS=c_ProofComponent.THEORY_LEMMAS, """Proofs of L1 ... Lk where:
- L1, ..., Lk are clauses corresponding to theory lemmas used in the SAT
proof.

In contrast to proofs given for preprocess, L1 ... Lk are clauses that are
added to the SAT solver after its main search.

Only valid immediately after an unsat response."""
    FULL=c_ProofComponent.FULL, """A proof of false whose free assumptions are a subset of the input formulas
F1), ... Fm.

Only valid immediately after an unsat response."""

from cvc5types cimport ProofFormat as c_ProofFormat
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class ProofFormat(DocEnum):
    """The ProofFormat enum"""
    NONE=c_ProofFormat.NONE, """Do not translate proof output."""
    DOT=c_ProofFormat.DOT, """Output DOT proof."""
    LFSC=c_ProofFormat.LFSC, """Output LFSC proof."""
    ALETHE=c_ProofFormat.ALETHE, """Output Alethe proof."""
    DEFAULT=c_ProofFormat.DEFAULT, """Use the proof format mode set in the solver options."""

from cvc5types cimport FindSynthTarget as c_FindSynthTarget
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class FindSynthTarget(DocEnum):
    """The FindSynthTarget enum"""
    ENUM=c_FindSynthTarget.ENUM, """Find the next term in the enumeration of the target grammar."""
    REWRITE=c_FindSynthTarget.REWRITE, """Find a pair of terms (t,s) in the target grammar which are equivalent
but do not rewrite to the same term in the given rewriter
(--sygus-rewrite=MODE). If so, the equality (= t s) is returned by
findSynth.

This can be used to synthesize rewrite rules. Note if the rewriter is set
to none (--sygus-rewrite=none), this indicates a possible rewrite when
implementing a rewriter from scratch."""
    REWRITE_UNSOUND=c_FindSynthTarget.REWRITE_UNSOUND, """Find a term t in the target grammar which rewrites to a term s that is
not equivalent to it. If so, the equality (= t s) is returned by
findSynth.

This can be used to test the correctness of the given rewriter. Any
returned rewrite indicates an unsoundness in the given rewriter."""
    REWRITE_INPUT=c_FindSynthTarget.REWRITE_INPUT, """Find a rewrite between pairs of terms (t,s) that are matchable with terms
in the input assertions where t and s are equivalent but do not rewrite
to the same term in the given rewriter (--sygus-rewrite=MODE).

This can be used to synthesize rewrite rules that apply to the current
problem."""
    QUERY=c_FindSynthTarget.QUERY, """Find a query over the given grammar. If the given grammar generates terms
that are not Boolean, we consider equalities over terms from the given
grammar.

The algorithm for determining which queries to generate is configured by
--sygus-query-gen=MODE. Queries that are internally solved can be
filtered by the option --sygus-query-gen-filter-solved."""

from cvc5types cimport InputLanguage as c_InputLanguage
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class InputLanguage(DocEnum):
    """The InputLanguage enum"""
    SMT_LIB_2_6=c_InputLanguage.SMT_LIB_2_6, """The SMT-LIB version 2.6 language"""
    SYGUS_2_1=c_InputLanguage.SYGUS_2_1, """The SyGuS version 2.1 language."""
    UNKNOWN=c_InputLanguage.UNKNOWN, """No language given."""
