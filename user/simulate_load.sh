#/bin/bash
while [ 1 ]
do
    count=0
    count=$(awk 'BEGIN{srand();print rand()*10}' | awk -F. '{print $1}')
    for((i=0;i<$count;i++))
    do
        ./uninterruptible >/dev/null 2>/dev/null &
    done
    echo "$count"
    sleep 5
    killall uninterruptible >/dev/null 2>/dev/null
done
