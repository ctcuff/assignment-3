make birthday_party
valgrind \
    --tool=memcheck \
    --leak-check=yes \
    --show-reachable=yes \
    --num-callers=20 \
    --track-fds=yes \
    ./out/birthday_party
