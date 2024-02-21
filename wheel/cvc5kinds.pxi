
from cvc5kinds cimport Kind as c_Kind
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class Kind(DocEnum):
    """The Kind enum"""
    INTERNAL_KIND=c_Kind.INTERNAL_KIND, """Internal kind.

This kind serves as an abstraction for internal kinds that are not exposed
via the API but may appear in terms returned by API functions, e.g.,
when querying the simplified form of a term.

.. note:: Should never be created via the API."""
    UNDEFINED_KIND=c_Kind.UNDEFINED_KIND, """Undefined kind.

.. note:: Should never be exposed or created via the API."""
    NULL_TERM=c_Kind.NULL_TERM, """Null kind.

The kind of a null term (:py:meth:`Term.Term()`).

.. note:: May not be explicitly created via API functions other than
          :py:meth:`Term.Term()`."""
    UNINTERPRETED_SORT_VALUE=c_Kind.UNINTERPRETED_SORT_VALUE, """The value of an uninterpreted constant.

.. note:: May be returned as the result of an API call, but terms of this
          kind may not be created explicitly via the API and may not
          appear in assertions."""
    EQUAL=c_Kind.EQUAL, """Equality, chainable.

- Arity: ``n > 1``

  - ``1..n:`` Terms of the same Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    DISTINCT=c_Kind.DISTINCT, """Disequality.

- Arity: ``n > 1``

  - ``1..n:`` Terms of the same Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CONSTANT=c_Kind.CONSTANT, """Free constant symbol.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkConst()`

.. note:: Not permitted in bindings (e.g., :py:obj:`FORALL`,
          :py:obj:`EXISTS`)."""
    VARIABLE=c_Kind.VARIABLE, """(Bound) variable.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkVar()`

.. note:: Only permitted in bindings and in lambda and quantifier bodies."""
    SEXPR=c_Kind.SEXPR, """Symbolic expression.

- Arity: ``n > 0``

  - ``1..n:`` Terms with same sorts

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    LAMBDA=c_Kind.LAMBDA, """Lambda expression.

- Arity: ``2``

  - ``1:`` Term of kind :py:obj:`VARIABLE_LIST`
  - ``2:`` Term of any Sort (the body of the lambda)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    WITNESS=c_Kind.WITNESS, """Witness.

The syntax of a witness term is similar to a quantified formula except that
only one variable is allowed.
For example, the term

.. code:: smtlib

    (witness ((x S)) F)

returns an element :math:`x` of Sort :math:`S` and asserts formula
:math:`F`.

The witness operator behaves like the description operator
(see https:
no :math:`x` that satisfies :math:`F`. But if such :math:`x` exists, the
witness operator does not enforce the following axiom which ensures
uniqueness up to logical equivalence:

.. math::

    \\forall x. F \\equiv G \\Rightarrow witness~x. F =  witness~x. G

For example, if there are two elements of Sort :math:`S` that satisfy
formula :math:`F`, then the following formula is satisfiable:

.. code:: smtlib

    (distinct
       (witness ((x Int)) F)
       (witness ((x Int)) F))

- Arity: ``3``

  - ``1:`` Term of kind :py:obj:`VARIABLE_LIST`
  - ``2:`` Term of Sort Bool (the body of the witness)
  - ``3:`` (optional) Term of kind :py:obj:`INST_PATTERN_LIST`

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. note::

    This kind is primarily used internally, but may be returned in
    models (e.g., for arithmetic terms in non-linear queries). However,
    it is not supported by the parser. Moreover, the user of the API
    should be cautious when using this operator. In general, all witness
    terms ``(witness ((x Int)) F)`` should be such that ``(exists ((x Int))
    F)`` is a valid formula. If this is not the case, then the semantics
    in formulas that use witness terms may be unintuitive. For example,
    the following formula is unsatisfiable:
    ``(or (= (witness ((x Int)) false) 0) (not (= (witness ((x Int))
    false) 0))``, whereas notice that ``(or (= z 0) (not (= z 0)))`` is
    true for any :math:`z`."""
    CONST_BOOLEAN=c_Kind.CONST_BOOLEAN, """Boolean constant.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTrue()`
  - :py:meth:`Solver.mkFalse()`
  - :py:meth:`Solver.mkBoolean()`"""
    NOT=c_Kind.NOT, """Logical negation.

- Arity: ``1``

  - ``1:`` Term of Sort Bool

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    AND=c_Kind.AND, """Logical conjunction.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Bool

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    IMPLIES=c_Kind.IMPLIES, """Logical implication.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Bool

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    OR=c_Kind.OR, """Logical disjunction.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Bool

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    XOR=c_Kind.XOR, """Logical exclusive disjunction, left associative.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Bool

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ITE=c_Kind.ITE, """If-then-else.

- Arity: ``3``

  - ``1:`` Term of Sort Bool
  - ``2:`` The 'then' term, Term of any Sort
  - ``3:`` The 'else' term, Term of the same sort as second argument

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    APPLY_UF=c_Kind.APPLY_UF, """Application of an uninterpreted function.

- Arity: ``n > 1``

  - ``1:`` Function Term
  - ``2..n:`` Function argument instantiation Terms of any first-class Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CARDINALITY_CONSTRAINT=c_Kind.CARDINALITY_CONSTRAINT, """Cardinality constraint on uninterpreted sort.

Interpreted as a predicate that is true when the cardinality of
uinterpreted Sort :math:`S` is less than or equal to an upper bound.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkCardinalityConstraint()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    HO_APPLY=c_Kind.HO_APPLY, """Higher-order applicative encoding of function application, left
associative.

- Arity: ``n = 2``

  - ``1:`` Function Term
  - ``2:`` Argument Term of the domain Sort of the function

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ADD=c_Kind.ADD, """Arithmetic addition.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    MULT=c_Kind.MULT, """Arithmetic multiplication.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    IAND=c_Kind.IAND, """Integer and.

Operator for bit-wise ``AND`` over integers, parameterized by a (positive)
bit-width :math:`k`.

.. code:: smtlib

    ((_ iand k) i_1 i_2)

is equivalent to

.. code:: smtlib

    ((_ iand k) i_1 i_2)
    (bv2int (bvand ((_ int2bv k) i_1) ((_ int2bv k) i_2)))

for all integers ``i_1``, ``i_2``.

- Arity: ``2``

  - ``1..2:`` Terms of Sort Int

- Indices: ``1``

  - ``1:`` Bit-width :math:`k`

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    POW2=c_Kind.POW2, """Power of two.

Operator for raising ``2`` to a non-negative integer power.

- Arity: ``1``

  - ``1:`` Term of Sort Int

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SUB=c_Kind.SUB, """Arithmetic subtraction, left associative.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    NEG=c_Kind.NEG, """Arithmetic negation.

- Arity: ``1``

  - ``1:`` Term of Sort Int or Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    DIVISION=c_Kind.DIVISION, """Real division, division by 0 undefined, left associative.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    INTS_DIVISION=c_Kind.INTS_DIVISION, """Integer division, division by 0 undefined, left associative.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    INTS_MODULUS=c_Kind.INTS_MODULUS, """Integer modulus, modulus by 0 undefined.

- Arity: ``2``

  - ``1:`` Term of Sort Int
  - ``2:`` Term of Sort Int

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ABS=c_Kind.ABS, """Absolute value.

- Arity: ``1``

  - ``1:`` Term of Sort Int or Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    POW=c_Kind.POW, """Arithmetic power.

- Arity: ``2``

  - ``1..2:`` Term of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    EXPONENTIAL=c_Kind.EXPONENTIAL, """Exponential function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SINE=c_Kind.SINE, """Sine function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    COSINE=c_Kind.COSINE, """Cosine function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    TANGENT=c_Kind.TANGENT, """Tangent function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    COSECANT=c_Kind.COSECANT, """Cosecant function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SECANT=c_Kind.SECANT, """Secant function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    COTANGENT=c_Kind.COTANGENT, """Cotangent function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ARCSINE=c_Kind.ARCSINE, """Arc sine function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ARCCOSINE=c_Kind.ARCCOSINE, """Arc cosine function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ARCTANGENT=c_Kind.ARCTANGENT, """Arc tangent function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ARCCOSECANT=c_Kind.ARCCOSECANT, """Arc cosecant function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ARCSECANT=c_Kind.ARCSECANT, """Arc secant function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    ARCCOTANGENT=c_Kind.ARCCOTANGENT, """Arc cotangent function.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SQRT=c_Kind.SQRT, """Square root.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    DIVISIBLE=c_Kind.DIVISIBLE, """Operator for the divisibility-by-:math:`k` predicate.

- Arity: ``1``

  - ``1:`` Term of Sort Int

- Indices: ``1``

  - ``1:`` The integer :math:`k` to divide by.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CONST_RATIONAL=c_Kind.CONST_RATIONAL, """Arbitrary-precision rational constant.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkReal()`
  - :py:meth:`Solver.mkReal()`"""
    CONST_INTEGER=c_Kind.CONST_INTEGER, """Arbitrary-precision integer constant.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkInteger()`"""
    LT=c_Kind.LT, """Less than, chainable.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    LEQ=c_Kind.LEQ, """Less than or equal, chainable.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    GT=c_Kind.GT, """Greater than, chainable.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    GEQ=c_Kind.GEQ, """Greater than or equal, chainable.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort Int or Real (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    IS_INTEGER=c_Kind.IS_INTEGER, """Is-integer predicate.

- Arity: ``1``

  - ``1:`` Term of Sort Int or Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    TO_INTEGER=c_Kind.TO_INTEGER, """Convert Term of sort Int or Real to Int via the floor function.

- Arity: ``1``

  - ``1:`` Term of Sort Int or Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    TO_REAL=c_Kind.TO_REAL, """Convert Term of Sort Int or Real to Real.

- Arity: ``1``

  - ``1:`` Term of Sort Int or Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    PI=c_Kind.PI, """Pi constant.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkPi()`

.. note:: :py:obj:`PI` is considered a special symbol of Sort
           Real, but is not a Real value, i.e.,
           :py:meth:`Term.isRealValue()` will return ``false``."""
    CONST_BITVECTOR=c_Kind.CONST_BITVECTOR, """Fixed-size bit-vector constant.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkBitVector()`"""
    BITVECTOR_CONCAT=c_Kind.BITVECTOR_CONCAT, """Concatenation of two or more bit-vectors.

- Arity: ``n > 1``

  - ``1..n:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_AND=c_Kind.BITVECTOR_AND, """Bit-wise and.

- Arity: ``n > 1``

  - ``1..n:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_OR=c_Kind.BITVECTOR_OR, """Bit-wise or.

- Arity: ``n > 1``

  - ``1..n:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_XOR=c_Kind.BITVECTOR_XOR, """Bit-wise xor.

- Arity: ``n > 1``

  - ``1..n:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_NOT=c_Kind.BITVECTOR_NOT, """Bit-wise negation.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_NAND=c_Kind.BITVECTOR_NAND, """Bit-wise nand.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_NOR=c_Kind.BITVECTOR_NOR, """Bit-wise nor.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_XNOR=c_Kind.BITVECTOR_XNOR, """Bit-wise xnor, left associative.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_COMP=c_Kind.BITVECTOR_COMP, """Equality comparison (returns bit-vector of size ``1``).

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_MULT=c_Kind.BITVECTOR_MULT, """Multiplication of two or more bit-vectors.

- Arity: ``n > 1``

  - ``1..n:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ADD=c_Kind.BITVECTOR_ADD, """Addition of two or more bit-vectors.

- Arity: ``n > 1``

  - ``1..n:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SUB=c_Kind.BITVECTOR_SUB, """Subtraction of two bit-vectors.

- Arity: ``n > 1``

  - ``1..n:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_NEG=c_Kind.BITVECTOR_NEG, """Negation of a bit-vector (two's complement).

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_UDIV=c_Kind.BITVECTOR_UDIV, """Unsigned bit-vector division.

Truncates towards ``0``. If the divisor is zero, the result is all ones.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_UREM=c_Kind.BITVECTOR_UREM, """Unsigned bit-vector remainder.

Remainder from unsigned bit-vector division. If the modulus is zero, the
result is the dividend.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SDIV=c_Kind.BITVECTOR_SDIV, """Signed bit-vector division.

Two's complement signed division of two bit-vectors. If the divisor is
zero and the dividend is positive, the result is all ones. If the divisor
is zero and the dividend is negative, the result is one.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SREM=c_Kind.BITVECTOR_SREM, """Signed bit-vector remainder (sign follows dividend).

Two's complement signed remainder of two bit-vectors where the sign
follows the dividend. If the modulus is zero, the result is the dividend.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SMOD=c_Kind.BITVECTOR_SMOD, """Signed bit-vector remainder (sign follows divisor).

Two's complement signed remainder where the sign follows the divisor. If
the modulus is zero, the result is the dividend.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SHL=c_Kind.BITVECTOR_SHL, """Bit-vector shift left.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_LSHR=c_Kind.BITVECTOR_LSHR, """Bit-vector logical shift right.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ASHR=c_Kind.BITVECTOR_ASHR, """Bit-vector arithmetic shift right.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ULT=c_Kind.BITVECTOR_ULT, """Bit-vector unsigned less than.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ULE=c_Kind.BITVECTOR_ULE, """Bit-vector unsigned less than or equal.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_UGT=c_Kind.BITVECTOR_UGT, """Bit-vector unsigned greater than.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_UGE=c_Kind.BITVECTOR_UGE, """Bit-vector unsigned greater than or equal.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SLT=c_Kind.BITVECTOR_SLT, """Bit-vector signed less than.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SLE=c_Kind.BITVECTOR_SLE, """Bit-vector signed less than or equal.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SGT=c_Kind.BITVECTOR_SGT, """Bit-vector signed greater than.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SGE=c_Kind.BITVECTOR_SGE, """Bit-vector signed greater than or equal.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ULTBV=c_Kind.BITVECTOR_ULTBV, """Bit-vector unsigned less than returning a bit-vector of size 1.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SLTBV=c_Kind.BITVECTOR_SLTBV, """Bit-vector signed less than returning a bit-vector of size ``1``.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ITE=c_Kind.BITVECTOR_ITE, """Bit-vector if-then-else.

Same semantics as regular ITE, but condition is bit-vector of size ``1``.

- Arity: ``3``

  - ``1:`` Term of bit-vector Sort of size `1`
  - ``1..3:`` Terms of bit-vector sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_REDOR=c_Kind.BITVECTOR_REDOR, """Bit-vector redor.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_REDAND=c_Kind.BITVECTOR_REDAND, """Bit-vector redand.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_NEGO=c_Kind.BITVECTOR_NEGO, """Bit-vector negation overflow detection.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_UADDO=c_Kind.BITVECTOR_UADDO, """Bit-vector unsigned addition overflow detection.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SADDO=c_Kind.BITVECTOR_SADDO, """Bit-vector signed addition overflow detection.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_UMULO=c_Kind.BITVECTOR_UMULO, """Bit-vector unsigned multiplication overflow detection.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SMULO=c_Kind.BITVECTOR_SMULO, """Bit-vector signed multiplication overflow detection.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_USUBO=c_Kind.BITVECTOR_USUBO, """Bit-vector unsigned subtraction overflow detection.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SSUBO=c_Kind.BITVECTOR_SSUBO, """Bit-vector signed subtraction overflow detection.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SDIVO=c_Kind.BITVECTOR_SDIVO, """Bit-vector signed division overflow detection.

- Arity: ``2``

  - ``1..2:`` Terms of bit-vector Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_EXTRACT=c_Kind.BITVECTOR_EXTRACT, """Bit-vector extract.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Indices: ``2``

  - ``1:`` The upper bit index.
  - ``2:`` The lower bit index.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_REPEAT=c_Kind.BITVECTOR_REPEAT, """Bit-vector repeat.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Indices: ``1``

  - ``1:`` The number of times to repeat the given term.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ZERO_EXTEND=c_Kind.BITVECTOR_ZERO_EXTEND, """Bit-vector zero extension.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Indices: ``1``

  - ``1:`` The number of zeroes to extend the given term with.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_SIGN_EXTEND=c_Kind.BITVECTOR_SIGN_EXTEND, """Bit-vector sign extension.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Indices: ``1``

  - ``1:`` The number of bits (of the value of the sign bit) to extend the given term with.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ROTATE_LEFT=c_Kind.BITVECTOR_ROTATE_LEFT, """Bit-vector rotate left.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Indices: ``1``

  - ``1:`` The number of bits to rotate the given term left.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_ROTATE_RIGHT=c_Kind.BITVECTOR_ROTATE_RIGHT, """Bit-vector rotate right.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Indices: ``1``

  - ``1:`` The number of bits to rotate the given term right.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    INT_TO_BITVECTOR=c_Kind.INT_TO_BITVECTOR, """Conversion from Int to bit-vector.

- Arity: ``1``

  - ``1:`` Term of Sort Int

- Indices: ``1``

  - ``1:`` The size of the bit-vector to convert to.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BITVECTOR_TO_NAT=c_Kind.BITVECTOR_TO_NAT, """Bit-vector conversion to (non-negative) integer.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CONST_FINITE_FIELD=c_Kind.CONST_FINITE_FIELD, """Finite field constant.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkFiniteFieldElem()`"""
    FINITE_FIELD_NEG=c_Kind.FINITE_FIELD_NEG, """Negation of a finite field element (additive inverse).

- Arity: ``1``

  - ``1:`` Term of finite field Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FINITE_FIELD_ADD=c_Kind.FINITE_FIELD_ADD, """Addition of two or more finite field elements.

- Arity: ``n > 1``

  - ``1..n:`` Terms of finite field Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FINITE_FIELD_BITSUM=c_Kind.FINITE_FIELD_BITSUM, """Bitsum of two or more finite field elements: x + 2y + 4z + ...

- Arity: ``n > 1``

  - ``1..n:`` Terms of finite field Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`"""
    FINITE_FIELD_MULT=c_Kind.FINITE_FIELD_MULT, """Multiplication of two or more finite field elements.

- Arity: ``n > 1``

  - ``1..n:`` Terms of finite field Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CONST_FLOATINGPOINT=c_Kind.CONST_FLOATINGPOINT, """Floating-point constant, created from IEEE-754 bit-vector representation
of the floating-point value.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkFloatingPoint()`"""
    CONST_ROUNDINGMODE=c_Kind.CONST_ROUNDINGMODE, """RoundingMode constant.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkRoundingMode()`"""
    FLOATINGPOINT_FP=c_Kind.FLOATINGPOINT_FP, """Create floating-point literal from bit-vector triple.

- Arity: ``3``

  - ``1:`` Term of bit-vector Sort of size `1` (sign bit)
  - ``2:`` Term of bit-vector Sort of exponent size (exponent)
  - ``3:`` Term of bit-vector Sort of significand size - 1 (significand without hidden bit)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_EQ=c_Kind.FLOATINGPOINT_EQ, """Floating-point equality.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_ABS=c_Kind.FLOATINGPOINT_ABS, """Floating-point absolute value.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_NEG=c_Kind.FLOATINGPOINT_NEG, """Floating-point negation.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_ADD=c_Kind.FLOATINGPOINT_ADD, """Floating-point addition.

- Arity: ``3``

  - ``1:`` Term of Sort RoundingMode
  - ``2..3:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_SUB=c_Kind.FLOATINGPOINT_SUB, """Floating-point sutraction.

- Arity: ``3``

  - ``1:`` Term of Sort RoundingMode
  - ``2..3:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_MULT=c_Kind.FLOATINGPOINT_MULT, """Floating-point multiply.

- Arity: ``3``

  - ``1:`` Term of Sort RoundingMode
  - ``2..3:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_DIV=c_Kind.FLOATINGPOINT_DIV, """Floating-point division.

- Arity: ``3``

  - ``1:`` Term of Sort RoundingMode
  - ``2..3:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_FMA=c_Kind.FLOATINGPOINT_FMA, """Floating-point fused multiply and add.

- Arity: ``4``

  - ``1:`` Term of Sort RoundingMode
  - ``2..4:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_SQRT=c_Kind.FLOATINGPOINT_SQRT, """Floating-point square root.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_REM=c_Kind.FLOATINGPOINT_REM, """Floating-point remainder.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_RTI=c_Kind.FLOATINGPOINT_RTI, """Floating-point round to integral.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_MIN=c_Kind.FLOATINGPOINT_MIN, """Floating-point minimum.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_MAX=c_Kind.FLOATINGPOINT_MAX, """Floating-point maximum.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_LEQ=c_Kind.FLOATINGPOINT_LEQ, """Floating-point less than or equal.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_LT=c_Kind.FLOATINGPOINT_LT, """Floating-point less than.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_GEQ=c_Kind.FLOATINGPOINT_GEQ, """Floating-point greater than or equal.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_GT=c_Kind.FLOATINGPOINT_GT, """Floating-point greater than.

- Arity: ``2``

  - ``1..2:`` Terms of floating-point Sort (sorts must match)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_IS_NORMAL=c_Kind.FLOATINGPOINT_IS_NORMAL, """Floating-point is normal tester.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_IS_SUBNORMAL=c_Kind.FLOATINGPOINT_IS_SUBNORMAL, """Floating-point is sub-normal tester.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_IS_ZERO=c_Kind.FLOATINGPOINT_IS_ZERO, """Floating-point is zero tester.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_IS_INF=c_Kind.FLOATINGPOINT_IS_INF, """Floating-point is infinite tester.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_IS_NAN=c_Kind.FLOATINGPOINT_IS_NAN, """Floating-point is NaN tester.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_IS_NEG=c_Kind.FLOATINGPOINT_IS_NEG, """Floating-point is negative tester.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_IS_POS=c_Kind.FLOATINGPOINT_IS_POS, """Floating-point is positive tester.

- Arity: ``1``

  - ``1:`` Term of floating-point Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_FP_FROM_IEEE_BV=c_Kind.FLOATINGPOINT_TO_FP_FROM_IEEE_BV, """Conversion to floating-point from IEEE-754 bit-vector.

- Arity: ``1``

  - ``1:`` Term of bit-vector Sort

- Indices: ``2``

  - ``1:`` The exponent size
  - ``2:`` The significand size

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_FP_FROM_FP=c_Kind.FLOATINGPOINT_TO_FP_FROM_FP, """Conversion to floating-point from floating-point.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of floating-point Sort

- Indices: ``2``

  - ``1:`` The exponent size
  - ``2:`` The significand size

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_FP_FROM_REAL=c_Kind.FLOATINGPOINT_TO_FP_FROM_REAL, """Conversion to floating-point from Real.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of Sort Real

- Indices: ``2``

  - ``1:`` The exponent size
  - ``2:`` The significand size

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_FP_FROM_SBV=c_Kind.FLOATINGPOINT_TO_FP_FROM_SBV, """Conversion to floating-point from signed bit-vector.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of bit-vector Sort

- Indices: ``2``

  - ``1:`` The exponent size
  - ``2:`` The significand size

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_FP_FROM_UBV=c_Kind.FLOATINGPOINT_TO_FP_FROM_UBV, """Conversion to floating-point from unsigned bit-vector.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of bit-vector Sort

- Indices: ``2``

  - ``1:`` The exponent size
  - ``2:`` The significand size

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_UBV=c_Kind.FLOATINGPOINT_TO_UBV, """Conversion to unsigned bit-vector from floating-point.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of floating-point Sort

- Indices: ``1``

  - ``1:`` The size of the bit-vector to convert to.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_SBV=c_Kind.FLOATINGPOINT_TO_SBV, """Conversion to signed bit-vector from floating-point.

- Arity: ``2``

  - ``1:`` Term of Sort RoundingMode
  - ``2:`` Term of floating-point Sort

- Indices: ``1``

  - ``1:`` The size of the bit-vector to convert to.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FLOATINGPOINT_TO_REAL=c_Kind.FLOATINGPOINT_TO_REAL, """Conversion to Real from floating-point.

- Arity: ``1``

  - ``1:`` Term of Sort Real

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SELECT=c_Kind.SELECT, """Array select.

- Arity: ``2``

  - ``1:`` Term of array Sort
  - ``2:`` Term of array index Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STORE=c_Kind.STORE, """Array store.

- Arity: ``3``

  - ``1:`` Term of array Sort
  - ``2:`` Term of array index Sort
  - ``3:`` Term of array element Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CONST_ARRAY=c_Kind.CONST_ARRAY, """Constant array.

- Arity: ``2``

  - ``1:`` Term of array Sort
  - ``2:`` Term of array element Sort (value)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    EQ_RANGE=c_Kind.EQ_RANGE, """Equality over arrays :math:`a` and :math:`b` over a given range
:math:`[i,j]`, i.e.,

.. math::

  \\forall k . i \\leq k \\leq j \\Rightarrow a[k] = b[k]

- Arity: ``4``

  - ``1:`` Term of array Sort (first array)
  - ``2:`` Term of array Sort (second array)
  - ``3:`` Term of array index Sort (lower bound of range, inclusive)
  - ``4:`` Term of array index Sort (upper bound of range, inclusive)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions.

.. note:: We currently support the creation of array equalities over index
          Sorts bit-vector, floating-point, Int and Real.
          Requires to enable option
          :ref:`arrays-exp<lbl-option-arrays-exp>`."""
    APPLY_CONSTRUCTOR=c_Kind.APPLY_CONSTRUCTOR, """Datatype constructor application.

- Arity: ``n > 0``

  - ``1:`` DatatypeConstructor Term (see :py:meth:`DatatypeConstructor.getTerm()`)
  - ``2..n:`` Terms of the Sorts of the selectors of the constructor (the arguments to the constructor)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    APPLY_SELECTOR=c_Kind.APPLY_SELECTOR, """Datatype selector application.

- Arity: ``2``

  - ``1:`` DatatypeSelector Term (see :py:meth:`DatatypeSelector.getTerm()`)
  - ``2:`` Term of the codomain Sort of the selector

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. note:: Undefined if misapplied."""
    APPLY_TESTER=c_Kind.APPLY_TESTER, """Datatype tester application.

- Arity: ``2``

  - ``1:`` Datatype tester Term (see :py:meth:`DatatypeConstructor.getTesterTerm()`)
  - ``2:`` Term of Datatype Sort (DatatypeConstructor must belong to this Datatype Sort)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    APPLY_UPDATER=c_Kind.APPLY_UPDATER, """Datatype update application.

- Arity: ``3``

  - ``1:`` Datatype updater Term (see :py:meth:`DatatypeSelector.getUpdaterTerm()`)
  - ``2:`` Term of Datatype Sort (DatatypeSelector of the updater must belong to a constructor of this Datatype Sort)
  - ``3:`` Term of the codomain Sort of the selector (the Term to update the field of the datatype term with)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. note:: Does not change the datatype argument if misapplied."""
    MATCH=c_Kind.MATCH, """Match expression.

This kind is primarily used in the parser to support the
SMT-LIBv2 ``match`` expression.

For example, the SMT-LIBv2 syntax for the following match term

.. code:: smtlib

     (match l (((cons h t) h) (nil 0)))

is represented by the AST

.. code:: lisp

    (MATCH l
        (MATCH_BIND_CASE (VARIABLE_LIST h t) (cons h t) h)
        (MATCH_CASE nil 0))

Terms of kind :py:obj:`MATCH_CASE` are constant case expressions,
which are used for nullary constructors. Kind
:py:obj:`MATCH_BIND_CASE` is used for constructors with selectors
and variable match patterns. If not all constructors are covered, at least
one catch-all variable pattern must be included.

- Arity: ``n > 1``

  - ``1..n:`` Terms of kind :py:obj:`MATCH_CASE` and :py:obj:`MATCH_BIND_CASE`

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    MATCH_CASE=c_Kind.MATCH_CASE, """Match case for nullary constructors.

A (constant) case expression to be used within a match expression.

- Arity: ``2``

  - ``1:`` Term of kind :py:obj:`APPLY_CONSTRUCTOR` (the pattern to match against)
  - ``2:`` Term of any Sort (the term the match term evaluates to)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    MATCH_BIND_CASE=c_Kind.MATCH_BIND_CASE, """Match case with binders, for constructors with selectors and variable
patterns.

A (non-constant) case expression to be used within a match expression.

- Arity: ``3``

  - For variable patterns:

    - ``1:`` Term of kind :py:obj:`VARIABLE_LIST` (containing the free variable of the case)
    - ``2:`` Term of kind :py:obj:`VARIABLE` (the pattern expression, the free variable of the case)
    - ``3:`` Term of any Sort (the term the pattern evaluates to)

  - For constructors with selectors:

    - ``1:`` Term of kind :py:obj:`VARIABLE_LIST` (containing the free variable of the case)
    - ``2:`` Term of kind :py:obj:`APPLY_CONSTRUCTOR` (the pattern expression, applying the set of variables to the constructor)
    - ``3:`` Term of any Sort (the term the match term evaluates to)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    TUPLE_PROJECT=c_Kind.TUPLE_PROJECT, """Tuple projection.

This operator takes a tuple as an argument and returns a tuple obtained by
concatenating components of its argument at the provided indices.

For example,

.. code:: smtlib

    ((_ tuple.project 1 2 2 3 1) (tuple 10 20 30 40))

yields

.. code:: smtlib

    (tuple 20 30 30 40 20)

- Arity: ``1``

  - ``1:`` Term of tuple Sort

- Indices: ``n``

  - ``1..n:`` The tuple indices to project

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    NULLABLE_LIFT=c_Kind.NULLABLE_LIFT, """Lifting operator for nullable terms.
This operator lifts a built-in operator or a user-defined function
to nullable terms.
For built-in kinds use mkNullableLift.
For user-defined functions use mkTerm.

- Arity: ``n > 1``

- ``1..n:`` Terms of nullable sort

- Create Term of this Kind with:
  - :py:meth:`Solver.mkNullableLift()`
  - :py:meth:`Solver.mkTerm()`"""
    SEP_NIL=c_Kind.SEP_NIL, """Separation logic nil.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkSepNil()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SEP_EMP=c_Kind.SEP_EMP, """Separation logic empty heap.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkSepEmp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SEP_PTO=c_Kind.SEP_PTO, """Separation logic points-to relation.

- Arity: ``2``

  - ``1:`` Term denoting the location of the points-to constraint
  - ``2:`` Term denoting the data of the points-to constraint

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SEP_STAR=c_Kind.SEP_STAR, """Separation logic star.

- Arity: ``n > 1``

  - ``1..n:`` Terms of sort Bool (the child constraints that hold in
              disjoint (separated) heaps)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SEP_WAND=c_Kind.SEP_WAND, """Separation logic magic wand.

- Arity: ``2``

  - ``1:`` Terms of Sort Bool (the antecendant of the magic wand constraint)
  - ``2:`` Terms of Sort Bool (conclusion of the magic wand constraint,
         which is asserted to hold in all heaps that are disjoint
         extensions of the antecedent)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SET_EMPTY=c_Kind.SET_EMPTY, """Empty set.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkEmptySet()`"""
    SET_UNION=c_Kind.SET_UNION, """Set union.

- Arity: ``2``

  - ``1..2:`` Terms of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_INTER=c_Kind.SET_INTER, """Set intersection.

- Arity: ``2``

  - ``1..2:`` Terms of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_MINUS=c_Kind.SET_MINUS, """Set subtraction.

- Arity: ``2``

  - ``1..2:`` Terms of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_SUBSET=c_Kind.SET_SUBSET, """Subset predicate.

Determines if the first set is a subset of the second set.

- Arity: ``2``

  - ``1..2:`` Terms of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_MEMBER=c_Kind.SET_MEMBER, """Set membership predicate.

Determines if the given set element is a member of the second set.

- Arity: ``2``

  - ``1:`` Term of any Sort (must match the element Sort of the given set Term)
  - ``2:`` Term of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_SINGLETON=c_Kind.SET_SINGLETON, """Singleton set.

Construct a singleton set from an element given as a parameter.
The returned set has the same Sort as the element.

- Arity: ``1``

  - ``1:`` Term of any Sort (the set element)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_INSERT=c_Kind.SET_INSERT, """The set obtained by inserting elements;

- Arity: ``n > 0``

  - ``1..n-1:`` Terms of any Sort (must match the element sort of the given set Term)
  - ``n:`` Term of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_CARD=c_Kind.SET_CARD, """Set cardinality.

- Arity: ``1``

  - ``1:`` Term of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_COMPLEMENT=c_Kind.SET_COMPLEMENT, """Set complement with respect to finite universe.

- Arity: ``1``

  - ``1:`` Term of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SET_UNIVERSE=c_Kind.SET_UNIVERSE, """Finite universe set.

All set variables must be interpreted as subsets of it.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkUniverseSet()`

.. note:: :py:obj:`SET_UNIVERSE` is considered a special symbol of
          the theory of sets and is not considered as a set value, i.e.,
          :py:meth:`Term.isSetValue()` will return ``false``."""
    SET_COMPREHENSION=c_Kind.SET_COMPREHENSION, """Set comprehension

A set comprehension is specified by a variable list :math:`x_1 ... x_n`,
a predicate :math:`P[x_1...x_n]`, and a term :math:`t[x_1...x_n]`. A
comprehension :math:`C` with the above form has members given by the
following semantics:

.. math::

 \\forall y. ( \\exists x_1...x_n. P[x_1...x_n] \\wedge t[x_1...x_n] = y )
 \\Leftrightarrow (set.member \\; y \\; C)

where :math:`y` ranges over the element Sort of the (set) Sort of the
comprehension. If :math:`t[x_1..x_n]` is not provided, it is equivalent
to :math:`y` in the above formula.

- Arity: ``3``

  - ``1:`` Term of Kind :py:obj:`VARIABLE_LIST`
  - ``2:`` Term of sort Bool (the predicate of the comprehension)
  - ``3:`` (optional) Term denoting the generator for the comprehension

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SET_CHOOSE=c_Kind.SET_CHOOSE, """Set choose.

Select an element from a given set. For a set :math:`A = \\{x\\}`, the term
(set.choose :math:`A`) is equivalent to the term :math:`x_1`. For an empty
set, it is an arbitrary value. For a set with cardinality > 1, it will
deterministically return an element in :math:`A`.

- Arity: ``1``

  - ``1:`` Term of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SET_IS_SINGLETON=c_Kind.SET_IS_SINGLETON, """Set is singleton tester.

- Arity: ``1``

  - ``1:`` Term of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SET_MAP=c_Kind.SET_MAP, """Set map.

This operator applies the first argument, a function of
Sort :math:`(\\rightarrow S_1 \\; S_2)`, to every element of the second
argument, a set of Sort (Set :math:`S_1`), and returns a set of Sort
(Set :math:`S_2`).

- Arity: ``2``

  - ``1:`` Term of function Sort :math:`(\\rightarrow S_1 \\; S_2)`
  - ``2:`` Term of set Sort (Set :math:`S_1`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SET_FILTER=c_Kind.SET_FILTER, """Set filter.

This operator filters the elements of a set.
(set.filter :math:`p \\; A`) takes a predicate :math:`p` of Sort
:math:`(\\rightarrow T \\; Bool)` as a first argument, and a set :math:`A`
of Sort (Set :math:`T`) as a second argument, and returns a subset of Sort
(Set :math:`T`) that includes all elements of :math:`A` that satisfy
:math:`p`.

- Arity: ``2``

  - ``1:`` Term of function Sort :math:`(\\rightarrow T \\; Bool)`
  - ``2:`` Term of bag Sort (Set :math:`T`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    SET_FOLD=c_Kind.SET_FOLD, """Set fold.

This operator combines elements of a set into a single value.
(set.fold :math:`f \\; t \\; A`) folds the elements of set :math:`A`
starting with Term :math:`t` and using the combining function :math:`f`.

- Arity: ``2``

  - ``1:`` Term of function Sort :math:`(\\rightarrow S_1 \\; S_2 \\; S_2)`
  - ``2:`` Term of Sort :math:`S_2` (the initial value)
  - ``3:`` Term of bag Sort (Set :math:`S_1`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    RELATION_JOIN=c_Kind.RELATION_JOIN, """Relation join.

- Arity: ``2``

  - ``1..2:`` Terms of relation Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    RELATION_PRODUCT=c_Kind.RELATION_PRODUCT, """Relation cartesian product.

- Arity: ``2``

  - ``1..2:`` Terms of relation Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    RELATION_TRANSPOSE=c_Kind.RELATION_TRANSPOSE, """Relation transpose.

- Arity: ``1``

  - ``1:`` Term of relation Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    RELATION_TCLOSURE=c_Kind.RELATION_TCLOSURE, """Relation transitive closure.

- Arity: ``1``

  - ``1:`` Term of relation Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    RELATION_JOIN_IMAGE=c_Kind.RELATION_JOIN_IMAGE, """Relation join image.

- Arity: ``2``

  - ``1..2:`` Terms of relation Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    RELATION_IDEN=c_Kind.RELATION_IDEN, """Relation identity.

- Arity: ``1``

  - ``1:`` Term of relation Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    RELATION_GROUP=c_Kind.RELATION_GROUP, """Relation group

:math:`((\\_ \\; rel.group \\; n_1 \\; \\dots \\; n_k) \\; A)` partitions tuples
of relation :math:`A` such that tuples that have the same projection
with indices :math:`n_1 \\; \\dots \\; n_k` are in the same part.
It returns a set of relations of type :math:`(Set \\; T)` where
:math:`T` is the type of :math:`A`.

- Arity: ``1``

  - ``1:`` Term of relation sort

- Indices: ``n``

  - ``1..n:``  Indices of the projection

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    RELATION_AGGREGATE=c_Kind.RELATION_AGGREGATE, """Relation aggregate operator has the form
:math:`((\\_ \\; rel.aggr \\; n_1 ... n_k) \\; f \\; i \\; A)`
where :math:`n_1, ..., n_k` are natural numbers,
:math:`f` is a function of type
:math:`(\\rightarrow (Tuple \\;  T_1 \\; ... \\; T_j)\\; T \\; T)`,
:math:`i` has the type :math:`T`,
and :math:`A` has type :math:`(Relation \\;  T_1 \\; ... \\; T_j)`.
The returned type is :math:`(Set \\; T)`.

This operator aggregates elements in A that have the same tuple projection
with indices n_1, ..., n_k using the combining function :math:`f`,
and initial value :math:`i`.

- Arity: ``3``

  - ``1:`` Term of sort :math:`(\\rightarrow (Tuple \\;  T_1 \\; ... \\; T_j)\\; T \\; T)`
  - ``2:`` Term of Sort :math:`T`
  - ``3:`` Term of relation sort :math:`Relation T_1 ... T_j`

- Indices: ``n``
  - ``1..n:`` Indices of the projection

- Create Term of this Kind with:
  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:
  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    RELATION_PROJECT=c_Kind.RELATION_PROJECT, """Relation projection operator extends tuple projection operator to sets.

- Arity: ``1``
  - ``1:`` Term of relation Sort

- Indices: ``n``
  - ``1..n:`` Indices of the projection

- Create Term of this Kind with:
  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:
  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_EMPTY=c_Kind.BAG_EMPTY, """Empty bag.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkEmptyBag()`"""
    BAG_UNION_MAX=c_Kind.BAG_UNION_MAX, """Bag max union.

- Arity: ``2``

  - ``1..2:`` Terms of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_UNION_DISJOINT=c_Kind.BAG_UNION_DISJOINT, """Bag disjoint union (sum).

- Arity: ``2``

  - ``1..2:`` Terms of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_INTER_MIN=c_Kind.BAG_INTER_MIN, """Bag intersection (min).

- Arity: ``2``

  - ``1..2:`` Terms of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_DIFFERENCE_SUBTRACT=c_Kind.BAG_DIFFERENCE_SUBTRACT, """Bag difference subtract.

Subtracts multiplicities of the second from the first.

- Arity: ``2``

  - ``1..2:`` Terms of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_DIFFERENCE_REMOVE=c_Kind.BAG_DIFFERENCE_REMOVE, """Bag difference remove.

Removes shared elements in the two bags.

- Arity: ``2``

  - ``1..2:`` Terms of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_SUBBAG=c_Kind.BAG_SUBBAG, """Bag inclusion predicate.

Determine if multiplicities of the first bag are less than or equal to
multiplicities of the second bag.

- Arity: ``2``

  - ``1..2:`` Terms of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_COUNT=c_Kind.BAG_COUNT, """Bag element multiplicity.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`"""
    BAG_MEMBER=c_Kind.BAG_MEMBER, """Bag membership predicate.

- Arity: ``2``

  - ``1:`` Term of any Sort (must match the element Sort of the given bag Term)
  - ``2:`` Terms of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_DUPLICATE_REMOVAL=c_Kind.BAG_DUPLICATE_REMOVAL, """Bag duplicate removal.

Eliminate duplicates in a given bag. The returned bag contains exactly the
same elements in the given bag, but with multiplicity one.

- Arity: ``1``

  - ``1:`` Term of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_MAKE=c_Kind.BAG_MAKE, """Bag make.

Construct a bag with the given element and given multiplicity.

- Arity: ``2``

  - ``1:`` Term of any Sort (the bag element)
  - ``2:`` Term of Sort Int (the multiplicity of the element)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    BAG_CARD=c_Kind.BAG_CARD, """Bag cardinality.

- Arity: ``1``

  - ``1:`` Term of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_CHOOSE=c_Kind.BAG_CHOOSE, """Bag choose.

Select an element from a given bag.

For a bag :math:`A = \\{(x,n)\\}` where :math:`n` is the multiplicity, then
the term (choose :math:`A`) is equivalent to the term :math:`x`. For an
empty bag, then it is an arbitrary value. For a bag that contains distinct
elements, it will deterministically return an element in :math:`A`.

- Arity: ``1``

  - ``1:`` Term of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_IS_SINGLETON=c_Kind.BAG_IS_SINGLETON, """Bag is singleton tester.

- Arity: ``1``

  - ``1:`` Term of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_FROM_SET=c_Kind.BAG_FROM_SET, """Conversion from set to bag.

- Arity: ``1``

  - ``1:`` Term of set Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_TO_SET=c_Kind.BAG_TO_SET, """Conversion from bag to set.

- Arity: ``1``

  - ``1:`` Term of bag Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_MAP=c_Kind.BAG_MAP, """Bag map.

This operator applies the first argument, a function of
Sort :math:`(\\rightarrow S_1 \\; S_2)`, to every element of the second
argument, a set of Sort (Bag :math:`S_1`), and returns a set of Sort
(Bag :math:`S_2`).

- Arity: ``2``

  - ``1:`` Term of function Sort :math:`(\\rightarrow S_1 \\; S_2)`
  - ``2:`` Term of bag Sort (Bag :math:`S_1`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_FILTER=c_Kind.BAG_FILTER, """Bag filter.

This operator filters the elements of a bag.
(bag.filter :math:`p \\; B`) takes a predicate :math:`p` of Sort
:math:`(\\rightarrow T \\; Bool)` as a first argument, and a bag :math:`B`
of Sort (Bag :math:`T`) as a second argument, and returns a subbag of Sort
(Bag :math:`T`) that includes all elements of :math:`B` that satisfy
:math:`p` with the same multiplicity.

- Arity: ``2``

  - ``1:`` Term of function Sort :math:`(\\rightarrow T \\; Bool)`
  - ``2:`` Term of bag Sort (Bag :math:`T`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_FOLD=c_Kind.BAG_FOLD, """Bag fold.

This operator combines elements of a bag into a single value.
(bag.fold :math:`f \\; t \\; B`) folds the elements of bag :math:`B`
starting with Term :math:`t` and using the combining function :math:`f`.

- Arity: ``2``

  - ``1:`` Term of function Sort :math:`(\\rightarrow S_1 \\; S_2 \\; S_2)`
  - ``2:`` Term of Sort :math:`S_2` (the initial value)
  - ``3:`` Term of bag Sort (Bag :math:`S_1`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    BAG_PARTITION=c_Kind.BAG_PARTITION, """Bag partition.

This operator partitions of a bag of elements into disjoint bags.
(bag.partition :math:`r \\; B`) partitions the elements of bag :math:`B`
of type :math:`(Bag \\; E)` based on the equivalence relations :math:`r` of
type :math:`(\\rightarrow \\; E \\; E \\; Bool)`.
It returns a bag of bags of type :math:`(Bag \\; (Bag \\; E))`.

- Arity: ``2``

  - ``1:`` Term of function Sort :math:`(\\rightarrow \\; E \\; E \\; Bool)`
  - ``2:`` Term of bag Sort (Bag :math:`E`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    TABLE_PRODUCT=c_Kind.TABLE_PRODUCT, """Table cross product.

- Arity: ``2``

  - ``1..2:`` Terms of table Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    TABLE_PROJECT=c_Kind.TABLE_PROJECT, """Table projection operator extends tuple projection operator to tables.

- Arity: ``1``
  - ``1:`` Term of table Sort

- Indices: ``n``
  - ``1..n:`` Indices of the projection

- Create Term of this Kind with:
  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:
  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    TABLE_AGGREGATE=c_Kind.TABLE_AGGREGATE, """Table aggregate operator has the form
:math:`((\\_ \\; table.aggr \\; n_1 ... n_k) \\; f \\; i \\; A)`
where :math:`n_1, ..., n_k` are natural numbers,
:math:`f` is a function of type
:math:`(\\rightarrow (Tuple \\;  T_1 \\; ... \\; T_j)\\; T \\; T)`,
:math:`i` has the type :math:`T`,
and :math:`A` has type :math:`(Table \\;  T_1 \\; ... \\; T_j)`.
The returned type is :math:`(Bag \\; T)`.

This operator aggregates elements in A that have the same tuple projection
with indices n_1, ..., n_k using the combining function :math:`f`,
and initial value :math:`i`.

- Arity: ``3``

  - ``1:`` Term of sort :math:`(\\rightarrow (Tuple \\;  T_1 \\; ... \\; T_j)\\; T \\; T)`
  - ``2:`` Term of Sort :math:`T`
  - ``3:`` Term of table sort :math:`Table T_1 ... T_j`

- Indices: ``n``
  - ``1..n:`` Indices of the projection

- Create Term of this Kind with:
  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:
  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    TABLE_JOIN=c_Kind.TABLE_JOIN, """Table join operator has the form
 :math:`((\\_ \\; table.join \\; m_1 \\; n_1 \\; \\dots \\; m_k \\; n_k) \\; A \\; B)`
 where :math:`m_1 \\; n_1 \\; \\dots \\; m_k \\; n_k` are natural numbers,
 and :math:`A, B` are tables.
 This operator filters the product of two bags based on the equality of
 projected tuples using indices :math:`m_1, \\dots, m_k` in table :math:`A`,
 and indices :math:`n_1, \\dots, n_k` in table :math:`B`.

- Arity: ``2``

  - ``1:`` Term of table Sort

  - ``2:`` Term of table Sort

- Indices: ``n``
  - ``1..n:``  Indices of the projection

- Create Term of this Kind with:
  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:
  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    TABLE_GROUP=c_Kind.TABLE_GROUP, """Table group

:math:`((\\_ \\; table.group \\; n_1 \\; \\dots \\; n_k) \\; A)` partitions tuples
of table :math:`A` such that tuples that have the same projection
with indices :math:`n_1 \\; \\dots \\; n_k` are in the same part.
It returns a bag of tables of type :math:`(Bag \\; T)` where
:math:`T` is the type of :math:`A`.

- Arity: ``1``

  - ``1:`` Term of table sort

- Indices: ``n``

  - ``1..n:``  Indices of the projection

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions."""
    STRING_CONCAT=c_Kind.STRING_CONCAT, """String concat.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_IN_REGEXP=c_Kind.STRING_IN_REGEXP, """String membership.

- Arity: ``2``

  - ``1:`` Term of Sort String
  - ``2:`` Term of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_LENGTH=c_Kind.STRING_LENGTH, """String length.

- Arity: ``1``

  - ``1:`` Term of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_SUBSTR=c_Kind.STRING_SUBSTR, """String substring.

Extracts a substring, starting at index :math:`i` and of length :math:`l`,
from a string :math:`s`.  If the start index is negative, the start index
is greater than the length of the string, or the length is negative, the
result is the empty string.

- Arity: ``3``

  - ``1:`` Term of Sort String
  - ``2:`` Term of Sort Int (index :math:`i`)
  - ``3:`` Term of Sort Int (length :math:`l`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_UPDATE=c_Kind.STRING_UPDATE, """String update.

Updates a string :math:`s` by replacing its context starting at an index
with string :math:`t`. If the start index is negative, the start index is
greater than the length of the string, the result is :math:`s`. Otherwise,
the length of the original string is preserved.

- Arity: ``3``

  - ``1:`` Term of Sort String
  - ``2:`` Term of Sort Int (index :math:`i`)
  - ``3:`` Term of Sort Strong (replacement string :math:`t`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_CHARAT=c_Kind.STRING_CHARAT, """String character at.

Returns the character at index :math:`i` from a string :math:`s`. If the
index is negative or the index is greater than the length of the string,
the result is the empty string. Otherwise the result is a string of
length 1.

- Arity: ``2``

  - ``1:`` Term of Sort String (string :math:`s`)
  - ``2:`` Term of Sort Int (index :math:`i`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_CONTAINS=c_Kind.STRING_CONTAINS, """String contains.

Determines whether a string :math:`s_1` contains another string
:math:`s_2`. If :math:`s_2` is empty, the result is always ``true``.

- Arity: ``2``

  - ``1:`` Term of Sort String (the string :math:`s_1`)
  - ``2:`` Term of Sort String (the string :math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_INDEXOF=c_Kind.STRING_INDEXOF, """String index-of.

Returns the index of a substring :math:`s_2` in a string :math:`s_1`
starting at index :math:`i`. If the index is negative or greater than the
length of string :math:`s_1` or the substring :math:`s_2` does not appear
in string :math:`s_1` after index :math:`i`, the result is -1.

- Arity: ``2``

  - ``1:`` Term of Sort String (substring :math:`s_1`)
  - ``2:`` Term of Sort String (substring :math:`s_2`)
  - ``3:`` Term of Sort Int (index :math:`i`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_INDEXOF_RE=c_Kind.STRING_INDEXOF_RE, """String index-of regular expression match.

Returns the first match of a regular expression :math:`r` in a
string :math:`s`. If the index is negative or greater than the length of
string :math:`s_1`, or :math:`r` does not match a substring in :math:`s`
after index :math:`i`, the result is -1.

- Arity: ``3``

  - ``1:`` Term of Sort String (string :math:`s`)
  - ``2:`` Term of Sort RegLan (regular expression :math:`r`)
  - ``3:`` Term of Sort Int (index :math:`i`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_REPLACE=c_Kind.STRING_REPLACE, """String replace.

Replaces a string :math:`s_2` in a string :math:`s_1` with string
:math:`s_3`. If :math:`s_2` does not appear in :math:`s_1`, :math:`s_1` is
returned unmodified.

- Arity: ``3``

  - ``1:`` Term of Sort String (string :math:`s_1`)
  - ``2:`` Term of Sort String (string :math:`s_2`)
  - ``3:`` Term of Sort String (string :math:`s_3`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_REPLACE_ALL=c_Kind.STRING_REPLACE_ALL, """String replace all.

Replaces all occurrences of a string :math:`s_2` in a string :math:`s_1`
with string :math:`s_3`. If :math:`s_2` does not appear in :math:`s_1`,
:math:`s_1` is returned unmodified.

- Arity: ``3``

  - ``1:`` Term of Sort String (:math:`s_1`)
  - ``2:`` Term of Sort String (:math:`s_2`)
  - ``3:`` Term of Sort String (:math:`s_3`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_REPLACE_RE=c_Kind.STRING_REPLACE_RE, """String replace regular expression match.

Replaces the first match of a regular expression :math:`r` in
string :math:`s_1` with string :math:`s_2`. If :math:`r` does not match a
substring of :math:`s_1`, :math:`s_1` is returned unmodified.

- Arity: ``3``

  - ``1:`` Term of Sort String (:math:`s_1`)
  - ``2:`` Term of Sort RegLan
  - ``3:`` Term of Sort String (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_REPLACE_RE_ALL=c_Kind.STRING_REPLACE_RE_ALL, """String replace all regular expression matches.

Replaces all matches of a regular expression :math:`r` in string
:math:`s_1` with string :math:`s_2`. If :math:`r` does not match a
substring of :math:`s_1`, string :math:`s_1` is returned unmodified.

- Arity: ``3``

  - ``1:`` Term of Sort String (:math:`s_1`)
  - ``2:`` Term of Sort RegLan
  - ``3:`` Term of Sort String (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_TO_LOWER=c_Kind.STRING_TO_LOWER, """String to lower case.

- Arity: ``1``

  - ``1:`` Term of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_TO_UPPER=c_Kind.STRING_TO_UPPER, """String to upper case.

- Arity: ``1``

  - ``1:`` Term of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_REV=c_Kind.STRING_REV, """String reverse.

- Arity: ``1``

  - ``1:`` Term of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_TO_CODE=c_Kind.STRING_TO_CODE, """String to code.

Returns the code point of a string if it has length one, or returns `-1`
otherwise.

- Arity: ``1``

  - ``1:`` Term of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_FROM_CODE=c_Kind.STRING_FROM_CODE, """String from code.

Returns a string containing a single character whose code point matches
the argument to this function, or the empty string if the argument is
out-of-bounds.

- Arity: ``1``

  - ``1:`` Term of Sort Int

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_LT=c_Kind.STRING_LT, """String less than.

Returns true if string :math:`s_1` is (strictly) less than :math:`s_2`
based on a lexiographic ordering over code points.

- Arity: ``2``

  - ``1:`` Term of Sort String (:math:`s_1`)
  - ``2:`` Term of Sort String (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_LEQ=c_Kind.STRING_LEQ, """String less than or equal.

Returns true if string :math:`s_1` is less than or equal to :math:`s_2`
based on a lexiographic ordering over code points.

- Arity: ``2``

  - ``1:`` Term of Sort String (:math:`s_1`)
  - ``2:`` Term of Sort String (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_PREFIX=c_Kind.STRING_PREFIX, """String prefix-of.

Determines whether a string :math:`s_1` is a prefix of string :math:`s_2`.
If string s1 is empty, this operator returns ``true``.

- Arity: ``2``

  - ``1:`` Term of Sort String (:math:`s_1`)
  - ``2:`` Term of Sort String (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_SUFFIX=c_Kind.STRING_SUFFIX, """String suffix-of.

Determines whether a string :math:`s_1` is a suffix of the second string.
If string :math:`s_1` is empty, this operator returns ``true``.

- Arity: ``2``

  - ``1:`` Term of Sort String (:math:`s_1`)
  - ``2:`` Term of Sort String (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_IS_DIGIT=c_Kind.STRING_IS_DIGIT, """String is-digit.

Returns true if given string is a digit (it is one of ``"0"``, ...,
``"9"``).

- Arity: ``1``

  - ``1:`` Term of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_FROM_INT=c_Kind.STRING_FROM_INT, """Conversion from Int to String.

If the integer is negative this operator returns the empty string.

- Arity: ``1``

  - ``1:`` Term of Sort Int

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    STRING_TO_INT=c_Kind.STRING_TO_INT, """String to integer (total function).

If the string does not contain an integer or the integer is negative, the
operator returns `-1`.

- Arity: ``1``

  - ``1:`` Term of Sort Int

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CONST_STRING=c_Kind.CONST_STRING, """Constant string.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkString()`"""
    STRING_TO_REGEXP=c_Kind.STRING_TO_REGEXP, """Conversion from string to regexp.

- Arity: ``1``

  - ``1:`` Term of Sort String

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_CONCAT=c_Kind.REGEXP_CONCAT, """Regular expression concatenation.

- Arity: ``2``

  - ``1..2:`` Terms of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_UNION=c_Kind.REGEXP_UNION, """Regular expression union.

- Arity: ``2``

  - ``1..2:`` Terms of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_INTER=c_Kind.REGEXP_INTER, """Regular expression intersection.

- Arity: ``2``

  - ``1..2:`` Terms of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_DIFF=c_Kind.REGEXP_DIFF, """Regular expression difference.

- Arity: ``2``

  - ``1..2:`` Terms of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_STAR=c_Kind.REGEXP_STAR, """Regular expression \\*.

- Arity: ``1``

  - ``1:`` Term of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_PLUS=c_Kind.REGEXP_PLUS, """Regular expression +.

- Arity: ``1``

  - ``1:`` Term of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_OPT=c_Kind.REGEXP_OPT, """Regular expression ?.

- Arity: ``1``

  - ``1:`` Term of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_RANGE=c_Kind.REGEXP_RANGE, """Regular expression range.

- Arity: ``2``

  - ``1:`` Term of Sort String (lower bound character for the range)
  - ``2:`` Term of Sort String (upper bound character for the range)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_REPEAT=c_Kind.REGEXP_REPEAT, """Operator for regular expression repeat.

- Arity: ``1``

  - ``1:`` Term of Sort RegLan

- Indices: ``1``

  - ``1:`` The number of repetitions

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_LOOP=c_Kind.REGEXP_LOOP, """Regular expression loop.

Regular expression loop from lower bound to upper bound number of
repetitions.

- Arity: ``1``

  - ``1:`` Term of Sort RegLan

- Indices: ``1``

  - ``1:`` The lower bound
  - ``2:`` The upper bound

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    REGEXP_NONE=c_Kind.REGEXP_NONE, """Regular expression none.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkRegexpNone()`"""
    REGEXP_ALL=c_Kind.REGEXP_ALL, """Regular expression all.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkRegexpAll()`"""
    REGEXP_ALLCHAR=c_Kind.REGEXP_ALLCHAR, """Regular expression all characters.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkRegexpAllchar()`"""
    REGEXP_COMPLEMENT=c_Kind.REGEXP_COMPLEMENT, """Regular expression complement.

- Arity: ``1``

  - ``1:`` Term of Sort RegLan

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_CONCAT=c_Kind.SEQ_CONCAT, """Sequence concat.

- Arity: ``n > 1``

  - ``1..n:`` Terms of sequence Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_LENGTH=c_Kind.SEQ_LENGTH, """Sequence length.

- Arity: ``1``

  - ``1:`` Term of sequence Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_EXTRACT=c_Kind.SEQ_EXTRACT, """Sequence extract.

Extracts a subsequence, starting at index :math:`i` and of length :math:`l`,
from a sequence :math:`s`.  If the start index is negative, the start index
is greater than the length of the sequence, or the length is negative, the
result is the empty sequence.

- Arity: ``3``

  - ``1:`` Term of sequence Sort
  - ``2:`` Term of Sort Int (index :math:`i`)
  - ``3:`` Term of Sort Int (length :math:`l`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_UPDATE=c_Kind.SEQ_UPDATE, """Sequence update.

Updates a sequence :math:`s` by replacing its context starting at an index
with string :math:`t`. If the start index is negative, the start index is
greater than the length of the sequence, the result is :math:`s`.
Otherwise, the length of the original sequence is preserved.

- Arity: ``3``

  - ``1:`` Term of sequence Sort
  - ``2:`` Term of Sort Int (index :math:`i`)
  - ``3:`` Term of sequence Sort (replacement sequence :math:`t`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_AT=c_Kind.SEQ_AT, """Sequence element at.

Returns the element at index :math:`i` from a sequence :math:`s`. If the index
is negative or the index is greater or equal to the length of the
sequence, the result is the empty sequence. Otherwise the result is a
sequence of length ``1``.

- Arity: ``2``

  - ``1:`` Term of sequence Sort
  - ``2:`` Term of Sort Int (index :math:`i`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_CONTAINS=c_Kind.SEQ_CONTAINS, """Sequence contains.

Checks whether a sequence :math:`s_1` contains another sequence
:math:`s_2`. If :math:`s_2` is empty, the result is always ``true``.

- Arity: ``2``

  - ``1:`` Term of sequence Sort (:math:`s_1`)
  - ``2:`` Term of sequence Sort (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_INDEXOF=c_Kind.SEQ_INDEXOF, """Sequence index-of.

Returns the index of a subsequence :math:`s_2` in a sequence :math:`s_1`
starting at index :math:`i`. If the index is negative or greater than the
length of sequence :math:`s_1` or the subsequence :math:`s_2` does not
appear in sequence :math:`s_1` after index :math:`i`, the result is -1.

- Arity: ``3``

  - ``1:`` Term of sequence Sort (:math:`s_1`)
  - ``2:`` Term of sequence Sort (:math:`s_2`)
  - ``3:`` Term of Sort Int (:math:`i`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_REPLACE=c_Kind.SEQ_REPLACE, """Sequence replace.

Replaces the first occurrence of a sequence :math:`s_2` in a
sequence :math:`s_1` with sequence :math:`s_3`. If :math:`s_2` does not
appear in :math:`s_1`, :math:`s_1` is returned unmodified.

- Arity: ``3``

  - ``1:`` Term of sequence Sort (:math:`s_1`)
  - ``2:`` Term of sequence Sort (:math:`s_2`)
  - ``3:`` Term of sequence Sort (:math:`s_3`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_REPLACE_ALL=c_Kind.SEQ_REPLACE_ALL, """Sequence replace all.

Replaces all occurrences of a sequence :math:`s_2` in a sequence
:math:`s_1` with sequence :math:`s_3`. If :math:`s_2` does not appear in
:math:`s_1`, sequence :math:`s_1` is returned unmodified.

- Arity: ``3``

  - ``1:`` Term of sequence Sort (:math:`s_1`)
  - ``2:`` Term of sequence Sort (:math:`s_2`)
  - ``3:`` Term of sequence Sort (:math:`s_3`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_REV=c_Kind.SEQ_REV, """Sequence reverse.

- Arity: ``1``

  - ``1:`` Term of sequence Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_PREFIX=c_Kind.SEQ_PREFIX, """Sequence prefix-of.

Checks whether a sequence :math:`s_1` is a prefix of sequence :math:`s_2`.
If sequence :math:`s_1` is empty, this operator returns ``true``.

- Arity: ``1``

  - ``1:`` Term of sequence Sort (:math:`s_1`)
  - ``2:`` Term of sequence Sort (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_SUFFIX=c_Kind.SEQ_SUFFIX, """Sequence suffix-of.

Checks whether a sequence :math:`s_1` is a suffix of sequence :math:`s_2`.
If sequence :math:`s_1` is empty, this operator returns ``true``.

- Arity: ``1``

  - ``1:`` Term of sequence Sort (:math:`s_1`)
  - ``2:`` Term of sequence Sort (:math:`s_2`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    CONST_SEQUENCE=c_Kind.CONST_SEQUENCE, """Constant sequence.

A constant sequence is a term that is equivalent to:

.. code:: smtlib

    (seq.++ (seq.unit c1) ... (seq.unit cn))

where :math:`n \\leq 0` and :math:`c_1, ..., c_n` are constants of some
sort. The elements can be extracted with :py:meth:`Term.getSequenceValue()`.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkEmptySequence()`"""
    SEQ_UNIT=c_Kind.SEQ_UNIT, """Sequence unit.

Corresponds to a sequence of length one with the given term.

- Arity: ``1``

  - ``1:`` Term of any Sort (the element term)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    SEQ_NTH=c_Kind.SEQ_NTH, """Sequence nth.

Corresponds to the nth element of a sequence.

- Arity: ``2``

  - ``1:`` Term of sequence Sort
  - ``2:`` Term of Sort Int (:math:`n`)

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    FORALL=c_Kind.FORALL, """Universally quantified formula.

- Arity: ``3``

  - ``1:`` Term of Kind :py:obj:`VARIABLE_LIST`
  - ``2:`` Term of Sort Bool (the quantifier body)
  - ``3:`` (optional) Term of Kind :py:obj:`INST_PATTERN`

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    EXISTS=c_Kind.EXISTS, """Existentially quantified formula.

- Arity: ``3``

  - ``1:`` Term of Kind :py:obj:`VARIABLE_LIST`
  - ``2:`` Term of Sort Bool (the quantifier body)
  - ``3:`` (optional) Term of Kind :py:obj:`INST_PATTERN`

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    VARIABLE_LIST=c_Kind.VARIABLE_LIST, """Variable list.

A list of variables (used to bind variables under a quantifier)

- Arity: ``n > 0``

  - ``1..n:`` Terms of Kind :py:obj:`VARIABLE`

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    INST_PATTERN=c_Kind.INST_PATTERN, """Instantiation pattern.

Specifies a (list of) terms to be used as a pattern for quantifier
instantiation.

- Arity: ``n > 0``

  - ``1..n:`` Terms of any Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. note:: Should only be used as a child of
          :py:obj:`INST_PATTERN_LIST`."""
    INST_NO_PATTERN=c_Kind.INST_NO_PATTERN, """Instantiation no-pattern.

Specifies a (list of) terms that should not be used as a pattern for
quantifier instantiation.

- Arity: ``n > 0``

  - ``1..n:`` Terms of any Sort

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. note:: Should only be used as a child of
          :py:obj:`INST_PATTERN_LIST`."""
    INST_POOL=c_Kind.INST_POOL, """Instantiation pool annotation.

Specifies an annotation for pool based instantiation.

In detail, pool symbols can be declared via the method
 - :py:meth:`Solver.declarePool()`

A pool symbol represents a set of terms of a given sort. An instantiation
pool annotation should either:
(1) have child sets matching the types of the quantified formula,
(2) have a child set of tuple type whose component types match the types
of the quantified formula.

For an example of (1), for a quantified formula:

.. code:: lisp

    (FORALL (VARIABLE_LIST x y) F (INST_PATTERN_LIST (INST_POOL p q)))

if :math:`x` and :math:`y` have Sorts :math:`S_1` and :math:`S_2`, then
pool symbols :math:`p` and :math:`q` should have Sorts (Set :math:`S_1`)
and (Set :math:`S_2`), respectively. This annotation specifies that the
quantified formula above should be instantiated with the product of all
terms that occur in the sets :math:`p` and :math:`q`.

Alternatively, as an example of (2), for a quantified formula:

.. code:: lisp

    (FORALL (VARIABLE_LIST x y) F (INST_PATTERN_LIST (INST_POOL s)))

:math:`s` should have Sort (Set (Tuple :math:`S_1` :math:`S_2`)). This
annotation specifies that the quantified formula above should be
instantiated with the pairs of values in :math:`s`.

- Arity: ``n > 0``

  - ``1..n:`` Terms that comprise the pools, which are one-to-one with the variables of the quantified formula to be instantiated

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions.

.. note:: Should only be used as a child of
          :py:obj:`INST_PATTERN_LIST`."""
    INST_ADD_TO_POOL=c_Kind.INST_ADD_TO_POOL, """A instantantiation-add-to-pool annotation.

An instantantiation-add-to-pool annotation indicates that when a quantified
formula is instantiated, the instantiated version of a term should be
added to the given pool.

For example, consider a quantified formula:

.. code:: lisp

    (FORALL (VARIABLE_LIST x) F
            (INST_PATTERN_LIST (INST_ADD_TO_POOL (ADD x 1) p)))

where assume that :math:`x` has type Int. When this quantified formula is
instantiated with, e.g., the term :math:`t`, the term ``(ADD t 1)`` is
added to pool :math:`p`.

- Arity: ``2``

  - ``1:`` The Term whose free variables are bound by the quantified formula.
  - ``2:`` The pool to add to, whose Sort should be a set of elements that match the Sort of the first argument.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions.

.. note:: Should only be used as a child of
          :py:obj:`INST_PATTERN_LIST`."""
    SKOLEM_ADD_TO_POOL=c_Kind.SKOLEM_ADD_TO_POOL, """A skolemization-add-to-pool annotation.

An skolemization-add-to-pool annotation indicates that when a quantified
formula is skolemized, the skolemized version of a term should be added to
the given pool.

For example, consider a quantified formula:

.. code:: lisp

    (FORALL (VARIABLE_LIST x) F
            (INST_PATTERN_LIST (SKOLEM_ADD_TO_POOL (ADD x 1) p)))

where assume that :math:`x` has type Int. When this quantified formula is
skolemized, e.g., with :math:`k` of type Int, then the term ``(ADD k 1)``
is added to the pool :math:`p`.

- Arity: ``2``

  - ``1:`` The Term whose free variables are bound by the quantified formula.
  - ``2:`` The pool to add to, whose Sort should be a set of elements that match the Sort of the first argument.

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. warning:: This kind is experimental and may be changed or removed in
             future versions.

.. note:: Should only be used as a child of
          :py:obj:`INST_PATTERN_LIST`."""
    INST_ATTRIBUTE=c_Kind.INST_ATTRIBUTE, """Instantiation attribute.

Specifies a custom property for a quantified formula given by a
term that is ascribed a user attribute.

- Arity: ``n > 0``

  - ``1:`` Term of Kind :py:obj:`CONST_STRING` (the keyword of the attribute)
  - ``2...n:`` Terms representing the values of the attribute

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`

.. note:: Should only be used as a child of
          :py:obj:`INST_PATTERN_LIST`."""
    INST_PATTERN_LIST=c_Kind.INST_PATTERN_LIST, """A list of instantiation patterns, attributes or annotations.

- Arity: ``n > 1``

  - ``1..n:`` Terms of Kind :py:obj:`INST_PATTERN`, :py:obj:`INST_NO_PATTERN`, :py:obj:`INST_POOL`, :py:obj:`INST_ADD_TO_POOL`, :py:obj:`SKOLEM_ADD_TO_POOL`, :py:obj:`INST_ATTRIBUTE`

- Create Term of this Kind with:

  - :py:meth:`Solver.mkTerm()`

- Create Op of this kind with:

  - :py:meth:`Solver.mkOp()`"""
    LAST_KIND=c_Kind.LAST_KIND, """Marks the upper-bound of this enumeration."""

from cvc5kinds cimport SortKind as c_SortKind
from enum import Enum

class DocEnum(Enum):
    def __new__(cls, value, doc=None):
        self = object.__new__(cls)
        self._value_ = value
        self.__doc__ = doc
        return self

class SortKind(DocEnum):
    """The SortKind enum"""
    INTERNAL_SORT_KIND=c_SortKind.INTERNAL_SORT_KIND, """Internal kind.

This kind serves as an abstraction for internal kinds that are not exposed
via the API but may appear in terms returned by API functions, e.g.,
when querying the simplified form of a term.

.. note:: Should never be created via the API."""
    UNDEFINED_SORT_KIND=c_SortKind.UNDEFINED_SORT_KIND, """Undefined kind.

.. note:: Should never be exposed or created via the API."""
    NULL_SORT=c_SortKind.NULL_SORT, """Null kind.

The kind of a null sort (Sort::Sort()).

.. note:: May not be explicitly created via API functions other than
          Sort::Sort()."""
    ABSTRACT_SORT=c_SortKind.ABSTRACT_SORT, """An abstract sort.

An abstract sort represents a sort whose parameters or argument sorts are
unspecified. For example, `mkAbstractSort(BITVECTOR_SORT)` returns a
sort that represents the sort of bit-vectors whose bit-width is
unspecified.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkAbstractSort()`"""
    ARRAY_SORT=c_SortKind.ARRAY_SORT, """An array sort, whose argument sorts are the index and element sorts of the
array.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkArraySort()`"""
    BAG_SORT=c_SortKind.BAG_SORT, """A bag sort, whose argument sort is the element sort of the bag.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkBagSort()`"""
    BOOLEAN_SORT=c_SortKind.BOOLEAN_SORT, """The Boolean sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.getBooleanSort()`"""
    BITVECTOR_SORT=c_SortKind.BITVECTOR_SORT, """A bit-vector sort, parameterized by an integer denoting its bit-width.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkBitVectorSort()`"""
    DATATYPE_SORT=c_SortKind.DATATYPE_SORT, """A datatype sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkDatatypeSort()`
  - :py:meth:`Solver.mkDatatypeSorts()`"""
    FINITE_FIELD_SORT=c_SortKind.FINITE_FIELD_SORT, """A finite field sort, parameterized by a size.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkFiniteFieldSort()`"""
    FLOATINGPOINT_SORT=c_SortKind.FLOATINGPOINT_SORT, """A floating-point sort, parameterized by two integers denoting its
exponent and significand bit-widths.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkFloatingPointSort()`"""
    FUNCTION_SORT=c_SortKind.FUNCTION_SORT, """A function sort with given domain sorts and codomain sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkFunctionSort()`"""
    INTEGER_SORT=c_SortKind.INTEGER_SORT, """The integer sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.getIntegerSort()`"""
    REAL_SORT=c_SortKind.REAL_SORT, """The real sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.getRealSort()`"""
    REGLAN_SORT=c_SortKind.REGLAN_SORT, """The regular language sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.getRegExpSort()`"""
    ROUNDINGMODE_SORT=c_SortKind.ROUNDINGMODE_SORT, """The rounding mode sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.getRoundingModeSort()`"""
    SEQUENCE_SORT=c_SortKind.SEQUENCE_SORT, """A sequence sort, whose argument sort is the element sort of the sequence.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkSequenceSort()`"""
    SET_SORT=c_SortKind.SET_SORT, """A set sort, whose argument sort is the element sort of the set.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkSetSort()`"""
    STRING_SORT=c_SortKind.STRING_SORT, """The string sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.getStringSort()`"""
    TUPLE_SORT=c_SortKind.TUPLE_SORT, """A tuple sort, whose argument sorts denote the sorts of the direct children
of the tuple.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkTupleSort()`"""
    NULLABLE_SORT=c_SortKind.NULLABLE_SORT, """A nullable sort, whose argument sort denotes the sort of the direct child
of the nullable.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkNullableSort()`"""
    UNINTERPRETED_SORT=c_SortKind.UNINTERPRETED_SORT, """An uninterpreted sort.

- Create Sort of this Kind with:

  - :py:meth:`Solver.mkUninterpretedSort()`"""
    LAST_SORT_KIND=c_SortKind.LAST_SORT_KIND, """Marks the upper-bound of this enumeration."""
