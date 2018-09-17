#!/bin/bash
function calc_load(){
    last_load_long=$1
    exp=$2
    active=$3
   
    load=$((exp*last_load_long))
    load=$((load+$((2048-exp))*active))
    load=$((load+1024))
    load=$((load/2048))
    load=$((load+10))
    predict_load=$(awk -v x=$load -v y=2048 'BEGIN{printf "%.02f",x/y}')

    echo $predict_load
}

if [[ ! -e /proc/load5s ]];then
    echo "/proc/load5s is not exist."
    exit 1
fi

echo -e "start predict ......\n"
last_load=$(cat /proc/loadavg)

last_load1=$(echo $last_load | awk '{print $1}')
last_load5=$(echo $last_load | awk '{print $2}')
last_load15=$(echo $last_load | awk '{print $3}')
last_load1_long=$(awk -v x=2048 -v y=$last_load1 'BEGIN{printf "%.0f",x*y}')
last_load5_long=$(awk -v x=2048 -v y=$last_load5 'BEGIN{printf "%.0f",x*y}')
last_load15_long=$(awk -v x=2048 -v y=$last_load15 'BEGIN{printf "%.0f",x*y}')

usleep 5001000

current_load=$(cat /proc/loadavg)
calc_load_tasks_counter=$(cat /proc/load5s)

current_load1=$(echo $current_load | awk '{print $1}')
current_load5=$(echo $current_load | awk '{print $2}')
current_load15=$(echo $current_load | awk '{print $3}')
active=$((2048*calc_load_tasks_counter))

# compute
predict_load1=$(calc_load $last_load1_long 1884 $active)
predict_load5=$(calc_load $last_load5_long 2014 $active)
predict_load15=$(calc_load $last_load15_long 2037 $active)

# result 
tabs=""
tabs=$tabs"1/5/15 last_load current_load predict_load \n"
tabs=$tabs"load1: $last_load1 $current_load1 $predict_load1 \n"
tabs=$tabs"load5: $last_load5 $current_load5 $predict_load5 \n"
tabs=$tabs"load15: $last_load15 $current_load15 $predict_load15 \n"
echo -e "$tabs" | column -t 
echo -e "\nload5s is: "$calc_load_tasks_counter
