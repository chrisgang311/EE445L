orig=25000
new=80000
>freq.txt
cat mp3.h | head -61 | tail -49 > fstring
cat fstring | while read line
do
front=`echo $line | cut -d$' ' -f1-2`
back=`echo $line | cut -d$' ' -f4-10`
frequency=`echo $line | cut -d$' ' -f3`
frequency=`echo $frequency | xargs` # trim
ans=$(echo "${frequency}*${new}/${orig}" |bc)
echo "ans = $ans and freq = $frequency"
echo "$front $ans $back" >> freq.txt
done
rm fstring
