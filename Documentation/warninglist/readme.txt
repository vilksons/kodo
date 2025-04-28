200 symbol "foo" is truncated to foo characters  
Cause: Symbol name exceeds maximum length limit.  
Fix: Shorten the symbol name.  

201 redefinition of constant/macro (symbol "foo")  
Cause: Duplicate definition of symbol.  
Fix: Remove duplicate definition or use #undef.  

202 number of arguments does not match definition  
Cause: Argument count mismatch in function call.  
Fix: Match parameter count to declaration.  

203 symbol is never used: "foo"  
Cause: Unreferenced variable or function.  
Fix: Remove unused symbol or implement usage.  

204 symbol is assigned a value that is never used: "foo"  
Cause: Unused assignment.  
Fix: Remove assignment or use the value.  

205 redundant code: constant expression is zero  
Cause: Expression always evaluates to zero.  
Fix: Remove or correct the expression.  

206 redundant test: constant expression is non-zero  
Cause: Condition always true/false.  
Fix: Simplify or correct the condition.  

207 unknown #pragma  
Cause: Unrecognized compiler directive.  
Fix: Use valid #pragma or remove.  

208 function with tag result used before definition, forcing reparse  
Cause: Forward reference to tagged function.  
Fix: Define function before use or add forward declaration.  

209 function "foo" should return a value  
Cause: Non-void function missing return.  
Fix: Add return statement or change return type.  

210 possible use of symbol before initialization: "foo"  
Cause: Variable used without being set.  
Fix: Initialize before use.  

211 possibly unintended assignment  
Cause: Assignment in conditional context.  
Fix: Use == for comparison if intended.  

212 possibly unintended bitwise operation  
Cause: Bitwise operator where logical may be intended.  
Fix: Use &&/|| if logical ops were meant.  

213 tag mismatch: expected foo foo but found foo  
Cause: Type tag inconsistency.  
Fix: Match expected and actual types.  

214 possibly a "const" array argument was intended: "foo"  
Cause: Array parameter not marked const.  
Fix: Add const qualifier if appropriate.  

215 expression has no effect  
Cause: Statement does nothing.  
Fix: Remove or correct the expression.  

216 nested comment  
Cause: Comment inside another comment.  
Fix: Remove nested comments.  

217 loose indentation  
Cause: Inconsistent code formatting.  
Fix: Maintain consistent indentation.  

218 old style prototypes used with optional semicolons  
Cause: Outdated function declaration syntax.  
Fix: Use modern syntax with required semicolons.  

219 local variable "foo" shadows a variable at a preceding level  
Cause: Variable name reuse in nested scope.  
Fix: Rename variable to avoid shadowing.  

220 expression with tag override must appear between parentheses  
Cause: Missing parentheses for tagged expression.  
Fix: Add parentheses around tagged expression.  

221 label name "foo" shadows tag name  
Cause: Label conflicts with type tag.  
Fix: Rename label or tag.  

222 number of digits exceeds rational number precision  
Cause: Too many digits for rational type.  
Fix: Reduce precision or change type.  

223 redundant "sizeof": argument size is always 1 (symbol "foo")  
Cause: sizeof used on single-cell value.  
Fix: Remove unnecessary sizeof.  

224 indeterminate array size in "sizeof" expression (symbol "foo")  
Cause: sizeof used on unsized array.  
Fix: Specify array size or use proper variable.  

225 unreachable code  
Cause: Code after return/break/etc.  
Fix: Remove or reorganize code flow.  

226 a variable is assigned to itself (symbol "foo")  
Cause: Self-assignment has no effect.  
Fix: Remove or correct assignment.  

227 more initializers than enum fields  
Cause: Excess values in enum initialization.  
Fix: Match initializer count to enum fields.  

228 length of initializer exceeds size of the enum field  
Cause: Value too large for enum type.  
Fix: Use smaller value or adjust enum.  

229 index tag mismatch (symbol "foo"): expected tag foo but found foo  
Cause: Array index type mismatch.  
Fix: Use correct index type.  

230 no implementation for state "foo" in function "foo", no fall-back  
Cause: Missing state implementation.  
Fix: Implement state or provide fallback.  

231 state specification on forward declaration is ignored  
Cause: States in forward declaration.  
Fix: Remove states from forward declaration.  

232 output file is written, but with compact encoding disabled  
Cause: Compiler setting issue.  
Fix: Enable compact encoding if needed.  

233 state variable "foo" shadows a global variable  
Cause: State variable name conflict.  
Fix: Rename state variable.  

234 function is deprecated (symbol "foo") foo  
Cause: Use of obsolete function.  
Fix: Use recommended alternative.  

235 public function lacks forward declaration (symbol "foo")  
Cause: Missing forward declaration.  
Fix: Add proper forward declaration.  

236 unknown parameter in substitution (incorrect #define pattern)  
Cause: Invalid macro parameter.  
Fix: Correct macro parameter usage.  

237 user warning: foo  
Cause: Script-generated warning.  
Fix: Address the warning condition.  

238 meaningless combination of class specifiers (foo)  
Cause: Conflicting modifiers.  
Fix: Remove incompatible specifiers.  

239 literal array/string passed to a non-const parameter  
Cause: Potential modification of literal.  
Fix: Add const to parameter or use variable.