#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR test_case_cat.txt"
)

declare -a extra=(
"-s test_1_cat.txt"
"-b -e -n -s -t -v test_1_cat.txt"
"-t test_3_cat.txt"
"-n test_2_cat.txt"
"no_file.txt"
"VARn -b test_1_cat.txt"
"-s -n -e test_4_cat.txt"
"test_1_cat.txt -n"
"VARn test_1_cat.txt"
"VARn test_1_cat.txt test_2_cat.txt"
"VARv test_5_cat.txt"
"-n test_case_cat.txt"
"-b test_case_cat.txt"
"-e test_case_cat.txt"
"-s test_case_cat.txt"
"-t test_case_cat.txt"
"VARn test_case_cat.txt test_5_cat.txt test_1_cat.txt"
"-b test_case_cat.txt test_2_cat.txt test_1_cat.txt"
"-e -n test_case_cat.txt test_3_cat.txt test_1_cat.txt"
"-s test_case_cat.txt test_2_cat.txt test_5_cat.txt"
"-t test_case_cat.txt test_3_cat.txt test_1_cat.txt"
"-s -t test_case_cat.txt test_4_cat.txt test_1_cat.txt"
"-e test_case_cat.txt test_4_cat.txt test_5_cat.txt"
"-b test_case_cat.txt test_1_cat.txt test_3_cat.txt"
"-t test_case_cat.txt test_5_cat.txt test_2_cat.txt"
"-v test_case_cat.txt test_5_cat.txt test_1_cat.txt"
"-s -b -n -e test_case_cat.txt test_5_cat.txt test_1_cat.txt test_2_cat.txt test_3_cat.txt test_4_cat.txt"
"-t nonexist"
"-e nonexist"
"-j nonexist"
"--pass nonexist"
)

declare -a gnu_test=(
"--number-nonblank test_2_cat.txt"
"--number test_4_cat.txt"
"--squeeze-blank test_3_cat.txt"
"--number-nonblank test_case_cat.txt test_4_cat.txt test_1_cat.txt"
"--number test_case_cat.txt test_2_cat.txt test_1_cat.txt"
"--squeeze-blank test_case_cat.txt test_3_cat.txt test_1_cat.txt"
"--number-nonblank --squeeze-blank test_case_cat.txt test_2_cat.txt test_4_cat.txt"
"--number-nonblank --squeeze-blank --number test_case_cat.txt test_3_cat.txt test_1_cat.txt"
"--number-nonblank --squeeze-blank --number lol"
)
declare -a gnu_orig=(
"-b test_2_cat.txt"
"VARn test_4_cat.txt"
"-s test_3_cat.txt"
"-b test_case_cat.txt test_4_cat.txt test_1_cat.txt"
"VARn test_case_cat.txt test_2_cat.txt test_1_cat.txt"
"-s test_case_cat.txt test_3_cat.txt test_1_cat.txt"
"-b -s test_case_cat.txt test_2_cat.txt test_4_cat.txt"
"-b -s -n test_case_cat.txt test_3_cat.txt test_1_cat.txt"
"-b -s -n lol"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m cat $t"
    else
      (( FAIL++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m cat $t"
    fi
    rm test_s21_cat.log test_sys_cat.log
}


for (( j=0; j<9; j++ ));
do
    var="-"
    t1=$(echo ${gnu_test[$j]} | sed "s/VAR/$var/")
    t2=$(echo ${gnu_orig[$j]} | sed "s/VAR/$var/")
    ./s21_cat $t1 > test_s21_cat.log
    cat $t2 > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
        (( SUCCESS++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m s21_cat $t1"
    else
        (( FAIL++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m s21_cat $t1"
    fi
    rm test_s21_cat.log test_sys_cat.log
done

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in b e n s t v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 4 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        for var3 in b e n s t v
        do
            for var4 in b e n s t v
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        testing $i
                    done
                fi
            done
        done
    done
done

echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
