#ifndef BUILD_LLVM_IR_CPP
#define BUILD_LLVM_IR_CPP

#include "build_llvm_ir.h"
#include "inner.h"
#include "parser.h"

namespace toy_c {

/**
 * @brief Construct a new build llvm ir::build llvm ir object
 *
 */
build_llvm_ir::build_llvm_ir() {
  tree    = std::move(parse.get_syntax_tree());
  context = std::make_unique<llvm::LLVMContext>();
  module  = std::make_unique<llvm::Module>("text_module", *context);
  builder = std::make_unique<llvm::IRBuilder<>>(*context);

  // enable in llvm 15.0.0
  // context->setOpaquePointers(false);
  build_mult_declaration_or_definition(1);
}

/**
 * @brief Destroy the build llvm ir::build llvm ir object
 *
 */
build_llvm_ir::~build_llvm_ir() {}

/**
 * @brief
 *
 * @param idx_declaration_or_definition
 * @return llvm::Type*
 */
llvm::Type *
build_llvm_ir::build_mult_declaration_or_definition(ast::idx idx_declaration_or_definition
) {
  for (ast::idx i = idx_declaration_or_definition,
                idx_declaration_declarator
                = tree[i].value.declaration_or_definition.idx_declaration_declarator;
       i != ast::null;
       i = tree[i].value.declaration_or_definition.idx_next_declaration_or_definition,
                idx_declaration_declarator
                = tree[i].value.declaration_or_definition.idx_declaration_declarator) {
    now_compound_statement
      = tree[i].value.declaration_or_definition.idx_compound_statement;

    for (ast::idx j = tree[i].value.declaration_or_definition.idx_initial_declarator;
         j != ast::null;
         j = tree[j].value.initial_declarator.idx_next_initial_declarator) {
      build_declaration_or_definition(idx_declaration_declarator, j);
    }
  }
  return nullptr;
}

/**
 * @brief
 *
 * @param idx
 * @return llvm::Type*
 */
llvm::Type *build_llvm_ir::build_declaration_declarator(ast::idx idx) {
  switch (tree[idx].value.declaration_declarator.type) {
    case ast::declarator_type::type_struct:
      return llvm::StructType::get(*context);
    case ast::declarator_type::type_char:
      return llvm::Type::getInt8Ty(*context);
    case ast::declarator_type::type_double:
      return llvm::Type::getDoubleTy(*context);
    case ast::declarator_type::type_float:
      return llvm::Type::getFloatTy(*context);
    case ast::declarator_type::type_short_int:
      return llvm::Type::getInt16Ty(*context);
    case ast::declarator_type::type_int:
    case ast::declarator_type::type_long_int:
      return llvm::Type::getInt32Ty(*context);
    case ast::declarator_type::type_long_long_int:
      return llvm::Type::getInt64Ty(*context);
    // TODO void type
    case ast::declarator_type::type_void:
    default:
      SWITCH_ERROR
  }
}

/**
 * @brief
 *
 * @param idx_argument_type_list
 * @return llvm::SmallVector<llvm::Type *>
 */
llvm::SmallVector<llvm::Type *>
build_llvm_ir::build_arguments_type_list(ast::idx idx_argument_type_list) {
  NODE_TYPE_WRONG(tree[idx_argument_type_list].type, ast::node_type::arguments_type_list)

  // traverse each argument declaration in the arguments type list
  llvm::SmallVector<llvm::Type *> arguments_type_list;
  for (ast::idx i
       = tree[idx_argument_type_list].value.arguments_type_list.idx_argument_declaration;
       i != ast::null;
       i = tree[i].value.arguments_declaration.idx_next_arguments_declaration) {
    arguments_type_list.push_back(build_argument_declaration(i));
  }

  return arguments_type_list;
}

// TODO
// all type declarator
// 1. array's pointer
// 2.
/**
 * @brief
 *
 * @param idx_argument_declaration
 * @return llvm::Type*
 */
llvm::Type *build_llvm_ir::build_argument_declaration(ast::idx idx_argument_declaration) {
  ast::idx idx_declaration_declarator
    = tree[idx_argument_declaration]
        .value.arguments_declaration.idx_declaration_declarator;

  ast::idx idx_declarator
    = tree[idx_argument_declaration].value.arguments_declaration.idx_declarator;

  llvm::Type *ptr_type_argument = build_type(idx_declaration_declarator, idx_declarator);

  return ptr_type_argument;
}

/**
 * @brief
 *
 * @param idx
 * @param ptr_declaration_declarator
 * TODO
 * 1. function pointer
 */
void build_llvm_ir::build_function_or_function_ptr(
  ast::idx    idx,
  llvm::Type *ptr_declaration_declarator
) {
  // llvm::SmallVector<llvm::Type*> func_args = build_arguments_type_list(idx);
  // llvm::FunctionType *ptr_func = llvm::FunctionType::get();
  return;
}

/**
 * @brief
 *
 * @param idx
 * @param ptr_declaration_declarator
 */
void build_llvm_ir::build_variable(ast::idx idx, llvm::Type *ptr_declaration_declarator) {
}

/**
 * @brief
 *
 * @param ptr_unit_type
 * @param idx_array_declarator
 * @return llvm::Type*
 */
llvm::Type *
build_llvm_ir::build_array(llvm::Type *ptr_unit_type, ast::idx idx_array_declarator) {
  ast::idx idx_next_array_declarator
    = tree[idx_array_declarator].value.array_declarator.idx_next_array_declarator;

  ast::idx idx_constant = tree[idx_array_declarator].value.array_declarator.idx_constant;
  if (idx_next_array_declarator != ast::null) {
    return llvm::ArrayType::get(
      build_array(ptr_unit_type, idx_next_array_declarator), build_uint64(idx_constant)
    );
  }
  else {
    return llvm::ArrayType::get(ptr_unit_type, build_uint64(idx_constant));
  }
}

/**
 * @brief
 *
 * @param idx_
 * @return std::uint64_t
 */
std::uint64_t build_llvm_ir::build_uint64(ast::idx idx_) {
  // TODO
  return 5;
}

/**
 * @brief
 *
 * @param ptr_unit_type
 * @param idx_array_declarator
 * @return llvm::Type*
 */
llvm::Type *build_llvm_ir::build_recurs_array(
  llvm::Type *ptr_unit_type,
  ast::idx    idx_array_declarator
) {
  ast::idx idx_next_array_declarator
    = tree[idx_array_declarator].value.array_declarator.idx_next_array_declarator;

  ast::idx idx_constant = tree[idx_array_declarator].value.array_declarator.idx_constant;
  if (idx_next_array_declarator != ast::null) {
    return llvm::ArrayType::get(
      build_recurs_array(ptr_unit_type, idx_next_array_declarator),
      build_uint64(idx_constant)
    );
  }
  else {
    return llvm::ArrayType::get(ptr_unit_type, build_uint64(idx_constant));
  }
}

/**
 * @brief
 *
 * @param idx
 * @return true
 * @return false
 */
bool build_llvm_ir::is_func_decl(ast::idx idx) {
  switch (tree[idx].type) {
    case ast::node_type::declaration_or_definition:
      return is_func_decl(
        tree[idx].value.declaration_or_definition.idx_declaration_declarator
      );
    case ast::node_type::initial_declarator_list:
      return is_func_decl(tree[idx].value.initial_declarator_list.idx_initial_declarator);
    case ast::node_type::initial_declarator:
      return is_func_decl(tree[idx].value.initial_declarator.idx_declarator);
    case ast::node_type::declarator:
      return tree[idx].value.declarator.idx_direct_declarator;
    case ast::node_type::direct_declarator:
      if (tree[idx].value.direct_declarator.idx_declarator != ast::null) {
        return is_func_decl(tree[idx].value.direct_declarator.idx_declarator);
      }
      else {
        if (tree[idx].value.direct_declarator.idx_arguments_type_list != ast::null) {
          return true;
        }
        else {
          return false;
        }
      }
    default:
      SWITCH_ERROR
  }
}

/**
 * @brief
 *
 */
void build_llvm_ir::output_llvm_ir() {
  // terminal output
  // module->dump();
  std::error_code      ec;
  llvm::raw_fd_ostream file(output_file_path, ec);
  module->print(file, nullptr);
  return;
}

/**
 * @brief function is used on initial declarator node cause a declaration include several
 * initial declarator
 *
 * @param idx
 * @return type_of_def_or_dec
 */
type_of_def_or_dec build_llvm_ir::which_type(ast::idx idx) {
  while (true) {
    switch (tree[idx].type) {
      case ast::node_type::initial_declarator:
        idx = tree[idx].value.initial_declarator.idx_declarator;
        break;
      case ast::node_type::declarator:
        idx = tree[idx].value.declarator.idx_direct_declarator;
        break;
      case ast::node_type::direct_declarator:
        if (tree[idx].value.direct_declarator.idx_arguments_type_list != ast::null) {
          return type_of_def_or_dec::is_func_decl;
        }
        else if (tree[idx].value.direct_declarator.idx_array_declarator != ast::null) {
          return type_of_def_or_dec::is_array;
        }
        else {
          return type_of_def_or_dec::is_var;
        }
      default:
        SWITCH_ERROR
    }
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx
 * @return true
 * @return false
 */
bool build_llvm_ir::is_struct(ast::idx idx) {
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx
 * @return true
 * @return false
 */
bool build_llvm_ir::is_union(ast::idx idx){NOT_REACHABLE}

/**
 * @brief
 *
 * @param ptr_type_declaration_declarator
 * @param idx_declarator
 * @return llvm::Type*
 */
llvm::Type *build_llvm_ir::build_pointer(
  llvm::Type *ptr_type_declaration_declarator,
  ast::idx    idx_declarator
) {
  for (auto i = tree[idx_declarator].value.declarator.is_ptr; i > 0; --i) {
    ptr_type_declaration_declarator
      = llvm::PointerType::get(ptr_type_declaration_declarator, 0);
  }
  return ptr_type_declaration_declarator;
}

/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @param idx_initial_declarator
 */
void build_llvm_ir::build_declaration_or_definition(
  ast::idx idx_declaration_declarator,
  ast::idx idx_initial_declarator
) {
  ast::idx idx_declarator
    = tree[idx_initial_declarator].value.initial_declarator.idx_declarator;
  ast::idx idx_direct_declarator
    = tree[idx_declarator].value.declarator.idx_direct_declarator;

  llvm::Type *ptr_type_declaration_declarator
    = build_declaration_declarator(idx_declaration_declarator);

  // return type, var type, arrary unit type
  llvm::Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

  // get function name
  ast::idx idx_identifier;
  for (ast::idx i = idx_declarator; tree[i].type != ast::node_type::identifier;) {
    switch (tree[i].type) {
      case ast::node_type::declarator:
        i = tree[i].value.declarator.idx_direct_declarator;
        break;
      case ast::node_type::direct_declarator:
        if (tree[i].value.direct_declarator.idx_identifier != ast::null) {
          i              = tree[i].value.direct_declarator.idx_identifier;
          idx_identifier = i;
        }
        else {
          i = tree[i].value.direct_declarator.idx_declarator;
        }
        break;
      default:
        fmt::print(
          fg(fmt::color::red), "node type {}\n", static_cast<unsigned char>(tree[i].type)
        );
        SWITCH_ERROR
    }
  }
  std::string name{tree[idx_identifier].value.identifier.name};

  // array or a pointer to array
  if (tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null) {
    llvm::Type *ptr_unit_type = ptr_type;
    ast::idx    idx_array_declarator
      = tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
    llvm::Type *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

    // a pointer to array
    while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null
    ) {
      idx_declarator = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
      build_pointer(ptr_array_type, idx_declarator);
      idx_direct_declarator = tree[idx_declarator].value.declarator.idx_direct_declarator;
    }
    builder->CreateAlloca(ptr_array_type);
    return;
  }
  // TODO
  // function
  else if (tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null) {
    llvm::Type *ptr_return_type = ptr_type;
    // get function type list
    llvm::SmallVector<llvm::Type *> argument_type_list = build_arguments_type_list(
      tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
    );

    llvm::FunctionType *ptr_func_type
      = llvm::FunctionType::get(ptr_return_type, argument_type_list, false);

    // 1. a pointer to function
    if (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null) {
      while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator
             != ast::null) {
        idx_declarator
          = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
        build_pointer(ptr_func_type, idx_declarator);
        idx_direct_declarator
          = tree[idx_declarator].value.declarator.idx_direct_declarator;
      }
    }
    // 2. a function
    else {
      // 3. a function declaration
      llvm::Function *ptr_func = llvm::Function::Create(
        ptr_func_type, llvm::Function::ExternalLinkage, name, *module
      );

      // 4. a function definition
      if (now_compound_statement != ast::null) {
        llvm::BasicBlock *ptr_block
          = llvm::BasicBlock::Create(*context, get_label(), ptr_func);
        builder->SetInsertPoint(ptr_block);
        ptr_now_func  = ptr_func;
        ptr_now_block = ptr_block;

        build_compound_statement(now_compound_statement, ptr_block);
      }
    }
  }
  // struct enum
  else if (now_compound_statement != ast::null) {
    TODO
  }
  // var
  else {
    llvm::Value *ptr_var = builder->CreateAlloca(ptr_type);
    // initializer
    if (tree[idx_initial_declarator].value.initial_declarator.idx_initializer != ast::null) {
      ast::idx idx_initializer
        = tree[idx_initial_declarator].value.initial_declarator.idx_initializer;
      ast::idx idx_assignment_expression
        = tree[idx_initializer].value.initializer.idx_assignment_expression;

      // int a, b = c = 1;
      //            |
      // if initializer is a assignment expression, the expression after first = operator
      // must be a left value
      //
      // binary expression
      if (tree[idx_assignment_expression].type == ast::node_type::binary_expression) {
        recursion_analysis_and_assign_type_to_node(
          idx_assignment_expression, ptr_type, nullptr
        );
        llvm::Value *initializer_value
          = build_binary_expression(idx_assignment_expression);
        builder->CreateStore(initializer_value, ptr_var);
      }
      // unary expression
      else if (tree[idx_assignment_expression].type == ast::node_type::unary_expression) {
        recursion_analysis_and_assign_type_to_node(
          idx_assignment_expression, ptr_type, nullptr
        );
        llvm::Value *initializer_value
          = build_unary_expression(idx_assignment_expression);
        builder->CreateStore(initializer_value, ptr_var);
      }
      // assignment expression
      else if (tree[idx_assignment_expression].type == ast::node_type::assignment_expression) {
        llvm::Value *initializer_value
          = build_assign_expression(idx_assignment_expression);
        builder->CreateStore(initializer_value, ptr_var);
      }
      else {
        NOT_REACHABLE
      }
    }
    insert_func_symbol(name, ptr_var, ptr_type);
  }
  return;
}

/**
 * @brief
 *
 * @param idx_declaration_declarator
 * @param idx_declarator
 * @return llvm::Type*
 */
llvm::Type *
build_llvm_ir::build_type(ast::idx idx_declaration_declarator, ast::idx idx_declarator) {
  llvm::Type *ptr_type_declaration_declarator
    = build_declaration_declarator(idx_declaration_declarator);

  // return type, var type, arrary unit type
  llvm::Type *ptr_type = build_pointer(ptr_type_declaration_declarator, idx_declarator);

  ast::idx idx_direct_declarator
    = tree[idx_declarator].value.declarator.idx_direct_declarator;

  // array or a pointer to array
  if (tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator != ast::null) {
    llvm::Type *ptr_unit_type = ptr_type;
    ast::idx    idx_array_declarator
      = tree[idx_direct_declarator].value.direct_declarator.idx_array_declarator;
    llvm::Type *ptr_array_type = build_array(ptr_unit_type, idx_array_declarator);

    // a pointer to array
    ast::idx idx_declarator;
    while (tree[idx_direct_declarator].value.direct_declarator.idx_declarator != ast::null
    ) {
      idx_declarator = tree[idx_direct_declarator].value.direct_declarator.idx_declarator;
      build_pointer(ptr_array_type, idx_declarator);
      idx_direct_declarator = tree[idx_declarator].value.declarator.idx_direct_declarator;
    }

    return ptr_array_type;
  }
  // TODO
  // function
  else if (tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list != ast::null) {
    llvm::Type *ptr_return_type = ptr_type;
    // get function type list
    llvm::SmallVector<llvm::Type *> argument_type_list = build_arguments_type_list(
      tree[idx_direct_declarator].value.direct_declarator.idx_arguments_type_list
    );

    llvm::FunctionType *ptr_func_type
      = llvm::FunctionType::get(ptr_return_type, argument_type_list, false);
    return ptr_func_type;
  }
  // var
  else {
    return ptr_type;
  }

  NOT_REACHABLE
}


/**
 * @brief
 *
 * @param idx_compound_statement
 * @param ptr_block
 * @return llvm::BasicBlock*
 */
llvm::BasicBlock *build_llvm_ir::build_compound_statement(
  ast::idx          idx_compound_statement,
  llvm::BasicBlock *ptr_block
) {
  ast::idx idx_block = tree[idx_compound_statement].value.compound_statement.idx_block;

  for (ast::idx i = idx_block; i != ast::null; i = tree[i].value.block.idx_next_block) {
    build_block(i);
  }
  // TODO
  return nullptr;
}

/**
 * @brief
 *
 * @param idx_block
 */
void build_llvm_ir::build_block(ast::idx idx_block) {
  ast::idx idx_declaration = tree[idx_block].value.block.idx_declaration;
  ast::idx idx_statement   = tree[idx_block].value.block.idx_statement;
  if (idx_declaration != ast::null) {
    build_mult_declaration_or_definition(idx_declaration);
  }
  else {
    switch (tree[idx_statement].type) {
      // TODO all type
      case ast::node_type::if_statement:
        build_if_statement(idx_statement);
        break;
      case ast::node_type::switch_statement:
        build_switch_statement(idx_statement);
        break;
      case ast::node_type::return_statement:
        build_return_statement(idx_statement);
        break;
      case ast::node_type::while_statement:
        build_while_statement(idx_statement);
        break;
      case ast::node_type::do_while_statement:
        build_do_while_statement(idx_statement);
        break;
      case ast::node_type::for_statement:
        build_for_statement(idx_statement);
        break;
      case ast::node_type::expression:
        build_expression(idx_statement);
        break;
      default:
        SWITCH_ERROR
    }
  }
  return;
}

/**
 * @brief
 *
 * @param idx_if_statement
 */
void build_llvm_ir::build_if_statement(ast::idx idx_if_statement) {
  // builder->SetInsertPoint(ptr_block);
  ast::idx idx_assign_expression
    = tree[idx_if_statement].value.if_statement.idx_assign_expression;
  ast::idx idx_else_body = tree[idx_if_statement].value.if_statement.idx_else_body;
  ast::idx idx_if_body   = tree[idx_if_statement].value.if_statement.idx_if_body;

  llvm::BasicBlock *ptr_block_if_body
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

  if (idx_else_body != ast::null) {
    llvm::BasicBlock *ptr_block_else_body
      = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
    llvm::BasicBlock *ptr_statement_end
      = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

    build_assign_expression(
      idx_assign_expression, nullptr, ptr_block_if_body, ptr_block_else_body, false
    );
    // if body
    builder->SetInsertPoint(ptr_block_if_body);
    build_compound_statement(idx_if_body);
    builder->CreateBr(ptr_statement_end);
    // else body
    builder->SetInsertPoint(ptr_block_else_body);
    build_compound_statement(idx_else_body);
    builder->CreateBr(ptr_statement_end);
    // end of if statement
    builder->SetInsertPoint(ptr_statement_end);
  }
  else {
    llvm::BasicBlock *ptr_block_statement_end
      = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
    build_assign_expression(
      idx_assign_expression, nullptr, ptr_block_if_body, ptr_block_statement_end, false
    );
    // builder->CreateBr(ptr_block_if_body);

    // if body
    builder->SetInsertPoint(ptr_block_if_body);
    build_compound_statement(idx_if_body);
    builder->CreateBr(ptr_block_statement_end);
    // end of if statement
    builder->SetInsertPoint(ptr_block_statement_end);
  }
}

/**
 * @brief
 *
 * @param idx_while_statement
 */
void build_llvm_ir::build_while_statement(ast::idx idx_while_statement) {
  ast::idx idx_assign_expression
    = tree[idx_while_statement].value.while_statement.idx_assignment_expression;
  ast::idx idx_compound_statement
    = tree[idx_while_statement].value.while_statement.idx_compound_statement;

  llvm::BasicBlock *ptr_while_condition
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
  llvm::BasicBlock *ptr_while_body
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
  llvm::BasicBlock *ptr_while_body_end
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

  // jump to while condition
  builder->CreateBr(ptr_while_condition);

  // while condition
  builder->SetInsertPoint(ptr_while_condition);

  build_assign_expression(
    idx_assign_expression, nullptr, ptr_while_body, ptr_while_body_end, false
  );

  // while body
  builder->SetInsertPoint(ptr_while_body);
  build_compound_statement(idx_compound_statement);
  builder->CreateBr(ptr_while_condition);

  // while body end
  builder->SetInsertPoint(ptr_while_body_end);
}

/**
 * @brief
 *
 * @param idx_switch_statement
 */
void build_llvm_ir::build_switch_statement(ast::idx idx_switch_statement) {
  TODO
}

/**
 * @brief
 *
 * @param idx_do_while_statement
 */
void build_llvm_ir::build_do_while_statement(ast::idx idx_do_while_statement) {
  ast::idx idx_assign_expression
    = tree[idx_do_while_statement].value.do_while_statement.idx_assign_statement;
  ast::idx idx_compound_statement
    = tree[idx_do_while_statement].value.do_while_statement.idx_compound_statement;

  llvm::BasicBlock *ptr_do_while_body
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
  llvm::BasicBlock *ptr_do_while_body_end
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
  builder->CreateBr(ptr_do_while_body);

  // do_while condition block
  builder->SetInsertPoint(ptr_do_while_body);
  build_compound_statement(idx_compound_statement);

  // do_while body block
  build_assign_expression(
    idx_assign_expression, nullptr, ptr_do_while_body, ptr_do_while_body_end, false
  );

  // do_while body end
  builder->SetInsertPoint(ptr_do_while_body_end);
}

/**
 * @brief
 *
 * @param idx_for_statement
 */
void build_llvm_ir::build_for_statement(ast::idx idx_for_statement) {
  /*
   *
   *  ; declare or assign expression
   *  1:
   *    int i = ...;
   *    br label %2
   *
   *  ; conditional expression
   *  2:
   *
   *    br ... label %3 label %4
   *
   *  ; compound statement
   *  3:
   *    ...
   *    ; change value
   *    ...
   *    br label %2
   *
   *  ; for statement end
   *  4:
   *    ...
   */

  ast::idx idx_change_assign_expression
    = tree[idx_for_statement].value.for_statement.idx_change_assign_expression;

  ast::idx idx_compound_statement
    = tree[idx_for_statement].value.for_statement.idx_compound_statement;

  ast::idx idx_conditional_assign_expression
    = tree[idx_for_statement].value.for_statement.idx_conditional_assign_expression;

  ast::idx idx_declaration = tree[idx_for_statement].value.for_statement.idx_declaration;

  llvm::BasicBlock *ptr_for_conditional_assign_expression
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

  llvm::BasicBlock *ptr_for_compound_statement
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

  llvm::BasicBlock *ptr_for_end
    = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

  // declarations
  build_mult_declaration_or_definition(idx_declaration);
  builder->CreateBr(ptr_for_conditional_assign_expression);

  builder->SetInsertPoint(ptr_for_conditional_assign_expression);
  // condition
  build_assign_expression(
    idx_conditional_assign_expression, nullptr, ptr_for_compound_statement, ptr_for_end,
    false
  );

  builder->SetInsertPoint(ptr_for_compound_statement);
  // compound statement
  build_compound_statement(idx_compound_statement);
  // change value
  build_assign_expression(idx_change_assign_expression);
  // jump to conditional expression
  builder->CreateBr(ptr_for_conditional_assign_expression);

  // for end
  builder->SetInsertPoint(ptr_for_end);
}

/**
 * @brief
 *
 * @param idx_return_statement
 */
void build_llvm_ir::build_return_statement(ast::idx idx_return_statement) {
  ast::idx idx_assign_expression
    = tree[idx_return_statement].value.return_statement.idx_assignment_expression;
  builder->CreateRet(
    build_assign_expression(idx_assign_expression, ptr_now_func->getReturnType())
  );
}

/**
 * @brief
 *
 * @param idx_function_declaration
 */
void build_llvm_ir::build_function(ast::idx idx_function_declaration) {}

/**
 * @brief
 *
 * @param idx_expression
 */
void build_llvm_ir::build_expression(ast::idx idx_expression) {
  for (ast::idx i = idx_expression; i != ast::null;
       i          = tree[i].value.expression.idx_next_expression) {
    build_assign_expression(tree[i].value.expression.idx_assignment_expression);
  }
  return;
}

/**
 * @brief
 *
 * @param idx_assign_expression
 * @param limit_type      = nullptr
 * @param ptr_true_block  = nullptr
 * @param ptr_false_block = nullptr
 * @param is_return_value = false
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_assign_expression(
  ast::idx          idx_assign_expression,
  llvm::Type       *limit_type,
  llvm::BasicBlock *ptr_true_block,
  llvm::BasicBlock *ptr_false_block,
  bool              is_return_value
) {
  // preceding analysis node's type
  analysis_and_assign_type_to_node(idx_assign_expression, limit_type);

  // is an binary expression
  if (tree[idx_assign_expression].type == ast::node_type::binary_expression) {
    return build_binary_expression(
      idx_assign_expression, ptr_true_block, ptr_false_block, is_return_value
    );
  }
  // is an unary expression
  if (tree[idx_assign_expression].type == ast::node_type::unary_expression) {
    if (!is_return_value) {
      llvm::Value *unary_value = build_unary_expression(idx_assign_expression);
      builder->CreateCondBr(
        builder->CreateICmpNE(
          unary_value, llvm::ConstantInt::get(unary_value->getType(), 0)
        ),
        ptr_true_block, ptr_false_block
      );
    }
    else {
      return build_unary_expression(idx_assign_expression);
    }
  }
  // is an assign expression
  else {
    ast::idx idx_binary_expression
      = tree[idx_assign_expression].value.assignment_expression.idx_binary_expression;

    // just a binary expression without assign
    // if
    llvm::Value *r_value = build_binary_expression(idx_binary_expression);
    llvm::Value *l_value;

    for (ast::idx i = idx_assign_expression; i != ast::null;
         i = tree[i].value.assignment_expression.idx_next_assignment_expression) {
      l_value = build_unary_expression(
        tree[i].value.assignment_expression.idx_unary_or_binary_expression
      );
      switch (tree[i].value.assignment_expression.assignment_type) {
        case token::assign:  //=
          builder->CreateStore(r_value, l_value);
          break;

        case token::plus_agn:  //+=
          builder->CreateStore(
            builder->CreateAdd(r_value, builder->CreateLoad(l_value->getType(), l_value)),
            l_value
          );
          break;

        case token::minus_agn:  //-=
          builder->CreateStore(
            builder->CreateSub(r_value, builder->CreateLoad(l_value->getType(), l_value)),
            l_value
          );
          break;

        case token::times_agn:  //*=
          builder->CreateStore(
            builder->CreateMul(r_value, builder->CreateLoad(l_value->getType(), l_value)),
            l_value
          );
          break;

        case token::div_agn:  // /=
          builder->CreateStore(
            builder->CreateSDiv(
              r_value, builder->CreateLoad(l_value->getType(), l_value)
            ),
            l_value
          );
          break;

        // TODO
        case token::mod_agn:  //%=
          break;

        case token::r_shift_agn:  //>>=
          builder->CreateStore(
            builder->CreateSDiv(
              r_value, builder->CreateLoad(l_value->getType(), l_value)
            ),
            l_value
          );
          break;

        case token::l_shift_agn:  //<<=
          builder->CreateStore(
            builder->CreateSDiv(
              r_value, builder->CreateLoad(l_value->getType(), l_value)
            ),
            l_value
          );
          break;

        case token::bit_and_agn:  //&=
          builder->CreateStore(
            builder->CreateSDiv(
              r_value, builder->CreateLoad(l_value->getType(), l_value)
            ),
            l_value
          );
          break;

        case token::bit_or_agn:  //|=
          builder->CreateStore(
            builder->CreateSDiv(
              r_value, builder->CreateLoad(l_value->getType(), l_value)
            ),
            l_value
          );
          break;

        case token::bit_xor_agn:  //^=
          builder->CreateStore(
            builder->CreateSDiv(
              r_value, builder->CreateLoad(l_value->getType(), l_value)
            ),
            l_value
          );
          break;

        default:
          SWITCH_ERROR
      }
      // TODO
      //
      if (tree[i].value.assignment_expression.idx_next_assignment_expression != ast::null) {
        r_value = builder->CreateLoad(l_value->getType(), l_value);
      }
    }
    // jump
    if (!is_return_value) {
      auto l_val = builder->CreateLoad(l_value->getType(), l_value);
      builder->CreateCondBr(
        builder->CreateICmpNE(l_val, llvm::ConstantInt::get(l_val->getType(), 0)),
        ptr_true_block, ptr_false_block
      );
    }
    return r_value;
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_unary_expression
 * @return llvm::Value*
 */
llvm::Value *
build_llvm_ir::build_compute_unary_expression_size(ast::idx idx_unary_expression){TODO}

/**
 * @brief
 *
 * @param idx_declarator
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_compute_declarator_size(ast::idx idx_declarator){TODO}


/**
 * @brief TODO
 *
 * @param idx_unary_expression
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_unary_expression(ast::idx idx_unary_expression) {
  //----------------------------------------------------------------
  // 1. sizeof
  //----------------------------------------------------------------
  if (tree[idx_unary_expression].value.unary_expression.is_sizeof) {
    // unary_expression
    if (tree[idx_unary_expression].type == ast::node_type::unary_expression) {
      return build_compute_unary_expression_size(
        tree[idx_unary_expression].value.unary_expression.idx_unary_expression
      );
    }
    // declarator
    else {
      return build_compute_declarator_size(
        tree[idx_unary_expression].value.unary_expression.idx_declaration_declarator
      );
    }
  }

  ast::idx idx_postfix_expression
    = tree[idx_unary_expression].value.unary_expression.idx_postfix_expression;

  ast::idx idx_primary_expression
    = tree[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

  //----------------------------------------------------------------
  // 2. unary operator
  //----------------------------------------------------------------
  if (tree[idx_unary_expression].value.unary_expression.unary_operator != token::invalid) {
    return build_unary_expression(idx_unary_expression);
  }
  //----------------------------------------------------------------
  // 3. postfix expression
  //----------------------------------------------------------------
  if (idx_postfix_expression != ast::null) {
    return build_postfix_expression(idx_postfix_expression);
  }

  //----------------------------------------------------------------
  // 4. declaration declarator
  // conversion
  //----------------------------------------------------------------
  if (tree[idx_unary_expression].value.unary_expression.idx_declaration_declarator) {
    // TODO
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_unary_expression
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_unary_operator(ast::idx idx_unary_expression) {
  llvm::Value *idx_unary_expression_value = build_unary_expression(
    tree[idx_unary_expression].value.unary_expression.idx_unary_expression
  );

  // TODO
  switch (tree[idx_unary_expression].value.unary_expression.unary_operator) {
    case token::invalid:
      NOT_REACHABLE
      break;
    // &
    case token::bit_and:
      NOT_REACHABLE
      break;
    // *
    case token::times:
      NOT_REACHABLE
      break;
    // ++
    case token::self_plus:
      NOT_REACHABLE
      break;
    // --
    case token::self_minus:
      NOT_REACHABLE
      break;
    // ~
    case token::bit_not:
      NOT_REACHABLE
      break;
    // !
    case token::log_not:
      NOT_REACHABLE
      break;
    default:
      SWITCH_ERROR
  }
}

/**
 * @brief
 *
 * @param name
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::find_value(std::string name) {
  if (func_symbol_table.find(name) != func_symbol_table.end()) {
    return std::get<0>(func_symbol_table.at(name));
  }
  if (global_symbol_table.find(name) != global_symbol_table.end()) {
    return std::get<0>(global_symbol_table.at(name));
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param name
 * @return llvm::Type*
 */
llvm::Type *build_llvm_ir::find_type(std::string name) {
  if (func_symbol_table.find(name) != func_symbol_table.end()) {
    return std::get<1>(func_symbol_table.at(name));
  }
  if (global_symbol_table.find(name) != global_symbol_table.end()) {
    return std::get<1>(global_symbol_table.at(name));
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_postfix_expression
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_postfix_expression(ast::idx idx_postfix_expression) {
  ast::idx idx_primary_expression
    = tree[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

  //--------------------------------
  // primary expression
  //--------------------------------

  //------------
  // identifier
  //------------
  if (tree[idx_primary_expression].value.primary_expression.idx_identifier != ast::null) {
    // TODO
    ast::idx idx_identifier
      = tree[idx_primary_expression].value.primary_expression.idx_identifier;
    llvm::Value *value_identifier
      = find_value(tree[idx_identifier].value.identifier.name);

    // CASE exist postfix operator
    if (tree[idx_postfix_expression].value.postfix_expression.idx_postfix_operator != ast::null) {
      TODO ast::idx idx_postfix_operator
        = tree[idx_postfix_expression].value.postfix_expression.idx_postfix_operator;
      while (idx_postfix_operator != ast::null) {
        switch (tree[idx_postfix_operator].value.postfix_operator.postfix_operator) {
          // TODO
          // ++
          case token::self_plus:
            break;
          // --
          case token::self_minus:
            break;
          // ->
          case token::ver:
            break;
          // .
          case token::period:
            break;
          // array [] or function call ()
          case token::invalid:
            break;
          default:
            SWITCH_ERROR
        }
        idx_postfix_operator
          = tree[idx_postfix_operator].value.postfix_operator.idx_next_postfix_operator;
      }
    }
    // CASE not exist postfix operator
    else {
      return builder->CreateLoad(value_identifier->getType(), value_identifier);
    }
  }
  //------------
  // constant
  //------------
  else if (tree[idx_primary_expression].value.primary_expression.idx_constant != ast::null) {
    return build_constant(
      tree[idx_primary_expression].value.primary_expression.idx_constant
    );
  }
  //------------
  // expression
  //------------
  else {
    return build_binary_expression(idx_postfix_expression);
  }
}

/**
 * @brief
 *
 * @param idx_constant
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_constant(ast::idx idx_constant) {
  llvm::Type *type = tree.get_constant(idx_constant).llvm_type;

  switch (tree.get_constant(idx_constant).type) {
    case ast::node_type::constant_string:
      // TODO
      // string
      return llvm::ConstantDataArray::getString(
        *context, tree.get_constant(idx_constant).value
      );

    // TODO
    // signed and unsigned
    case ast::node_type::constant_negative_float_number:
      return llvm::ConstantFP::get(
        type, std::stoull(tree.get_constant(idx_constant).value)
      );

    case ast::node_type::constant_float_number:
      return llvm::ConstantFP::get(
        type, std::stoull(tree.get_constant(idx_constant).value)
      );

    case ast::node_type::constant_integer_number:
      return llvm::ConstantInt::get(
        type, std::stoull(tree.get_constant(idx_constant).value), true
      );

    case ast::node_type::constant_negative_integer_number:
      return llvm::ConstantInt::get(
        type, std::stoull(tree.get_constant(idx_constant).value), true
      );

    default:
      SWITCH_ERROR
  }
}

/**
 * @brief
 *
 * @param idx_primary_expression
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_primary_expression(ast::idx idx_primary_expression
){NOT_REACHABLE}

/**
 * @brief
 *
 * @param idx_binary_expression
 * @param ptr_true_block
 * @param ptr_false_block
 * @param is_return_value
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_binary_expression(
  ast::idx          idx_binary_expression,
  llvm::BasicBlock *ptr_true_block,
  llvm::BasicBlock *ptr_false_block,
  bool              is_return_value
) {
  if (tree[idx_binary_expression].type == ast::node_type::unary_expression) {
    return build_unary_expression(idx_binary_expression);
    // return builder->CreateLoad(value->getType(), value);
  }

  // llvm::Value *value_l
  //=
  // build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_left_node);
  // llvm::Value *value_r
  //=
  // build_binary_expression(tree[idx_binary_expression].value.binary_expression.idx_right_node);
  llvm::Value *value_l      = nullptr;
  llvm::Value *value_r      = nullptr;
  llvm::Value *return_value = nullptr;

  switch (tree[idx_binary_expression].value.binary_expression.token_operator) {
    // TODO
    // float num
    case token::log_and: {
      auto i = idx_binary_expression;
      for (; tree[i].type == ast::node_type::binary_expression
             && tree[i].value.binary_expression.token_operator == token::log_and;
           i = tree[i].value.binary_expression.idx_right_node) {
        ast::idx idx_left_node  = tree[i].value.binary_expression.idx_left_node;
        ast::idx idx_right_node = tree[i].value.binary_expression.idx_right_node;

        if (tree[idx_left_node].type == ast::node_type::binary_expression
            && (tree[idx_left_node].value.binary_expression.token_operator
                  == token::log_or
                || tree[idx_left_node].value.binary_expression.token_operator
                     == token::log_and)) {
          auto ptr_next_true_block
            = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

          build_binary_expression(idx_left_node, ptr_next_true_block, ptr_false_block);

          builder->SetInsertPoint(ptr_next_true_block);
        }
        else {
          // left node
          value_l
            = build_binary_expression(tree[i].value.binary_expression.idx_left_node);
          // judge left node
          auto ret_val = builder->CreateICmpNE(
            value_l, llvm::ConstantInt::get(value_l->getType(), 0)
          );
          // create next true block
          auto ptr_next_true_block
            = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
          // conditional jump
          builder->CreateCondBr(ret_val, ptr_next_true_block, ptr_false_block);
          // start true block
          builder->SetInsertPoint(ptr_next_true_block);
        }
      }

      // last binary expression's right node
      if (tree[i].type == ast::node_type::binary_expression
          && (tree[i].value.binary_expression.token_operator == token::log_or
              || tree[i].value.binary_expression.token_operator
                   == token::log_and)) {
        build_binary_expression(i, ptr_true_block, ptr_false_block);
        return nullptr;
      }
      else {
        llvm::Value *last_right_value = build_binary_expression(i);
        llvm::Value *ret_val          = builder->CreateICmpNE(
                   last_right_value, llvm::ConstantInt::get(last_right_value->getType(), 0)
                 );
        // jump
        builder->CreateCondBr(ret_val, ptr_true_block, ptr_false_block);
        builder->SetInsertPoint(ptr_true_block);

        return ret_val;
      }

      //****************************************************************
      // DROP
      //****************************************************************
      // left value
      /*
      value_l = build_binary_expression(
          tree[idx_binary_expression].value.binary_expression.idx_left_node,
          ptr_true_block,
          ptr_false_block
      );

      ptr_true_block = llvm::BasicBlock::Create(*context, get_label(),
      ptr_now_func);

      ret_val = builder->CreateICmpNE(
          value_l,
          llvm::ConstantInt::get(value_l->getType(), 0)
      );

      builder->CreateCondBr(
          ret_val,
          ptr_true_block,
          ptr_false_block
      );
      builder->SetInsertPoint(ptr_true_block);

      // right value
      value_r = build_binary_expression(
          tree[idx_binary_expression].value.binary_expression.idx_right_node,
          ptr_true_block,
          ptr_false_block
      );

      ptr_true_block
      = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

      ret_val = builder->CreateICmpNE(
          value_r, llvm::ConstantInt::get(value_r->getType(), 0)
      );

      builder->CreateCondBr(ret_val, ptr_true_block, ptr_false_block);
      builder->SetInsertPoint(ptr_true_block);

      return ret_val;
       */
    }

    case token::log_or: {
      auto i = idx_binary_expression;
      for (; tree[i].type == ast::node_type::binary_expression
             && tree[i].value.binary_expression.token_operator == token::log_or;
           i = tree[i].value.binary_expression.idx_right_node) {
        // left node can be a binary expression or a unary expression
        // if expression tree's root node's operator is || or &&
        // node don't need to compute the binary expression's return value
        // only jump to the T/F blocks
        ast::idx idx_left_node  = tree[i].value.binary_expression.idx_left_node;
        ast::idx idx_right_node = tree[i].value.binary_expression.idx_right_node;
        if (tree[idx_left_node].type == ast::node_type::binary_expression
            && (tree[idx_left_node].value.binary_expression.token_operator
                  == token::log_or
                || tree[idx_left_node].value.binary_expression.token_operator
                     == token::log_and)) {
          // create next false block
          auto ptr_next_false_block
            = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

          build_binary_expression(idx_left_node, ptr_true_block, ptr_next_false_block);

          // start false block
          builder->SetInsertPoint(ptr_next_false_block);
        }
        else {
          value_l = build_binary_expression(idx_left_node);
          // judge left node
          auto ret_val = builder->CreateICmpNE(
            value_l, llvm::ConstantInt::get(value_l->getType(), 0)
          );
          // create next false block
          auto ptr_next_false_block
            = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);
          // conditional jump
          builder->CreateCondBr(ret_val, ptr_true_block, ptr_next_false_block);
          // start false block
          builder->SetInsertPoint(ptr_next_false_block);
        }
      }

      // last binary expression's right node
      if (tree[i].type == ast::node_type::binary_expression
          && (tree[i].value.binary_expression.token_operator == token::log_or
              || tree[i].value.binary_expression.token_operator
                   == token::log_and)) {
        build_binary_expression(i, ptr_true_block, ptr_false_block);
        builder->SetInsertPoint(ptr_true_block);
        return nullptr;
      }
      else {
        llvm::Value *last_right_value = build_binary_expression(i);
        llvm::Value *ret_val          = builder->CreateICmpNE(
                   last_right_value, llvm::ConstantInt::get(last_right_value->getType(), 0)
                 );
        // jump
        builder->CreateCondBr(ret_val, ptr_true_block, ptr_false_block);
        builder->SetInsertPoint(ptr_true_block);

        return ret_val;
      }

      //****************************************************************
      // DROP
      //****************************************************************
      /*
      //left value
      value_l = build_binary_expression(
          tree[idx_binary_expression].value.binary_expression.idx_left_node,
          ptr_true_block,
          ptr_false_block
      );

      ptr_false_block = llvm::BasicBlock::Create(*context, get_label(),
      ptr_now_func);

      ret_val = builder->CreateICmpNE(
          value_l,
          llvm::ConstantInt::get(value_l->getType(), 0)
      );

      builder->CreateCondBr(
          ret_val,
          ptr_true_block,
          ptr_false_block
      );

      builder->SetInsertPoint(ptr_false_block);

      // right value
      value_r = build_binary_expression(
          tree[idx_binary_expression].value.binary_expression.idx_right_node,
          ptr_true_block,
          ptr_false_block
      );

      ptr_false_block
      = llvm::BasicBlock::Create(*context, get_label(), ptr_now_func);

      ret_val = builder->CreateICmpNE(
          value_r, llvm::ConstantInt::get(value_r->getType(), 0)
      );

      builder->CreateCondBr(ret_val, ptr_true_block, ptr_false_block);
      builder->SetInsertPoint(ptr_false_block);

      return ret_val;
      */
    }

    // TODO
    case token::bit_and:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;

    // TODO
    case token::bit_or:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;

    // TODO
    case token::bit_xor:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;

    case token::equ:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      return_value = builder->CreateICmpEQ(value_l, value_r);
      break;

    case token::not_equ: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      return_value = builder->CreateICmpNE(value_l, value_r);
      break;
    }

    case token::great: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      return_value = builder->CreateICmpSGT(value_l, value_r);
      break;
    }

    case token::great_equ: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      return_value = builder->CreateICmpSGE(value_l, value_r);
      break;
    }

    case token::less: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      value_l->dump();
      value_r->dump();
      return_value = builder->CreateICmpSLT(value_l, value_r);
      break;
    }

    case token::less_equ: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      return_value = builder->CreateICmpSLE(value_l, value_r);
      break;
    }

    case token::l_shift: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;
    }

    // TODO
    case token::r_shift: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;
    }

    // TODO
    case token::plus: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      return_value = builder->CreateAdd(value_l, value_r);
      break;
    }

    case token::times: {
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      return_value = builder->CreateFSub(value_l, value_r);
      break;
    }

    case token::minus:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;

    case token::div:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;

    case token::mod:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;

    default:
      SWITCH_ERROR
  }

  if (!is_return_value) {
    auto result = builder->CreateICmpNE(
      return_value, llvm::ConstantInt::get(return_value->getType(), 0)
    );
    builder->CreateCondBr(result, ptr_true_block, ptr_false_block);
  }
  return return_value;
}

/**
 * @brief
 *
 * @param idx_log_and_operator
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_log_and_chain(ast::idx idx_log_and_operator) {
  if (tree[idx_log_and_operator].value.binary_expression.token_operator != token::log_and) {
    return build_binary_expression(idx_log_and_operator);
  }
  ast::idx l_value = tree[idx_log_and_operator].value.binary_expression.idx_left_node;
  ast::idx r_value = tree[idx_log_and_operator].value.binary_expression.idx_right_node;

  build_log_and_chain(l_value);
  build_log_and_chain(r_value);
  return nullptr;
}


/**
 * @brief
 *
 * @param idx_log_or_operator
 * @return llvm::Value*
 */
llvm::Value *build_llvm_ir::build_log_or_chain(ast::idx idx_log_or_operator) {
  return nullptr;
}

/**
 * @brief
 *
 * @param name
 * @param ptr_var
 */
void build_llvm_ir::insert_func_symbol(
  std::string  name,
  llvm::Value *ptr_var,
  llvm::Type  *ptr_type
) {
  if (func_symbol_table.find(name) != func_symbol_table.end()
      || global_symbol_table.find(name) != global_symbol_table.end()) {
    fmt::print("redeclaration of {}", name);
    exit(0);
  }
  // func_symbol_table.insert({name, std::tuple(ptr_var, ptr_type)});
  func_symbol_table.insert({
    name, {ptr_var, ptr_type}
  });
}

/**
 * @brief
 *
 * @param name
 * @param ptr_var
 */
void build_llvm_ir::insert_global_symbol(
  std::string  name,
  llvm::Value *ptr_var,
  llvm::Type  *ptr_type
) {
  if (func_symbol_table.find(name) != func_symbol_table.end()
      || global_symbol_table.find(name) != global_symbol_table.end()) {
    fmt::print("redeclaration of {}", name);
    exit(0);
  }
  global_symbol_table.insert({
    name, {ptr_var, ptr_type}
  });
}

/**
 * @brief
 *
 * @return std::string
 */
std::string build_llvm_ir::get_label() {
  return std::to_string(++label_num);
}


/**
 * @brief
 *
 * @param idx_assign_expression
 * @param limit_type = nullptr
 * @return llvm::Type*
 */
llvm::Type *build_llvm_ir::analysis_and_assign_type_to_node(
  ast::idx    idx_assign_expression,
  llvm::Type *limit_type
) {
  // is an unary or binary expression
  if (tree[idx_assign_expression].type == ast::node_type::binary_expression || tree[idx_assign_expression].type == ast::node_type::unary_expression) {
    return recursion_analysis_and_assign_type_to_node(
      idx_assign_expression, nullptr, limit_type
    );
  }

  ast::idx    idx_left_of_assign_operator   = ast::null;
  ast::idx    idx_right_of_assign_operator  = ast::null;
  llvm::Type *type_left_of_assign_operator  = nullptr;
  llvm::Type *type_right_of_assign_operator = nullptr;

  //
  // assign expression
  // a -= b += c = 4;
  //              ^
  //              | just last location expression's type is unknown,
  //              | the expression in front of it must be l_value which with type has
  //              been \ | define.
  // so in this aspect just recurse and analysis the last node which idx is the first \
  // in chain of assignment node, but every expression node must with a type make behind
  // \ procedure easier, so every node's number unary_expression, and the first node's
  // binary \ _expression will be assign in recurse
  //
  // the assign expression chain had been reverse when parser it.
  //
  // +-------+    +------+    +------+
  // | 4 c = | -> | b += | -> | a -= |
  // +-------+    +------+    +------+
  //   | |          |           |
  //   | unary      unary       unary
  //   binary
  // first node   back node


  // first node
  idx_left_of_assign_operator
    = tree[idx_assign_expression]
        .value.assignment_expression.idx_unary_or_binary_expression;
  idx_right_of_assign_operator
    = tree[idx_assign_expression].value.assignment_expression.idx_binary_expression;

  type_left_of_assign_operator = recursion_analysis_and_assign_type_to_node(
    idx_left_of_assign_operator, nullptr, limit_type
  );

  type_right_of_assign_operator = recursion_analysis_and_assign_type_to_node(
    idx_right_of_assign_operator, type_left_of_assign_operator, nullptr
  );

  // each back node has type so we don't need to deliver node type at operator \
  // left or father node type , just return leftest node type
  llvm::Type *leftest_type = nullptr;
  for (ast::idx i = tree[idx_assign_expression]
                      .value.assignment_expression.idx_next_assignment_expression;
       i != ast::null;
       i = tree[i].value.assignment_expression.idx_next_assignment_expression) {
    leftest_type = recursion_analysis_and_assign_type_to_node(
      tree[i].value.assignment_expression.idx_unary_or_binary_expression, nullptr, nullptr
    );
  }
  return leftest_type;
}

/**
 * @brief
 *
 * @param idx_binary_or_unary_expression
 * @param left_value_type
 * @param father_node_type
 * @return llvm::Type*
 */
llvm::Type *build_llvm_ir::recursion_analysis_and_assign_type_to_node(
  ast::idx    idx_binary_or_unary_expression,
  llvm::Type *left_value_type,
  llvm::Type *father_node_type
) {
  // -----------------------------
  // CASE:1 is an binary expression
  // -----------------------------
  if (tree[idx_binary_or_unary_expression].type == ast::node_type::binary_expression) {
    ast::idx idx_left_node
      = tree[idx_binary_or_unary_expression].value.binary_expression.idx_left_node;
    ast::idx idx_right_node
      = tree[idx_binary_or_unary_expression].value.binary_expression.idx_right_node;

    llvm::Type *type_of_this_binary_expression_node = nullptr;

    llvm::Type *l_node_type = recursion_analysis_and_assign_type_to_node(
      idx_left_node, left_value_type, nullptr
    );
    llvm::Type *r_node_type = recursion_analysis_and_assign_type_to_node(
      idx_right_node, left_value_type, nullptr
    );

    // judge left or right node's type
    // one side of son node has specific type
    if (l_node_type != nullptr || r_node_type != nullptr) {
      // both left and right node's type is the same type
      if (l_node_type == r_node_type) {
        type_of_this_binary_expression_node = l_node_type;
      }
      // l_node_type is unknown
      else if (l_node_type == nullptr) {
        assign_type_to_all_son_node(idx_left_node, r_node_type);
        type_of_this_binary_expression_node = r_node_type;
      }
      // r_node_type is unknown
      else if (r_node_type == nullptr) {
        assign_type_to_all_son_node(idx_right_node, l_node_type);
        type_of_this_binary_expression_node = l_node_type;
      }
      // left and right node's type is different
      else {
        TODO
        // c89 conversion rules ??
      }
    }
    // both left and right node's type is unknown
    // assign all son node left_value_type
    else {
      type_of_this_binary_expression_node = left_value_type;
      assign_type_to_all_son_node(
        idx_binary_or_unary_expression, type_of_this_binary_expression_node
      );
    }
    tree[idx_binary_or_unary_expression].llvm_type = type_of_this_binary_expression_node;
    return type_of_this_binary_expression_node;
  }
  // -----------------------------
  // CASE:2 is an unary expression
  // -----------------------------
  else if (tree[idx_binary_or_unary_expression].type == ast::node_type::unary_expression) {
    // sizeof return a constant value
    if (tree[idx_binary_or_unary_expression].value.unary_expression.is_sizeof) {
      assign_type_to_all_son_node(idx_binary_or_unary_expression, father_node_type);
      return father_node_type;
    }
    // unary operator <unary expression>
    else if (tree[idx_binary_or_unary_expression].value.unary_expression.unary_operator != token::invalid) {
      TODO
    }
    // conversion : (type)unary expression
    else if (tree[idx_binary_or_unary_expression].value.unary_expression.idx_declaration_declarator != ast::null) {
      TODO
    }
    // postfix expression
    else if (tree[idx_binary_or_unary_expression].value.unary_expression.idx_postfix_expression != ast::null) {
      ast::idx idx_postfix_expression = tree[idx_binary_or_unary_expression]
                                          .value.unary_expression.idx_postfix_expression;
      ast::idx idx_primary_expression
        = tree[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

      // identifier
      if (tree[idx_primary_expression].value.primary_expression.idx_identifier != ast::null) {
        ast::idx idx_identifier
          = tree[idx_primary_expression].value.primary_expression.idx_identifier;
        return find_type(std::string(tree[idx_identifier].value.identifier.name));
        // return
        // find_value(std::string(tree[idx_identifier].value.identifier.name))->getType();
      }
      // constant
      else if (tree[idx_primary_expression].value.primary_expression.idx_constant != ast::null) {
        ast::idx idx_constant
          = tree[idx_primary_expression].value.primary_expression.idx_constant;
        if (father_node_type != nullptr) {
          tree.get_constant(idx_constant).llvm_type = father_node_type;
          return father_node_type;
        }
        else if (left_value_type != nullptr) {
          tree.get_constant(idx_constant).llvm_type = left_value_type;
          return left_value_type;
        }
        else {
          // both father_node_type and left_value_type are nullptr
          // return nullptr wait for top stack frame function to assign
          return nullptr;
        }
      }
      // expression
      else if (tree[idx_primary_expression].value.primary_expression.idx_expression != ast::null) {
        if (father_node_type != nullptr) {
          return analysis_and_assign_type_to_node(
            tree[idx_primary_expression].value.primary_expression.idx_expression,
            father_node_type
          );
        }
        else if (left_value_type != nullptr) {
          return analysis_and_assign_type_to_node(
            tree[idx_primary_expression].value.primary_expression.idx_expression,
            left_value_type
          );
        }
        else {
          TODO NOT_REACHABLE
        }
      }
      NOT_REACHABLE
    }
    NOT_REACHABLE
  }
  NOT_REACHABLE
}

/**
 * @brief
 *
 * @param idx_binary_or_unary_expression
 * @param type
 */
void build_llvm_ir::assign_type_to_all_son_node(
  ast::idx    idx_binary_or_unary_expression,
  llvm::Type *type
) {
  tree[idx_binary_or_unary_expression].llvm_type = type;

  // binary expression
  if (tree[idx_binary_or_unary_expression].type == ast::node_type::binary_expression) {
    tree[idx_binary_or_unary_expression].llvm_type = type;
    assign_type_to_all_son_node(
      tree[idx_binary_or_unary_expression].value.binary_expression.idx_left_node, type
    );
    assign_type_to_all_son_node(
      tree[idx_binary_or_unary_expression].value.binary_expression.idx_right_node, type
    );
  }
  // unary expression
  else if (tree[idx_binary_or_unary_expression].type == ast::node_type::unary_expression) {
    tree[idx_binary_or_unary_expression].llvm_type = type;
    // unary operator <unary expression>
    if (tree[idx_binary_or_unary_expression].value.unary_expression.unary_operator != token::invalid) {
      assign_type_to_all_son_node(
        tree[idx_binary_or_unary_expression].value.unary_expression.idx_unary_expression,
        type
      );
    }
    // (type) unary expression
    else if (tree[idx_binary_or_unary_expression].value.unary_expression.idx_declaration_declarator != ast::null) {
      TODO
    }
    // postfix expression
    else if (tree[idx_binary_or_unary_expression].value.unary_expression.idx_postfix_expression != ast::null) {
      ast::idx idx_postfix_expression = tree[idx_binary_or_unary_expression]
                                          .value.unary_expression.idx_postfix_expression;
      ast::idx idx_primary_expression
        = tree[idx_postfix_expression].value.postfix_expression.idx_primary_expression;
      // constant
      if (tree[idx_primary_expression].value.primary_expression.idx_constant != ast::null) {
        ast::idx idx_constant
          = tree[idx_primary_expression].value.primary_expression.idx_constant;
        tree.get_constant(idx_constant).llvm_type = type;
      }
      // expression
      else if (tree[idx_primary_expression].value.primary_expression.idx_expression != ast::null) {
        ast::idx idx_expression
          = tree[idx_primary_expression].value.primary_expression.idx_expression;
        assign_type_to_all_son_node(idx_expression, type);
      }
      // identifier
      else if (tree[idx_primary_expression].value.primary_expression.idx_identifier != ast::null) {
        NOT_REACHABLE
      }
      else {
        NOT_REACHABLE
      }
    }
    else {
      NOT_REACHABLE
    }
  }
  else {
    NOT_REACHABLE
  }
}

}  // namespace toy_c

#endif