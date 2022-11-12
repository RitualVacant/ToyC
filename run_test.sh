#! /usr/bin/bash

# PATH
current_path=`pwd`

# EXCUT
excutable_files_path=${current_path}"/build/ToyC"

# INPUT FIlE PATH
input_files_prefix_path="/test/c_files_for_input/"
input_files_prefix_path=${current_path}${input_files_prefix_path}
input_files=()
for i in `ls ${input_files_prefix_path}`
do
    input_files+=($i)
done

# OUTPUT PATH
# txt
output_txt_files_prefix_path="/test/token_output/"
output_txt_files_prefix_path=${current_path}${output_txt_files_prefix_path}
printf "output_txt_files_prefix_path: ${output_txt_files_prefix_path}\n"
# json
output_json_files_prefix_path="/test/json_tree_output/"
output_json_files_prefix_path=${current_path}${output_json_files_prefix_path}
printf "output_json_files_prefix_path: ${output_json_files_prefix_path}\n"
# llvm
output_llvm_files_prefix_path="/test/llvm_output/"
output_llvm_files_prefix_path=${current_path}${output_llvm_files_prefix_path}
printf "output_llvm_files_prefix_path: ${output_llvm_files_prefix_path}\n"
# spec
output_spec_json_files_prefix_path="/test/spec_tree_output/"
output_spec_json_files_prefix_path=${current_path}${output_spec_json_files_prefix_path}
printf "output_spec_json_files_prefix_path: ${output_spec_json_files_prefix_path}\n"

length=${#input_files[@]}
printf "input file number: "${length}"\n"

mode_list=(scan tree ir spt)
postfix_files_list=(txt pu ll pu)
list_output_files_prefix_path=(${output_txt_files_prefix_path} ${output_json_files_prefix_path} ${output_llvm_files_prefix_path} ${output_spec_json_files_prefix_path})


for ((i=0; i<${#mode_list[@]}; i++))
do
    printf "\n"
    printf "===================\n"
    printf "\033[1m\033[45;33m ${mode_list[${i}]} \033[0m\n";
    printf "===================\n\n"
    for ((j=0; j<length; j++))
    do
        printf ${input_files[${j}]}"\n"
        input_file_name=${input_files[${j}]}
        output_file_name=${input_files[${j}]}
        output_file_name=${output_file_name%.*}'.'${postfix_files_list[$i]}
        #printf "${excutable_files_path} -i ${input_files_prefix_path}${input_file_name} -o ${list_output_files_prefix_path[${i}]}${output_file_name} -m ${mode_list[i]}\n"
        ${excutable_files_path} -i ${input_files_prefix_path}${input_file_name} -o ${list_output_files_prefix_path[${i}]}${output_file_name} -m ${mode_list[i]}
        printf "%s\n" "--------------------"
    done
done

# for i in ${!input_files[@]}
# do
#     echo "------"
#     echo "下标为：$i，数组的值为：${input_files[$i]}"
#     echo "------"
# done

#   for input_file_name in ${input_files}
#   do
#       printf ${input_file_name}"\n"
#       output_file_name=${input_file_name%.*}".txt"
#       touch ${output_txt_files_prefix_path}${output_file_name}
#       ${excutable_files_path} -i ${input_files_prefix_path}${input_file_name} -o ${output_txt_files_prefix_path}${output_file_name} -m ${mode}
#       printf "%s\n" "--------------------"
#   done

#for ((i=0; i<$length; ++i))
#do
#    #touch ./${$input_files[$i]%.c}
#    input_file_name=$input_files[$i]
#    echo ${input_file_name}
#    output_file_name=${input_file_name:0-0:2}
#    echo ${output_file_name}
#    #touch ./json_tree_output/${output_file_name}".json"
#    #output_file_name=${output_file_name%_*}
#    #output_file_name=${input_file_name:0:3}
#    #echo ${output_file_name}
#done


#for in_file in $input_files
#do
#    #"${excutable_files_path} -i ${input_files[${idx}]} -o ${output_tokens[${idx}]} -m scan"
#    echo ${in_file}
#    #echo $in_file
#done

# for idx in `seq 0 1 14`
# do
#     # ${excutable_files_path}" -o"${input_files[idx]}" -i "${output_json[idx]}" -m tree"
#     echo ${input_files[idx]}" "${output_json[idx]}
# done
#
# for idx in `seq 0 1 14`
# do
#     # ${excutable_files_path}" -o "${input_files[idx]}" -i "${output_llvm_ir[idx]}" -m llvm"
#     echo ${input_files[idx]}" "${output_llvm_ir[idx]}
# done
