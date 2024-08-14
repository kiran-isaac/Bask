RESTOREME=$(pwd)
BASEDIR=$(dirname $0)
cd $BASEDIR

red_echo() {
    echo -e "\e[31m$1\e[0m"
}

yellow_echo() {
    echo -e "\e[33m$1\e[0m"
}

green_echo() {
    echo -e "\e[32m$1\e[0m"
}

clean_line() {
    echo -e -n "\033[1K\r"
}

COMPILER=../../build/KL
TESTS=$(find . -name "*.kl" | sort)

#list of failed tests
FAILED_TESTS=""

for t in $TESTS
do
    echo -n "Testing $t" 
    $COMPILER $t > ir.ll 2> KL_output.log

    if [ $? -ne 0 ]; then
        clean_line
        red_echo "Failed to compile$t"
        cat KL_output.log
        FAILED_TESTS="$FAILED_TESTS $t"
        rm -f *.log *.ll
        continue
    fi
    rm -f KL_output.log
    /usr/bin/clang -Wno-override-module ir.ll -xir -o a.out -O3 > $(basename $t)-CLANG.log 2>&1;
    if [ $? -ne 0 ]; then
        clean_line
        red_echo "Failed to assemble $t"
        cat ir.ll
        cat $(basename $t)-CLANG.log
        FAILED_TESTS="$FAILED_TESTS $t"
        rm -f *.log 
        continue
    fi
    rm -f *.log
    ./a.out;
    exit_code=$?
    if [ $exit_code -ne 0 ]; then
        clean_line
        red_echo "Incorrect output for $t. Expected 0, got $exit_code"
        cat ir.ll
        FAILED_TESTS="$FAILED_TESTS $t"
        rm -f *.log *.ll
    fi

    clean_line

    green_echo "Testing $t: passed"

    rm -f a.out *.log *.ll
done

echo ""
echo "Tests finished"

if [ -z "$FAILED_TESTS" ]; then
    green_echo "All tests passed"
else
    # number of failed tests
    red_echo "$(echo $FAILED_TESTS | wc -w) tests failed"
    # add full path to failed tests
    red_echo "Failed tests: $FAILED_TESTS"
fi

cd $RESTOREME