RESTOREME=$(pwd)
BASEDIR=$(dirname $0)
cd $BASEDIR

# All shell scripts in subdirs
suites=$(find . -name "TESTSUITE*.sh" | sort)

echo $(pwd)

for suite in $suites
do
  echo "Running tests in $suite"
  bash $suite  
done

cd $RESTOREME