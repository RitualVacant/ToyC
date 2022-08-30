#ifndef BUILD_LLVM_IR_CPP
#define BUILD_LLVM_IR_CPP

#include "build_llvm_ir.h"
#include "inner.h"
#include "parser.h"

namespace toy_c {

build_llvm_ir::build_llvm_ir() {
  tree    = std::move(parse.get_syntax_tree());
  context = std::make_unique<llvm::LLVMContext>();
  module  = std::make_unique<llvm::Module>("text_module", *context);
  builder = std::make_unique<llvm::IRBuilder<>>(*context);

  // enable in llvm 15.0.0
  // context->setOpaquePointers(false);
  build_mult_declaration_or_definition(1);
}

build_llvm_ir::~build_llvm_ir() {}

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
llvm::Type *build_llvm_ir::build_argument_declaration(ast::idx idx_argument_declaration) {
  ast::idx idx_declaration_declarator
    = tree[idx_argument_declaration]
        .value.arguments_declaration.idx_declaration_declarator;

  ast::idx idx_declarator
    = tree[idx_argument_declaration].value.arguments_declaration.idx_declarator;

  llvm::Type *ptr_type_argument = build_type(idx_declaration_declarator, idx_declarator);

  return ptr_type_argument;
}

// TODO
// 1. function pointer
//
void build_llvm_ir::build_function_or_function_ptr(
  ast::idx    idx,
  llvm::Type *ptr_declaration_declarator
) {
  // llvm::SmallVector<llvm::Type*> func_args = build_arguments_type_list(idx);
  // llvm::FunctionType *ptr_func = llvm::FunctionType::get();
  return;
}

void build_llvm_ir::build_variable(ast::idx idx, llvm::Type *ptr_declaration_declarator) {
}

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

std::uint64_t build_llvm_ir::build_uint64(ast::idx idx_) {
  // TODO
  return 5;
}

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

void build_llvm_ir::output_llvm_ir() {
  module->dump();
  std::error_code      ec;
  llvm::raw_fd_ostream file("/home/lzj/code/cpp/script/test/llvm.ll", ec);
  module->print(file, nullptr);
  return;
}

// function is used on initial declarator node
// cause a declaration include several initial declarator
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

bool build_llvm_ir::is_struct(ast::idx idx) {
  NOT_REACHABLE
}

bool build_llvm_ir::is_union(ast::idx idx){NOT_REACHABLE}

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
    ptr_now_func_type = ptr_func_type;

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
    // TODO
    // which map
    insert_func_symbol(name, ptr_var);
  }

  return;

  NOT_REACHABLE
}

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
      idx_assign_expression, ptr_block_if_body, ptr_block_else_body, false
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
      idx_assign_expression, ptr_block_if_body, ptr_block_statement_end, false
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
    idx_assign_expression, ptr_while_body, ptr_while_body_end, false
  );

  // while body
  builder->SetInsertPoint(ptr_while_body);
  build_compound_statement(idx_compound_statement);
  builder->CreateBr(ptr_while_condition);

  // while body end
  builder->SetInsertPoint(ptr_while_body_end);
}

void build_llvm_ir::build_switch_statement(ast::idx idx_switch_statement) {
  TODO
}

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
    idx_assign_expression, ptr_do_while_body, ptr_do_while_body_end, false
  );

  // do_while body end
  builder->SetInsertPoint(ptr_do_while_body_end);
}

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
    idx_conditional_assign_expression, ptr_for_compound_statement, ptr_for_end, false
  );

  builder->SetInsertPoint(ptr_for_compound_statement);
  // compound statement
  build_compound_statement(idx_compound_statement);
  // change value
  build_assign_expression(idx_change_assign_expression, nullptr);
  // jump to conditional expression
  builder->CreateBr(ptr_for_conditional_assign_expression);

  // for end
  builder->SetInsertPoint(ptr_for_end);
}

void build_llvm_ir::build_return_statement(ast::idx idx_return_statement) {
  builder->CreateRet(build_assign_expression(
    tree[idx_return_statement].value.return_statement.idx_assignment_expression
  ));
}

void build_llvm_ir::build_function(ast::idx idx_function_declaration) {}

void build_llvm_ir::build_expression(ast::idx idx_expression) {
  for (ast::idx i = idx_expression; i != ast::null;
       i          = tree[i].value.expression.idx_next_expression) {
    build_assign_expression(tree[i].value.expression.idx_assignment_expression, nullptr);
  }
  return;
}

llvm::Value *build_llvm_ir::build_assign_expression(
  ast::idx          idx_assign_expression,
  llvm::BasicBlock *ptr_true_block,
  llvm::BasicBlock *ptr_false_block,
  bool              is_return_value
) {
  // assign expression
  if (tree[idx_assign_expression].type == ast::node_type::binary_expression) {
    return build_binary_expression(
      idx_assign_expression, ptr_true_block, ptr_false_block, is_return_value
    );
  }


  ast::idx idx_binary_expression
    = tree[idx_assign_expression].value.assignment_expression.idx_binary_expression;

  // just a binary expression without assign
  // if
  llvm::Value *r_value = build_binary_expression(idx_binary_expression, nullptr);
  llvm::Value *l_value;

  for (ast::idx i = idx_assign_expression; i != ast::null;
       i          = tree[i].value.assignment_expression.idx_next_assignment_expression) {
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
          builder->CreateSDiv(r_value, builder->CreateLoad(l_value->getType(), l_value)),
          l_value
        );
        break;

      // TODO
      case token::mod_agn:  //%=
        break;

      case token::r_shift_agn:  //>>=
        builder->CreateStore(
          builder->CreateSDiv(r_value, builder->CreateLoad(l_value->getType(), l_value)),
          l_value
        );
        break;

      case token::l_shift_agn:  //<<=
        builder->CreateStore(
          builder->CreateSDiv(r_value, builder->CreateLoad(l_value->getType(), l_value)),
          l_value
        );
        break;

      case token::bit_and_agn:  //&=
        builder->CreateStore(
          builder->CreateSDiv(r_value, builder->CreateLoad(l_value->getType(), l_value)),
          l_value
        );
        break;

      case token::bit_or_agn:  //|=
        builder->CreateStore(
          builder->CreateSDiv(r_value, builder->CreateLoad(l_value->getType(), l_value)),
          l_value
        );
        break;

      case token::bit_xor_agn:  //^=
        builder->CreateStore(
          builder->CreateSDiv(r_value, builder->CreateLoad(l_value->getType(), l_value)),
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

// TODO
llvm::Value *build_llvm_ir::build_unary_expression(ast::idx idx_unary_expression) {
  //----------------------------------------------------------------
  // 1. sizeof
  //----------------------------------------------------------------
  if (tree[idx_unary_expression].value.unary_expression.is_sizeof) {
    // TODO
  }

  ast::idx idx_postfix_expression
    = tree[idx_unary_expression].value.unary_expression.idx_postfix_expression;

  ast::idx idx_primary_expression
    = tree[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

  //----------------------------------------------------------------
  // 2. unary operator
  //----------------------------------------------------------------
  if (tree[idx_unary_expression].value.unary_expression.unary_operator != token::invalid) {
    build_unary_expression(idx_unary_expression);
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
}

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

llvm::Value *build_llvm_ir::find_value(std::string name) {
  if (func_symbol_table.find(name) != func_symbol_table.end()) {
    return func_symbol_table.at(name);
  }
  if (global_symbol_table.find(name) != global_symbol_table.end()) {
    return global_symbol_table.at(name);
  }
  NOT_REACHABLE
}

llvm::Value *build_llvm_ir::build_postfix_expression(ast::idx idx_postfix_expression) {
  ast::idx idx_primary_expression
    = tree[idx_postfix_expression].value.postfix_expression.idx_primary_expression;

  //--------------------------------
  // primary expression
  //--------------------------------
  llvm::Value *primary_expression_value;
  //------------
  // identifier
  //------------
  if (tree[idx_primary_expression].value.primary_expression.idx_identifier != ast::null) {
    // TODO
    ast::idx idx_idntifier
      = tree[idx_primary_expression].value.primary_expression.idx_identifier;
    primary_expression_value = find_value(tree[idx_idntifier].value.identifier.name);
  }
  //------------
  // constant
  //------------
  else if (tree[idx_primary_expression].value.primary_expression.idx_constant != ast::null) {
    primary_expression_value
      = build_constant(tree[idx_primary_expression].value.primary_expression.idx_constant
      );
  }
  //------------
  // expression
  //------------
  else {
    primary_expression_value = build_binary_expression(idx_postfix_expression);
  }

  //--------------------------------
  // exist postfix operator
  //--------------------------------
  llvm::Value *postfix_expression_value = primary_expression_value;
  if (tree[idx_postfix_expression].value.postfix_expression.idx_postfix_operator != ast::null) {
    ast::idx idx_postfix_operator
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
  return postfix_expression_value;
}

llvm::Value *build_llvm_ir::build_constant(ast::idx idx_constant) {
  llvm::Type *type;

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
        type, std::stoull(tree.get_constant(idx_constant).value), false
      );

    case ast::node_type::constant_negative_integer_number:
      return llvm::ConstantInt::get(
        type, std::stoull(tree.get_constant(idx_constant).value), true
      );

    default:
      SWITCH_ERROR
  }
}

llvm::Value *build_llvm_ir::build_primary_expression(ast::idx idx_primary_expression
){NOT_REACHABLE}

llvm::Value *build_llvm_ir::build_binary_expression(
  ast::idx          idx_binary_expression,
  llvm::BasicBlock *ptr_true_block,
  llvm::BasicBlock *ptr_false_block,
  bool              is_return_value
) {
  if (tree[idx_binary_expression].type == ast::node_type::unary_expression) {
    llvm::Value *value = build_unary_expression(idx_binary_expression);
    return builder->CreateLoad(value->getType(), value);
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

    case token::bit_and:
      value_l = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_left_node
      );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;
      TODO

        case token::bit_or
          : value_l
            = build_binary_expression(
              tree[idx_binary_expression].value.binary_expression.idx_left_node
            );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;
      TODO

        case token::bit_xor
          : value_l
            = build_binary_expression(
              tree[idx_binary_expression].value.binary_expression.idx_left_node
            );
      value_r = build_binary_expression(
        tree[idx_binary_expression].value.binary_expression.idx_right_node
      );
      break;
      TODO

        case token::equ
          : value_l
            = build_binary_expression(
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
      TODO

        default : SWITCH_ERROR
  }

  if (!is_return_value) {
    auto result = builder->CreateICmpNE(
      return_value, llvm::ConstantInt::get(return_value->getType(), 0)
    );
    builder->CreateCondBr(result, ptr_true_block, ptr_false_block);
  }
  return return_value;
}

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

llvm::Value *build_llvm_ir::build_log_or_chain(ast::idx idx_log_or_operator) {
  return nullptr;
}

void build_llvm_ir::insert_func_symbol(std::string name, llvm::Value *ptr_var) {
  if (func_symbol_table.find(name) != func_symbol_table.end()
      || global_symbol_table.find(name) != global_symbol_table.end()) {
    fmt::print("redeclaration of {}", name);
    exit(0);
  }
  func_symbol_table.insert({name, ptr_var});
}

void build_llvm_ir::insert_symbol_symbol(std::string name, llvm::Value *ptr_var) {
  if (func_symbol_table.find(name) != func_symbol_table.end()
      || global_symbol_table.find(name) != global_symbol_table.end()) {
    fmt::print("redeclaration of {}", name);
    exit(0);
  }
  func_symbol_table.insert({name, ptr_var});
}

std::string build_llvm_ir::get_label() {
  return std::to_string(++label_num);
}

}  // namespace toy_c

#endif
