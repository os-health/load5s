# load5s
this is load5s ko.

mkdir os-health
cd os-health
yum install git -y 
git clone https://github.com/os-health/load5s.git
uname -r
wget https://mirrors.aliyun.com/centos-vault/6.9/updates/x86_64/Packages/kernel-devel-2.6.32-696.6.3.el6.x86_64.rpm
rpm -ivh kernel-devel-2.6.32-696.6.3.el6.x86_64.rpm 
cd load5s/kernel/
make 
insmod load5s.ko
cd ./../user/
gcc uninterruptible.c -o uninterruptible
nohup ./simulate_load.sh >/dev/null 2>&1 &
# sleep > 5s
./load_predict.sh 
