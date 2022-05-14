# $1: which dataset to use

# Run with python (more efficient)
python3 -u graph-gen.py $1

# Run with shell script (low efficiency)
# cat datasets/$1/out.$1 > graph.txt

# i=0
# result=$(sed -n $(echo $(($i+1))),$(echo $(($i+1)))p graph.txt | grep %)

# while [ -n "$result" ]
# do
#    i=$(echo $(($i+1)))
#    result=$(sed -n $(echo $(($i+1))),$(echo $(($i+1)))p graph.txt | grep %)
# done

# sed -i 1,${i}d graph.txt

# i=1
# contents=""
# while [ $i -le $(wc -l < graph.txt) ]
# do
#    j=0
#    sentence=$(sed -n ${i},${i}p graph.txt)
#    for word in $sentence
#    do
#        j=$(echo $(($j+1)))
#        if [ $j != 3 ]
#        then
#            contents="$contents$word "
#        fi
#    done
#    contents="$contents\n"
#    i=$(echo $(($i+1)))
# done

# echo $contents > graph.txt