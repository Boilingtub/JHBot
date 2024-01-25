#! /bin/bash

serverpid=$(pidof python)
echo "\n" > overflow_test_results.log
while [ 1 -le 2 ]
do
    #echo "\n\n\n" >> overflow_test_results
    python test_bot.py gen
    #echo "\n\n\n" >> overflow_test_results.log
    echo "jan" | sudo -S pmap ${serverpid} | grep "total" >> overflow_test_results.log
done
