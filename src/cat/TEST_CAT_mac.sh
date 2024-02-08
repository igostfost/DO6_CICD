SUCCESS=0
FAIL=0
DIFF_RES=""
FLAGS="-n" 
TEST_FILE="cat_file_test.txt"
LOGER="cat_log.txt"
echo "" > cat_log.txt

for var in $FLAGS
do
    TEST="$var $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
done


for var in $FLAGS 
do
 for var2 in $FLAGS
 do
    if [ $var != $var2 ]
    then
    TEST="$var $var2 $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
    fi
  done
done

for var in $FLAGS
do
 for var2 in $FLAGS
 do
  for var3 in $FLAGS
  do
    if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
    then
    TEST="$var $var2 $var3 $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
    fi
    done
  done
done

for var in $FLAGS
do
 for var2 in $FLAGS
 do
  for var3 in $FLAGS
  do
   for var4 in $FLAGS
   do
    if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ] && [ $var != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
    then
    TEST="$var $var2 $var3 $var4 $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
    fi
     done
    done
  done
done


if [ $FAIL == 0 ]; then
    rm $LOGER
fi
echo SUCCESS: $SUCCESS
echo FAIL: $FAIL