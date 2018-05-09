#include "codegen.hpp"
#include "types.hpp"
#include "expressions.hpp"
#include "declarations.hpp"
#include "statements.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>

/// Associates declarations with values.
using variable_map = std::unordered_map<const decl*, llvm::Value*>;

// -------------------------------------------------------------------------- //
// Root context

/// The root code generation context. This provides facilities for 
/// translating Beaker types and constants into LLVM types and constants.
struct cg_context
{
  cg_context()
    : ll(new llvm::LLVMContext())
  { }

  /// Destroys the codegen context.
  ~cg_context() { delete ll; }

  /// Returns the LLVM context.
  llvm::LLVMContext *get_context() const { return ll; }

  // Names

  /// Returns a name for the declaration.
  std::string get_name(const decl* d);

  // Types

  /// Generate the corresponding type for `t`.
  llvm::Type* get_type(const type* t);
  llvm::Type* get_bool_type(const bool_type* t);
  llvm::Type* get_char_type(const char_type* t);
  llvm::Type* get_int_type(const int_type* t);
  llvm::Type* get_float_type(const float_type* t);
  llvm::Type* get_ref_type(const ref_type* t);
  llvm::Type* get_fn_type(const fn_type* t);

  /// Returns the corresponding type for the declaration `d`.
  llvm::Type* get_type(const typed_decl* d);

  /// The underlying LLVM context.
  llvm::LLVMContext* ll;
};

// -------------------------------------------------------------------------- //
// Module context

/// The module generation context provides facilities for generating 
/// and referencing module-level declarations.
struct cg_module
{
  cg_module(cg_context& cxt, const prog_decl* prog);

  /// Returns the LLVM context.
  llvm::LLVMContext* get_context() const { return parent->get_context(); }

  /// Returns the underlying LLVM module.
  llvm::Module* get_module() const { return mod; }

  //  Names

  /// Generates a declaration name for `d`.
  std::string get_name(const decl* d) { return parent->get_name(d); }

  // Types

  /// Generate a corresponding type to `t`.
  llvm::Type* get_type(const type* t) { return parent->get_type(t); }

  /// Generates a type corresponding to the type `d`.
  llvm::Type* get_type(const typed_decl* d) { return parent->get_type(d); }

  // Global values

  /// Associate the value `v` with the global declaration `d`.
  void declare(const decl* d, llvm::GlobalValue* v);

  /// Returns the global value corresponding to `d` or nullptr.
  llvm::GlobalValue* lookup(const decl* d) const;

  // Declaration generation

  /// Process expressions as top-level declarations.
  void generate();
  void generate(const decl* d);
  void generate_var_decl(const var_decl* d);
  void generate_fn_decl(const fn_decl* d);

  /// The parent context.
  cg_context* parent; 

  /// The corresponding translation unit.
  const prog_decl* prog;  
  
  /// The underlying LLVM module.
  llvm::Module* mod; 

  /// A lookup table for global modules.
  variable_map globals;
};

// -------------------------------------------------------------------------- //
// Function context

/// Provides the codegen context for expressions.
struct cg_function
{
  cg_function(cg_module& m, const fn_decl* d);

  // Context

  /// Returns the LLVM context.
  llvm::LLVMContext* get_context() const { return parent->get_context(); }

  /// Returns the owning LLVM module.
  llvm::Module* get_module() const { return parent->get_module(); }

  /// Returns the underlying LLVM Function.
  llvm::Function* get_function() const { return fn; }

  // Names

  /// Returns the name for the declaration `d`.
  std::string get_name(const decl* d) { return parent->get_name(d); }

  // Types

  /// Generates the type corresponding to the expression `e`.
  llvm::Type* get_type(const type* t) { return parent->get_type(t); }

  /// Generates the type corresponding to the expression `e`.
  llvm::Type* get_type(const expr* e) { return get_type(e->get_type()); }

  /// Generate the corresponding type for `t`.
  llvm::Type* get_type(const typed_decl* t) { return parent->get_type(t); }

  // Local variables

  /// Declare a new local value.
  void declare(const decl* x, llvm::Value* v);

  /// Lookup a value. This may return a global value.
  llvm::Value* lookup(const decl* x) const;

  // Function definition

  void define();

  // Block management

  /// Returns the entry block. This is where local variables are allocated.
  llvm::BasicBlock* get_entry_block() const { return entry; }

  /// Returns the current block.
  llvm::BasicBlock* get_current_block() const { return curr; }

  /// Returns a new block with the given name. The block is unlinked until
  /// it is emitted.
  llvm::BasicBlock* make_block(const char* label);

  /// Emits a new block, making it active.
  void emit_block(llvm::BasicBlock* bb);

  // Instruction generation

  /// Generate a list of instructions to compute the value of e.
  llvm::Value* generate_expr(const expr* e);
  llvm::Value* generate_bool_expr(const bool_expr* e);
  llvm::Value* generate_int_expr(const int_expr* e);
  llvm::Value* generate_float_expr(const float_expr* e);
  llvm::Value* generate_id_expr(const id_expr* e);
  llvm::Value* generate_unop_expr(const unop_expr* e);
  llvm::Value* generate_arithmetic_expr(const unop_expr* e);
  llvm::Value* generate_int_expr(const unop_expr* e);
  llvm::Value* generate_float_expr(const unop_expr* e);
  llvm::Value* generate_bitwise_expr(const unop_expr* e);
  llvm::Value* generate_logical_expr(const unop_expr* e);
  llvm::Value* generate_address_expr(const unop_expr* e);
  llvm::Value* generate_deref_expr(const unop_expr* e);
  llvm::Value* generate_binop_expr(const binop_expr* e);
  llvm::Value* generate_arithmetic_expr(const binop_expr* e);
  llvm::Value* generate_int_expr(const binop_expr* e);
  llvm::Value* generate_float_expr(const binop_expr* e);
  llvm::Value* generate_bitwise_expr(const binop_expr* e);
  llvm::Value* generate_logical_expr(const binop_expr* e);
  llvm::Value* generate_and_expr(const binop_expr* e);
  llvm::Value* generate_or_expr(const binop_expr* e);
  llvm::Value* generate_relational_expr(const binop_expr* e);
  llvm::Value* generate_call_expr(const call_expr* e);
  llvm::Value* generate_index_expr(const index_expr* e);
  llvm::Value* generate_cast_expr(const cast_expr* e);
  llvm::Value* generate_cond_expr(const cond_expr* e);
  llvm::Value* generate_assign_expr(const assign_expr* e);
  llvm::Value* generate_conv_expr(const conv_expr* e);

  // Statements
  void generate_stmt(const stmt* s);
  void generate_block_stmt(const block_stmt* s);
  void generate_when_stmt(const when_stmt* s);
  void generate_if_stmt(const if_stmt* s);
  void generate_while_stmt(const while_stmt* s);
  void generate_break_stmt(const break_stmt* s);
  void generate_cont_stmt(const cont_stmt* s);
  void generate_ret_stmt(const ret_stmt* s);
  void generate_decl_stmt(const decl_stmt* s);
  void generate_expr_stmt(const expr_stmt* s);

  // Local declarations
  void generate_decl(const decl* d);
  void generate_var_decl(const var_decl* d);

  void make_variable(const var_decl* d);
  void make_reference(const var_decl* d);

  /// The parent module context.
  cg_module* parent;

  /// The function original function
  const fn_decl* src;
  
  /// The underlying function being defined
  llvm::Function* fn;
  
  /// The entry block.
  llvm::BasicBlock* entry;

  /// The current block.
  llvm::BasicBlock* curr; 
  
  /// Local variables.
  variable_map locals;
};

// -------------------------------------------------------------------------- //
// Context implementation

std::string
cg_context::get_name(const decl* d)
{
  assert(d->get_name());
  return *d->get_name();
}

/// Generate the corresponding type.
llvm::Type* 
cg_context::get_type(const type* t)
{
  // Make sure we're looking at the semantic, not lexical type.
  switch (t->get_kind()) {
  case type::bool_kind:
    return get_bool_type(static_cast<const bool_type*>(t));
  case type::char_kind:
    return get_char_type(static_cast<const char_type*>(t));
  case type::int_kind:
    return get_int_type(static_cast<const int_type*>(t));
  case type::float_kind:
    return get_float_type(static_cast<const float_type*>(t));
  case type::ref_kind:
    return get_ref_type(static_cast<const ref_type*>(t));
  case type::fn_kind:
    return get_fn_type(static_cast<const fn_type*>(t));
  default:
    throw std::logic_error("invalid type");
  }
}

/// The corresponding type is i1.
llvm::Type*
cg_context::get_bool_type(const bool_type* t)
{
  return llvm::Type::getInt1Ty(*ll);
}

/// The corresponding type is i8.
llvm::Type*
cg_context::get_char_type(const char_type* t)
{
  return llvm::Type::getInt8Ty(*ll);
}

/// The corresponding type is i32.
llvm::Type*
cg_context::get_int_type(const int_type* t)
{
  return llvm::Type::getInt32Ty(*ll);
}

/// The corresponding type is float.
llvm::Type*
cg_context::get_float_type(const float_type* t)
{
  return llvm::Type::getFloatTy(*ll);
}

/// Returns a pointer to the object type.
llvm::Type*
cg_context::get_ref_type(const ref_type* t)
{
  llvm::Type* obj = get_type(t->get_object_type());
  return obj->getPointerTo();
}

/// Generate the type as a pointer. The actual function type can extracted
/// as needed for creating functions.
llvm::Type* 
cg_context::get_fn_type(const fn_type* t)
{
  const type_list& ps = t->get_parameter_types();
  std::vector<llvm::Type*> parms(ps.size());
  std::transform(ps.begin(), ps.end(), parms.begin(), [this](const type* p) {
    return get_type(p);
  });
  llvm::Type* ret = get_type(t->get_return_type());
  llvm::Type* base = llvm::FunctionType::get(ret, parms, false);
  return base->getPointerTo();
}

llvm::Type*
cg_context::get_type(const typed_decl* d)
{
  return get_type(d->get_type());
}

// -------------------------------------------------------------------------- //
// Module implementation

/// \todo Derive the name of the output file from compiler options.
cg_module::cg_module(cg_context& cxt, const prog_decl* prog)
  : parent(&cxt), 
    prog(prog), 
    mod(new llvm::Module("a.ll", *get_context()))
{ }

void
cg_module::declare(const decl* d, llvm::GlobalValue* v)
{
  assert(globals.count(d) == 0);
  globals.emplace(d, v);
}

llvm::GlobalValue*
cg_module::lookup(const decl* d) const
{
  auto iter = globals.find(d);
  if (iter != globals.end())
    return llvm::cast<llvm::GlobalValue>(iter->second);
  else
    return nullptr;
}

/// Process top-level declarations.
void 
cg_module::generate()
{
  for (const decl* d : prog->get_declarations())
    generate(d);
}

void
cg_module::generate(const decl* d)
{
  switch (d->get_kind()) {
  case decl::var_kind:
    return generate_var_decl(static_cast<const var_decl*>(d));
  
  case decl::fn_kind:
    return generate_fn_decl(static_cast<const fn_decl*>(d));

  default: 
    throw std::logic_error("invalid declaration");
  }
}

/// Generate a variable.
///
/// \todo To declare a global variable, we need to determine if it is
/// statically or dynamically initialized. A variable is statically 
/// initialized if it's initializer can be constant folded. Right now,
/// global variables are simply zero-initialized.
///
/// \todo Make a variable initialization context like we do for functions?
/// That might be pretty elegant.
void 
cg_module::generate_var_decl(const var_decl* d)
{
  std::string n = get_name(d);
  llvm::Type* t = get_type(d->get_type());
  llvm::Constant* c = llvm::Constant::getNullValue(t);
  llvm::GlobalVariable* var = new llvm::GlobalVariable(
      *mod, t, false, llvm::GlobalVariable::ExternalLinkage, c, n);

  // Create the binding.
  declare(d, var);
}

/// Generate a function from the fn expression.
void 
cg_module::generate_fn_decl(const fn_decl* d)
{
  cg_function fn(*this, d);
  fn.define();
}

// -------------------------------------------------------------------------- //
// Function implementation

static llvm::FunctionType*
get_fn_type(llvm::Type* t)
{
  assert(llvm::isa<llvm::PointerType>(t));
  return llvm::cast<llvm::FunctionType>(t->getPointerElementType());
}

cg_function::cg_function(cg_module& m, const fn_decl* d)
  : parent(&m), src(d), fn(), entry(), curr()
{
  std::string n = get_name(d);
  llvm::Type* t = get_type(d);
  fn = llvm::Function::Create(
      get_fn_type(t), llvm::Function::ExternalLinkage, n, get_module());

  // Create a binding in the module.
  parent->declare(d, fn);
  
  // Build and emit the entry block.
  entry = make_block("entry");
  emit_block(entry);

  llvm::IRBuilder<> ir(get_current_block());
  
  // Configure function parameters and declare them as locals.
  assert(d->get_parameters().size() == fn->arg_size());
  auto pi = d->get_parameters().begin();
  auto ai = fn->arg_begin();
  while (ai != fn->arg_end()) {
    const parm_decl* parm = static_cast<const parm_decl*>(*pi);
    llvm::Argument& arg = *ai;

    // Configure each parameter.
    arg.setName(get_name(parm));

    // Declare local variable for each parameter and initialize it
    // with wits corresponding value.
    llvm::Value* var = ir.CreateAlloca(arg.getType(), nullptr, arg.getName());
    declare(parm, var);

    // Initialize with the value of the argument.
    ir.CreateStore(&arg, var);
    
    ++ai;
    ++pi;
  }

}

void
cg_function::declare(const decl* d, llvm::Value* v)
{
  assert(locals.count(d) == 0);
  locals.emplace(d, v);
}

llvm::Value*
cg_function::lookup(const decl* d) const
{
  auto iter = locals.find(d);
  if (iter != locals.end())
    return iter->second;
  else
    return parent->lookup(d);
}

llvm::BasicBlock*
cg_function::make_block(const char* label)
{
  return llvm::BasicBlock::Create(*get_context(), label);
}

void
cg_function::emit_block(llvm::BasicBlock* bb)
{
  bb->insertInto(get_function());
  curr = bb;
}

/// Creates a return instruction for the expression.
void
cg_function::define()
{
  generate_stmt(src->get_body());
}

llvm::Value*
cg_function::generate_expr(const expr* e)
{
  switch (e->get_kind()) {
  case expr::bool_kind:
    return generate_bool_expr(static_cast<const bool_expr*>(e));
  case expr::int_kind:
    return generate_int_expr(static_cast<const int_expr*>(e));
  case expr::float_kind:
    return generate_float_expr(static_cast<const float_expr*>(e));
  case expr::id_kind:
    return generate_id_expr(static_cast<const id_expr*>(e));
  case expr::unop_kind:
    return generate_unop_expr(static_cast<const unop_expr*>(e));
  case expr::binop_kind:
    return generate_binop_expr(static_cast<const binop_expr*>(e));
  case expr::call_kind:
    return generate_call_expr(static_cast<const call_expr*>(e));
  case expr::index_kind:
    return generate_index_expr(static_cast<const index_expr*>(e));
  case expr::cond_kind:
    return generate_cond_expr(static_cast<const cond_expr*>(e));
  case expr::assign_kind:
    return generate_assign_expr(static_cast<const assign_expr*>(e));
  case expr::conv_kind:
    return generate_conv_expr(static_cast<const conv_expr*>(e));
  default: 
    throw std::runtime_error("invalid expression");
  }
}

llvm::Value*
cg_function::generate_bool_expr(const bool_expr* e)
{
  return llvm::ConstantInt::get(get_type(e), e->get_value(), false);
}

llvm::Value*
cg_function::generate_int_expr(const int_expr* e)
{
  return llvm::ConstantInt::get(get_type(e), e->get_value(), true);
}

llvm::Value*
cg_function::generate_float_expr(const float_expr* e)
{
  return llvm::ConstantFP::get(get_type(e), e->get_value());
}

llvm::Value*
cg_function::generate_id_expr(const id_expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_unop_expr(const unop_expr* e)
{
  return nullptr;
}

// Note that &e is equivalent to e. This is because e is already an address.
llvm::Value*
cg_function::generate_address_expr(const unop_expr* e)
{
  return nullptr;
}

// Note that *e is equivalent to e. This is because e is already an address.
llvm::Value*
cg_function::generate_deref_expr(const unop_expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_binop_expr(const binop_expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_relational_expr(const binop_expr* e)
{
  // llvm::Value* lhs = generate_expr(e->get_lhs());
  // llvm::Value* rhs = generate_expr(e->get_rhs());
  // llvm::IRBuilder<> ir(get_current_block());
  // switch (e->get_operator()) {
  // case op_eq:
  //   return ir.CreateICmpEQ(lhs, rhs);
  // case op_ne:
  //   return ir.CreateICmpNE(lhs, rhs);
  // case op_lt:
  //   return ir.CreateICmpSLT(lhs, rhs);
  // case op_gt:
  //   return ir.CreateICmpSGT(lhs, rhs);
  // case op_le:
  //   return ir.CreateICmpSLE(lhs, rhs);
  // case op_ge:
  //   return ir.CreateICmpSGE(lhs, rhs);
  // default:
  //   throw std::logic_error("invalid operator");
  // }
  return nullptr;
}

llvm::Value*
cg_function::generate_call_expr(const call_expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_index_expr(const index_expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_assign_expr(const assign_expr* e)
{
  return nullptr;
}

llvm::Value*
cg_function::generate_cond_expr(const cond_expr* e)
{
  return nullptr;
}

// FIXME: Clean this up.
llvm::Value*
cg_function::generate_conv_expr(const conv_expr* c)
{
  return nullptr;
}

void
cg_function::generate_stmt(const stmt* s)
{
  switch (s->get_kind()) {
  case stmt::block_kind:
    return generate_block_stmt(static_cast<const block_stmt*>(s));
  case stmt::when_kind:
    return generate_when_stmt(static_cast<const when_stmt*>(s));
  case stmt::if_kind:
    return generate_if_stmt(static_cast<const if_stmt*>(s));
  case stmt::while_kind:
    return generate_while_stmt(static_cast<const while_stmt*>(s));
  case stmt::break_kind:
    return generate_break_stmt(static_cast<const break_stmt*>(s));
  case stmt::cont_kind:
    return generate_cont_stmt(static_cast<const cont_stmt*>(s));
  case stmt::ret_kind:
    return generate_ret_stmt(static_cast<const ret_stmt*>(s));
  case stmt::decl_kind:
    return generate_decl_stmt(static_cast<const decl_stmt*>(s));
  case stmt::expr_kind:
    return generate_expr_stmt(static_cast<const expr_stmt*>(s));
  }
}

void
cg_function::generate_block_stmt(const block_stmt* s)
{
}

void
cg_function::generate_when_stmt(const when_stmt* s)
{
}

void
cg_function::generate_if_stmt(const if_stmt* s)
{
}

void
cg_function::generate_while_stmt(const while_stmt* s)
{
}

void
cg_function::generate_break_stmt(const break_stmt* e)
{
}

void
cg_function::generate_cont_stmt(const cont_stmt* e)
{
}

void
cg_function::generate_ret_stmt(const ret_stmt* e)
{
}

void
cg_function::generate_decl_stmt(const decl_stmt* e)
{
}

void
cg_function::generate_expr_stmt(const expr_stmt* e)
{
}

void
generate(const decl* d)
{
  assert(d->is_program());
  
  // Establish the translation context.
  cg_context cg;

  // Create the module, and generate its declarations.
  cg_module mod(cg, static_cast<const prog_decl*>(d));
  mod.generate();

  // Dump the generated module to 
  llvm::outs() << *mod.get_module();
}
