100 cannot read from file: "foo"
Cause: The file does not exist, is not accessible, or the path is incorrect.
Fix: Check file existence, permissions, and path correctness.

101 cannot write to file: "foo"
Cause: Insufficient permissions or invalid path.
Fix: Verify write permissions and path validity.

102 table overflow: "foo"
Cause: Symbol table or other compiler structure exceeded its limit.
Fix: Reduce code complexity or split into smaller scripts.

103 insufficient memory
Cause: System or compiler ran out of memory.
Fix: Close other applications or simplify script.

104 invalid assembler instruction "foo"
Cause: Unsupported or malformed assembly instruction.
Fix: Remove or correct the assembly code.

105 numeric overflow, exceeding capacity
Cause: Value exceeds data type limits.
Fix: Use appropriate data types or reduce values.

106 compiled script exceeds the maximum memory size (foo bytes)
Cause: Script too large for SA-MP limits.
Fix: Optimize or split the script.

107 too many error messages on one line
Cause: Multiple syntax errors in single line.
Fix: Fix all syntax issues on that line.

108 codepage mapping file not found
Cause: Missing character encoding file.
Fix: Ensure codepage file exists in correct path.

109 invalid path: "foo"
Cause: Malformed or inaccessible file path.
Fix: Correct the path syntax and permissions.

110 assertion failed: foo
Cause: Assertion condition evaluated to false.
Fix: Check why assertion failed in code logic.

111 user error: foo
Cause: Script called abort() or similar.
Fix: Review script logic triggering the error.

001 expected token: "foo", but found "foo"
Cause: Syntax error, wrong token encountered.
Fix: Correct syntax according to language rules.

002 only a single statement (or expression) can follow each "case"
Cause: Multiple statements after case without braces.
Fix: Use compound block with braces for multiple statements.

003 declaration of a local variable must appear in a compound block
Cause: Variable declared outside allowed scope.
Fix: Move declaration inside appropriate block.

004 function "foo" is not implemented
Cause: Function declared but not defined.
Fix: Implement the missing function.

005 function may not have arguments
Cause: Function type doesn't allow arguments.
Fix: Remove arguments or change function type.

006 must be assigned to an array
Cause: Non-array variable used where array required.
Fix: Use proper array variable.

007 operator cannot be redefined
Cause: Attempt to override built-in operator.
Fix: Remove operator redefinition.

008 must be a constant expression; assumed zero
Cause: Non-constant value used where constant required.
Fix: Use literal or const-qualified value.

009 invalid array size (negative, zero or out of bounds)
Cause: Invalid dimension specified for array.
Fix: Use positive, non-zero size within limits.

010 invalid function or declaration
Cause: Malformed function syntax or declaration.
Fix: Correct function syntax.

011 stack offset/data address must be a multiple of cell size
Cause: Memory alignment violation.
Fix: Adjust data alignment.

012 invalid function call, not a valid address
Cause: Called non-function or invalid pointer.
Fix: Ensure valid function address.

013 no entry point (no public functions)
Cause: Script lacks required public functions.
Fix: Add at least one public function.

014 invalid statement; not in switch
Cause: Case/default outside switch block.
Fix: Move inside switch or remove.

015 "default" case must be the last case in switch statement
Cause: Default not at end of switch.
Fix: Move default case to end.

016 multiple defaults in "switch"
Cause: More than one default case.
Fix: Keep only one default case.

017 undefined symbol "foo"
Cause: Using undeclared identifier.
Fix: Declare symbol before use.

018 initialization data exceeds declared size
Cause: Too many initializer values.
Fix: Match initializer count to size.

019 not a label: "foo"
Cause: Invalid label syntax or usage.
Fix: Correct label syntax or usage.

020 invalid symbol name "foo"
Cause: Used reserved word or malformed name.
Fix: Choose valid identifier name.

021 symbol already defined: "foo"
Cause: Duplicate declaration.
Fix: Remove duplicate or rename.

022 must be lvalue (non-constant)
Cause: Assignment to non-modifiable value.
Fix: Assign to variable, not constant.

023 array assignment must be simple assignment
Cause: Complex operation in array init.
Fix: Use simple assignments for arrays.

024 "break" or "continue" is out of context
Cause: Used outside loop/switch.
Fix: Use only within proper constructs.

025 function heading differs from prototype
Cause: Declaration/definition mismatch.
Fix: Make consistent.

026 no matching "#if..."
Cause: Unmatched conditional directive.
Fix: Add matching #endif.

027 invalid character constant
Cause: Malformed character literal.
Fix: Use proper character syntax.

028 invalid subscript (not an array or too many subscripts): "foo"
Cause: Non-array indexed or excessive dimensions.
Fix: Correct array usage.

029 invalid expression, assumed zero
Cause: Unparseable expression.
Fix: Rewrite valid expression.

030 compound statement not closed at the end of file (started at line foo)
Cause: Missing closing brace.
Fix: Add } to close block.

031 unknown directive
Cause: Invalid preprocessor command.
Fix: Use valid directive or remove.

032 array index out of bounds (variable "foo")
Cause: Index exceeds array size.
Fix: Use valid index.

033 array must be indexed (variable "foo")
Cause: Array used without index.
Fix: Add proper indexing.

034 argument does not have a default value (argument foo)
Cause: Missing required argument.
Fix: Provide all required arguments.

035 argument type mismatch (argument foo)
Cause: Wrong parameter type passed.
Fix: Match declared types.

036 empty statement
Cause: Extraneous semicolon.
Fix: Remove or add proper statement.

037 invalid string (possibly non-terminated string)
Cause: Missing closing quote.
Fix: Terminate string properly.

038 extra characters on line
Cause: Garbage after statement.
Fix: Remove or correct syntax.

039 constant symbol has no size
Cause: Invalid constant usage.
Fix: Use proper constant expression.

040 duplicate "case" label (value foo)
Cause: Repeated case value.
Fix: Remove duplicate case.

041 invalid ellipsis, array size is not known
Cause: ... used improperly.
Fix: Specify array size.

042 invalid combination of class specifiers
Cause: Conflicting modifiers.
Fix: Use compatible specifiers.

043 character constant exceeds range for packed string
Cause: Character value too large.
Fix: Use valid character value.

044 positional parameters must precede all named parameters
Cause: Named before positional args.
Fix: Reorder parameters.

045 too many function arguments
Cause: Exceeded parameter limit.
Fix: Reduce arguments or refactor.

046 unknown array size (variable "foo")
Cause: Array declared without size.
Fix: Specify array dimensions.

047 array sizes do not match, or destination array is too small
Cause: Size mismatch in array operation.
Fix: Use equally sized arrays.

048 array dimensions do not match
Cause: Different dimension counts.
Fix: Match array structures.

049 invalid line continuation
Cause: Malformed line continuation.
Fix: Correct backslash usage.

050 invalid range
Cause: Invalid range specification.
Fix: Use proper range syntax.

051 invalid subscript, use "[ ]" operators on major dimensions
Cause: Wrong array access syntax.
Fix: Use correct bracket syntax.

052 multi-dimensional arrays must be fully initialized
Cause: Partial initialization.
Fix: Provide complete initializers.

053 exceeding maximum number of dimensions
Cause: Too many array dimensions.
Fix: Reduce dimensionality.

054 unmatched closing brace ("}")
Cause: Extra closing brace.
Fix: Remove or add opening brace.

055 start of function body without function header
Cause: Code block without function declaration.
Fix: Add proper function header.

056 arrays, local variables and function arguments cannot be public (variable "foo")
Cause: Invalid public declaration.
Fix: Remove public qualifier.

057 unfinished expression before compiler directive
Cause: Directive interrupts expression.
Fix: Complete expression first.

058 duplicate argument; same argument is passed twice
Cause: Repeated parameter.
Fix: Remove duplicate.

059 function argument may not have a default value (variable "foo")
Cause: Default on non-optional param.
Fix: Remove default value.

060 multiple "#else" directives between "#if ... #endif"
Cause: More than one #else.
Fix: Keep single #else.

061 "#elseif" directive follows an "#else" directive
Cause: #elseif after #else.
Fix: Reorder conditionals.

062 number of operands does not fit the operator
Cause: Wrong operand count.
Fix: Provide correct operands.

063 function result tag of operator "foo" must be "foo"
Cause: Operator return type mismatch.
Fix: Match required return type.

064 cannot change predefined operators
Cause: Attempt to modify built-in.
Fix: Don't override core operators.

065 function argument may only have a single tag (argument foo)
Cause: Multiple type tags.
Fix: Use single type specifier.

066 function argument may not be a reference argument or an array (argument "foo")
Cause: Invalid parameter type.
Fix: Change parameter type.

067 variable cannot be both a reference and an array (variable "foo")
Cause: Conflicting modifiers.
Fix: Choose one type.

068 invalid rational number precision in #pragma
Cause: Bad rational number setting.
Fix: Use valid precision.

069 rational number format already defined
Cause: Duplicate definition.
Fix: Remove redundant #pragma.

070 rational number support was not enabled
Cause: Missing required #pragma.
Fix: Enable rational support.

071 user-defined operator must be declared before use (function "foo")
Cause: Forward reference to operator.
Fix: Declare before using.

072 "sizeof" operator is invalid on "function" symbols
Cause: sizeof used on function.
Fix: Apply to data only.

073 function argument must be an array (argument "foo")
Cause: Non-array passed to array param.
Fix: Pass array argument.

074 #define pattern must start with an alphabetic character
Cause: Invalid macro name.
Fix: Start with letter.

075 input line too long (after substitutions)
Cause: Macro expansion too large.
Fix: Simplify or split line.

076 syntax error in the expression, or invalid function call
Cause: General syntax error.
Fix: Correct expression syntax.

077 malformed UTF-8 encoding, or corrupted file: foo
Cause: Invalid file encoding.
Fix: Use proper UTF-8 encoding.

078 function uses both "return" and "return <value>"
Cause: Mixed return types.
Fix: Use consistent return style.

079 inconsistent return types (array & non-array)
Cause: Array/non-array mismatch.
Fix: Make return types consistent.

080 unknown symbol, or not a constant symbol (symbol "foo")
Cause: Undeclared or non-const symbol.
Fix: Declare properly as const.

081 cannot take a tag as a default value for an indexed array parameter (symbol "foo")
Cause: Invalid default for array param.
Fix: Remove or change default.

082 user-defined operators and native functions may not have states
Cause: Invalid state usage.
Fix: Remove state specifier.

083 a function or variable may only belong to a single automaton (symbol "foo")
Cause: Multiple automata specified.
Fix: Assign to single automaton.

084 state conflict: one of the states is already assigned to another implementation (symbol "foo")
Cause: State redefined.
Fix: Use unique state names.

085 no states are defined for symbol "foo"
Cause: Missing state definitions.
Fix: Add proper state declarations.

086 unknown automaton "foo"
Cause: Undeclared automaton.
Fix: Declare automaton first.

087 unknown state "foo" for automaton "foo"
Cause: Undefined state.
Fix: Define state properly.

088 public variables and local variables may not have states (symbol "foo")
Cause: Invalid state usage.
Fix: Remove state from variable.

089 state variables may not be initialized (symbol "foo")
Cause: Invalid initialization.
Fix: Remove initializer.

090 public functions may not return arrays (symbol "foo")
Cause: Array return from public func.
Fix: Change return type.

091 ambiguous constant; tag override is required (symbol "foo")
Cause: Unclear constant type.
Fix: Add explicit type tag.

092 functions may not return arrays of unknown size (symbol "foo")
Cause: Undimensioned array return.
Fix: Specify array size.

093 "__addressof" operator is invalid in preprocessor expressions
Cause: Invalid preprocessor usage.
Fix: Use only in runtime code.