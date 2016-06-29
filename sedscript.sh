valgrind --log-file=.vallog $1
sed -n -e "s/^.*\(in use at exit: [1-9]* bytes.*\)/\1/gp" < .vallog
