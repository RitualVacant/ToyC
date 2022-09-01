#! /usr/bin/bash

# PATH
current_path=`pwd`

# EXCUT
excutable_files_path=${current_path}"/build/ToyC"

# INPUT PATH
input_files_prefix_path="/test/c_files_for_input/"
input_files_prefix_path=${current_path}${input_files_prefix_path}
input_files=$(ls ${input_files_prefix_path})

# OUTPUT PATH
# txt
output_txt_files_prefix_path="/test/token_output/"
output_txt_files_prefix_path=${current_path}${output_txt_files_prefix_path}

# json

# length=${#input_files[@]}
mode_list=(scan tree ir)

for mode in ${mode_list[@]}
do
    printf "****************\n"
    printf "* %s         *\n" ${mode}
    printf "****************\n\n"

    for input_file_name in ${input_files}
    do
        printf ${input_file_name}"\n"
        output_file_name=${input_file_name%.*}".txt"
        touch ${output_txt_files_prefix_path}${output_file_name}
        ${excutable_files_path} -i ${input_files_prefix_path}${input_file_name} -o ${output_txt_files_prefix_path}${output_file_name} -m ${mode}
        printf "%s\n" "--------------------"
    done
done

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
