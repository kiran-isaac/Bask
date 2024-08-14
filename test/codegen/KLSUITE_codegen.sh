RESTOREME=$(pwd)
BASEDIR=$(dirname $0)
cd $BASEDIR

COMPILER=../../build/KL
TESTS=*.kl

#list of failed tests
FAILED_TESTS=""

for t in $TESTS
do
    echo -n "Testing $t" 
    $COMPILER $t | /usr/bin/clang -x ir - -o a.out -O3 > /dev/null 2>&1;
    if [ $? -ne 0 ]; then
        echo -e "\nFailed to compile $t"
        FAILED_TESTS="$FAILED_TESTS $t"
        continue
    fi
    ./a.out;
    if [ $? -ne 0 ]; then
        echo -e "\nIncorrect output for $t"
        FAILED_TESTS="$FAILED_TESTS $t"
    fi

    echo " - OK"

    rm -f a.out
done

echo -e "\nTests finished"

if [ -z "$FAILED_TESTS" ]; then
    echo "All tests passed"
else
    echo "Failed tests: $FAILED_TESTS"
fi

cd $RESTOREME