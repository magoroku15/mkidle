while :
do
  mkidle=`cat /tmp/mkidle`
  clock=`cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq`
  echo -n $mkidle " " $clock " "
  cat /sys/devices/system/cpu/online
  sleep 1
done
