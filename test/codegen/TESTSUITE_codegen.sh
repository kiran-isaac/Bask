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

COMPILER=baskc
LIB=/usr/local/lib/bask/lib.bc
TESTS=$(find . -name "*.bsk" | sort)

#list of failed tests
FAILED_TESTS=""

for t in $TESTS
do
    echo -n "Testing $t" 
    $COMPILER $t 2> BASK_output.log > /dev/null

    if [ $? -ne 0 ]; then
        clean_line
        red_echo "Failed to compile $t"
        cat BASK_output.log
        FAILED_TESTS="$FAILED_TESTS $t"
        rm -f *.log *.out
        continue
    fi
    rm -f BASK_output.log
    rm -f *.log
    ./a.out > /dev/null 2> /dev/null;
    exit_code=$?
    if [ $exit_code -ne 0 ]; then
        clean_line
        red_echo "Incorrect output for $t. Expected 0, got $exit_code"
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