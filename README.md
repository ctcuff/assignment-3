# The Birthday Presents Party

## Running the code

Assuming you're running on a Unix-like system, you'll need to have [make](https://www.gnu.org/software/make/) installed.

In the root directory, run `make birthday_party`. You can also just run `make` to compile everything at once

This will compile the necessary sources into an executable located in the `out` directory. Note that nothing will be printed to the console by default. If you'd like to see output, you can instead run: `make birthday_party FLAGS=-DVERBOSE`

To tun the program, run: `./out/birthday_party`

If you compiled the program in verbose mode, I suggest running it like so instead: `./out/birthday_party > ./out/birthday.txt`

## How It Works

This solution makes use of a course-grained linked list. In the linked list, methods like `insert`, `remove`, and other methods accessed by multiple threads are locked at the beginning and unlocked at its completion using a mutex.

To start, I create an initially empty unordered set called `cards` and another unordered set called `giftBag`. `giftBag` contains integers from 0 to `NUM_GUESTS - 1`. When each thread (servant) runs, it picks a random task. `TASK_ADD_PRESENT` represents adding a present to the linked list. In this scenario, a lock is placed on the gift bag and the first item is removed and inserted into the linked list (_Note: The linked list inserts all items in order_). `TASK_WRITE_CARD` represents a servant removing a gift from the linked list to write a thank you card. Here, I just remove the head of the linked list and insert the removed guest's ID into the `cards` set. Lastly, `TASK_SEARCH_FOR_PRESENT` represents the minotaur randomly asking the servants to check if a random guest is in the chain.

## Execution Time

When running in verbose mode, the program finished anywhere from 10 to 20 seconds depending on your system (print statements slow down execution to about 50 to 60 seconds). One of the setbacks with this solution is the need for locks in so many places. The linked list needs a lock when inserting, removing, or searching for anything and both the gift bag and card sets need locks when inserting and removing. Another major contribution towards execution time is the nature of randomness. Each thread chooses it's task randomly, so there may be an occurrence where the minotaur rarely asks to search the list resulting in a shorter execution time.


# Atmospheric Temperature Reading Module

The previous section's requirements apply. In the root directory, run: `make temperature` to compile and `./out/temperature` to run. By default, the program is set to run for 72 hours (simulated of course). If you'd like to change the simulated time, you can run `make temperature FLAGS=-DHOURS=<HOURS>`. For example: `make temperature FLAGS=-DHOURS=240`. Afterwards, you can re-run the program with the applied change.

## How It Works

This solution makes use of a single mutex and 2 shared vectors. For this problem, 8 threads need to write 60 random sensor readings every hour (1 reading every minute). That's 480 readings every hour. As such, I have a vector of length 480 that each thread writes to. Thread #1 writes to indices 0 - 59, thread #2 writes to indices 60 - 119, and so on. This ensures that each thread only writes to its own specific portion of the array eliminating the need for locks during writing. I have a second vector called `sensorsReady`. Before a sensor takes a reading, it sets `sensorsReady[threadIndex]` to `false`, then `true` after a reading is taken. Afterwards, the thread will periodically check this sensors array to make sure the other sensors have taken a reading before continuing. At the end of every hour, The first thread (which is the designated reporter thread) generates a report outputting the 5 highest and lowest temperatures along with the largest temperature difference and at what interval that difference occurred.

## Execution Time
Execution time for this solution depends on how many "hours" the solution runs. When run for a simulated 72 hours (the default time), this solution takes about 500ms. Because each thread needs to "spin" while waiting for the other sensors, this spinning action adds about 5 - 10ms per run. The tradeoff is that we can eliminate the need for deadlock or starvation because each thread only writes to its own block of memory.
